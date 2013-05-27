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



#define LOGOUT 1

// new mesh cube
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

void *op_new_set(const char *name)
{
	t_context *C = ctx_get();
	term_print(C->term,"+ set");

	t_node *node = set_add(name);

	return node;
}

void *op_add_set(t_brick *brick)
{
	t_context *C = ctx_get();
	C->scene->store=1;
	C->scene->store=0;
	return NULL;
}

void *op_add_viewport(t_brick *brick)
{
	t_context *C = ctx_get();
	term_print(C->term,"+ viewport");

	C->scene->store=1;

	t_node *node = viewport_add("viewport");

	C->scene->store=0;

	return node;
}

void *op_new_camera(const char *name)
{
	t_context *C = ctx_get();
	term_print(C->term,"+ camera");

	C->scene->store=1;

	t_node *node_object=object_add("camera",name);
	t_object *object=node_object->data;

	t_node *node_camera=camera_make(name);

	object->cls->link(object,node_camera);

	C->scene->store=0;
	return node_object;
}

void *op_add_camera(t_brick *brick)
{
	t_node *node = op_new_camera("default");
	return node;
}

void* op_add_camera_main(void)
{
	t_context *C = ctx_get();
	term_print(C->term,"+ camera");

	C->scene->store=1;

	t_node *node_object=object_add("camera","main_camera");
	t_object *object=node_object->data;

	t_node *node_camera=camera_make("main_camera");

	object->cls->link(object,node_camera);

	C->scene->store=0;
	return node_object;
}

// new light
void *op_add_light(t_brick *brick)
{
	t_context *C = ctx_get();
	term_print(C->term,"+ light");

	C->scene->store=1;

		light_make("light");

	C->scene->store=0;
	return NULL;
}

// new cube
void *op_add_cube(t_brick *brick)
{ 
	t_context *C=ctx_get();
	term_print(C->term,"+ cube");

	C->scene->store=1;

		t_node *node_mesh=op_new_cube("cube");
		t_node *node_object=object_add("mesh","cube");

		t_object *object=node_object->data;
		object->cls->link(object,node_mesh);

	C->scene->store=0;
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
	t_context *C=ctx_get();
	term_print(C->term,"+ uv cube");

	C->scene->store=1;

		t_node *node_mesh = op_new_cube("me_uv_cube");
		t_node *node_object=object_add("mesh","ob_uvcube");

		// add uvs
		t_mesh *mesh = node_mesh->data;
		set_name(mesh->texture_name,"image");
		mesh->state.with_texture=1;
		mesh_uv_add(mesh,uvlst,48);

		t_object *object=node_object->data;
		object->cls->link(object,node_mesh);

	C->scene->store=0;
	return NULL;
}	

t_node *op_add_new_wire_cube(t_node *n)
{
	t_context *C=ctx_get();

	C->scene->store=1;

		t_node *node_mesh = op_new_cube("me_wire_cube");
		t_node *node_object=object_add("mesh","ob_wire_cube");

		t_mesh *mesh = node_mesh->data;

		// set draw modes
		mesh->state.with_line=1;
		
		// add lines
		mesh_line_cube_add(mesh);

		t_object *object = node_object->data;
		object->cls->link(object,node_mesh);

	C->scene->store=0;

	return node_object;
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
	
	
