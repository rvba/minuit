/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "scene.h"
#include "node.h"
#include "vlst.h"
#include "mesh.h"
#include "draw.h"
#include "texture.h"
#include "material.h"

void draw_mesh(t_draw *draw, t_scene *scene, t_mesh *mesh)
{
	t_node *node;
	t_texture *texture=NULL;

	// BUFFERS
	if(mesh->state.buffer_type!=buffer_vbo) mesh_init_buffers(mesh,buffer_vbo); 

	float *color=draw->front_color;


	// TEXTURE
	if(draw->with_texture && mesh->state.with_texture)
	{
		node = scene_get_node_by_type_name( scene, dt_texture, mesh->texture_name);
		texture = ( t_texture *) node->data;

		if(texture && mesh->quad_uv)
		{
			glEnable(GL_TEXTURE_2D);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer( 2, GL_INT, 0, mesh->quad_uv->data);
			glBindTexture(GL_TEXTURE_2D,texture->id_gl); 

		}
	}

	// MATERIAL
	if (draw->with_material)
	{
		t_material *material = mesh->material;

		if(material)
		{
			float color[4];

			color[0]=material->color[0];
			color[1]=material->color[1]; 
			color[2]=material->color[2];
			color[3]=material->color[3];

			glMaterialfv(GL_FRONT,GL_SPECULAR, mesh->material->specular);
			glMaterialfv(GL_FRONT,GL_SHININESS, mesh->material->shininess);
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, color);
		}
	}


	// vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);

	glShadeModel(GL_FLAT);

	// draw node

	// SELECTION MODE for drawing wire cube to solid cube
	if(draw->mode==mode_selection)
	{
		if (draw->with_light)
		{
			glEnable(GL_LIGHTING);
			GLfloat model_ambient[] = {1,1,1,1};
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT,model_ambient);
		}
		// quads 
		//if (mesh->quad_faces && mesh->state.has_quad)
		if (mesh->state.has_quad)
		{
			glVertexPointer(3,GL_FLOAT,0,(float *)mesh->quad_vertex->data);
			if(mesh->quad_normal && draw->with_normal) glNormalPointer(GL_FLOAT,0,mesh->quad_normal->data);
			if(mesh->quad_color && draw->mode==mode_selection) glColorPointer(3,GL_FLOAT,0,mesh->quad_color->data);
			if(mesh->quad_face) glDrawElements(GL_QUADS,mesh->var.tot_quad_face*4,GL_UNSIGNED_INT,mesh->quad_face->data); 
		}

		// triangles
		if(mesh->state.has_tri)
		{
			glVertexPointer(3,GL_FLOAT,0,(float *)mesh->tri_vertex->data);
			if(mesh->tri_normal && draw->with_normal) glNormalPointer(GL_FLOAT,0,mesh->tri_normal->data);
			if(mesh->tri_color && draw->mode==mode_selection) glColorPointer(3,GL_FLOAT,0,mesh->tri_color->data);
			if(mesh->tri_face) glDrawElements(GL_TRIANGLES,mesh->var.tot_tri_face*3,GL_UNSIGNED_INT,mesh->tri_face->data); 

		}
	}
	// DRAW MODE draw faces
	else if(draw->with_face && mesh->state.has_face)
	{
		if (draw->with_light)
		{
			glEnable(GL_LIGHTING);
			GLfloat model_ambient[] = {1,1,1,1};
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT,model_ambient);
		}
		
		if(draw->mode==mode_selection) glEnableClientState(GL_COLOR_ARRAY);
		if(draw->with_normal) glEnableClientState(GL_NORMAL_ARRAY);

		// quads 

		if (mesh->state.has_quad)
		{
			glVertexPointer(3,GL_FLOAT,0,(float *)mesh->quad_vertex->data);
			if(mesh->quad_normal && draw->with_normal) glNormalPointer(GL_FLOAT,0,mesh->quad_normal->data);
			if(mesh->quad_color && draw->mode==mode_selection) glColorPointer(3,GL_FLOAT,0,mesh->quad_color->data);
			if(mesh->quad_face) glDrawElements(GL_QUADS,mesh->var.tot_quad_face*4,GL_UNSIGNED_INT,mesh->quad_face->data); 
		}


		// triangles

		if(mesh->state.has_tri)
		{
			if (mesh->tri_vertex)
			{
				glVertexPointer(3,GL_FLOAT,0,mesh->tri_vertex->data);
				if(mesh->tri_normal && draw->with_normal) glNormalPointer(GL_FLOAT,0,mesh->tri_normal->data);
				if(mesh->tri_color && draw->mode==mode_selection) glColorPointer(3,GL_FLOAT,0,mesh->tri_color->data);//XXX
				if(mesh->tri_face) glDrawElements(GL_TRIANGLES,mesh->var.tot_tri_face*3,GL_UNSIGNED_INT,mesh->tri_face->data);
			}
		}
	}

	// edges
	if(mesh->state.with_line) draw_mesh_edge( draw, mesh);

	// outline
	if(
		mesh->state.is_selected
		&& draw->mode == mode_draw
		&& draw->with_face)
	{
		if(mesh->state.has_face)
		{
			glDisable(GL_LIGHTING);
			glPushAttrib( GL_ALL_ATTRIB_BITS );
			glEnable( GL_POLYGON_OFFSET_FILL );
			glPolygonOffset( -2.5f, -2.5f );
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			glLineWidth( 4.0f );
			glColor3f(color[0],color[1],color[2]);

			if(mesh->state.has_quad) glDrawElements(GL_QUADS,mesh->var.tot_quad_face*4,GL_UNSIGNED_INT,mesh->quad_face->data); 

			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			glEnable( GL_LIGHTING );
			glColor3f( 0.0f, 0.0f, 0.0f );

			if(mesh->state.has_quad) glDrawElements(GL_QUADS,mesh->var.tot_quad_face*4,GL_UNSIGNED_INT,mesh->quad_face->data); 

			glPopAttrib();

		}
	}


	// unbind texture
	if(texture) glBindTexture(GL_TEXTURE_2D,0); 

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	// Points
	if(draw->with_point)
	{
		draw_mesh_points(draw,mesh);
	}
}
