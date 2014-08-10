/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "screen.h"
#include "camera.h"
#include "ctx.h"
#include "opengl.h"
#include "ui.h"
#include "event.h"
#include "base.h"
#include "block.h"
#include "viewport.h"
#include "draw.h"
#include "app.h"
#include "sketch.h"

void ui_draw_menu(void)
{
	t_context *C=ctx_get();
	t_node *node=C->scene->selected;
	t_block *menu=NULL;

	if(C->ui->show_menu)
	{
		C->event->ui.use_scale = 0;
		node = scene_node_get( C->scene, dt_block, "menu_mouse");
		menu = ( t_block *) node->data;
		menu->cls->draw(menu);
		C->event->ui.use_scale = 1;
	}
}

void ui_draw_bar(void)
{
	t_context *C=ctx_get();
	t_node *node=C->scene->selected;
	t_block *menu=NULL;

	glPushMatrix();
	if(C->ui->show_intro && 0)
	{
		glTranslatef(0,2,0);
		C->event->ui.use_scale = 0;
		node = scene_node_get( C->scene, dt_block, "bar");
		menu = ( t_block *) node->data;
		menu->cls->draw(menu);
		C->event->ui.use_scale = 1;
	}
	glPopMatrix();
}

void ui_draw_mouse(void)
{
	t_context *C = ctx_get();
	if(C->ui->show_mouse && (C->draw->mode != mode_selection))
	{
		float *color=C->ui->front_color;
		float pos[3];

		vset(pos,0,0,0);

		pos[0] = (float) C->app->mouse->x;
		pos[1] = (float) C->app->mouse->y;
		pos[2] = 0;

		glPushMatrix();
		glLoadIdentity();
		C->event->ui.use_point_global_width = 0;
		skt_point(pos,C->ui->mouse_size,color);
		C->event->ui.use_point_global_width = 1;
		glPopMatrix();
	}
}

void screen_main(t_screen *screen)
{
	screen_switch_2d( screen);

	glPushMatrix();
	glLoadIdentity();

		ui_draw_mouse();
		ui_draw_menu();
		ui_draw_bar();
		ui_draw_icon_freeze();

	glPopMatrix();
}
