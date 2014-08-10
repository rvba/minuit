/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include "vlst.h"
#include "draw.h"
#include "sketch.h"
#include "mesh.h"


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
