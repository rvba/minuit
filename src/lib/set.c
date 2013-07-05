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
#include "ui.h"
#include "file.h"

#include "term.h"

#include "list.h"

#include "block.h"
#include "brick.h"

#include "set.h"
#include "rhizome.h"


void set_setup(t_set *set)
{
	t_link *link;
	t_rhizome *rhizome;
	t_block *block;

	int frame_based = 0;

	if(set->blocks)
	{
		for(link=set->blocks->first;link;link=link->next)
		{
			block = link->data;
			if(block->state.frame_based) frame_based = 1;
		}
	}

	if(set->rhizomes)
	{
		for(link=set->rhizomes->first;link;link=link->next)
		{
			rhizome = link->data;
			if(rhizome->frame_based) frame_based = 1;
		}
	}

	set->frame_based = frame_based;

	// Do Process
	set->process = 1;
	set->processing = 0;
}

void set_exec(t_set *set)
{
	t_link *l;

	if(set->blocks)
	{
		l = set->blocks->first;

		for(;l;l=l->next)
		{
			t_block *b;
			b=l->data;

			// Exec Single Blocks
			block_exec(b);
		}
	}

	if(set->rhizomes)
	{
		l = set->rhizomes->first;

		for(;l;l=l->next)
		{
			t_rhizome *g;
			g=l->data;

			// Exec Graphs
			rhizome_exec(g);
		}
	}

	set->process = 0;
//	set->processing = 0;
}

void set_draw(t_set *set)
{
	t_link *l;

	if(set->blocks)
	{
		l = set->blocks->first;

		for(;l;l=l->next)
		{
			t_block *b;
			b=l->data;

			// Draw Single Blocks
			b->cls->draw(b);
		}
	}

	if(set->rhizomes)
	{
		l = set->rhizomes->first;

		for(;l;l=l->next)
		{
			t_rhizome *g;
			g=l->data;

			// Draw Graphs
			rhizome_draw(g);
		}
	}
}
	

void set_block_pop(t_set *set, t_block *block)
{
	t_lst *lst = set->blocks;
	list_remove_by_id(lst, block->id);

	set_setup(set);
}

void set_block_push(t_set *set, t_block *block)
{
	t_context *C = ctx_get();
	t_lst *lst = set->blocks;
	scene_store(C->scene,1);
	list_add(lst, block);
	scene_store(C->scene,0);

	block->set = set;

	// Setup
	set_setup(set);
}

t_set *set_rebind(t_scene *sc, void **ptr)
{
	t_set *set=(t_set *)ptr;

	rebind(sc,"set","blocks",(void **)&set->blocks);
	rebind(sc,"set","rhizomes",(void **)&set->rhizomes);

	return set;
}

t_node *set_add(const char *name)
{
	t_node *node_set = set_make(name);
	return node_set;
}

// MAKE

t_node *set_make(const char *name)
{
	t_context *C = ctx_get();

	// New Set
	t_node *node_set = scene_add(C->scene,nt_set,name);
	t_set *set = node_set->data;

	// Add Blocks List
	t_node *node_blocks = scene_add(C->scene,nt_list,name);
	t_lst *lst_blocks = node_blocks->data;
	set->blocks = lst_blocks;

	// Add Graphs List
	t_node *node_rhizomes = scene_add(C->scene,nt_list,name);
	t_lst *lst_rhizomes = node_rhizomes->data;
	set->rhizomes = lst_rhizomes;

	return node_set;
};

// CLONE

t_set *set_clone(t_set *set)
{
	if(set)
	{
		t_set *clone = set_new(set->name);
		clone->blocks = lst_clone(set->blocks, dt_block);
		clone->rhizomes = lst_clone(set->rhizomes, dt_rhizome);

		return clone;
	}
	else
	{
		return NULL;
	}
}

void _set_free(t_set *set)
{
	if(set->blocks) _list_free(set->blocks, dt_block);
	if(set->rhizomes) _list_free(set->rhizomes, dt_rhizome);

	free(set);
}

// FREE

void set_free(t_set *set)
{
	t_context *C = ctx_get();
	if(set->blocks) scene_struct_delete(C->scene,set->blocks);
	if(set->rhizomes) scene_struct_delete(C->scene,set->rhizomes);
}

// NEW
	
t_set *set_new(const char *name)
{
	t_set *set = (t_set *)malloc(sizeof(t_set));

	set->id = 0;
	set->id_chunk = 0;
	set_name(set->name,name);
	set->users = 0;

	set->blocks = NULL;
	set->rhizomes = NULL;

	set->frame_based = 0;
	set->process = 0;
	set->process_id = 0;
	set->processing = 0;

	return set;
}


