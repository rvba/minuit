/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "screen.h"
#include "context.h"
#include "opengl.h"
#include "ui.h"
#include "block.h"
#include "util.h"
#include "event.h"

void screen_bricks( t_screen *screen)
{
	screen_switch_2d( screen);
	t_context *C = ctx_get();

	glPushMatrix();
	glLoadIdentity();

		glTranslatef(C->ui->pan_x,C->ui->pan_y,0);
		float zoom = C->ui->zoom;
		glScalef(zoom,zoom,zoom);

		glPushMatrix();
		glLoadIdentity();

			C->event->ui.use_scale = 0;

			t_link *link;
			t_block *block;

			for(link = screen->blocks->first; link; link = link->next)
			{
				block = link->data;
				block->cls->draw(block);
				glTranslatef(0,block->height,0);
			}

			C->event->ui.use_scale = 1;

		glPopMatrix();

	glPopMatrix();
}
