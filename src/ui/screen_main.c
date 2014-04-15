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
	screen_switch_2d( screen);

	glPushMatrix();
	glLoadIdentity();

		ui_draw_mouse();
		ui_draw_grid();
		ui_draw_menu();
		ui_draw_bar();

	glPopMatrix();
}
