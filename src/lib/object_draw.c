/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "object.h"
#include "context.h"
#include "scene.h"
#include "draw.h"
#include "mesh.h"
#include "app.h"

void cls_object_draw_light(t_object *object)
{
}

void cls_object_draw_camera(t_object *object)
{
}

void cls_object_draw_point(t_object *object)
{
	t_context *C=ctx_get();
	t_draw *draw=C->draw;
	glPushMatrix();

	glTranslatef(object->loc[0],object->loc[1],object->loc[2]);
	glScalef(object->size[0],object->size[1],object->size[2]);

	int size;
	float p[3]={0,0,0};
	float black[3]={0,0,0};
	float white[3]={1,1,1};
	float red[3]={1,0,0};
	float green[3]={.2,1,.1};
	float cc[3];
	float *color;


	if(draw->mode==mode_selection)
	{
		size=10;
		cconv(cc,object->idcol);
		color=cc;
		//vprint3i(object->idcol,'\n');
	}
	else
	{
		if(object->is_selected)
		{
			color=green;
			size=20;
			object->loc[0]+=C->app->mouse->delta_x*.1;
		}

		else if(object->hover)
		{
			color=red;
			size=10;
		}
		else
		{
			color=white;
			size=1;
		}
	}

	skt_point(p,size,color);

	glPopMatrix();
}

void cls_object_draw_mesh(t_object *object)
{
	t_context *C=ctx_get();
	t_draw *draw=C->draw;
	t_scene *scene=C->scene;

	if(object->mesh)
	{
		t_mesh *mesh=object->mesh;

		float x  = (float)(180 * object->rot[0] / PI );
		float y  = (float)(180 * object->rot[1] / PI );
		float z  = (float)(180 * object->rot[2] / PI );
	
		glPushMatrix();

		glTranslatef(object->loc[0],object->loc[1],object->loc[2]);
		glScalef(object->size[0],object->size[1],object->size[2]);
		glRotatef(z,0,0,1);
		glRotatef(y,0,1,0);
		glRotatef(x,1,0,0);

		if(mesh)
		{
			// selected
			if(object->is_selected)
			{
				mesh->state.is_selected=1;
			}
			else
			{
				mesh->state.is_selected=0;
			}

			// draw
			if(draw->mode_direct)
			{
				draw_mesh_direct(draw,scene,mesh);
			}
			else
			{
				draw_mesh(draw,scene,mesh);
			}

		}

		glPopMatrix();
	}
}



