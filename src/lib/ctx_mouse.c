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
		brick->state.is_left_pressed=1;
		brick->state.is_right_pressed=0;

		brick->state.is_mouse_over_plug_in=0;
		brick->state.is_mouse_over_plug_out=0;
		return 1;
	}
	else if
		(
		C->event->color[0] == brick->idcol_right[0] &&
		C->event->color[1] == brick->idcol_right[1] &&
		C->event->color[2] == brick->idcol_right[2] 
		)
	{

		brick->state.is_left_pressed=0;
		brick->state.is_right_pressed=1;

		brick->state.is_mouse_over_plug_in=0;
		brick->state.is_mouse_over_plug_out=0;
		return 1;
	}
	else
	{
		brick->state.is_left_pressed=0;
		brick->state.is_right_pressed=0;

		if(brick->state.draw_plugs)
		{
			if(is_mouse_over_plug(C,&brick->plug_in))
			{
				brick->state.is_mouse_over_plug_in=1;
				brick->state.is_mouse_over_plug_out=0;
				return 1;
			}
			else if(is_mouse_over_plug(C,&brick->plug_out))
			{
				brick->state.is_mouse_over_plug_in=0;
				brick->state.is_mouse_over_plug_out=1;
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
