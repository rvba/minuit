/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "context.h"
#include "scene.h"
#include "node.h"
#include "app.h"
#include "screen.h"
#include "camera.h"
#include "ui.h"
#include "list.h"
#include "txt.h"
#include "block.h"
#include "sketch.h"
#include "event.h"
#include "ctx.h"

void screen_on(t_screen *screen)
{
	screen->is_visible=1;
	screen->is_active=1;
}

void screen_off(t_screen *screen)
{
	screen->is_visible=0;
	screen->is_active=0;
}

void screen_block_add(t_screen *screen, t_block *block)
{
	t_context *C = ctx_get();
//	C->scene->store = 1;

	if(screen->blocks)
	{
		list_add(screen->blocks, block);
	}
	else
	{
		t_node *node = scene_add(C->scene, dt_lst, "lst");
		t_lst *lst = node->data;
		screen->blocks = lst;

		screen_block_add(screen, block);
	}

	//C->scene->store = 0;
}


void screen_switch_by_name(char *name)
{
	t_context *C=ctx_get();
	// check if screen exists
	t_node *node=lst_find_node_by_name(C->ui->screens,name);

	if(node)
	{
		t_link *l;

		for(l=C->scene->screens->first;l;l=l->next) //XXX scene->screens ? ui->screens ?
		{
			t_node *this_node=l->data;
			t_screen *this_screen=this_node->data;

			// enable screen
			if(is(this_screen->name,name))
			{
				this_screen->is_visible=1;
				this_screen->is_active=1;
				C->ui->screen_link_active=l;
			}
			// disable screen
			else
			{
				this_screen->is_visible=0;
				this_screen->is_active=0;
			}
		}
	}	
}

void screen_switch_to_main(void)
{
	screen_switch_by_name("screen_main");
}

// GENERIC SCREEN

void screen_generic(t_screen *screen)
{
	t_context *C=ctx_get();
	t_camera *camera = C->ui->camera;
	op_camera_switch_2d(C,camera);

	glPushMatrix();
	glLoadIdentity();

	ui_draw_mouse();
	ui_draw_menu();

	glPushMatrix();
	float scale=4;
	float p[3]={(C->app->window->width)/4,(C->app->window->height)/2,0};
	float color[]={0,0,0,0};
	glTranslatef(p[0],p[1],p[2]);
	glScalef(scale,scale,scale);
	txt_draw_direct_2d(screen->name,p,color,scale);
	glPopMatrix();

	glPopMatrix();

	op_camera_switch_3d(C, camera);
}

// MAIN SCREEN

void screen_main(t_screen *screen)
{
	t_context *C=ctx_get();
	t_camera *camera = C->ui->camera;

	op_camera_switch_2d(C,camera);

	glPushMatrix();
	glLoadIdentity();

	ui_draw_mouse();
	ui_draw_intro();
	ui_draw_lines();
	ui_draw_status_bar();
	ui_draw_term();
	ui_draw_grid();

	ui_draw_menu();

	glPopMatrix();
	op_camera_switch_3d(C, camera);
}

void screen_sets(t_screen *screen)
{
	t_context *C=ctx_get();
	t_camera *camera = C->ui->camera;

	op_camera_switch_2d(C,camera);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(C->ui->pan_x,C->ui->pan_y,0);
	float zoom = C->ui->zoom;
	glScalef(zoom,zoom,zoom);


	ui_draw_mouse();
	ui_draw_intro();
	ui_draw_lines();
	ui_draw_status_bar();
	ui_draw_term();
	ui_draw_grid();

	glPushMatrix();
	glLoadIdentity();
	ui_draw_menu();
	glPopMatrix();

	ui_draw_sets();

	glPopMatrix();
	op_camera_switch_3d(C, camera);
}

void screen_bricks_draw(t_context *C,const char *name)
{
	t_node *node = scene_node_get(C->scene,"block",name);
	t_block *block = node->data;
	block->cls->draw(block);
	glTranslatef(0,block->height,0);
}


void screen_bricks(t_screen *screen)
{
	t_context *C=ctx_get();
	t_camera *camera = C->ui->camera;

	op_camera_switch_2d(C,camera);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(C->ui->pan_x,C->ui->pan_y,0);
	float zoom = C->ui->zoom;
	glScalef(zoom,zoom,zoom);


	glPushMatrix();
	glLoadIdentity();


	C->event->ui.use_scale = 0;

	/*
	screen_bricks_draw(C,"menu_scalar");
	screen_bricks_draw(C,"menu_time");
	screen_bricks_draw(C,"menu_operator");
	screen_bricks_draw(C,"menu_vector");
	screen_bricks_draw(C,"menu_logic");
	screen_bricks_draw(C,"menu_maths");
	screen_bricks_draw(C,"menu_lst");
	*/

	t_link *link;
	t_block *block;

	for(link = screen->blocks->first; link; link = link->next)
	{
		block = link->data;
		block->cls->draw(block);
		glTranslatef(0,block->height,0);
	}

	C->event->ui.use_scale = 1;

	/*
	float height = (float) C->app->window->height;
	skt_rectangle(0,0,200,0,200,height,0,height);
	*/

	glPopMatrix();

	glPopMatrix();
	op_camera_switch_3d(C, camera);
}

// FREE

void screen_free(t_screen *screen)
{
}

// REBIND

t_screen *screen_rebind(t_scene *scene, void *ptr)
{
	t_screen *screen = (t_screen *) ptr;

	rebind(scene,"screen","blocks",(void **)&screen->blocks);

	return ptr;
}

// NEW

t_screen *screen_new(const char *name)
{
	t_screen *screen=(t_screen *)malloc(sizeof(t_screen));

	screen->id=0;
	screen->id_chunk=0;
	set_name(screen->name,name);
	screen->users=0;

	screen->is_visible=0;
	screen->is_active=0;
	screen->zoom=1;
	screen->pan_x=0;
	screen->pan_y=0;
	screen->draw=NULL;
	screen->keymap=NULL;

	screen->blocks = NULL;
	
	return screen;
}

// INIT

void screen_init(void)
{
	screen_main_make();
	screen_browser_make();
	screen_sets_make();
	screen_bricks_make();
}



