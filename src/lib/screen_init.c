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

#include "op.h"
#include "ctx.h"
#include "app.h"
#include "event.h"
#include "screen.h"
#include "memory.h"
#include "camera.h"
#include "ui.h"
#include "file.h"
#include "system.h"

#include "list.h"

#include "block.h"
#include "brick.h"

// NEW

t_node *screen_new_generic(char *name)
{
	t_context *C=ctx_get();
	//t_node *node=scene_add(C->scene,"screen",name);
	t_node *node=scene_add(C->scene,nt_screen,name);
	t_screen *screen=node->data;

	screen->keymap=keymap_generic;
	screen->draw=screen_generic;

	screen->is_active=0;
	screen->is_visible=0;

	lst_add(C->ui->screens,node,"screen_generic");

	return node;
};

// MAKE MAIN

void screen_main_make(void)
{
	t_context *C=ctx_get();
	t_node *node=scene_add(C->scene,nt_screen,"screen_main");
	t_screen *screen=node->data;

	screen->keymap=keymap_main;
	screen->draw=screen_main;

	screen->is_active=1;
	screen->is_visible=1;

	t_link *link=lst_add(C->ui->screens,node,"screen_main");
	C->ui->screen_link_active=link;
};

// SCREEN DESK

void screen_sets_make(void)
{
	t_context *C=ctx_get();
	t_node *node=scene_add(C->scene,nt_screen,"screen_sets");
	t_screen *screen=node->data;

	screen->keymap=keymap_main;
	screen->draw=screen_sets;

	screen->is_active=0;
	screen->is_visible=0;

	t_link *link=lst_add(C->ui->screens,node,"screen_sets");
	C->ui->screen_link_active=link;
};

// SCREEN BRICKS

void screen_block_add_by_name(t_context *C, t_screen *screen, const char *name)
{
	t_node *node = scene_node_get(C->scene,"block",name);
	t_block *block = node->data;
	screen_block_add(screen, block);
}

void screen_bricks_make(void)
{
	t_context *C=ctx_get();
	t_node *node=scene_add(C->scene,nt_screen,"screen_bricks");
	t_screen *screen=node->data;

	screen->keymap=keymap_generic;
	screen->draw=screen_bricks;

	screen->is_active=0;
	screen->is_visible=0;

	lst_add(C->ui->screens,node,"screen_bricks");

	screen_block_add_by_name(C, screen, "menu_scalar");
	screen_block_add_by_name(C, screen, "menu_time");
	screen_block_add_by_name(C, screen, "menu_operator");
	screen_block_add_by_name(C, screen, "menu_vector");
	screen_block_add_by_name(C, screen, "menu_logic");
	screen_block_add_by_name(C, screen, "menu_maths");
	screen_block_add_by_name(C, screen, "menu_lst");
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
				C->ui->screen_link_active=C->scene->screens->first;
				C->ui->screen_direction=1;
			}
		}

		if(update)
		{
			for(l=C->scene->screens->first;l;l=l->next)
			{
				n=C->ui->screen_link_active->data;
				int id=n->id;
				t_node *this_node=l->data;
				if(this_node->id==id)
				{
					t_screen *this_screen=this_node->data;
					this_screen->is_visible=1;
					this_screen->is_active=1;
				}
				else
				{
					t_screen *this_screen=this_node->data;
					this_screen->is_visible=0;
					this_screen->is_active=0;
				}
			}

		}
	}	
}










	
