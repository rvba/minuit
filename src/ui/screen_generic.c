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

void screen_generic(t_screen *screen)
{
	t_context *C=ctx_get();
	t_camera *camera = C->ui->camera;
	op_camera_switch_2d(C,camera);

	glPushMatrix();
		glLoadIdentity();
		ui_draw_menu();
	glPopMatrix();

	glPushMatrix();
		float scale=4;
		float p[3]={(C->app->window->width)/4,(C->app->window->height)/2,0};
		float color[]={0,0,0,0};
		glTranslatef(p[0],p[1],p[2]);
		glScalef(scale,scale,scale);
		txt_draw_direct_2d(screen->id.name, p, color, scale);
	glPopMatrix();

	op_camera_switch_3d(C, camera);
}
