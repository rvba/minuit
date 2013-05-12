/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

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
	
	return screen;
}

void screen_init(void)
{
	screen_main_make();
	screen_browser_make();
	screen_sets_make();
}



