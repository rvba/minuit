/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "context.h"
#include "scene.h"
#include "node.h"
#include "app.h"
#include "event.h"
#include "camera.h"
#include "ui.h"
#include "list.h"
#include "util.h"

void ctx_camera_movment(t_context *C, t_camera *camera)
{
	t_app *app=C->app;

	float dx = (float) -C->ui->mouse_dx * .1;
	float dy = (float) C->ui->mouse_dy * .1;

	// When No UI event
	if(!C->event->is_brick_transformed && !C->event->is_mouse_over_brick)
	{
		// Release Camera Rotation
		if(C->event->camera_rotation)
		{
			if( C->ui->mouse_state == UI_MIDDLE_RELEASED) C->event->camera_rotation = 0;
		}
		// Start Camera Rotation 
		else
		{
			if(
				(
				C->ui->mouse_state == UI_RIGHT_PRESSED
			&&	app->keyboard->shift
				)
				||
				C->ui->mouse_state == UI_MIDDLE_PRESSED
			)
			{
				C->event->camera_rotation = 1;
			}
		}

		// perspective
		if (camera->type == camera_frustum)
		{
			if(C->event->camera_rotation)
			{
				if( C->ui->mouse_motion == UI_MOUSE_MOTION)
				// camera rotate
				op_camera_rotate(C, camera, (float)dx,(float)dy);
			}
			else if (( C->ui->mouse_state == UI_MIDDLE_PRESSED) && (app->keyboard->shift==1))	
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
			}

			if (app->mouse->wheel == wheel_down)
			{
				if(C->app->keyboard->shift)
					C->ui->zoom+=.1;
				else
					op_camera_zoom(C, camera,1);
			}
		}
		// ortho
		else
		{
			if (app->mouse->wheel == wheel_up)
			{
				op_camera_set_ortho_zoom(C, camera,-1);
			}

			if (app->mouse->wheel == wheel_down)
			{
				op_camera_set_ortho_zoom(C,camera,1);
			}
			if ( C->ui->mouse_state == UI_LEFT_PRESSED && app->keyboard->shift)
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

	// Reset 
	C->app->mouse->wheel=wheel_idle;
}
	
