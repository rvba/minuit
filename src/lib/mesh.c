/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "ctx.h"

void mesh_init(t_scene *sc,t_mesh *mesh)
{
	int col[3];
	scene_color_get(sc,col);
	vcp3i(mesh->idcol,col);
}

void cls_mesh_init(t_mesh *mesh)
{
	t_context *C=ctx_get();
	mesh_init(C->scene,mesh);
}

t_mesh_cls cls_mesh=
{
	.cls_type="mesh",
	.init=cls_mesh_init,
};

void *mesh_get_ref(t_mesh *mesh, const char *ref)
{
	void *p;

	if(is(ref,"vertex"))  				p=&mesh->vertex; 
	else if(is(ref,"colors"))  			p=&mesh->colors; 
	else if(is(ref,"faces"))  			p=&mesh->quads; 
	else if(is(ref,"tot vertex"))  			p=&mesh->var.tot_vertex; 
	else if(is(ref,"tot quad face"))  		p=&mesh->var.tot_quad_face; 
	else
	{
		printf("[ERROR mesh_get_ref] Unknown ref [%s] \n",ref);
		return NULL;
	}

	return p;
}


// REBIND

t_mesh *mesh_rebind(t_scene *sc,void *ptr)
{
	t_mesh *mesh=(t_mesh *)ptr;
	
	// REBIND

	rebind(sc,"mesh","texture",(void **)&mesh->texture);
	rebind(sc,"mesh","material",(void **)&mesh->material);
	rebind(sc,"mesh","lines",(void **)&mesh->lines);

	rebind(sc,"mesh","vertex",(void **)&mesh->vertex);
	rebind(sc,"mesh","quads",(void **)&mesh->quads);
	rebind(sc,"mesh","tris",(void **)&mesh->tris);
	rebind(sc,"mesh","uvs",(void **)&mesh->uvs);
	rebind(sc,"mesh","colors",(void **)&mesh->colors);

	// RESET

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

	// reset state
	mesh->state.is_selected=0;
	mesh->state.buffer_type=buffer_empty;
	mesh->state.is_buffer_built=0;

	return mesh;
}

// ADD BRICK VERTEX

void mesh_add_brick_vertex(t_context *C,t_mesh *mesh)
{
	// block
	t_node *_node_block=block_make("vertex","block");
	t_block *_block=_node_block->data;

	// outline
	_block->state.draw_outline=1;

	// add vertex
	scene_add_ref(C->scene,"struct_ref","mesh","vertex",&mesh->vertex,mesh);
	add_part_vlst(C,_block,dt_vlst,"vertex",mesh->vertex);
	t_brick *brick_count = block_brick_get(_block,"count:");

	// Bind
	brick_binding_add(brick_count, dt_int, &mesh->var.tot_vertex);

	// Ref
	scene_add_ref(C->scene,"struct_ref","mesh","tot vertex",&mesh->var.tot_vertex,mesh);

	// add to global list
	op_add_global(C,_block);
}

void mesh_add_brick_faces(t_mesh *mesh)
{
	t_context *C = ctx_get();
	// block
	t_node *_node_block=block_make("faces","block");
	t_block *_block=_node_block->data;

	// outline
	_block->state.draw_outline=1;

	scene_add_ref(C->scene,"struct_ref","mesh","faces",&mesh->quads,mesh);
	add_part_vlst(C,_block,dt_vlst,"quads",mesh->quads);
	t_brick *brick_count = block_brick_get(_block,"count:");

	// Bind
	brick_binding_add(brick_count, dt_uint, &mesh->var.tot_quad_face);

	// Ref
	scene_add_ref(C->scene,"struct_ref","mesh","tot quad face",&mesh->var.tot_quad_face,mesh);

	// add to global list
	op_add_global(C,_block);
}

// ADD BRICK MESH

void mesh_add_brick_mesh(t_context *C,t_mesh *mesh)
{
	// block

	t_node *node_block=block_make("mesh","block");
	t_block *block=node_block->data;

	// outline
	block->state.draw_outline=1;

	// add pointers
	add_part_pointer(C,block,dt_mesh,mesh->name,mesh);
	add_part_pointer(C,block,dt_vertex,"v",mesh);
	add_part_pointer(C,block,dt_face,"f",mesh);

	// init
	mesh_init(C->scene,mesh);

	// add to global list
	op_add_global(C,block);
}

void mesh_add_brick_color(t_mesh *mesh)
{
	t_context *C = ctx_get();

	// block
	t_node *_node_block = block_make("color","block");
	t_block *_block = _node_block->data;

	// outline
	_block->state.draw_outline = 1;

	// add vertex

	scene_add_ref(C->scene,"struct_ref","mesh","colors",&mesh->colors,mesh);
	add_part_vlst(C,_block,dt_vlst,"colors",mesh->colors);

	// add to global list
	op_add_global(C,_block);

	// link
	if(mesh->vertex)
	{
		t_vlst *vertex = mesh->vertex;
		t_vlst *colors = mesh->colors;

		colors->is_linked = 1 ;
		colors->link = vertex ;

		vertex->is_linked = 1 ;
		vertex->link = colors ;
	}
}

