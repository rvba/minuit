/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "node.h"
#include "scene.h"

#include "util.h"
#include "op.h"
#include "ctx.h"
#include "viewport.h"
#include "camera.h"
#include "object.h"
#include "draw.h"

#include "brick.h"
#include "memory.h"

// DRAW

void viewport_draw_scene(t_viewport *viewport)
{
	t_context *C = ctx_get();

	t_camera *camera = viewport->camera;
	op_camera_update(C, camera);
	op_3d_orientation(); 
	draw_scene(C->draw,C->scene);
}

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
	rebind(sc,"viewport","draw",(void **)&viewport->draw);

	return viewport;
}

t_node *viewport_add(const char *name)
{
	t_node *node=viewport_make(name);
	return node;
}

// CLONE

t_viewport *viewport_clone(t_viewport *viewport)
{
	if(viewport)
	{
		t_viewport *clone = viewport_new(viewport->id.name);

		clone->width = viewport->width;
		clone->height = viewport->height;
		
		clone->x = viewport->x;
		clone->y = viewport->y;
		clone->camera = camera_clone(viewport->camera);
		clone->draw = viewport->draw;

		return clone;
	}
	else
	{
		return NULL;
	}
}


// MAKE

t_node *viewport_make(const char *name)
{
	t_context *C = ctx_get();

	t_node *node_viewport = scene_add(C->scene, dt_viewport, name);
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

void _viewport_free(t_viewport *viewport)
{
	if(viewport->camera) _camera_free(viewport->camera);
	mem_free( viewport, sizeof( t_viewport));
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
	t_viewport *viewport = (t_viewport *) mem_malloc(sizeof(t_viewport));

	id_init(&viewport->id, name);

	viewport->width = 0;
	viewport->height = 0;
	viewport->x = 0;
	viewport->y = 0;
	viewport->camera = NULL;
	viewport->draw = NULL;

	return viewport;
}
