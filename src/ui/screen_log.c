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

void screen_log( t_screen *screen)
{
	screen_switch_2d( screen);

	glPushMatrix();
	glLoadIdentity();

		ui_draw_term();

	glPopMatrix();
}
