/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include "op.h"
#include "ctx.h"
#include "node.h"
#include "scene.h"
#include "vlst.h"
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
#include "geometry.h"
#include "app.h"
#include "ui.h"
#include "vlst.h"

void op_add_data( t_context *C, const char *name, int size, void *data)
{
	scene_store( C->scene, 1);
	scene_add_data_var( C->scene, name, "data", size, data);
	scene_store( C->scene, 0);
}

t_vlst *op_add_vlst( t_context *C, const char *name, t_data_type type, int length, int count, void *data)
{
	scene_store( C->scene, 1);
	t_vlst *vlst = vlst_make( name, type, length, count, data);
	scene_store( C->scene, 0);
	return vlst;
}

// GEO

t_geo *op_new_geo( t_context *C, const char *name)
{
	t_geo *geo = geo_make( C,"geo");
	return geo;
}

t_geo_point *op_new_geo_point( t_context *C, const char *name)
{
	t_geo_point *geo_point = geo_point_make( C,"point");
	return geo_point;
}

t_geo_edge *op_new_geo_edge( t_context *C, const char *name)
{
	t_geo_edge *geo_edge = geo_edge_make( C, "edge");
	return geo_edge;
}

t_geo_array *op_new_geo_array( t_context *C,const char *name)
{
	t_geo_array *geo_array = geo_array_make( C, "array");
	return geo_array;
}


// VLST
t_vlst *op_new_vlst(const char *name, t_data_type type, int size)
 {
	 t_vlst *vlst = vlst_make("vlst", type, size, 0, NULL);
	 /*
	 t_block *block = vlst->ref;
	 if( block)
	 {
		t_brick *brick = block_brick_get( block, "vlst");
	 	brick->plug_intern.state.store_data_memory = 0;
	 }
	 */
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

t_node *op_new_mesh( const char *name, int tot_vertex, int tot_quad, int tot_tri, float *verts, int *quads, int *tris)
{
        int tot_face = tot_quad + tot_tri;
	t_node *cube = mesh_make(name,tot_vertex,tot_face,tot_quad,tot_tri,verts,quads,tris);

	return cube;
}

t_node *op_new_mesh_raw( const char *name, int size)
{
	int tot_vertex=size;
	int tot_face=0;
	int tot_quad=0;
	int tot_tri=0;

	float *verts=NULL; 	//u_volume
	int *quads=NULL;		//u_volume
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

t_node *op_new_square( const char *name)
{
	t_node *node = op_new_plane( name);
	t_mesh *mesh = node->data;

	mesh->state.has_face = 0;
	mesh->state.has_quad = 0;
	mesh->quads = NULL;

	return node;
}


// EMPTY MESH

t_node *op_new_empty_mesh(const char *name)
{
	t_node *node = mesh_make(name,0,0,0,0,NULL,NULL,NULL);
	return node;
}

void *op_new_set(const char *name)
{
	term_log("+ set");

	t_node *node = set_add(name);

	return node;
}

void *op_add_set(t_brick *brick)
{
	return NULL;
}

void *op_add_screen( t_brick *brick)
{
	t_context *C = ctx_get();
	term_log("+ screen");

	scene_store(C->scene,1);
	C->scene->has_generic_viewport = 1;

	//t_screen *screen = screen_add_3d( "screen_view3d", screen_view3d);
	t_screen *screen = screen_add_3d( "screen_view3d", screen_view3d);
	screen->draw = screen_view3d;

	screen->is_active = 1;
	screen->is_visible = 1;


	scene_store(C->scene,0);

	return NULL;
}

void *op_new_camera(const char *name)
{
	t_context *C = ctx_get();
	term_log("+ camera");

			int w = C->app->window->width;
			int h = C->app->window->height;

	scene_store(C->scene,1);

	t_node *node_camera=camera_make(name);
	t_camera *camera = ( t_camera *) node_camera->data;
	op_camera_frustum_init(camera, w, h);

	t_node *node_object=object_make( dt_camera, name);
	t_object *object = ( t_object *) node_object->data;
	object->cls->link(object,node_camera);


	scene_store(C->scene,0);
	return node_object;
}


void *op_add_camera(t_brick *brick)
{
	t_node *node = ( t_node *) op_new_camera("default");
	return node;
}

void *op_add_object_void( const char *name)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	t_node *node_object = object_make( dt_void, name);
	t_object *object = ( t_object *) node_object->data;
	scene_store(C->scene,0);
	return object;
}

void *op_add_mesh(const char *name)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);


		t_node *node_mesh=mesh_make_empty("mesh");
		t_node *node_object = object_make( dt_mesh, "cube");

		t_object *object = ( t_object *) node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);

	return object;
}

void *op_add_mesh_data(const char *name, int tot_vertex, int tot_quad, int tot_tri, float *verts, int *quads, int *tris)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);


		t_node *node_mesh = op_new_mesh("mesh", tot_vertex, tot_quad, tot_tri, verts, quads, tris);
		t_node *node_object = object_make( dt_mesh, name);

		t_object *object = ( t_object *) node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);

	return object;
}

