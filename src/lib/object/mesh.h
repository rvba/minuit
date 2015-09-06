/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __MESH_H_
#define __MESH_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Node;
struct Scene;
struct VLst;
struct Material;
struct Texture;

typedef struct Mesh t_mesh;
typedef struct Mesh_Class t_mesh_cls;
typedef struct Mesh_Var t_mesh_var;
typedef struct Mesh_State t_mesh_state;

typedef enum Mesh_Buffer_Type
{
	buffer_empty,
	buffer_direct,
	buffer_vbo

}t_mesh_buffer_type;

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
	int has_face:1;
	int has_tri:1;
	int has_quad:1;
	int has_color:1;

	int with_texture:1;
	int with_line:1;
	int with_point:1;

	int is_selected:1;
	int is_hover:1;
	int is_edit_mode:1;
	int need_update:1;

	t_mesh_buffer_type buffer_type;
	int is_buffer_built;
	int selected_vertex;
	int hover_vertex;
};

struct Mesh_Class
{
	char cls_type[_NAME_];
	void (* init)(t_mesh *mesh);
};

struct Mesh 
{
	t_id id;
	t_mesh_cls *cls;

	int idcol[3];

	// data

	t_mesh_state state;
	t_mesh_var var;

	// geometry

	struct VLst *vertex;
	struct VLst *quads;
	struct VLst *tris;
	struct VLst *uvs;
	struct VLst *colors;

	struct VLst *quad_vertex;
	struct VLst *quad_face;
	struct VLst *quad_normal;
	struct VLst *quad_color;
	struct VLst *quad_uv;

	struct VLst *tri_vertex;
	struct VLst *tri_face;
	struct VLst *tri_normal;
	struct VLst *tri_color;
	struct VLst *tri_uv;

	struct VLst *edges;
	struct VLst *edges_color;

	// materials

	int texture_id;
	char texture_name[_NAME_];	
	struct Texture *texture;
	struct Material *material;

	struct Block *ref;
};

void 		mesh_vertex_add(t_mesh *mesh, float *v);
void		mesh_uv_add(t_mesh *mesh,int *uvs,int totuv);
void * 		mesh_new(const char *name);
void 		mesh_free(t_mesh *mesh);
void 		mesh_update(t_mesh *mesh);
struct Node*	mesh_make(
			const char *name,
			int totvert,
			int totface,
			int totquad,
			int tottri,
			float *verts,
			int *quads,
			int *tris);

struct Node *	mesh_make_empty(const char *name);
t_mesh *	mesh_clone(t_mesh *mesh);
void *		mesh_get_ref(t_mesh *mesh, const char *ref);
void		mesh_rebind(struct Scene *sc,void *ptr);
void 		mesh_init(struct Scene *sc,t_mesh *mesh);
t_mesh *	mesh_clone(t_mesh *mesh);
void		mesh_show(t_mesh *mesh);

// MESH_BUFFER

void mesh_init_buffer_direct(t_mesh *mesh);
void mesh_init_buffers(t_mesh *mesh,t_mesh_buffer_type type);
void mesh_add_brick_color(t_mesh *mesh);
void mesh_add_default_color(t_mesh *mesh);
void mesh_add_brick_faces(t_mesh *mesh);

#ifdef __cplusplus
}
#endif

#endif
