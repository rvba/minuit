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

void screen_bricks( t_screen *screen)
{
	t_context *C=ctx_get();
	t_camera *camera = C->ui->camera;

	op_camera_switch_2d(C,camera);

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

	op_camera_switch_3d(C, camera);
}
