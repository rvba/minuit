/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "base.h"
#include "ctx.h"
#include "scene.h"
#include "draw.h"
#include "sketch.h"
#include "mesh.h"
#include "material.h"
#include "vlst.h"

void draw_mesh_edges(t_draw *draw, t_mesh *mesh)
{
	t_vlst *edges = mesh->edges;
	t_vlst *points = mesh->vertex;

	if(edges)
	{
		if(draw->mode == mode_selection)
		{
		}
		else
		{
			int count = edges->count;
			int a,b;
			float *x;
			float *y;
			int *v = ( int *) edges->data;
			float *p = ( float *) points->data;
			int l = points->length;
			int i;
			int j=0;
			float *color;
			int width = 1;

			int limit = points->count * l;

			for(i=0; i < count; i++)
			{
				a = v[j];
				b = v[j+1];

				if((a >= limit || b >= limit) || (a < 0 || b < 0))
				{
					printf("[DRAW ERROR] EDGE IS OFF LIMIT\n");
				}
				else
				{
					x =  p + (a * l);
					y = p + (b * l);

					if(draw->with_edge_color && mesh->edges_color)
					{
						color = ( float *) grf_float( mesh->edges_color->data, i*3);
					}
					else
					{
						t_context *C = ctx_get();
						if( !C->draw->edge_use_front_color) color = C->draw->back_color;
						else color = C->draw->front_color;
					}

					//float color[] = {1,0,0,0,};

					skt_line(x,y,width,color);
				}

				j+=2;
			}

		}
	}
}

void draw_mesh_direct_quads(t_draw *draw, t_mesh *mesh)
{
	t_vlst *vertex=mesh->vertex;
	t_vlst *quads=mesh->quads;

	if(vertex)
	{
		GLfloat *v = ( GLfloat *) vertex->data;	// vertices 
		int i,j,n;
		// quads
		if(quads)
		{
			int *q = ( int *) quads->data;		// quad indices

			float outline[4*3];

			j=0;
			for(i=0;i<quads->count;i++)
			{
				glBegin(GL_QUADS);
				for(n=0;n<4;n++)
				{
					// normal
					if(draw->with_normal)
					{
						t_vlst *vlst = mesh->quad_normal;
						if(vlst)
						{
							float *normals = ( float *) vlst->data;
							glNormal3f(normals[(i*3)+0],normals[(i*3)+1],normals[(i*3)+2]);
						}
					}

					// selection
					if(draw->mode == mode_selection)
					{
						float c[3];
						c[0] = (float)mesh->idcol[0]/255;
						c[1] = (float)mesh->idcol[1]/255;
						c[2] = (float)mesh->idcol[2]/255;
						glColor3f(c[0],c[1],c[2]);
					}

					// vertex
					if(draw->with_face)
						glVertex3f(v[(q[j]*3)],v[(q[j]*3)+1],v[(q[j]*3)+2]);

					outline[(n*3)+0] = v[(q[j]*3)+0]; 
					outline[(n*3)+1] = v[(q[j]*3)+1]; 
					outline[(n*3)+2] = v[(q[j]*3)+2]; 

					j++;
				}
				glEnd();

				if(draw->with_face_outline)
				{
					glDisable(GL_LIGHTING);
					if(draw->with_face)
						skt_closedline(outline,4,draw->back_color,1);
					else
						skt_closedline(outline,4,draw->front_color,1);
					glEnable(GL_LIGHTING);
				}
			}
		}

	}
}

