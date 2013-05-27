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

void cls_object_draw_light(t_object *object)
{
}

void cls_object_draw_camera(t_object *object)
{
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



