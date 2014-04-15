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
#include "viewport.h"
#include "camera.h"
#include "object.h"
#include "draw.h"

#include "brick.h"
#include "memory.h"
#include "app.h"

#include "block.h"

int opt_viewport_show_controls = 0;

// DRAW

void *viewport_get_ref(t_viewport *viewport, const char *ref)
{
	void *p;

	if(is(ref,"width"))  				p=&viewport->width; 
	else if(is(ref,"height"))  			p=&viewport->height; 
	else if(is(ref,"x"))  				p=&viewport->x; 
	else if(is(ref,"y"))  				p=&viewport->y; 
	else if(is(ref,"fullscreen"))  				p=&viewport->fullscreen; 
	else
	{
		printf("[ERROR mesh_get_ref] Unknown ref [%s] \n",ref);
		return NULL;
	}

	return p;
}

void viewport_switch_2d( t_viewport *viewport)
{
	t_camera *camera = viewport->camera;
	int width = viewport->width;
	int height = viewport->height;

	glViewport( 0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double left = camera->left;
	double right = camera->right;	
	double bottom = camera->bottom;
	double top = camera->top;

	glOrtho(
		0 + left,
		width + right,
		0 + bottom,
		height + top,
		-1,
		1
		);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(
			camera->ortho_location[0],
			camera->ortho_location[1],
			camera->ortho_location[2]
			);

	glRotatef(camera->angle,
		camera->ortho_rotation[0],
		camera->ortho_rotation[1],
		camera->ortho_rotation[2]
		);
}


void viewport_draw_controls( t_viewport *viewport)
{
	t_context *C = ctx_get();
	op_camera_switch_2d( C, viewport->camera);
	glPushMatrix();
	glLoadIdentity();

	t_block *block = viewport->controls;
	if (block)
	{
		block->cls->draw( block);
	}

	glPopMatrix();
	op_camera_switch_3d( C, viewport->camera);
}

void viewport_draw_scene(t_viewport *viewport)
{
	t_context *C = ctx_get();
	t_camera *camera = viewport->camera;

			int w = C->app->window->width;
			int h = C->app->window->height;

	if( C->app->window->change)
	{
		op_camera_frustum_init( camera, w, h);
		C->app->window->change = 0;
	}

	if( opt_viewport_show_controls)
	{
		viewport_draw_controls( viewport);
	}

	if( viewport->fullscreen) op_camera_update(C, camera);
	else  _op_camera_update(C, camera, viewport);
	
	op_3d_orientation(); 
	draw_scene(C->draw,C->scene);
}

void viewport_draw(t_viewport *viewport)
{
	t_context *C = ctx_get();

	op_3d_orientation(); 
	draw_scene(C->draw,C->scene);
}

// ADD 

t_viewport *viewport_rebind( t_scene *sc, void *ptr)
{
	t_viewport *viewport=( t_viewport *) ptr;

	rebind(sc, "viewport", "camera", (void **) &viewport->camera);
	rebind(sc, "viewport", "draw", (void **) &viewport->draw);
	rebind(sc, "viewport", "blocks", (void **) &viewport->controls);

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

void viewport_add_controls( t_viewport *viewport)
{
	t_context *C = ctx_get();

	scene_add_ref(C->scene,"struct_ref","viewport","width",&viewport->width,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","height",&viewport->height,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","x",&viewport->x,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","y",&viewport->y,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","fullscreen",&viewport->fullscreen,viewport);

	t_block *block = add_block_block( C, "viewport_controls");
	if( block)
	{
		block->pos[0] = 0;
		block->pos[1] = 1;
		add_brick_slider_int( C, block, "width", &viewport->width);
		add_brick_slider_int( C, block, "height", &viewport->height);
		add_brick_slider_int( C, block, "x", &viewport->x);
		add_brick_slider_int( C, block, "y", &viewport->y);
		add_brick_switch( C, block, "fullscreen", &viewport->fullscreen);
	}

	viewport->controls = block;

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
	viewport->width = C->app->window->width;
	viewport->height = C->app->window->height;
	viewport->x = 0;
	viewport->y = 0;

	scene_add_ref(C->scene,"struct_ref","viewport","width",&viewport->width,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","height",&viewport->height,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","x",&viewport->x,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","y",&viewport->y,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","fullscreen",&viewport->fullscreen,viewport);

	t_block *block = add_block_block( C, "viewport_controls");
	if( block)
	{
		block->pos[0] = 0;
		block->pos[1] = 1;
		add_brick_slider_int( C, block, "width", &viewport->width);
		add_brick_slider_int( C, block, "height", &viewport->height);
		add_brick_slider_int( C, block, "x", &viewport->x);
		add_brick_slider_int( C, block, "y", &viewport->y);
		add_brick_switch( C, block, "fullscreen", &viewport->fullscreen);
	}

	viewport->controls = block;

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
		scene_delete(C->scene,viewport->camera);
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
	viewport->show_controls = 0;
	viewport->fullscreen = 1;

	return viewport;
}
