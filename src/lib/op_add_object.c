/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

#include "context.h"
#include "node.h"
#include "scene.h"

#include "ctx.h"
#include "event.h"
#include "term.h"
#include "mesh.h"
#include "viewport.h"
#include "camera.h"
#include "light.h"
#include "set.h"
#include "object.h"
#include "file.h"
#include "screen.h"

#include "block.h"
#include "brick.h"



#define LOGOUT 1

// VLST
t_vlst *op_new_vlst(const char *name, t_data_type type, int size)
 {
	 t_vlst *vlst = vlst_make("vlst", type, size, 0);
	 return vlst;
 }

// CUBE
t_node *op_new_cube(const char *name)
{
	int tot_vertex=8;
	int tot_face=6;
	int tot_quad=6;
	int tot_tri=0;

	float *verts=cube_verts; 	//u_volume
	int *quads=cube_quads;		//u_volume
	int *tris = NULL;
		
	t_node *cube = mesh_make(name,tot_vertex,tot_face,tot_quad,tot_tri,verts,quads,tris);

	return cube;
}

// PLANE
t_node *op_new_plane(const char *name)
{
	int tot_vertex=4;
	int tot_face=1;
	int tot_quad=1;
	int tot_tri=0;

	float *verts=plane_verts; 	//u_volume
	int *quads=plane_quads;		//u_volume
	int *tris = NULL;
		
	t_node *plane = mesh_make(name,tot_vertex,tot_face,tot_quad,tot_tri,verts,quads,tris);

	return plane;
}

// EMPTY MESH

t_node *op_new_empty_mesh(const char *name)
{
	t_node *node = mesh_make(name,0,0,0,0,NULL,NULL,NULL);
	return node;
}

void *op_new_set(const char *name)
{
	t_context *C = ctx_get();
	term_print(C->term,"+ set");

	t_node *node = set_add(name);

	return node;
}

void *op_add_set(t_brick *brick)
{
	return NULL;
}

void *op_add_viewport(t_brick *brick)
{
	t_context *C = ctx_get();
	term_print(C->term,"+ viewport");

	scene_store(C->scene,1);
	C->scene->has_generic_viewport = 1;

	t_node *node = viewport_add("viewport");
	t_viewport *viewport = node->data;

	viewport->draw = viewport_draw_scene;

	scene_store(C->scene,0);

	return node;
}

void *op_new_camera(const char *name)
{
	t_context *C = ctx_get();
	term_print(C->term,"+ camera");

	scene_store(C->scene,1);

	t_node *node_object=object_add("camera",name);
	t_object *object=node_object->data;

	t_node *node_camera=camera_make(name);

	object->cls->link(object,node_camera);

	scene_store(C->scene,0);
	return node_object;
}


void *op_add_camera(t_brick *brick)
{
	t_node *node = op_new_camera("default");
	return node;
}

void *op_add_object(const char *name)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	t_node *node_object=object_add("point",name);
	t_object *object=node_object->data;
	scene_store(C->scene,0);
	return object;
}

void *op_add_mesh(const char *name)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);


		t_node *node_mesh=mesh_make_empty("mesh");
		t_node *node_object=object_add("mesh","cube");

		t_object *object=node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);

	return object;
}

void* op_add_camera_main(void)
{
	t_context *C = ctx_get();
	term_print(C->term,"+ camera");

	scene_store(C->scene,1);

	t_node *node_object=object_add("camera","main_camera");
	t_object *object=node_object->data;

	t_node *node_camera=camera_make("main_camera");

	object->cls->link(object,node_camera);

	scene_store(C->scene,0);
	return node_object;
}

// new light
void *op_add_light(t_brick *brick)
{
	t_context *C = ctx_get();
	term_print(C->term,"+ light");

	scene_store(C->scene,1);

		light_make("light");

	scene_store(C->scene,0);
	return NULL;
}