void mesh_add_default_color(t_mesh *mesh)
{
	int tot_vertex=mesh->var.tot_vertex;

	mesh->colors=vlst_make("color",dt_float, 3, tot_vertex);
	mesh->state.has_color=1;

	t_vlst *vlst=mesh->colors;
	float *colors=vlst->data;

	int i;
	for(i=0;i<tot_vertex;i++)
	{
		vset3f(colors,1,1,1);
		colors+=3;
	}

	mesh_add_brick_color(mesh);
}

// MAKE

t_node *mesh_make(
			const char name[],
			int totvert,
			int totface,
			int totquad,
			int tottri,
			float *verts,
			int *quads,
			int *tris)
{
	t_context *C = ctx_get();

	// add mesh
	t_node *node_mesh=scene_add(C->scene,nt_mesh,name);
	t_mesh *mesh=node_mesh->data;

	// add material
	t_node *node_material=scene_add(C->scene,nt_material,name);
	t_material *material=node_material->data;
	mesh->material=material;

	// store vars
	mesh->var.tot_face=totface;
	mesh->var.tot_vertex=totvert;
	mesh->var.tot_quad_face=totquad;
	mesh->var.tot_tri_face=tottri;

	// add vertex list
	mesh->vertex=vlst_make("vertex", dt_float, 3, totvert);
	vlst_add_data(mesh->vertex,verts);

	// add quad list
	if(quads)
	{
		mesh->state.has_quad=1;
		mesh->state.has_face=1;

		mesh->quads=vlst_make("quads", dt_uint, 4, totquad);
		vlst_add_data(mesh->quads,quads);

		t_vlst *vquads = mesh->quads;
		vquads->has_limit_high = 1;
		vquads->has_limit_low = 1;
	}

	// add tri list
	if(tris)
	{
		mesh->state.has_tri=1;
		mesh->state.has_face=1;

		mesh->tris=vlst_make("tris", dt_uint, 3,tottri);
		vlst_add_data(mesh->tris,tris);
	}

	// add brick vertex
	mesh_add_brick_vertex(C,mesh);

	// add brick mesh
	mesh_add_brick_mesh(C,mesh);

	return node_mesh;
}

// INIT

void mesh_state_init(t_mesh_state *state)
{
	state->has_face=0;
	state->has_tri=0;
	state->has_quad=0;
	state->has_color=0;

	state->with_texture=0;
	state->with_line=0;
	state->with_point=0;
	state->is_selected=0;

	state->need_update=0;
	state->buffer_type=buffer_empty;
	state->is_buffer_built=0;
}

void mesh_var_init(t_mesh_var *var)
{
	var->tot_vertex=0;
	var->tot_face=0;
	var->tot_quad_face=0;
	var->tot_tri_face=0;
	var->totline=0;
	var->offset=0;
}

// NEW

t_mesh *mesh_new(const char *name)
{
	t_mesh *mesh = (t_mesh *)malloc(sizeof(t_mesh));

	set_name(mesh->name,name);
	mesh->id=0;
	mesh->id_chunk=0;
	mesh->users=0;

	mesh_state_init(&mesh->state);
	mesh_var_init(&mesh->var);

	mesh->texture=NULL;
	mesh->material=NULL;
	mesh->lines=NULL;
	mesh->texture_id=0;
	mesh->texture=NULL;
	mesh->material=NULL;

	mesh->vertex=NULL;
	mesh->quads=NULL;
	mesh->tris=NULL;
	mesh->uvs=NULL;
	mesh->colors=NULL;

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

	return mesh;
}

// FREE

void mesh_free(t_mesh *mesh)
{
	t_context *C=ctx_get();
	t_scene *sc=C->scene;

	if (mesh->vertex) scene_struct_delete(sc,mesh->vertex); 
	if (mesh->quads) scene_struct_delete(sc,mesh->quads);
	if (mesh->tris) scene_struct_delete(sc,mesh->tris);
	if (mesh->uvs) scene_struct_delete(sc,mesh->uvs);

	if (mesh->quad_vertex) scene_struct_delete(sc,mesh->quad_vertex);
	if (mesh->quad_face) scene_struct_delete(sc,mesh->quad_face);
	if (mesh->quad_normal) scene_struct_delete(sc,mesh->quad_normal);
	if (mesh->quad_color) scene_struct_delete(sc,mesh->quad_color); 
	if (mesh->quad_uv) scene_struct_delete(sc,mesh->quad_uv);

	if (mesh->tri_vertex) scene_struct_delete(sc,mesh->tri_vertex);
	if (mesh->tri_face) scene_struct_delete(sc,mesh->tri_face);
	if (mesh->tri_normal) scene_struct_delete(sc,mesh->tri_normal);
	if (mesh->tri_color) scene_struct_delete(sc,mesh->tri_color);
	if (mesh->tri_uv) scene_struct_delete(sc,mesh->tri_uv);

	//XXX
	// material
	// texture
	//lines
}




