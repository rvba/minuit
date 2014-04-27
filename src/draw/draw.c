/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "draw.h"
#include "sketch.h"
#include "mesh.h"
#include "scene.h"
#include "light.h"
#include "list.h"
#include "vlst.h"
#include "object.h"

#include "ctx.h"
#include "op.h"
#include "camera.h"
#include "event.h"

#include "sketch.h"
#include "ui.h"
#include "memory.h"
#include "node.h"
#include "material.h"
#include "texture.h"

t_draw *DRAW;

GLenum LightList[] = {
	GL_LIGHT0,
	GL_LIGHT1,
	GL_LIGHT2,
	GL_LIGHT3,
};

void draw_switch_scene( t_draw *draw)
{
	if( draw->with_scene) draw->with_scene = 0;
	else draw->with_scene = 1;
}

void draw_switch_axis_world(t_draw *draw)
{
	if (draw->show_axis_world) draw->show_axis_world=0;
	else draw->show_axis_world=1;
}

void draw_clear(float r,float g,float b,float a)
{
	glClearColor(r,g,b,a);
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0,0,0,0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
}

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
			skt_point(pos,width,color);
		}
	}
}

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

void draw_get_mesh_color(t_mesh *mesh, float *c)
{
	c[0] = (float)mesh->idcol[0]/255;
	c[1] = (float)mesh->idcol[1]/255;
	c[2] = (float)mesh->idcol[2]/255;
}

void build_color_index(float *c,int count)
{
	int i;
	int j=0;
	int cc[3]={0,0,0};

	for(i=0;i<count;i++)
	{
		float ccc[3];
		cconv(ccc,cc);

		c[j+0] = ccc[0];
		c[j+1] = ccc[1];
		c[j+2] = ccc[2];

		j+=3;
		color_id(cc);
	}
}

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
						color = C->draw->front_color;
					}

					skt_line(x,y,width,color);
				}

				j+=2;
			}

		}
	}
}

void draw_mesh_points(t_draw *draw, t_mesh *mesh)
{
	t_vlst *vlst_vertex = mesh->vertex;

	if( vlst_vertex)
	{
		t_vlst *vlst_colors = mesh->colors;
		float *vertex = ( float *) vlst_vertex->data;
		float *colors = NULL;
		float *color = NULL;
		float selection_color[3];
		float selected_color[3]={1,0,0};
		float yellow[3] = {1,1,0};
		float green[3] = {0,1,0};
		float red[3] = {1,0,0};

		int count = mesh->var.tot_vertex;
		if(!count) count = 1;
		float  ci[count*3];

		if(draw->mode == mode_selection)
		{
			if(mesh->state.is_edit_mode)
			{
				build_color_index(ci,count);
				colors = ci;
			}
			else
			{
				draw_get_mesh_color(mesh,selection_color);
				color = selection_color;
			}
		}
		else
		{
			if(mesh->state.is_edit_mode)
			{
				color = yellow;
			}
			else if(mesh->state.is_selected)
			{
				color = selected_color;
			}
			else if(vlst_colors)
			{
				colors = ( float *) vlst_colors->data; 
			}
			else
			{
				color = ( float *) draw->front_color;
			}
		}
		
		if(color || colors)
		{
			draw_points(draw,mesh->var.tot_vertex,vertex,colors,color);
		}

		if(mesh->state.is_edit_mode && (mesh->state.selected_vertex >= 0))
		{
			int indice = mesh->state.selected_vertex;
			if( indice >= 0 && indice < count)
			{
				t_vlst *vlst = mesh->vertex;
				float *v = ( float *) vlst->data;
				int i = mesh->state.selected_vertex * 3;
				skt_point(v+i,3,red);
			}
		}

		if(mesh->state.is_edit_mode && (mesh->state.hover_vertex >= 0))
		{
			int indice = mesh->state.hover_vertex;
			if( indice >= 0 && indice < count)
			{
				t_vlst *vlst = mesh->vertex;
				float *v = ( float *) vlst->data;
				int i = mesh->state.hover_vertex * 3;
				skt_point(v+i,3,green);
			}
		}
	}
}

