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
#include "viewport.h"
#include "camera.h"
#include "memory.h"

t_camera *screen_camera(t_screen *screen)
{
	t_lst *lst = screen->viewports;
	t_link *link = lst->first;
	t_viewport *viewport = link->data;
	t_camera *camera = viewport->camera;

	return camera;
}

t_screen *screen_default(const char *name, void (* draw)(t_screen *s))
{
	t_context *C=ctx_get();

	t_node *node=scene_add(C->scene,dt_screen,name);
	t_screen *screen=node->data;

	screen->keymap=keymap_generic;
	screen->draw=draw;

	lst_add(C->ui->screens,node,name);

	// Lst
	t_node *node_lst = scene_add( C->scene, dt_list, "lst");
	t_lst *lst = node_lst->data;

	screen->viewports = lst;

	// Viewport
	t_node *node_viewport = scene_add( C->scene, dt_viewport, name);
	t_viewport *viewport = node_viewport->data;

	// Camera
	t_node *node_camera = scene_add( C->scene, dt_camera, name);
	t_camera *camera = node_camera->data;

	viewport->camera = camera;

	lst_add(screen->viewports, viewport, name);

	return screen;
}

void screen_always(t_screen *screen)
{
	screen->is_active=1;
	screen->is_visible=1;
	screen->always_active=1;
	screen->always_visible=1;
}

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

// BLOCK ADD

void screen_block_add(t_screen *screen, t_block *block)
{
	t_context *C = ctx_get();

	if(screen->blocks)
	{
		list_add_data(screen->blocks, block);
	}
	else
	{
		t_node *node = scene_add(C->scene, dt_lst, "lst");
		t_lst *lst = node->data;
		screen->blocks = lst;

		screen_block_add(screen, block);
	}
}

// SWITCH BY NAME

void screen_switch_by_name(char *name)
{
	t_context *C=ctx_get();
	// check if screen exists
	t_node *node = list_find_node_by_name(C->ui->screens,name);

	if(node)
	{
		t_link *l;

		for(l=C->scene->screens->first;l;l=l->next) 
		{
			t_node *this_node=l->data;
			t_screen *this_screen=this_node->data;

			// enable screen
			if(is(this_screen->id.name, name))
			{
				this_screen->is_visible=1;
				this_screen->is_active=1;
				C->ui->screen_link_active=l;
			}
			// disable screen
			else
			{
				if(!this_screen->always_visible) this_screen->is_visible=0;
				if(!this_screen->always_active) this_screen->is_active=0;
			}
		}
	}	
}

// SWITCH TO MAIN

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

	ui_draw_menu();

	glPushMatrix();
	float scale=4;
	float p[3]={(C->app->window->width)/4,(C->app->window->height)/2,0};
	float color[]={0,0,0,0};
	glTranslatef(p[0],p[1],p[2]);
	glScalef(scale,scale,scale);
	txt_draw_direct_2d(screen->id.name, p, color, scale);
	glPopMatrix();

	glPopMatrix();

	op_camera_switch_3d(C, camera);
}

// SCREEN MAIN

void screen_main(t_screen *screen)
{
	t_context *C=ctx_get();
	t_camera *camera = C->ui->camera;

	op_camera_switch_2d(C,camera);

	glPushMatrix();
	glLoadIdentity();

	ui_draw_mouse();
	op_camera_switch_2d(C,camera);
	ui_draw_term();
	ui_draw_grid();

	ui_draw_menu();
	ui_draw_bar();

	glPopMatrix();
	op_camera_switch_3d(C, camera);
}

// SCREEN SETS

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
	op_camera_switch_2d(C,camera);
	ui_draw_grid();

	glPushMatrix();
	glLoadIdentity();
	glPopMatrix();

	ui_draw_sets();

	glPopMatrix();
	op_camera_switch_3d(C, camera);
}

// SCREEN BRICKS

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

	t_link *link;
	t_block *block;

	for(link = screen->blocks->first; link; link = link->next)
	{
		block = link->data;
		block->cls->draw(block);
		glTranslatef(0,block->height,0);
	}

	C->event->ui.use_scale = 1;

	glPopMatrix();

	glPopMatrix();
	op_camera_switch_3d(C, camera);
}

// CLONE

t_screen *screen_clone(t_screen *screen)
{
	if(screen)
	{
		t_screen *clone = screen_new(screen->id.name);

		clone->is_active = screen->is_active;
		clone->is_visible = screen->is_visible;
		clone->always_active = screen->always_active;
		clone->always_visible = screen->always_visible;
		clone->width = screen->width;
		clone->height = screen->height;
		clone->zoom = screen->zoom;
		clone->pan_x = screen->pan_x;
		clone->pan_y = screen->pan_y;

		clone->blocks = list_clone(screen->blocks, dt_block);
		clone->viewports = list_clone(screen->viewports, dt_viewport);

		clone->draw = screen->draw;
		clone->keymap = screen->keymap;

		return clone;
	}
	else
	{
		return NULL;
	}
}

// FREE

void _screen_free(t_screen *screen)
{
	if(screen->blocks) list_free_data(screen->blocks, dt_block);
	if(screen->viewports) list_free_data(screen->viewports,dt_viewport);

	mem_free( screen, sizeof( t_screen));
}

void screen_free(t_screen *screen)
{
}

// REBIND

t_screen *screen_rebind(t_scene *scene, void *ptr)
{
	t_screen *screen = (t_screen *) ptr;

	rebind(scene,"screen","blocks",(void **)&screen->blocks);
	rebind(scene,"screen","viewports",(void **)&screen->viewports);

	return ptr;
}

// NEW

t_screen *screen_new(const char *name)
{
	t_screen *screen=(t_screen *)mem_malloc(sizeof(t_screen));

	id_init(&screen->id, name);

	screen->is_visible=0;
	screen->is_active=0;
	screen->always_active=0;
	screen->always_visible=0;
	screen->zoom=1;
	screen->pan_x=0;
	screen->pan_y=0;
	screen->draw=NULL;
	screen->keymap=NULL;

	screen->blocks = NULL;
	screen->viewports = NULL;
	screen->data = NULL;
	
	return screen;
}




