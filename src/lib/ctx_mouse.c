/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "app.h"
#include "event.h"
#include "block.h"
#include "brick.h"
#include "plug.h"
#include "ui.h"
#include "draw.h"
#include "screen.h"
#include "ctx.h"

int is_mouse_over_background(t_context *C)
{
	if(
		C->event->color[0]==255 && // == backgound_color[0]
		C->event->color[1]==255 &&
		C->event->color[2]==255 
		)
		return 1;
	else
		return 0;
}

int is_mouse_over_plug(t_context *C,t_plug *plug)
{
	if(
		C->event->color[0] == plug->idcol[0] &&
		C->event->color[1] == plug->idcol[1] &&
		C->event->color[2] == plug->idcol[2] 
	)	
	{
		return 1;
	}

	return 0;
}

int is_mouse_over(t_context *C,int *c)
{
	if(
		C->event->color[0] == c[0] &&
		C->event->color[1] == c[1] &&
		C->event->color[2] == c[2] 
	)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int is_mouse_over_brick(t_context *C,t_brick *brick)
{
	if(
		C->event->color[0] == brick->idcol_left[0] &&
		C->event->color[1] == brick->idcol_left[1] &&
		C->event->color[2] == brick->idcol_left[2] 
	)
	{
		brick->brick_state.is_left_pressed=1;
		brick->brick_state.is_right_pressed=0;

		brick->brick_state.is_mouse_over_plug_in=0;
		brick->brick_state.is_mouse_over_plug_out=0;
		return 1;
	}
	else if
		(
		C->event->color[0] == brick->idcol_right[0] &&
		C->event->color[1] == brick->idcol_right[1] &&
		C->event->color[2] == brick->idcol_right[2] 
		)
	{

		brick->brick_state.is_left_pressed=0;
		brick->brick_state.is_right_pressed=1;

		brick->brick_state.is_mouse_over_plug_in=0;
		brick->brick_state.is_mouse_over_plug_out=0;
		return 1;
	}
	else
	{
		brick->brick_state.is_left_pressed=0;
		brick->brick_state.is_right_pressed=0;

		if(brick->brick_state.draw_plugs)
		{
			if(is_mouse_over_plug(C,&brick->plug_in))
			{
				brick->brick_state.is_mouse_over_plug_in=1;
				brick->brick_state.is_mouse_over_plug_out=0;
				return 1;
			}
			else if(is_mouse_over_plug(C,&brick->plug_out))
			{
				brick->brick_state.is_mouse_over_plug_in=0;
				brick->brick_state.is_mouse_over_plug_out=1;
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
}

void ctx_get_mouse_pos(t_context *C,float *r)
{
	float x = (float)(C->app->mouse->x-C->ui->pan_x)/C->ui->zoom;
	float y = (float)(C->app->mouse->y-C->ui->pan_y)/C->ui->zoom;

	r[0]=x;
	r[1]=y;
}

void ctx_mouse( t_context *C)
{
	t_screen *s = C->ui->screen_active;
	t_mouse *mouse = C->app->mouse;

	if( mouse->event)
	{
		if( mouse->motion_passive)
		{
			event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, MOUSE_MOTION_PASSIVE);
			mouse->motion_passive = 0;
		}

		if( mouse->motion)
		{
			event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, MOUSE_MOTION);
			mouse->motion = 0;
		}

		if( mouse->button)
		{
			if( mouse->button_left == button_pressed)
			{
				event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, MOUSE_LEFT_PRESSED);
			}
			else if( mouse->button_left == button_released)
			{
				event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, MOUSE_LEFT_RELEASED);
			}

			if( mouse->button_right == button_pressed)
			{
				event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, MOUSE_RIGHT_PRESSED);
			}
			else if( mouse->button_right == button_released)
			{
				event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, MOUSE_RIGHT_RELEASED);
			}

			if( mouse->button_middle == button_pressed)
			{
				event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, MOUSE_MIDDLE_PRESSED);
			}
			else if( mouse->button_middle == button_released)
			{
				event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, MOUSE_MIDDLE_RELEASED);
			}

			mouse->button_left = button_idle;
			mouse->button_right = button_idle;
			mouse->button_middle = button_idle;
			mouse->button = 0;
		}

		mouse->event = 0;
	}

	if( C->app->keyboard->special)
	{
		event_add( C->event, s, C->app->mouse->x, C->app->mouse->y, C->app->keyboard->special);
	}

	ctx_ui_mouse_motion( C, C->app->mouse->x, C->app->mouse->y);
}
