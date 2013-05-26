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

// DRAW

void viewport_draw(t_viewport *viewport)
{
	t_context *C = ctx_get();

	t_camera *camera = viewport->camera;
	op_camera_update(C, camera);
	op_3d_orientation(); 
	draw_scene(C->draw,C->scene);
}

// ADD 

t_viewport *viewport_rebind(t_scene *sc, void **ptr)
{
	t_viewport *viewport=(t_viewport *)ptr;

	rebind(sc,"viewport","camera",(void **)&viewport->camera);

	return viewport;
}

t_node *viewport_add(const char *name)
{
	// make object
	t_node *node=viewport_make(name);


	/*
	t_object *object=node->data;
	// add data node
	scene_add_data_node(C->scene,"app_node","object",name,node);

	// add global reference
	t_node *node_block=block_make(name,"block");
	t_block *block=node_block->data;
	object->ref=block;

	// add selector
	add_part_selector(C,block,name,node);

	// add to global list
	op_add_global(C,block);

	// outline
	block->state.draw_outline=1;
	*/

	return node;
}


// MAKE

t_node *viewport_make(const char *name)
{
	t_context *C = ctx_get();

	t_node *node_viewport = scene_add(C->scene, nt_viewport, name);
	t_node *node_camera = op_new_camera("vp_camera");
	t_object *object = node_camera->data;

	t_viewport *viewport = node_viewport->data;
	t_camera *camera = object->data;

	viewport->camera = camera;
	viewport->width = 100;
	viewport->height = 100;
	viewport->x = 0;
	viewport->y = 0;

	return node_viewport;
}

void viewport_free(t_viewport *viewport)
{
	t_context *C = ctx_get();
	if(viewport->camera)
		scene_struct_delete(C->scene,viewport->camera);
}

// NEW

t_viewport *viewport_new(const char *name)
{
	t_viewport *viewport = (t_viewport *) malloc(sizeof(t_viewport));

	viewport->id=0;
	viewport->id_chunk=0;
	set_name(viewport->name,name);
	viewport->users=0;

	viewport->width = 0;
	viewport->height = 0;
	viewport->x = 0;
	viewport->y = 0;
	viewport->camera = NULL;

	return viewport;
}