void* op_add_camera_main(void)
{
	t_context *C = ctx_get();
	term_log("+ camera");

	scene_store(C->scene,1);

	t_node *node_object = object_make( dt_camera, "main_camera");
	t_object *object = ( t_object *) node_object->data;

	t_node *node_camera=camera_make("main_camera");

	object->cls->link(object,node_camera);

	scene_store(C->scene,0);
	return node_object;
}

// new light
void *op_add_light(t_brick *brick)
{
	t_context *C = ctx_get();
	term_log("+ light");

	scene_store(C->scene,1);

		light_make("light");

	scene_store(C->scene,0);
	return NULL;
}

// new cube

void *op_add_cube( t_brick *brick)
{ 
	t_context *C=ctx_get();
	opt_op_use_random = 1;
	term_log("+ mesh raw");

	scene_store(C->scene,1);

		t_node *node_mesh=op_new_cube( "cube");
		t_node *node_object = object_make( dt_mesh, "mesh_raw");

		t_object *object = ( t_object *) node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);
	opt_op_use_random = 0;
	return NULL;
}

t_node *op_add_mesh_raw( const char *name, int size)
{ 
	t_context *C=ctx_get();
	term_log("+ mesh raw");

	scene_store(C->scene,1);

		t_node *node_mesh=op_new_mesh_raw( "mesh_raw", size);
		t_node *node_object = object_make( dt_mesh, "mesh_raw");

		t_object *object = ( t_object *) node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);
	return node_object;
}

void *op_add_plane(t_brick *brick)
{ 
	t_context *C=ctx_get();
	term_log("+ plane");

	scene_store(C->scene,1);

		t_node *node_mesh=op_new_plane("plane");
		t_node *node_object = object_make( dt_mesh, "plane");

		t_object *object = ( t_object *) node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);
	return NULL;
}

void *op_add_object_square(t_brick *brick)
{ 
	t_context *C=ctx_get();
	term_log("+ square");

	scene_store(C->scene,1);

		t_node *node_mesh=op_new_square("plane");
		t_node *node_object = object_make( dt_mesh, "plane");

		t_object *object = ( t_object *) node_object->data;
		object->cls->link(object,node_mesh);

	scene_store(C->scene,0);
	return object;
}

void *op_add_viewport(t_brick *brick)
{
	op_add_screen(NULL);

	return NULL;
}

void *op_add_default_viewport(t_brick *brick)
{
	opt_op_use_random = 1;
	op_add_light(NULL);
	op_add_cube(NULL);
	//op_add_viewport(NULL);
	op_add_screen(NULL);
	opt_op_use_random = 0;

	return NULL;
}

// default 
void *op_add_default(t_brick *brick)
{
	opt_op_use_random = 1;
	op_add_light(NULL);
	op_add_cube(NULL);
	op_add_screen(NULL);
	opt_op_use_random = 0;

	return NULL;
}


void *op_add_mn(t_brick *brick)
{
	t_context *C = ctx_get();
	browser_enter( C, add_mn);
	/*
	C->event->callback=add_mn;
	ctx_ui_event_add( UI_EVENT_BROWSER_SHOW);
	*/

	return NULL;
}

void *op_add_empty_object(t_brick *brick)
{
	t_context *C=ctx_get();
	scene_store(C->scene,1);
	t_node *node = object_make( dt_mesh, "cube");
	scene_store(C->scene,0);
	return node;
}

void *op_add_empty_mesh(t_brick *brick)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	t_node *node = op_new_empty_mesh("mesh");
	scene_store(C->scene,0);
	return node;
}

void op_mesh_add_brick_vertex(t_context *C,t_mesh *mesh)
{
	// New Block
	t_node *_node_block=add_block(C,"vertex");
	t_block *_block= ( t_block *) _node_block->data;

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

void *op_add_empty_float_vlst(t_brick *brick)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	op_new_vlst("vlst", dt_float, 3);
	scene_store(C->scene,0);

	return NULL;
}

void *op_add_empty_int_vlst(t_brick *brick)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	op_new_vlst("vlst", dt_int, 2);
	scene_store(C->scene,0);

	return NULL;
}

void *op_add_empty_geometry(t_brick *brick)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	op_new_geo( C, "geo");
	scene_store(C->scene,0);

	return NULL;
}

void *op_add_empty_geo_point(t_brick *brick)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	op_new_geo_point( C, "point");
	scene_store(C->scene,0);

	return NULL;
}

void *op_add_empty_geo_edge(t_brick *brick)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	op_new_geo_edge( C, "edge");
	scene_store(C->scene,0);

	return NULL;
}

void *op_add_geo_array(t_brick *brick)
{
	t_context *C = ctx_get();
	scene_store(C->scene,1);
	op_new_geo_array( C, "array");
	scene_store(C->scene,0);

	return NULL;
}

void *op_add_image( t_brick *brick)
{
	t_context *C=ctx_get();
	scene_store(C->scene,1);
	t_node *node = object_make( dt_image, "image");
	scene_store(C->scene,0);

	return node;
}
	
	