void draw_mesh_direct_tris(t_draw *draw, t_mesh *mesh)
{
	t_vlst *vertex=mesh->vertex;
	t_vlst *tris=mesh->tris;

	if(vertex)
	{
		GLfloat *v = ( GLfloat *) vertex->data;	// vertices 
		int i,j,n;
		// quads
		if(tris)
		{
			int *q = ( int *) tris->data;		// quad indices

			float outline[4*3];

			j=0;
			for(i=0;i<tris->count;i++)
			{
				glBegin(GL_TRIANGLES);
				for(n=0;n<3;n++)
				{
					// normal
					if(draw->with_normal)
					{
						t_vlst *vlst = mesh->tri_normal;
						if( vlst)
						{
							float *normals = ( float *) vlst->data;
							glNormal3f(normals[(i*3)+0],normals[(i*3)+1],normals[(i*3)+2]);
						}
					}

					// selection
					if(draw->mode == mode_selection)
					{
						float c[3];
						c[0] = (float)mesh->idcol[0]/255;
						c[1] = (float)mesh->idcol[1]/255;
						c[2] = (float)mesh->idcol[2]/255;
						glColor3f(c[0],c[1],c[2]);
					}

					// vertex
					if(draw->with_face)
						glVertex3f(v[(q[j]*3)],v[(q[j]*3)+1],v[(q[j]*3)+2]);

					outline[(n*3)+0] = v[(q[j]*3)+0]; 
					outline[(n*3)+1] = v[(q[j]*3)+1]; 
					outline[(n*3)+2] = v[(q[j]*3)+2]; 

					j++;
				}
				glEnd();

				if(draw->with_face_outline)
				{
					glDisable(GL_LIGHTING);
					if(draw->with_face)
						skt_closedline(outline,3,draw->back_color,1);
					else
						skt_closedline(outline,3,draw->front_color,1);
					glEnable(GL_LIGHTING);
				}
			}
		}

	}
}

void draw_mesh_direct_faces(t_draw *draw, t_mesh *mesh)
{
	draw_mesh_direct_quads( draw, mesh);
	draw_mesh_direct_tris( draw, mesh);
}

void draw_mesh_direct_selection(t_draw *draw, t_mesh *mesh)
{
	float *color=draw->front_color;
	if(mesh->state.has_face)
	{
		glDisable(GL_LIGHTING);
		glPushAttrib( GL_ALL_ATTRIB_BITS );
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( -2.5f, -2.5f );
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glLineWidth( 1.0f );
		glColor3f(color[0],color[1],color[2]);

		if(mesh->state.has_quad) draw_mesh_direct_faces(draw, mesh); 
		if(mesh->state.has_tri) draw_mesh_direct_tris(draw, mesh); 

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glColor3f( 0.0f, 0.0f, 0.0f );

		if(mesh->state.has_quad) draw_mesh_direct_faces(draw, mesh); 
		if(mesh->state.has_tri) draw_mesh_direct_tris(draw, mesh); 

		glDisable( GL_POLYGON_OFFSET_FILL );
		glEnable( GL_LIGHTING );
		glPopAttrib();

	}
}

void draw_mesh_direct_selection_stencil(t_draw *draw, t_mesh *mesh)
{
	t_context *C = ctx_get();
	float *color=draw->front_color;
	if(mesh->state.has_face)
	{
		C->draw->with_normal = 0;
		glPushAttrib( GL_ALL_ATTRIB_BITS );
		glDisable(GL_LIGHTING);
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFFFF);
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE);

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(0,0,0);

		if(mesh->state.has_quad) draw_mesh_direct_faces(draw, mesh); 
		if(mesh->state.has_tri) draw_mesh_direct_tris(draw, mesh); 

		glDisable( GL_LIGHTING);

		glStencilFunc( GL_NOTEQUAL, 1, 0xFFFF);
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE);
		glLineWidth( 2);
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		//glColor3f(1,1,1);
		glColor3f(color[0], color[1], color[2]);

		if(mesh->state.has_quad) draw_mesh_direct_faces(draw, mesh); 

		glPopAttrib();

		C->draw->with_normal = 1;

	}
}

void draw_mesh_direct(t_draw *draw,t_scene *scene,t_mesh *mesh)
{
	// Material
	if (draw->with_material && draw->mode != mode_selection)
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

	// Points
	if(draw->with_point)
	{
		draw_mesh_points(draw,mesh);
	}

	// Edges
	if(draw->with_edge)
	{
		draw_mesh_edges(draw,mesh);
	}

	glShadeModel(GL_FLAT);

	// Light
	if (draw->with_light && draw->mode != mode_selection)
	{
		glEnable(GL_LIGHTING);
		float f = 1.5;
		GLfloat model_ambient[] = {f,f,f,1};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,model_ambient);
	}

	// Faces
	draw_mesh_direct_faces(draw, mesh);

	// Outline
	if(
		mesh->state.is_selected
		&& draw->mode == mode_draw
		&& draw->with_face)
	{
		draw_mesh_direct_selection_stencil(draw, mesh);

	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
}


