/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "context.h"
#include "scene.h"
#include "node.h"
#include "app.h"
#include "screen.h"
#include "camera.h"
#include "ui.h"
#include "list.h"
#include "txt.h"
#include "block.h"
#include "sketch.h"
#include "event.h"
#include "viewport.h"
#include "camera.h"
#include "memory.h"

void screen_draw_controls( t_screen *screen)
{
	if( opt_viewport_show_controls)
	{
		screen_switch_2d( screen);
		t_viewport *viewport = screen_viewport_get( screen);

		glPushMatrix();
		glLoadIdentity();

		t_block *block = viewport->controls;
		if (block)
		{
			block->cls->draw( block);
		}

		glPopMatrix();
	}
}

t_viewport *screen_viewport_get( t_screen *screen)
{
	t_viewport *viewport = NULL;
	t_lst *lst = screen->viewports;
	if( lst)
	{
		t_link *l = lst->first;
		if( l)
		{
			viewport= l->data;
		}
		else
		{
			printf("Can't find link\n");
		}
	}
	else printf("[SCREEN] Can't find lst\n");
	return viewport;
}

void screen_switch_3d( t_screen *screen)
{
	t_context *C = ctx_get();
	t_viewport *viewport = screen_viewport_get( screen);

	if( viewport)
	{
		t_camera *camera = viewport->camera;
		op_camera_switch_3d(C, camera);
		
		int w = C->app->window->width;
		int h = C->app->window->height;

		if( camera->update_frustum)
		{
			op_camera_frustum_init( camera, w, h);
			C->app->window->change = 0;
		}

		if( viewport->fullscreen) op_camera_update(C, camera);
		else  _op_camera_update(C, camera, viewport);
		
		op_3d_orientation(); 
	}
}

void screen_switch_2d( t_screen *screen)
{
	/*
	t_viewport *viewport = screen_viewport_get( screen);
	if( viewport)
	{
		t_context *C = ctx_get();
		t_camera *camera = viewport->camera;

		int px = 0;
		int py = 0;
		int mx = C->app->mouse->x;
		int my = C->app->mouse->y;

		int width = viewport->width;
		int height = viewport->height;

		camera->ortho_zoom = 1;

		camera_set_frustum( camera, width, height);

		//camera_set_restrict_matrix( camera, mx, my);
		camera->restrict_matrix = 0;
		camera_set_viewport( camera, px, py, width, height);
		camera_set_projection( camera, camera_ortho);
		//_op_camera_switch_2d( C, camera, width, height);
		op_camera_switch_2d( C, camera);
	}
	else
	{
		printf("[SCREEN] Can't find viewport\n");
	}
	*/

	t_context *C = ctx_get();
	t_viewport *viewport = screen_viewport_get( screen);

	if( viewport)
	{
		t_camera *camera = viewport->camera;
		op_camera_update( C, camera);
		op_camera_switch_2d(C,camera);
	}
}

t_screen *screen_default(const char *name, void (* draw)(t_screen *s))
{
	t_context *C=ctx_get();

	t_node *node=scene_add(C->scene,dt_screen,name);
	t_screen *screen=node->data;

	screen->keymap=keymap_generic;
	screen->draw=draw;

	// Add to UI Screens
	lst_add(C->ui->screens,node,name);

	// Lst
	t_node *node_lst = scene_add( C->scene, dt_list, "lst");
	t_lst *lst = node_lst->data;

	screen->viewports = lst;

	// Viewport
	t_node *node_viewport = scene_add( C->scene, dt_viewport, name);
	t_viewport *viewport = node_viewport->data;

	// Camera
	t_node *node_camera = scene_add( C->scene, dt_camera, name);
	t_camera *camera = node_camera->data;

	viewport->camera = camera;
	viewport->width = 1600;
	viewport->height = 1200;

	lst_add(screen->viewports, viewport, name);

	return screen;
}

