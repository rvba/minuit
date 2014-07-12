/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "ctx.h"
#include "opengl.h"
#include "draw.h"
#include "sketch.h"
#include "ui.h"
#include "event.h"
#include "mesh.h"

void draw_points(t_draw *draw, int count, float *points, float *colors, float *color)
{
	t_context *C = ctx_get();

	if(colors) glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	float *c = C->ui->front_color;

	if(C->skt->point_smooth) glEnable(GL_POINT_SMOOTH);
	else glDisable(GL_POINT_SMOOTH);

	float scale;
	if(C->event->ui.use_point_global_width)
		scale = C->skt->scale * C->skt->point_size;
	else
		scale = 1;

	int width = 1;

	glPointSize(width * scale);
	glVertexPointer(3,GL_FLOAT,0,points);

	if(draw->mode == mode_selection)
	{
		scale*=10;
	}

	if(colors)
	{
		glColorPointer(3,GL_FLOAT,0,colors);
	}
	else if(color)
	{
		glColor3f(color[0],color[1],color[2]);
	}
	else
	{
		glColor3f(c[0],c[1],c[2]);
	}



	glDrawArrays(GL_POINTS,0,count);

	if(colors) glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}
