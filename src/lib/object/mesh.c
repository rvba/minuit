/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "node.h"
#include "base.h"
#include "mesh.h"
#include "ctx.h"
#include "scene.h"
#include "list.h"
#include "vlst.h"
#include "block.h"
#include "brick.h"
#include "op.h"
#include "list.h"
#include "ui.h"
#include "memory.h"
#include "material.h"
#include "texture.h"

void mesh_show(t_mesh *mesh)
{
	printf("MESH\n");
	printf("tot_vertex: %d\n", mesh->var.tot_vertex);
	printf("tot_face: %d\n", mesh->var.tot_face);

	printf("VERTICES\n");
	if(mesh->vertex) vlst_show(mesh->vertex);
	printf("COLORS\n");
	if(mesh->colors) vlst_show(mesh->colors);
	printf("QUADS\n");
	if(mesh->quads) vlst_show( mesh->quads);
	printf("TRIS\n");
	if(mesh->tris) vlst_show( mesh->tris);
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

	if(iseq(ref,"vertex"))  				p=&mesh->vertex; 
	else if(iseq(ref,"colors"))  			p=&mesh->colors; 
	else if(iseq(ref,"faces"))  			p=&mesh->quads; 
	else if(iseq(ref,"tot vertex"))  			p=&mesh->var.tot_vertex; 
	else if(iseq(ref,"tot quad face"))  		p=&mesh->var.tot_quad_face; 
	else
	{
		printf("[ERROR mesh_get_ref] Unknown ref [%s] \n",ref);
		return NULL;
	}

	return p;
}

// REBIND

void mesh_rebind(t_scene *sc,void *ptr)
{
	t_mesh *mesh=(t_mesh *)ptr;
	
	rebind(sc,"mesh","texture",(void **)&mesh->texture);
	rebind(sc,"mesh","material",(void **)&mesh->material);
	rebind(sc,"mesh","edges",(void **)&mesh->edges);

	rebind(sc,"mesh","vertex",(void **)&mesh->vertex);
	rebind(sc,"mesh","quads",(void **)&mesh->quads);
	rebind(sc,"mesh","tris",(void **)&mesh->tris);
	rebind(sc,"mesh","uvs",(void **)&mesh->uvs);
	rebind(sc,"mesh","colors",(void **)&mesh->colors);
	rebind(sc,"mesh","ref",(void **)&mesh->ref);

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
}

/*
	********************************************
	BRICKS
	*******************************************	*/

// FACES

void mesh_add_brick_faces(t_mesh *mesh)
{
	t_context *C = ctx_get();

	t_node *_node_block=add_block(C,"faces");
	t_block *_block=_node_block->data;

	scene_add_ref(C->scene,"struct_ref","mesh","faces",&mesh->quads,mesh);
	add_part_vlst(C,_block,dt_vlst,"quads",mesh->quads);
	t_brick *brick_count = block_brick_get(_block,"count:");

	// Bind
	brick_binding_add(brick_count, dt_uint, &mesh->var.tot_quad_face);

	// Ref
	scene_add_ref(C->scene,"struct_ref","mesh","tot quad face",&mesh->var.tot_quad_face,mesh);

	// Add Global offset
	add_block_offset(C,_block);
}

// COLOR

void mesh_add_brick_color(t_mesh *mesh)
{
	t_context *C = ctx_get();

	// New Block
	t_node *_node_block=add_block(C,"color");
	t_block *_block = _node_block->data;

	// add vertex

	scene_add_ref(C->scene,"struct_ref","mesh","colors",&mesh->colors,mesh);
	add_part_vlst(C,_block,dt_vlst,"colors",mesh->colors);

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

	// Add Global offset
	add_block_offset(C,_block);
}

