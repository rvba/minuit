/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

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

// MAKE DESK

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










	
