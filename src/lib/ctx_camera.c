/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

void ctx_camera_movment(t_context *C)
{
	t_app *app=C->app;
	t_camera *camera = C->camera;
	int dx = -app->mouse->sign_x * app->mouse->dx;
	int dy = app->mouse->sign_y * app->mouse->dy; 

	// When No UI event
	if(!C->event->is_brick_transformed)
	{
		// Start Camera Rotation 
		if(C->event->camera_rotation)
		{
			if(
				(app->mouse->button_right == button_released)
			&&	!(app->mouse->button_middle == button_pressed)
				
				)
				C->event->camera_rotation = 0;
		}
		// Release Camera Rotation
		else
		{
			if(
				(
				app->mouse->button_right == button_pressed
			&&	app->keyboard->shift
				)
				||
				app->mouse->button_middle == button_pressed
			)
				C->event->camera_rotation = 1;
		}

		// perspective
		if (camera->type == camera_frustum)
		{
			if(C->event->camera_rotation)
		
			{
				// camera rotate
				op_camera_rotate(C,(float)dx,(float)dy);
			}
			else if ((app->mouse->button_middle == button_pressed) && (app->keyboard->shift==1))	
			{
				// camera translate	
				op_camera_translate(C);
			}
			else if (app->mouse->button_left == button_pressed)	
			{
				if(app->keyboard->shift)
				{
					C->ui->pan_x+=(C->app->mouse->dx*2*C->app->mouse->sign_x);
					C->ui->pan_y+=(C->app->mouse->dy*2*C->app->mouse->sign_y);
				}
			}
			else if (app->mouse->button_right == button_pressed)
			{
			}


			if (app->mouse->wheel == wheel_up)
			{
				if(C->app->keyboard->shift)
					C->ui->zoom-=.1;
				else
					op_camera_zoom(C,-1);

				app->mouse->wheel=wheel_idle;
			}

			if (app->mouse->wheel == wheel_down)
			{
				if(C->app->keyboard->shift)
					C->ui->zoom+=.1;
				else
					op_camera_zoom(C,1);

				app->mouse->wheel=wheel_idle;
			}
		}
		// ortho
		else
		{
			if (app->mouse->wheel == wheel_up)
			{
				op_camera_set_ortho_zoom(C,-1);
				app->mouse->wheel=wheel_idle;
			}

			if (app->mouse->wheel == wheel_down)
			{
				op_camera_set_ortho_zoom(C,1);
				app->mouse->wheel=wheel_idle;
			}
			if (app->mouse->button_left == button_pressed && app->keyboard->shift)
			{
				op_camera_set_ortho_pan(C);
			}
		}
	}
}

void op_check_window(t_context *C)
{
	if(C->app->window->change)
	{
		op_camera_frustum_init(C->camera);
		C->app->window->change=0;
	}
}

void ctx_camera(t_context *C)
{
	C->camera->is_moving=0;
	op_check_window(C);
	op_camera_change_speed(C->camera);
	ctx_camera_movment(C);
}
	
