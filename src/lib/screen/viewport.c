/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "ctx.h"
#include "node.h"
#include "scene.h"

#include "base.h"
#include "op.h"
#include "viewport.h"
#include "camera.h"
#include "object.h"
#include "draw.h"

#include "brick.h"
#include "memory.h"
#include "app.h"

#include "block.h"
#include "sketch.h"
#include "ui.h"

int opt_viewport_show_controls = 0;

// DRAW

void *viewport_get_ref(t_viewport *viewport, const char *ref)
{
	void *p;

	if(iseq(ref,"width"))  				p=&viewport->width; 
	else if(iseq(ref,"height"))  			p=&viewport->height; 
	else if(iseq(ref,"x"))  				p=&viewport->x; 
	else if(iseq(ref,"y"))  				p=&viewport->y; 
	else if(iseq(ref,"fullscreen"))  			p=&viewport->fullscreen; 
	else if(iseq(ref,"show_outline"))  		p=&viewport->show_outline; 
	else
	{
		printf("[ERROR mesh_get_ref] Unknown ref [%s] \n",ref);
		return NULL;
	}

	return p;
}

void viewport_switch_3d( t_viewport *viewport)
{

	int width;
	int height;
	int x;
	int y;

	t_context *C = ctx_get();
	t_camera *camera = viewport->camera;

	if( viewport->fullscreen)
	{
		width = C->app->window->width;
		height = C->app->window->height;
		x = 0;
		y = 0;
	}
	else
	{
		width = viewport->width;
		height = viewport->height;
		x = viewport->x;
		y = viewport->y;
	}

	if( camera->update_frustum)
	{
		op_camera_frustum_init( camera, width, height);
		C->app->window->change = 0;
	}

	glViewport( x, y, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//if( camera->restrict_matrix) op_camera_restrict( C, camera);
	glFrustum(
		camera->left,
		camera->right,
		camera->bottom,
		camera->top,
		camera->near,
		camera->far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		camera->eye[0],
		camera->eye[1],
		camera->eye[2],
		camera->target[0],
		camera->target[1],
		camera->target[2],
		camera->up[0],
		camera->up[1],
		camera->up[2]
		);

	float zenith = camera->zenith;
	glTranslatef(camera->pos[0],camera->pos[1],camera->pos[2]);

	// Camera Up rotation
	glRotatef(zenith,camera->cross[0],camera->cross[1],camera->cross[2]);

	op_3d_orientation(); 
}

void viewport_switch_2d( t_viewport *viewport)
{
	t_context *C = ctx_get();
	t_camera *camera = viewport->camera;

	int width;
	int height;
	int x;
	int y;
	float zoom = camera->ortho_zoom;

	if( viewport->fullscreen || viewport->use_ui)
	{
		width = C->app->window->width;
		height = C->app->window->height;
		x = 0;
		y = 0;
	}
	else
	{
		width = viewport->width;
		height = viewport->height;
		x = viewport->x;
		y = viewport->y;
	}

	if( camera->update_frustum)
	{
		op_camera_frustum_init( camera, width, height);
		C->app->window->change = 0;
	}

	glViewport( x, y, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double left, right, top, bottom,near,far;

	if( viewport->use_ui)
	{
		left =  0 + camera->left;
		right = width + camera->right;	
		bottom = 0 + camera->bottom;
		top = height + camera->top;
		near = -1;
		far = 1;
	}
	else
	{
		left = camera->left * zoom;
		right = camera->right * zoom;	
		bottom = camera->bottom * zoom;
		top = camera->top * zoom;
		near = camera->ortho_near;
		far = camera->ortho_far;
	}

	glOrtho(
		left,
		right,
		bottom,
		top,
		near,
		far
		);

	viewport->left = left;
	viewport->right = right;
	viewport->bottom = bottom;
	viewport->top = top;
	viewport->near = near;
	viewport->far = far;

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

void viewport_draw_outline( t_viewport *viewport)
{
	t_context *C = ctx_get();
	int w = 1;
	float *color = C->ui->front_color;
	float p[3]={ 0, 0, 0};
	float width, height;
	if( viewport->fullscreen)
	{
		width = C->app->window->width;
		height = C->app->window->height;
	}
	else
	{
		vset( p, viewport->x, viewport->y, 0);
		width = viewport->width;
		height = viewport->height;
	}
	skt_line_rectangle( p, width, height, w, color);
}

void viewport_draw_controls( t_viewport *viewport)
{
	t_block *block = viewport->controls;
	if (block)
	{
		block->cls->draw( block);
	}
}

void viewport_draw(t_viewport *viewport)
{
	t_context *C = ctx_get();

	op_3d_orientation(); 
	draw_scene(C->draw,C->scene);
}

// ADD 

void viewport_rebind( t_scene *sc, void *ptr)
{
	t_viewport *viewport=( t_viewport *) ptr;

	rebind(sc, "viewport", "camera", (void **) &viewport->camera);
	rebind(sc, "viewport", "draw", (void **) &viewport->draw);
	rebind(sc, "viewport", "blocks", (void **) &viewport->controls);
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
		clone->camera = camera_copy( viewport->camera);
		clone->draw = viewport->draw;

		return clone;
	}
	else
	{
		return NULL;
	}
}

void viewport_set_format( t_viewport *viewport, const char *format)
{
	int width, height;
	if( iseq( format, "A4"))
	{
		width = 210;
		height = 297;
	}

	viewport->width = width;
	viewport->height = height;
}

void viewport_set_center( t_viewport *viewport)
{
	t_context *C = ctx_get();
	int v_width = viewport->width;
	int v_height = viewport->height;
	int w_width = C->app->window->width;
	int w_height = C->app->window->height;

	int x = ( w_width - v_width) / 2;
	int y = ( w_height - v_height) / 2;

	viewport->x = x;
	viewport->y = y;
}

void viewport_set_full_height( t_viewport *viewport)
{
	t_context *C = ctx_get();
	int v_width = viewport->width;
	int v_height = viewport->height;
	int w_height = C->app->window->height;

	float ratio = (float) w_height / (float) v_height;
	int n_w = (int) ( ratio * (float) v_width);
	viewport->width = n_w;
	viewport->height = w_height;
}

void *op_viewport_set_dimensions( t_brick *brick)
{
	t_viewport *viewport = ( t_viewport *) brick->data;
	viewport_set_format( viewport, "A4");

	return NULL;
}

void *op_viewport_center( t_brick *brick)
{
	t_viewport *viewport = ( t_viewport *) brick->data;
	viewport_set_center( viewport);
	return NULL;
}

void *op_viewport_full_height( t_brick *brick)
{
	t_viewport *viewport = ( t_viewport *) brick->data;
	viewport_set_full_height( viewport);

	return NULL;
}

void viewport_register( t_context *C)
{
	op_add_register( C, "op_viewport_center", op_viewport_center);
	op_add_register( C, "op_viewport_set_dimensions", op_viewport_set_dimensions);
	op_add_register( C, "op_viewport_full_height", op_viewport_full_height);
}

void viewport_add_trigger( t_context *C, t_viewport *viewport, const char *name, void *( * f)( t_brick *brick))
{
	t_block *block = viewport->controls;
	t_node *node = add_brick_trigger( C, block, name, f);
	t_brick *brick = ( t_brick *) node->data;
	brick->data = viewport;
}

void viewport_add_controls( t_viewport *viewport)
{
	t_context *C = ctx_get();

	scene_add_ref(C->scene,"struct_ref","viewport","width",&viewport->width,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","height",&viewport->height,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","x",&viewport->x,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","y",&viewport->y,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","fullscreen",&viewport->fullscreen,viewport);
	scene_add_ref(C->scene,"struct_ref","viewport","show_outline",&viewport->show_outline,viewport);

	t_block *block = add_block_block( C, "viewport_controls");
	viewport->controls = block;

	set_draw_plug = 0;

	block->pos[0] = 0;
	block->pos[1] = 1;
	add_brick_slider_int( C, block, "width", &viewport->width,NULL);
	add_brick_slider_int( C, block, "height", &viewport->height,NULL);
	add_brick_slider_int( C, block, "x", &viewport->x,NULL);
	add_brick_slider_int( C, block, "y", &viewport->y,NULL);
	add_brick_switch( C, block, "fullscreen", &viewport->fullscreen, NULL);
	add_brick_switch( C, block, "outline", &viewport->show_outline, NULL);

	viewport_add_trigger( C, viewport, "A4", op_viewport_set_dimensions);
	viewport_add_trigger( C, viewport, "center", op_viewport_center);
	viewport_add_trigger( C, viewport, "full height", op_viewport_full_height);

	set_draw_plug = 1;
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

void *viewport_new(const char *name)
{
	t_viewport *viewport = (t_viewport *) mem_malloc(sizeof(t_viewport));

	id_init(&viewport->id, name);

	viewport->width = 0;
	viewport->height = 0;
	viewport->x = 0;
	viewport->y = 0;
	viewport->left = 0;
	viewport->right = 0;
	viewport->bottom = 0;
	viewport->top = 0;
	viewport->near = 0;
	viewport->far = 0;
	viewport->camera = NULL;
	viewport->draw = NULL;
	viewport->show_controls = 0;
	viewport->show_outline = 0;
	viewport->fullscreen = 1;
	viewport->use_fullscreen = 1;
	viewport->use_ui = 1;

	return viewport;
}
