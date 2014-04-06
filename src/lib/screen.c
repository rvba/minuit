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