void mesh_add_default_color(t_mesh *mesh)
{
	int tot_vertex=mesh->var.tot_vertex;

	mesh->colors=vlst_make("color",dt_float, 3, tot_vertex, NULL);
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

// MESH

void mesh_add_brick_mesh(t_context *C, t_node *node_mesh)
{
	// New Block
	t_node *node_block=add_block(C,"mesh");
	t_block *block=node_block->data;
	t_mesh *mesh = node_mesh->data;
	add_part_selector(C,block,mesh->id.name,node_mesh,dt_mesh);
	mesh->ref = block;

}

/*
	********************************************
	MAKE
	*******************************************	*/

// INIT

void mesh_init(t_scene *sc,t_mesh *mesh)
{
	int col[3];
	scene_color_get(sc,col);
	vcp3i(mesh->idcol,col);
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
	t_node *node_mesh=scene_add(C->scene,dt_mesh,name);
	t_mesh *mesh=node_mesh->data;

	// add material
	t_node *node_material=scene_add(C->scene,dt_material,name);
	t_material *material=node_material->data;
	mesh->material=material;

	// store vars
	mesh->var.tot_face=totface;
	mesh->var.tot_vertex=totvert;
	mesh->var.tot_quad_face=totquad;
	mesh->var.tot_tri_face=tottri;

	if(C->ui->add_bricks && C->ui->add_bricks_mesh) 
		mesh_add_brick_mesh(C,node_mesh);

	// add data node
	scene_add_data_node( C->scene, "app_node", "mesh", name, node_mesh);

	// add vertex list
	if( totvert)
	{
		if(verts)
		{
			mesh->vertex=vlst_make("vertex", dt_float, 3, totvert, NULL);
			vlst_add_data(mesh->vertex,verts);

		
		}
		else
		{
			mesh->vertex=vlst_make("vertex", dt_float, 3, totvert, NULL);
			vlst_init( mesh->vertex);
		}
	}

	// add quad list
	if(quads)
	{
		mesh->state.has_quad=1;
		mesh->state.has_face=1;

		mesh->quads=vlst_make("quads", dt_uint, 4, totquad, NULL);
		vlst_add_data(mesh->quads,quads);
	}

	// add tri list
	if(tris)
	{
		mesh->state.has_tri=1;
		mesh->state.has_face=1;

		mesh->tris=vlst_make("tris", dt_uint, 3,tottri, NULL);
		vlst_add_data(mesh->tris,tris);
	}


	// init
	mesh_init(C->scene,mesh);

	return node_mesh;
}


t_node *mesh_make_empty(const char *name)
{
	return mesh_make(name,0,0,0,0,NULL,NULL,NULL);
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
	state->selected_vertex = -1;
	state->hover_vertex = -1;
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

// CLONE

t_mesh *mesh_clone(t_mesh *mesh)
{
	if(mesh)
	{
		t_mesh *clone = mesh_new(mesh->id.name);

		clone->state.has_face = mesh->state.has_face;
		clone->state.has_tri = mesh->state.has_tri;
		clone->state.has_quad = mesh->state.has_quad;
		clone->state.has_color = mesh->state.has_color;
		clone->state.with_texture = mesh->state.with_texture;
		clone->state.with_line = mesh->state.with_line;
		clone->state.with_point = mesh->state.with_point;
		clone->state.is_selected = mesh->state.is_selected;
		clone->state.need_update = mesh->state.need_update;
		clone->state.buffer_type = mesh->state.buffer_type;
		clone->state.is_buffer_built = mesh->state.is_buffer_built;

		clone->var.tot_vertex = mesh->var.tot_vertex;
		clone->var.tot_face = mesh->var.tot_face;
		clone->var.tot_quad_face = mesh->var.tot_quad_face;
		clone->var.tot_tri_face = mesh->var.tot_tri_face;
		clone->var.totline = mesh->var.totline;
		clone->var.offset = mesh->var.offset;

		clone->texture = texture_clone(mesh->texture);
		clone->material = material_clone(mesh->material);
		clone->edges = vlst_copy(mesh->edges);
		clone->texture_id = mesh->texture_id;
		clone->vertex = vlst_copy(mesh->vertex);
		clone->quads = vlst_copy(mesh->quads);
		clone->tris = vlst_copy(mesh->tris);
		clone->uvs = vlst_copy(mesh->uvs);
		clone->colors = vlst_copy(mesh->colors);
		clone->quad_vertex = vlst_copy(mesh->quad_vertex);
		clone->quad_face = vlst_copy(mesh->quad_face);
		clone->quad_normal = vlst_copy(mesh->quad_normal);
		clone->quad_color = vlst_copy(mesh->quad_color);
		clone->quad_uv = vlst_copy(mesh->quad_uv);
		clone->tri_vertex = vlst_copy(mesh->tri_vertex);
		clone->tri_face = vlst_copy(mesh->tri_face);
		clone->tri_normal = vlst_copy(mesh->tri_normal);
		clone->tri_color = vlst_copy(mesh->tri_color);
		clone->tri_uv = vlst_copy(mesh->tri_uv);

		return clone;
	}
	else
	{
		// Light Object
		return NULL;
	}
}

// NEW

void *mesh_new(const char *name)
{
	t_mesh *mesh = (t_mesh *)mem_malloc(sizeof(t_mesh));

	id_init(&mesh->id, name);

	mesh_state_init(&mesh->state);
	mesh_var_init(&mesh->var);

	mesh->texture=NULL;
	mesh->material=NULL;
	mesh->texture_id=0;

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

	mesh->edges = NULL;
	mesh->edges_color = NULL;

	mesh->ref = NULL;

	return mesh;
}

// FREE
void mesh_free(t_mesh *mesh)
{
	t_context *C=ctx_get();
	t_scene *sc=C->scene;

	if (mesh->vertex) scene_delete(sc,mesh->vertex); 
	if (mesh->quads) scene_delete(sc,mesh->quads);
	if (mesh->tris) scene_delete(sc,mesh->tris);
	if (mesh->uvs) scene_delete(sc,mesh->uvs);

	if (mesh->quad_vertex) scene_delete(sc,mesh->quad_vertex);
	if (mesh->quad_face) scene_delete(sc,mesh->quad_face);
	if (mesh->quad_normal) scene_delete(sc,mesh->quad_normal);
	if (mesh->quad_color) scene_delete(sc,mesh->quad_color); 
	if (mesh->quad_uv) scene_delete(sc,mesh->quad_uv);

	if (mesh->tri_vertex) scene_delete(sc,mesh->tri_vertex);
	if (mesh->tri_face) scene_delete(sc,mesh->tri_face);
	if (mesh->tri_normal) scene_delete(sc,mesh->tri_normal);
	if (mesh->tri_color) scene_delete(sc,mesh->tri_color);
	if (mesh->tri_uv) scene_delete(sc,mesh->tri_uv);

	if( mesh->ref) scene_delete(sc, mesh->ref);
	if( mesh->edges) scene_delete(sc, mesh->edges);
	if( mesh->edges_color) scene_delete(sc, mesh->edges_color);
	// material
}




