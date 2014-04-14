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
#include "context.h"
#include "opengl.h"
#include "ui.h"
#include "event.h"
#include "util.h"
#include "block.h"

#include "app.h"
#include "txt.h"
#include "viewport.h"

void screen_view3d(t_screen *screen)
{
	t_context *C=ctx_get();
	t_viewport *viewport = screen_viewport_get( screen);
	if( viewport)
	{
		t_camera *camera = viewport->camera;
		op_camera_switch_3d(C, camera);
		
		int w = C->app->window->width;
		int h = C->app->window->height;

		if( C->app->window->change)
		{
			op_camera_frustum_init( camera, w, h);
			C->app->window->change = 0;
		}

		if( opt_viewport_show_controls)
		{
			viewport_draw_controls( viewport);
		}

		if( viewport->fullscreen) op_camera_update(C, camera);
		else  _op_camera_update(C, camera, viewport);
		
		op_3d_orientation(); 

		draw_scene(C->draw,C->scene);
	}
}
