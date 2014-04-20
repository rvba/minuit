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

void screen_sets(t_screen *screen)
{
	screen_switch_2d( screen);

	glPushMatrix();
	glLoadIdentity();

		ui_draw_mouse();
		t_context *C = ctx_get();
		glTranslatef(C->ui->pan_x,C->ui->pan_y,0);
		float zoom = C->ui->zoom;
		glScalef(zoom,zoom,zoom);

		ui_draw_sets();

	glPopMatrix();
}
