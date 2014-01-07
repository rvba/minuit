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

int ctx_mouse_hover_brick_left( t_context *C, t_brick *brick)
{
	if( COLOR_MATCH( C->event->color, brick->idcol_left)) return 1;
	else return 0;
}

int ctx_mouse_hover_brick_right( t_context *C, t_brick *brick)
{
	if( COLOR_MATCH( C->event->color, brick->idcol_right)) return 1;
	else return 0;
}

int ctx_mouse_hover_brick_plug_in( t_context *C, t_brick *brick)
{
	if( COLOR_MATCH( C->event->color, brick->plug_in.idcol)) return 1;
	else return 0;
}

int ctx_mouse_hover_brick_plug_out( t_context *C, t_brick *brick)
{
	if( COLOR_MATCH( C->event->color, brick->plug_out.idcol)) return 1;
	else return 0;
}

int ctx_mouse_hover_brick( t_context *C, t_brick *brick)
{
	if	( ctx_mouse_hover_brick_right( C, brick)) return 1;
	else if	( ctx_mouse_hover_brick_left( C, brick)) return 1;
	else if	( ctx_mouse_hover_brick_plug_in( C, brick)) return 1;
	else if	( ctx_mouse_hover_brick_plug_out( C, brick)) return 1;
	else return 0;
}

void ctx_get_mouse_pos(t_context *C,float *r)
{
	float x = (float)( C->app->mouse->x - C->ui->pan_x) / C->ui->zoom;
	float y = (float)( C->app->mouse->y - C->ui->pan_y) / C->ui->zoom;

	r[0]=x;
	r[1]=y;
}

void ctx_ui_mouse_motion( t_context *C, int x, int y)
{
	t_ui *ui = C->ui;
	ui->mouse_x = x;
	ui->mouse_y = y;

	int dx;
	int dy;

	int last_x = ui->mouse_last_x;
	int last_y = ui->mouse_last_y;

	dx = x - last_x;
	dy = y - last_y;

	ui->mouse_dx = dx;
	ui->mouse_dy = dy;

	ui->mouse_last_x = x;
	ui->mouse_last_y = y;

}

inline int mouse_pressed( int state)
{
	if(
		state == UI_LEFT_PRESSED ||
		state == UI_RIGHT_PRESSED ||
		state == UI_MIDDLE_PRESSED)
		return 1;
	else
		return 0;
}

void ctx_ui_mouse_delta( t_ui *ui)
{
	if( mouse_pressed( ui->mouse_state))
	{
		if( ui->mouse_drag)
		{
			ui->mouse_delta_x = ui->mouse_x - ui->mouse_last_x_pressed;
			ui->mouse_delta_y = ui->mouse_y - ui->mouse_last_y_pressed;
		}
		else
		{
			ui->mouse_last_x_pressed = ui->mouse_x;
			ui->mouse_last_y_pressed = ui->mouse_y;
		}
	}
	else
	{
		ui->mouse_delta_x_last = ui->mouse_delta_x;
		ui->mouse_delta_y_last = ui->mouse_delta_y;
		ui->mouse_delta_x = 0;
		ui->mouse_delta_y = 0;
		ui->mouse_last_x_pressed = 0;
		ui->mouse_last_y_pressed = 0;
	}
}


void ctx_ui_mouse_set( t_context *C, t_event *event)
{
	int type = event->type;
	C->ui->mouse_motion = UI_MOUSE_STATIC;

	switch( type)
	{
		case MOUSE_LEFT_PRESSED: C->ui->mouse_state = UI_LEFT_PRESSED; break;
		case MOUSE_LEFT_RELEASED: C->ui->mouse_state = UI_LEFT_RELEASED; break;

		case MOUSE_RIGHT_PRESSED: C->ui->mouse_state = UI_RIGHT_PRESSED; break;
		case MOUSE_RIGHT_RELEASED: C->ui->mouse_state = UI_RIGHT_RELEASED; break;

		case MOUSE_MIDDLE_PRESSED: C->ui->mouse_state = UI_MIDDLE_PRESSED; break;
		case MOUSE_MIDDLE_RELEASED: C->ui->mouse_state = UI_MIDDLE_RELEASED; break;
	}

	if( type == MOUSE_MOTION)		C->ui->mouse_motion = UI_MOUSE_MOTION;
	else if( type == MOUSE_MOTION_PASSIVE)	C->ui->mouse_motion = UI_MOUSE_MOTION_PASSIVE;
	
	if( mouse_pressed( C->ui->mouse_state) && C->ui->mouse_motion == UI_MOUSE_MOTION) C->ui->mouse_drag = 1;
	else C->ui->mouse_drag = 0;

	ctx_ui_mouse_delta( C->ui);
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
