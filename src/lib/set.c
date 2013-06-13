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

void set_block_pop(t_set *set, t_block *block)
{
	t_lst *lst = set->blocks;
	list_remove_by_id(lst, block->id);
}

void set_block_push(t_set *set, t_block *block)
{
	t_context *C = ctx_get();
	t_lst *lst = set->blocks;
	C->scene->store = 1;
	list_add(lst, block);
	C->scene->store = 0;
}

t_set *set_rebind(t_scene *sc, void **ptr)
{
	t_set *set=(t_set *)ptr;

	rebind(sc,"set","blocks",(void **)&set->blocks);
	rebind(sc,"set","graphs",(void **)&set->graphs);

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
	t_node *node_graphs = scene_add(C->scene,nt_list,name);
	t_lst *lst_graphs = node_graphs->data;
	set->graphs = lst_graphs;

	return node_set;
};

// CLONE

t_set *set_clone(t_set *set)
{
	if(set)
	{
		t_set *clone = set_new(set->name);
		clone->blocks = lst_clone(set->blocks, dt_block);
		clone->graphs = lst_clone(set->graphs, dt_graph);

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
	if(set->graphs) _list_free(set->graphs, dt_graph);

	free(set);
}

// FREE

void set_free(t_set *set)
{
	t_context *C = ctx_get();
	if(set->blocks) scene_struct_delete(C->scene,set->blocks);
	if(set->graphs) scene_struct_delete(C->scene,set->graphs);
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
	set->graphs = NULL;

	return set;
}