void draw_mesh_direct_faces(t_draw *draw, t_mesh *mesh)
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
			t_vlst *vlst_quad = mesh->quad_normal;
			float *normals = ( float *) vlst_quad->data;

			float outline[4*3];

			j=0;
			for(i=0;i<quads->count;i++)
			{
				glBegin(GL_QUADS);
				for(n=0;n<4;n++)
				{
					// normal
					if(draw->with_normal)
						glNormal3f(normals[(i*3)+0],normals[(i*3)+1],normals[(i*3)+2]);

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

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glColor3f( 0.0f, 0.0f, 0.0f );

		if(mesh->state.has_quad) draw_mesh_direct_faces(draw, mesh); 

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
		GLfloat model_ambient[] = {1,1,1,1};
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

void draw_mesh(t_draw *draw, t_scene *scene, t_mesh *mesh)
{
	t_node *node;
	t_texture *texture=NULL;

	// BUFFERS
	if(mesh->state.buffer_type!=buffer_vbo) mesh_init_buffers(mesh,buffer_vbo); 

	float *color=draw->front_color;

	if(draw->mode==mode_selection) glEnableClientState(GL_COLOR_ARRAY);
	if(draw->with_normal) glEnableClientState(GL_NORMAL_ARRAY);

	// TEXTURE
	if(draw->with_texture && mesh->state.with_texture)
	{
		node = scene_node_get( scene, dt_texture, mesh->texture_name);
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
	//else if(draw->with_face && mesh->state.has_face)
	else if(draw->with_face)
	{
		if (draw->with_light)
		{
			glEnable(GL_LIGHTING);
			GLfloat model_ambient[] = {1,1,1,1};
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT,model_ambient);
		}

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

	// lines
	//if(mesh->state.with_line) draw_mesh_line(draw,mesh);

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

void draw_lights(t_draw *draw, t_scene *scene)
{
	t_node *node;
	t_link *link;

	// draw lights nodes
	t_lst *lst = scene_lst_get( scene, dt_light);
	if( lst)
	{
	for(link=lst->first;link;link=link->next)
	{
		node = ( t_node *) link->data;
		draw_light(draw,node);
	}

	for(link=lst->first;link;link=link->next)
	{
		node = ( t_node *) link->data;
		draw_add_light(node); 
	}
	}
}

void type_font_3d(char* string,float pos[3])
{
	glRasterPos3f(pos[0],pos[1],pos[2]);

	int n = (int)strlen(string);
	void * font = GLUT_BITMAP_8_BY_13;
	int i;
	for (i=0;i<n;i++)
	{
    		glutBitmapCharacter(font, string[i]);
	}
}

void draw_objects(t_draw *draw, t_scene *scene)
{
	t_link *link;
	t_node *node;

	t_lst *lst = scene_lst_get( scene, dt_object);
	link=lst->first;

	for(link=lst->first;link;link=link->next)
	{
		node = ( t_node *) link->data;
		t_object *object = ( t_object *) node->data;

		if(!object)
		{
			printf("[ERROR draw_objects] No data\n");
			break;
		}
		else
		{
			//printf("ob:%s\n",ob->name);
			object->cls->draw(object);
		}
	}
}


/*axis*/

void draw_axis_world(t_draw *draw)
{
	if (draw->show_axis_world) 
	{
		float v0[] = {0,0,0};
		float vx[] = {1,0,0};
		float vy[] = {0,1,0};
		float vz[] = {0,0,1};
		float red[]={1,0,0};
		float green[]={0,1,0};
		float blue[]={0,0,1};

		glDisable(GL_DEPTH_TEST);

		skt_vector(v0,vx,1,red);
		skt_vector(v0,vy,1,green);
		skt_vector(v0,vz,1,blue);

		glEnable(GL_DEPTH_TEST);
	}
}

void draw_init(t_draw *draw)
{
	float c[4];
	bzero(c,4);
	cconv(c,draw->background_color);

	// clear
	if(draw->with_clear)
	{
		//draw_clear(c[0],c[1],c[2],c[3]);
		draw_clear(c[0],c[1],c[2],0);
	}

	// set depth
	if(draw->with_depth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	
	// polygon offset
	if(draw->with_polygon_offset)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0,1.0);
	}
	else
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	if(draw->with_blend)
	{
		glEnable(GL_BLEND);
    		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	t_context *C = ctx_get();
	C->event->ui.use_line_global_width = 1;
	skt_update( C);

}

void draw_scene(t_draw *draw, t_scene *scene)
{
	if(scene->edit_mode) draw->edit_mode = 1;
	else draw->edit_mode = 0;

	draw_lights(draw,scene);
	draw_objects(draw,scene);
	draw_axis_world(draw);
}

t_draw *draw_new(void)
{
	t_draw *draw = (t_draw *)mem_malloc(sizeof(t_draw));

	DRAW=draw;
	
	draw->background_color[0]=DRAW_COLOR_BACKGROUND_RED;
	draw->background_color[1]=DRAW_COLOR_BACKGROUND_GREEN;
	draw->background_color[2]=DRAW_COLOR_BACKGROUND_BLUE;
	draw->background_color[3]=DRAW_COLOR_BACKGROUND_ALPHA;

	vset4f(draw->front_color,0,0,0,0);
	vset4f(draw->back_color,1,1,1,0);

	draw->mode=mode_draw;
	draw->color=DRAW_COLOR;

	draw->with_selection_pass=DRAW_SELECTION_PASS; 
	draw->with_draw_pass=DRAW_WITH_DRAW;

	draw->with_clear=DRAW_WITH_CLEAR;
	draw->with_ui=DRAW_WITH_UI;

	draw->with_restrict_matrix=0;

	draw->mode_direct=1;

	draw->show_axis_world=DRAW_SHOW_AXIS_WORLD;
	draw->with_normal=DRAW_WITH_NORMAL;
	draw->with_texture=DRAW_WITH_TEXTURE;
	draw->with_material=DRAW_WITH_MATERIAL;
	draw->with_point=DRAW_WITH_POINT;
	draw->with_point_id=DRAW_WITH_POINT_ID;
	draw->with_face=DRAW_WITH_FACE;
	draw->with_face_outline=DRAW_WITH_FACE_OUTLINE;
	draw->with_edge = DRAW_WITH_EDGE;
	draw->with_edge_color = DRAW_WITH_EDGE_COLOR;
	draw->with_highlight=DRAW_WITH_HIGHLIGHT;
	draw->with_light=DRAW_WITH_LIGHT;
	draw->with_depth=DRAW_WITH_DEPTH;
	draw->with_polygon_offset=DRAW_WITH_POLYGON_OFFSET;
	draw->with_alpha=DRAW_WITH_ALPHA;
	draw->with_blend=DRAW_WITH_BLEND;
	draw->with_scene = 1;
	draw->with_grid=0;

	draw->with_shaders = 1;

	draw->draw_lights = 1;
	draw->edit_mode = 0;

	return draw;
}



