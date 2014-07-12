/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "util.h"
#include "draw.h"
#include "node.h"
#include "light.h"
#include "sketch.h"

GLenum LightList[] = {
	GL_LIGHT0,
	GL_LIGHT1,
	GL_LIGHT2,
	GL_LIGHT3,
};


void draw_add_light(t_node *node)
{
	t_light *light = ( t_light *) node->data;

	float *p=light->pos;

	float pos[4];
	pos[0]=p[0];
	pos[1]=p[1];
	pos[2]=p[2];
	pos[3]=1;

	GLenum id = LightList[light->id_gl];

	glLightfv(id, GL_POSITION,pos);
	glLightfv(id, GL_DIFFUSE, light->diffuse);
	glLightfv(id, GL_SPECULAR, light->specular);

	glEnable(id);
}

void draw_light(t_draw *draw, t_node *node)
{
	draw_add_light( node);

	if(draw->draw_lights)
	{
		t_light *light = ( t_light *) node->data;
		float *pos=light->pos;

		if(draw->mode==mode_selection)
		{	
			int *col=light->idcol;
			float c[3];
			cconv(c,col);

			skt_point(pos,20,c);
		}
		else
		{
			float color[]={0,0,0};
			int width=5;
			t_skt *skt = skt_get();
			int s = skt->point_smooth;
			skt->point_smooth = 1;

			skt_point(pos,width,color);

			skt->point_smooth = s;
		}
	}
}
