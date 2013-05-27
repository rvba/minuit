/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "minuit.h"

#ifndef __MESH_H_
#define __MESH_H_

#include "texture.h"
#include "material.h"

#define MESH_LENGTH_TYPE 20

typedef struct Mesh t_mesh;
typedef struct Mesh_Class t_mesh_cls;
typedef struct Mesh_Var t_mesh_var;
typedef struct Mesh_State t_mesh_state;

typedef enum Mesh_Buffer_Type t_mesh_buffer_type;

enum Mesh_Buffer_Type
{
	buffer_empty,
	buffer_direct,
	buffer_vbo
};

struct Mesh_Var
{
	int tot_vertex;
	int tot_face;
	int tot_quad_face;
	int tot_tri_face;
	int totline;
	int offset;
};

struct Mesh_State
{
	// OFF
	int has_face:1;
	int has_tri:1;
	int has_quad:1;
	int has_color:1;

	int with_texture:1;
	int with_line:1;
	int with_point:1;

	int is_selected:1;
	int need_update:1;

	t_mesh_buffer_type buffer_type;
	int is_buffer_built;
};

struct Mesh_Class
{
	char cls_type[_NAME_];
	void (* init)(t_mesh *mesh);
};

struct Mesh 
{
	// generic

	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	t_mesh_cls *cls;

	int idcol[3];

	// data

	t_mesh_state state;
	t_mesh_var var;

	// geometry

	t_vlst *vertex;
	t_vlst *quads;
	t_vlst *tris;
	t_vlst *uvs;
	t_vlst *colors;

	t_vlst *quad_vertex;
	t_vlst *quad_face;
	t_vlst *quad_normal;
	t_vlst *quad_color;
	t_vlst *quad_uv;

	t_vlst *tri_vertex;
	t_vlst *tri_face;
	t_vlst *tri_normal;
	t_vlst *tri_color;
	t_vlst *tri_uv;

	int *lines;

	// materials

	int texture_id;
	char texture_name[_NAME_];	
	t_texture *texture;
	t_material *material;

};

void		mesh_uv_add(t_mesh *mesh,int *uvs,int totuv);
t_mesh*		mesh_new(const char *name);
void 		mesh_free(t_mesh *mesh);
void 		mesh_update(t_mesh *mesh);

t_node*		mesh_make(
			const char *name,
			int totvert,
			int totface,
			int totquad,
			int tottri,
			float *verts,
			int *quads,
			int *tris);

void *mesh_get_ref(t_mesh *mesh, const char *ref);
t_mesh*		mesh_rebind(t_scene *sc,void *ptr);
void 		mesh_init(t_scene *sc,t_mesh *mesh);


// MESH_BUFFER

void mesh_init_buffers(t_mesh *mesh,t_mesh_buffer_type type);
void mesh_add_brick_color(t_mesh *mesh);
void mesh_add_default_color(t_mesh *mesh);
void mesh_add_brick_faces(t_mesh *mesh);

// OP_MESH

void 		mesh_object_add(t_node *node,t_node *object);
void 		mesh_line_add(t_mesh *mesh,int *lines,int totline);
void 		mesh_line_cube_add(t_mesh *mesh);
void 		mesh_calc_vertex_array(t_mesh *mesh);
void 		mesh_build_vertex_array(t_mesh *mesh);
void 		mesh_do_quad_colors(t_mesh *mesh,int *col);
void 		mesh_do_tri_colors(t_mesh *mesh,int *col);
void 		mesh_do_quad_normals(t_mesh *mesh);
void 		mesh_do_tri_normals(t_mesh *mesh);
void 		mesh_recalc_normals(t_mesh *mesh);
void 		mesh_show_normals(t_mesh *mesh);
void 		mesh_show_vertices(t_mesh *mesh);
void 		mesh_show_colors(t_mesh *mesh);
void 		mesh_show_faces(t_mesh *mesh);
void 		mesh_show_uv(t_mesh *mesh);
void 		mesh_show(t_mesh *mesh);

#endif
