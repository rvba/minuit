/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "base.h"
#include "ctx.h"
#include "node.h"
#include "scene.h"
#include "op.h"
#include "app.h"
#include "event.h"
#include "screen.h"
#include "camera.h"
#include "ui.h"
#include "file.h"
#include "list.h"
#include "block.h"
#include "brick.h"
#include "viewport.h"

// SCREEN BRICKS

void screen_block_add_by_name(t_context *C, t_screen *screen, const char *name)
{
	t_node *node = scene_get_node_by_type_name(C->scene, dt_block, name);
	if( node)
	{
		t_block *block = ( t_block *) node->data;
		screen_block_add(screen, block);
	}
}

// MAKE MAIN

t_screen *screen_main_make( t_context *C)
{
	t_screen *screen = screen_default( "screen_main", screen_main);
	screen->is_active = 1;
	screen->is_visible = 1;

	screen->keymap = keymap_main;

	return screen;
};

t_screen *screen_object_make( t_context *C)
{
	t_screen *screen = screen_default( "screen_object", screen_object);

	screen->is_active = 1;
	screen->is_visible = 1;
	screen->always_active = 1;
	screen->always_visible = 1;
	return screen;
};

// SCREEN DESK

t_screen *screen_sets_make( t_context *C)
{
	t_screen *screen = screen_default( "screen_sets", screen_sets);
	return screen;
};

// SCREEN BRICKS

t_screen *screen_bricks_menu_make( t_context *C)
{
	t_screen *screen = screen_default( "screen_bricks", screen_bricks);

	screen_block_add_by_name(C, screen, "menu_scalar");
	screen_block_add_by_name(C, screen, "menu_time");
	screen_block_add_by_name(C, screen, "menu_operator");
	screen_block_add_by_name(C, screen, "menu_vector");
	screen_block_add_by_name(C, screen, "menu_logic");
	screen_block_add_by_name(C, screen, "menu_maths");

	return screen;
};

t_screen *screen_intro_make( t_context *C)
{
	t_node *node = scene_add( C->scene, dt_screen, "screen_intro");
	t_screen *screen = ( t_screen *) node->data;

	screen->keymap = keymap_generic;
	screen->draw = screen_intro;

	screen->is_active = 1;
	screen->is_visible = 1;
	screen->always_active = 1;
	screen->always_visible = 1;

	lst_add( C->ui->screens, node, "screen_intro");

	// Lst
	t_node *node_lst = scene_add( C->scene, dt_list, "lst");
	t_lst *lst = ( t_lst *) node_lst->data;
	screen->viewports = lst;

	// Viewport
	t_node *node_viewport = scene_add( C->scene, dt_viewport, "viewport_intro");
	t_viewport *viewport = ( t_viewport *) node_viewport->data;

	// Camera
	t_node *node_camera = scene_add( C->scene, dt_camera, "viewport_camera");
	t_camera *camera = ( t_camera *) node_camera->data;
	viewport->camera = camera;
	lst_add(screen->viewports, viewport, "viewport");

	return screen;
};

t_screen *screen_log_make( t_context *C)
{
	t_screen *screen = screen_default( "screen_log", screen_log);

	screen->is_active = 1;
	screen->is_visible = 1;
	screen->always_active = 1;
	screen->always_visible = 1;
	
	return screen;
};

t_screen *screen_info_make( t_context *C)
{
	t_screen *screen = screen_default( "screen_info", screen_info);

	/*
	screen->is_active = 1;
	screen->is_visible = 1;
	screen->always_active = 1;
	screen->always_visible = 1;
	*/
	
	return screen;
};

// SCREEN SWITCH

void op_screen_switch()
{
	t_context *C=ctx_get();

	t_link *l;
	t_node *n;
	int update=0;

	if(C->ui->screen_link_active)
	{
		t_link *link=C->ui->screen_link_active;

		//up
		if(C->ui->screen_direction)
		{
			if(link->next)
			{
				update=1;
				C->ui->screen_link_active=link->next;
			}
			else
			{
				update=1;
				t_lst *lst = scene_lst_get( C->scene, dt_screen);
				C->ui->screen_link_active=lst->first;
				C->ui->screen_direction=1;
			}
		}

		if(update)
		{
			t_lst *lst = scene_lst_get( C->scene, dt_screen);
			for(l=lst->first;l;l=l->next)
			{
				n = ( t_node *) C->ui->screen_link_active->data;
				int id=n->id;
				t_node *this_node = ( t_node *) l->data;
				if(this_node->id==id)
				{
					t_screen *this_screen = ( t_screen *) this_node->data;
					this_screen->is_visible=1;
					this_screen->is_active=1;
				}
				else
				{
					t_screen *this_screen = ( t_screen *) this_node->data;
					this_screen->is_visible=0;
					this_screen->is_active=0;
				}
			}

		}
	}	
}

// INIT

void screen_init( t_context *C)
{
	t_screen *screen_main = screen_main_make( C);
	C->ui->screen_active = screen_main;

	screen_object_make( C);
	screen_browser_make( C);
	screen_sets_make( C);
	screen_bricks_menu_make( C);
	screen_intro_make( C);
	screen_log_make( C);
	screen_info_make( C);
}



	