t_screen *screen_add_3d(const char *name, void (* draw)(t_screen *s))
{
	t_context *C=ctx_get();

	t_node *node=scene_add(C->scene,dt_screen,name);
	t_screen *screen=node->data;

	screen->keymap=keymap_generic;
	screen->draw=draw;

	lst_add(C->ui->screens,node,name);

	// Lst
	t_node *node_lst = scene_add( C->scene, dt_list, "lst_screen");
	t_lst *lst = node_lst->data;

	screen->viewports = lst;

	// Viewport
	t_node *node_viewport = scene_add( C->scene, dt_viewport, name);
	t_viewport *viewport = node_viewport->data;

	// Camera
	t_node *node_camera = scene_add( C->scene, dt_camera, name);
	t_camera *camera = node_camera->data;

	viewport->camera = camera;
	viewport_add_controls( viewport);

	screen->width = C->app->window->width;
	screen->height = C->app->window->height;

	viewport->width = C->app->window->width;
	viewport->height = C->app->window->height;

	list_add_data(screen->viewports, viewport);

	return screen;
}

void screen_always(t_screen *screen)
{
	screen->is_active=1;
	screen->is_visible=1;
	screen->always_active=1;
	screen->always_visible=1;
}

void screen_on(t_screen *screen)
{
	screen->is_visible=1;
	screen->is_active=1;
}

void screen_off(t_screen *screen)
{
	screen->is_visible=0;
	screen->is_active=0;
}

// BLOCK ADD

void screen_block_add(t_screen *screen, t_block *block)
{
	t_context *C = ctx_get();

	if(screen->blocks)
	{
		list_add_data(screen->blocks, block);
	}
	else
	{
		t_node *node = scene_add(C->scene, dt_lst, "lst");
		t_lst *lst = node->data;
		screen->blocks = lst;

		screen_block_add(screen, block);
	}
}

// SWITCH BY NAME

void screen_switch_by_name( const char *name)
{
	t_context *C=ctx_get();
	// check if screen exists
	t_node *node = list_find_node_by_name(C->ui->screens,name);

	if(node)
	{
		t_link *l;

		for(l=C->scene->screens->first;l;l=l->next) 
		{
			t_node *this_node=l->data;
			t_screen *this_screen=this_node->data;

			// enable screen
			if(is(this_screen->id.name, name))
			{
				this_screen->is_visible=1;
				this_screen->is_active=1;
				C->ui->screen_link_active=l;
			}
			// disable screen
			else
			{
				if(!this_screen->always_visible) this_screen->is_visible=0;
				if(!this_screen->always_active) this_screen->is_active=0;
			}
		}
	}	
}

// SWITCH TO MAIN

void screen_switch_to_main(void)
{
	screen_switch_by_name("screen_main");
}

// CLONE

t_screen *screen_clone(t_screen *screen)
{
	if(screen)
	{
		t_screen *clone = screen_new(screen->id.name);

		clone->is_active = screen->is_active;
		clone->is_visible = screen->is_visible;
		clone->always_active = screen->always_active;
		clone->always_visible = screen->always_visible;
		clone->width = screen->width;
		clone->height = screen->height;
		clone->zoom = screen->zoom;
		clone->pan_x = screen->pan_x;
		clone->pan_y = screen->pan_y;

		clone->blocks = list_clone(screen->blocks, dt_block);
		clone->viewports = list_clone(screen->viewports, dt_viewport);

		clone->draw = screen->draw;
		clone->keymap = screen->keymap;

		return clone;
	}
	else
	{
		return NULL;
	}
}

// FREE

void _screen_free(t_screen *screen)
{
	if(screen->blocks) list_free_data(screen->blocks, dt_block);
	if(screen->viewports) list_free_data(screen->viewports,dt_viewport);

	mem_free( screen, sizeof( t_screen));
}

void screen_free(t_screen *screen)
{
}

// REBIND

t_screen *screen_rebind(t_scene *scene, void *ptr)
{
	t_screen *screen = (t_screen *) ptr;

	rebind(scene,"screen","blocks",(void **)&screen->blocks);
	rebind(scene,"screen","viewports",(void **)&screen->viewports);

	if( is( screen->id.name, "screen_view3d"))
	{
		t_context *C = ctx_get();
		C->scene->has_generic_viewport = 1;
	}

	return ptr;
}

// NEW

t_screen *screen_new(const char *name)
{
	t_screen *screen=(t_screen *)mem_malloc(sizeof(t_screen));

	id_init(&screen->id, name);

	screen->is_visible=0;
	screen->is_active=0;
	screen->always_active=0;
	screen->always_visible=0;
	screen->zoom=1;
	screen->pan_x=0;
	screen->pan_y=0;
	screen->draw=NULL;
	screen->keymap=NULL;

	screen->blocks = NULL;
	screen->viewports = NULL;
	screen->data = NULL;
	
	return screen;
}




