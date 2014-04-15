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

#include "viewport.h"

void screen_main(t_screen *screen)
{
	t_context *C = ctx_get();
	t_viewport *viewport = screen_viewport_get( screen);

	if( viewport)
	{
		t_camera *camera = viewport->camera;
		op_camera_update( C, camera);

		glPushMatrix();
			glLoadIdentity();

			op_camera_switch_2d(C,camera);

			ui_draw_mouse();
			ui_draw_grid();
			ui_draw_menu();
			ui_draw_bar();

		glPopMatrix();
	}
}
