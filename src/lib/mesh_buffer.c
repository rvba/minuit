/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "util.h"
#include "mesh.h"
#include "list.h"
#include "context.h"
#include "block.h"
#include "ctx.h"

// ADD GEOMETRY

void mesh_line_add(t_mesh *mesh,int *lines,int totline)
{
	int i;
	size_t size = sizeof(int)*totline;
	int *_lines=(int *)malloc(size);

	mesh->lines = _lines;
	mesh->var.totline=totline;

	for (i=0;i<totline;i++)
	{
		mesh->lines[i]=lines[i];
	}

	t_context *C=ctx_get();
	scene_add_data_var(C->scene,"line","line_data",sizeof(int)*totline,_lines);
}

void mesh_uv_add(t_mesh *mesh,int *uvs,int totuv)
{
	mesh->quad_uv=vlst_make("quad_uv",dt_int, 2, totuv);
	vlst_add_data(mesh->quad_uv,uvs);
}

void mesh_line_cube_add(t_mesh *mesh)
{
	mesh_line_add(mesh,mesh_cube_lines,mesh_cube_tot_lines);
}

// INIT


void mesh_buffer_add_vbo(t_mesh *mesh)
{
	if(mesh->quads)
	{
		int tot_quad=mesh->var.tot_quad_face;

		if(!mesh->quad_vertex) mesh->quad_vertex=vlst_make("quad_vertex", dt_float, 3, tot_quad*4);
		if(!mesh->quad_face) mesh->quad_face=vlst_make("quad_face", dt_uint, 4,tot_quad);
		if(!mesh->quad_color) mesh->quad_color=vlst_make("quad_color", dt_float, 3, tot_quad * 4);
		if(!mesh->quad_normal) mesh->quad_normal=vlst_make("quad_normal", dt_float , 3,tot_quad * 4);

	}

	if(mesh->tris)
	{
		int tot_tri=mesh->var.tot_tri_face;

		if(!mesh->tri_vertex) mesh->tri_vertex=vlst_make("tri_vertex", dt_float, 3, tot_tri*3);
		if(!mesh->tri_face) mesh->tri_face=vlst_make("tri_face", dt_uint, 3, tot_tri);
		if(!mesh->tri_color) mesh->tri_color=vlst_make("tri_color", dt_float, 3, tot_tri*3);
		if(!mesh->tri_normal) mesh->tri_normal=vlst_make("tri_normal", dt_float, 3, tot_tri*3);

	}
}

void mesh_buffer_build_vbo(t_mesh *mesh)
{
	if(mesh->state.has_quad)
	{
		vlst_vertex("quad",mesh->quad_vertex,mesh->vertex,mesh->quads);
		vlst_build(mesh->quad_face,mesh->quads);
		vlst_color_3f(mesh->quad_color,mesh->idcol);
		vlst_normal_4f(mesh->quad_normal,mesh->quad_vertex,mesh->quad_face);
	}

	if(mesh->state.has_tri)
	{
		vlst_vertex("tri",mesh->tri_vertex,mesh->vertex,mesh->tris);
		vlst_build(mesh->tri_face,mesh->tris);
		vlst_color_3f(mesh->tri_color,mesh->idcol);
		vlst_normal_3f(mesh->tri_normal,mesh->tri_vertex,mesh->tri_face);
	}
}

void mesh_buffer_build_direct(t_mesh *mesh)
{
	if(mesh->state.has_quad)
	{
		vlst_normal_4f_direct(mesh->quad_normal,mesh->vertex,mesh->quads);
	}
}

void mesh_buffer_add_direct(t_mesh *mesh)
{
	//t_context *C=ctx_get();

	if(mesh->state.has_quad)
	{
		int tot_quad=mesh->var.tot_quad_face;

		if(!mesh->quad_normal) mesh->quad_normal=vlst_make("quad_normal", dt_float, 3, tot_quad);
	}
}

void mesh_buffer_reset(t_mesh *mesh)
{
	t_context *C=ctx_get();
	t_scene *scene=C->scene;

	if(mesh->quad_vertex) scene_struct_delete(scene,mesh->quad_vertex);
	if(mesh->quad_face) scene_struct_delete(scene,mesh->quad_face);
	if(mesh->quad_color) scene_struct_delete(scene,mesh->quad_color);
	if(mesh->quad_normal) scene_struct_delete(scene,mesh->quad_normal);

	if(mesh->tri_vertex) scene_struct_delete(scene,mesh->tri_vertex);
	if(mesh->tri_face) scene_struct_delete(scene,mesh->tri_face);
	if(mesh->tri_color) scene_struct_delete(scene,mesh->tri_color);
	if(mesh->tri_normal) scene_struct_delete(scene,mesh->tri_normal);

	mesh->quad_vertex=NULL;
	mesh->quad_face=NULL;
	mesh->quad_normal=NULL;
	mesh->quad_color=NULL;
	mesh->quad_uv=NULL;

	mesh->tri_vertex=NULL;
	mesh->tri_face=NULL;
	mesh->tri_normal=NULL;
	mesh->tri_color=NULL;
	mesh->tri_uv=NULL;

	mesh->state.is_buffer_built=0;
	mesh->state.buffer_type=buffer_empty;
}

void mesh_init_buffer_vbo(t_mesh *mesh)
{
	mesh_buffer_add_vbo(mesh);
	mesh_buffer_build_vbo(mesh);

	mesh->state.is_buffer_built=1;
	mesh->state.buffer_type=buffer_vbo;
}

void mesh_init_buffer_direct(t_mesh *mesh)
{
	mesh_buffer_add_direct(mesh);
	mesh_buffer_build_direct(mesh);

	mesh->state.is_buffer_built=1;
	mesh->state.buffer_type=buffer_direct;
}

void mesh_init_buffers(t_mesh *mesh,t_mesh_buffer_type type)
{
	// reset
	if(mesh->state.is_buffer_built) mesh_buffer_reset(mesh);

	// set new type
	mesh->state.buffer_type=type;

	switch(mesh->state.buffer_type)
	{
		case(buffer_empty):break;
		case(buffer_vbo): mesh_init_buffer_vbo(mesh);break;
		case(buffer_direct): mesh_init_buffer_direct(mesh);break;
		default:printf("[WARNING mesh_init_buffer] Not a buffer type\n");
	}

	// reset state
	mesh->state.need_update=0;
}