// new cube
void *op_add_cube(t_brick *brick)
{ 
	t_context *C=ctx_get();
	term_print(C->term,"+ cube");

	scene_store(C->scene,1);

		t_node *node_mesh=op_new_cube("cube");
		t_node *node_object=object_add("mesh","cube");

		t_object *object=node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);
	return NULL;
}

void *op_add_plane(t_brick *brick)
{ 
	t_context *C=ctx_get();
	term_print(C->term,"+ plane");

	scene_store(C->scene,1);

		t_node *node_mesh=op_new_plane("plane");
		t_node *node_object=object_add("mesh","plane");

		t_object *object=node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);
	return NULL;
}

// default 
void *op_add_default(t_brick *brick)
{
	op_add_light(NULL);
	op_add_cube(NULL);
	op_add_viewport(NULL);
	return NULL;
}

void *op_add_uv_cube(t_brick *brick)
{
	/*
	t_context *C=ctx_get();
	term_print(C->term,"+ uv cube");

	scene_store(C->scene,1);

		t_node *node_mesh = op_new_cube("me_uv_cube");
		t_node *node_object=object_add("mesh","ob_uvcube");

		// add uvs
		t_mesh *mesh = node_mesh->data;
		set_name(mesh->texture_name,"image");
		mesh->state.with_texture=1;
		mesh_uv_add(mesh,uvlst,48);

		t_object *object=node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);
	*/
	return NULL;
}	

t_node *op_add_new_wire_cube(t_node *n)
{
	/*
	t_context *C=ctx_get();

	scene_store(C->scene,1);

		t_node *node_mesh = op_new_cube("me_wire_cube");
		t_node *node_object=object_add("mesh","ob_wire_cube");

		t_mesh *mesh = node_mesh->data;

		// set draw modes
		mesh->state.with_line=1;
		
		// add lines
		mesh_line_cube_add(mesh);

		t_object *object = node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);

	return node_object;
	*/
	return NULL;
}

void *op_add_wire_cube(t_brick *brick)
{
	t_context *C=ctx_get();
	term_print(C->term,"+ wire cube");
	op_add_new_wire_cube(NULL);
	return NULL;
}

void add_mn(void)
{
	t_context *C=ctx_get();
	char *filename=C->event->standby_string;
	if(filename)
	{
		t_file *file = file_new(filename);

		file_init(file);

		if(is(file->ext,"mn"))
		{
			load_file(C,file->location);
		}
		else
		{
			printf("Not a mn file\n");
		}
	}
}

void *op_add_mn(t_brick *brick)
{
	t_context *C=ctx_get();
	C->event->callback=add_mn;
	screen_switch_by_name("screen_browser");

	return NULL;
}

void *op_add_empty_object(t_brick *brick)
{
	t_context *C=ctx_get();
	scene_store(C->scene,1);
	t_node *node_object=object_add("mesh","cube");
	(void) node_object;
	scene_store(C->scene,0);
	return NULL;
}

void *op_add_empty_mesh(t_brick *brick)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	t_node *node = op_new_empty_mesh("mesh");
	(void) node;
	scene_store(C->scene,0);
	return NULL;
}

void op_mesh_add_brick_vertex(t_context *C,t_mesh *mesh)
{
	// New Block
	t_node *_node_block=add_block(C,"vertex");
	t_block *_block=_node_block->data;

	// add vertex
	scene_add_ref(C->scene,"struct_ref","mesh","vertex",&mesh->vertex,mesh);
	add_part_vlst(C,_block,dt_vlst,"vertex",mesh->vertex);
	t_brick *brick_count = block_brick_get(_block,"count:");

	// Bind
	brick_binding_add(brick_count, dt_int, &mesh->var.tot_vertex);

	// Ref
	scene_add_ref(C->scene,"struct_ref","mesh","tot vertex",&mesh->var.tot_vertex,mesh);

	// Add Global offset
	add_block_offset(C,_block);
}

void *op_add_empty_vlst(t_brick *brick)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	op_new_vlst("vlst", dt_float, 3);
	scene_store(C->scene,0);

	return NULL;
}
	
	
