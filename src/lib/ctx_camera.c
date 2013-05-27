/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "app.h"
#include "event.h"

void ctx_camera_movment(t_context *C, t_camera *camera)
{
	t_app *app=C->app;
	int dx = -app->mouse->sign_x * app->mouse->dx;
	int dy = app->mouse->sign_y * app->mouse->dy; 

	// When No UI event
	if(!C->event->is_brick_transformed)
	{
		// Release Camera Rotation
		if(C->event->camera_rotation)
		{
			if(
				(app->mouse->button_right == button_released)
			&&	!(app->mouse->button_middle == button_pressed)
				
				)
				C->event->camera_rotation = 0;
		}
		// Start Camera Rotation 
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
				op_camera_rotate(C, camera, (float)dx,(float)dy);
			}
			else if ((app->mouse->button_middle == button_pressed) && (app->keyboard->shift==1))	
			{
				// camera translate	
				op_camera_translate(C, camera);
			}

			if (app->mouse->wheel == wheel_up)
			{
				if(C->app->keyboard->shift)
					C->ui->zoom-=.1;
				else
					op_camera_zoom(C, camera, -1);

				// XXX
				app->mouse->wheel=wheel_idle;
			}

			if (app->mouse->wheel == wheel_down)
			{
				if(C->app->keyboard->shift)
					C->ui->zoom+=.1;
				else
					op_camera_zoom(C, camera,1);

				// XXX
				app->mouse->wheel=wheel_idle;
			}
		}
		// ortho
		else
		{
			if (app->mouse->wheel == wheel_up)
			{
				op_camera_set_ortho_zoom(C, camera,-1);
				app->mouse->wheel=wheel_idle;
			}

			if (app->mouse->wheel == wheel_down)
			{
				op_camera_set_ortho_zoom(C,camera,1);
				app->mouse->wheel=wheel_idle;
			}
			if (app->mouse->button_left == button_pressed && app->keyboard->shift)
			{
				op_camera_set_ortho_pan(C, camera);
			}
		}
	}
}

void op_check_window(t_context *C, t_camera *camera)
{
	op_camera_frustum_init(camera);
	C->app->window->change=0;
}

void ctx_camera(t_context *C)
{
	t_link *link;
	t_node *node;
	t_camera *camera;

	for(link = C->scene->cameras->first; link; link = link->next)
	{
		node = link->data;
		camera = node->data;

		camera->is_moving = 0;

		op_check_window(C, camera);
		op_camera_change_speed(camera);
		ctx_camera_movment(C, camera);
	}
}
	
