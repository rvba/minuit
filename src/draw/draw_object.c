/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "object.h"
#include "ctx.h"
#include "scene.h"
#include "draw.h"
#include "mesh.h"
#include "app.h"
#include "event.h"
#include "base.h"
#include "sketch.h"
#include "screen.h"
#include "block.h"

#include "vlst.h"
#include "ui.h"
#include "texture.h"

void cls_object_draw_light(t_object *object)
{
}

void cls_object_draw_camera(t_object *object)
{
}

void cls_object_screen_image( t_object *object)
{
	t_context *C = ctx_get();
	texture_update( object->texture, object->data);
	ui_texture_draw( C, object->texture , 0, 0, 1 ,1);
}

void cls_object_draw_image( t_object *object)
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
		//float black[3]={0,0,0};
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

void find_vertex(t_context *C, t_mesh *mesh)
{
	//int count = mesh->var.tot_vertex;
	//int i;
	int *color = C->event->color;
	//printf("color:");
	//vprint3i(color,'\n');
	int index = color[0] + (color[1] * 255) + (color[2] * 255);
	int b = color[0] + color[1] + color[2];
	//printf("b %d\n",b);
	if(b != 765)
	{
		//mesh->state.selected_vertex = index;
		mesh->state.hover_vertex = index;
		//printf("indice:%d od %d\n",index,count);
	}
	else
	{
		mesh->state.hover_vertex = -1;
	}
}

void object_draw_blocks( t_context *C,  t_object *object)
{
	if( object->blocks)
	{
		if( object->blocks->first)
		{
			t_link *link = object->blocks->first;
			t_block *block = link->data;
			block->cls->draw( block);
		}
	}
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
				mesh_update( mesh);
				// selected
				if(object->is_selected)
				{
					mesh->state.is_selected=1;
					if(object->is_edit_mode)
					{
						mesh->state.is_edit_mode = 1;
						find_vertex(C,mesh);
					}
					else
					{
						mesh->state.is_edit_mode = 0;
					}
				}
				else
				{
					mesh->state.is_selected = 0;
					mesh->state.is_edit_mode = 0;
				}

				// shader
				if( draw->with_shaders && object->shader) object->shader( object, OBJECT_SHADER_ON);

				// draw
				if(draw->mode_direct)
				{
					draw_mesh_direct(draw,scene,mesh);
				}
				else
				{
					draw_mesh(draw,scene,mesh);
				}

				// shader
				if( draw->with_shaders && object->shader) object->shader( object, OBJECT_SHADER_OFF);

			}

		glPopMatrix();
	}
}

void cls_object_draw_void( t_object *object)
{
	float x  = (float)(180 * object->rot[0] / PI );
	float y  = (float)(180 * object->rot[1] / PI );
	float z  = (float)(180 * object->rot[2] / PI );

	glPushMatrix();

		glTranslatef(object->loc[0],object->loc[1],object->loc[2]);
		glScalef(object->size[0],object->size[1],object->size[2]);
		glRotatef(z,0,0,1);
		glRotatef(y,0,1,0);
		glRotatef(x,1,0,0);

		if(object->draw) object->draw( object);

	glPopMatrix();
}



