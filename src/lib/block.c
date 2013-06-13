/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "scene.h"
#include "node.h"
#include "util.h"
#include "op.h"
#include "ctx.h"
#include "list.h"
#include "block.h"
#include "brick.h"
#include "graph.h"
#include "set.h"

// Reset Update State
void block_reset(t_block *block)
{
	t_link *link;
	t_brick *brick;
	t_plug *plug;

	for(link=block->bricks->first;link;link=link->next)
	{
		brick = link->data;
		plug = &brick->plug_intern;
		plug->state.is_updated = 0;
	}
}

// Trigger Brick
void block_brick_trigger(t_plug *plug)
{
	t_brick *brick = plug->brick;

	if(!plug->state.is_updated)
	{
		brick->cls->trigger(brick);
	}
}

// Open | Close Loop
void block_set_loop_state(t_block *block, int state)
{
	t_graph *graph = block->graph;
	if(graph)
	{
		// Set Loop On
		if(state)
		{
			graph->start_loop = 1;
			graph->end_loop = 0;
		}
		// Set Loop Off
		else
		{
			graph->start_loop = 0;
			graph->end_loop = 1;
		}
	}
}

// Trigger All Bricks
void block_exec(t_block *block)
{
	t_link *link;
	t_brick *brick;
	t_plug *plug;

	// Reset State
	block_reset(block);

	// Trigger
	for(link=block->bricks->first;link;link=link->next)
	{
		brick = link->data;
		plug = &brick->plug_intern;

		// First: Parents
		if(plug->parents)
		{
			t_link *l;
			for(l=plug->parents->first;l;l=l->next)
			{
				t_plug *p = l->data;
				block_brick_trigger(p);
			}
		}

		// Then: this brick
		block_brick_trigger(plug);
	}
}

// Check If Block Is in List
int block_in_lst(t_block *block, t_lst *lst)
{
	t_link *l;
	t_block *b;
	for(l=lst->first;l;l=l->next)
	{
		b = l->data;
		if(b->id == block->id) return 1;
	}
	return 0;
}

// GET GRAPH

t_lst *block_graph_get(t_context *C, t_plug *plug, t_lst *lst)
{
	t_link *l;
	t_plug *p;
	t_plug *d;
	t_brick *brick;
	t_block *block;

	brick = plug->brick;
	block = brick->block;

	// Process Block If Not In List
	// Prevent from Looping
	if(!block_in_lst(block,lst))
	{
		// Add To List
		list_add(lst,block);

		// Check All Bricks in Block
		for(l=block->bricks->first;l;l=l->next)
		{
			brick=l->data;

			// Plug In
			p=&brick->plug_in;

			// If Connected
			if(p->state.is_connected) 
			{
				d = p->src;
				// And Caller is Not the Same (Plug)
				if((d->id != plug->id) && (p->id != plug->id))
				{
					// Get Graph
					block_graph_get(C,d,lst);
				}
			}

			// Plug Out
			p=&brick->plug_out;

			// If Connected
			if(p->state.is_connected)
			{
				d = p->dst;
				// And Caller is Not the Same (Plug)
				if((d->id != plug->id) && (p->id != plug->id))
				{
					// Get Graph
					block_graph_get(C,d,lst);
				}
			}
		}
	}

	return lst;
}

// SPLIT

void block_graph_split(t_block *block_self, t_plug *plug_self, t_block *block_dst, t_plug *plug_dst)
{
	t_context *C = ctx_get();

	block_self->state.is_root = 0;
	block_dst->state.is_root = 0;

	t_lst *lst_self = lst_new("lst");
	t_lst *lst_dst = lst_new("lst");

	// Get Graph For Both BLocks
	block_graph_get(C,plug_self,lst_self);
	block_graph_get(C,plug_dst,lst_dst);

	// IF Graphs Are Not Equal (means One Block is not in Both Graphs)
	if(!block_in_lst(block_self,lst_dst))
	{
		t_graph *old_graph = block_self->graph;

		// Buil New Graph
		if(lst_self->tot > 1)
		{
			graph_build_from_list(lst_self);
		}
		// Or Not
		else
		{
			graph_block_remove(block_self->graph, block_self);
			block_self->set = get_current_set(C); 

		}

		if(lst_dst->tot > 1)
		{
			graph_build_from_list(lst_dst);
		}
		else
		{
			graph_block_remove(block_dst->graph, block_dst);
			block_dst->set = get_current_set(C); 
		}

		// Delete Old Graph
		graph_delete(old_graph);
	}

	free(lst_self);
	free(lst_dst);
}

// Add Block To Graph
void block_graph_add(t_block *self, t_block *dst)
{
	t_context *C = ctx_get();

	// Each Block Have A Graph
	if(self->graph && dst->graph)
	{
		graph_merge(self->graph,dst->graph);
	}
	// Only One Graph
	else if(self->graph)
	{
		graph_block_add(self->graph, dst);
	}
	// Only One Graph
	else if(dst->graph)
	{
		graph_block_add(dst->graph, self);
	}
	// No Graph
	else
	{
		// Build New Graph
		C->scene->store = 1;
		t_graph *graph = graph_add("graph");
		C->scene->store = 0;

		// Add Blocks
		graph_block_add(graph, self);
		graph_block_add(graph, dst);
	}
}

void block_set_graph_order(t_block *block, int order)
{
	t_link *link;
	t_brick *brick;

	block->graph_order = order;

	for(link = block->bricks->first; link; link = link->next)
	{
		brick = link->data;
		brick->graph_order = order;
	}
}

int block_is_in_lst(t_lst *lst, t_block *block_dst)
{
	if(lst->first)
	{
		t_link *link;
		t_block *block_src;

		for(link = lst->first; link; link = link->next)
		{
			block_src  = link->data;
			if(block_src->id == block_dst->id)
			{
				return 1;
			}
		}

		return 0;
	}
	else
	{
		return 0;
	}
}

int block_is_connected(const char *gate, t_block *block)
{
	t_link *link;
	t_brick *brick;
	t_plug *plug;

	int plug_in;
	int follow;

	if(is(gate,"in")) plug_in = 1;
	else plug_in = 0;

	if(block->bricks->first)
	{
		for(link = block->bricks->first; link; link = link->next)
		{
			brick = link->data;

			if(plug_in)
			{
				plug = &brick->plug_in;
				follow = plug->state.follow_in;
			}
			else
			{
				plug = &brick->plug_out;
				follow = plug->state.follow_out;
			}

			if(plug->state.is_connected && follow)
			{
				return 1;
			}

		}

		return 0;
	}
	else
	{
		return 0;
	}

}

t_lst *block_get_connections(const char *gate,t_block *block)
{
	t_link *link;
	t_brick *brick;
	t_brick *brick_dst;
	t_block *block_dst;
	t_plug *plug_dst;
	t_plug *plug;
	int plug_in;
	int follow;
	t_lst *lst = NULL;

	if(is(gate,"in")) plug_in = 1;
	else plug_in = 0;

	if(block->bricks->first)
	{
		for(link = block->bricks->first; link; link = link->next)
		{
			brick = link->data;

			if(plug_in)
			{
				plug = &brick->plug_in;
				follow = plug->state.follow_in;
			}
			else
			{
				plug = &brick->plug_out;
				follow = plug->state.follow_out;
			}

			if(plug->state.is_connected && follow)
			{
				if(!lst) lst = lst_new("connections");

				if(plug_in) plug_dst = plug->src;
				else plug_dst = plug->dst;

				brick_dst = plug_dst->brick;

				block_dst = brick_dst->block;

				if(!block_is_in_lst(lst,block_dst))
				{
					lst_add(lst,block_dst,"block");
				}
			}

		}
	}

	return lst;
}

void _add_block(t_context *C,t_block *block)
{
	// get list
	t_set *set = get_current_set(C);
	t_lst *list = set->blocks;

	// add to main list
	list_add_global(list,block);
}

t_block *block_dupli(t_block *block)
{
	t_context *C=ctx_get();
	C->scene->store=1;

	t_node *clone_node=block_make(block->name,block->type);
	t_block *clone_block=clone_node->data;

	clone_block->state.draw_outline = block->state.draw_outline;

	_add_block(C,clone_block);

	vcp(clone_block->pos,block->pos);

	t_link *l;
	t_brick *b;

	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		brick_dupli(clone_block,b);
	}

	C->scene->store=0;

	return clone_block;
}

t_block *block_copy(t_block *block)
{
	t_context *C=ctx_get();
	C->scene->store=1;

	t_node *clone_node = block_make(block->name,block->type);
	t_block *clone_block = clone_node->data;

	clone_block->state.draw_outline = block->state.draw_outline;

	_add_block(C,clone_block);

	vcp(clone_block->pos,block->pos);

	t_link *l;
	t_brick *b;

	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		brick_copy(clone_block,b);
	}

	C->scene->store=0;

	return clone_block;
}

void block_init(t_scene *sc,t_block *block);

void _block_init(t_block *block)
{
	t_context *C=ctx_get();
	block_init(C->scene,block);
}

void cls_block_link(t_block *self,t_node *target)
{
	if(target->type==nt_list)
	{
		t_lst *lst=target->data;
		self->bricks=lst;
		target->users++;
	}

	else printf("[ERROR:cls_block_link] Unknown node type %s",node_name_get(target->type));
}

void cls_block_link(t_block *self,t_node *target);
void cls_block_make(t_block *block);
void cls_block_make_block(t_block *block);
void cls_block_make_menu(t_block *block);
void cls_block_make_ref(t_block *block);

// menu
t_block_class block_menu =
{
	.cls_type="block",
	.type="menu",
	.make=cls_block_make_menu,
	.link=cls_block_link,
	.draw=cls_block_draw_block,
	.update=cls_block_menu_update,
	.init=_block_init,
};

// block
t_block_class block_block =
{
	.cls_type="block",
	.type="block",
	.make=cls_block_make_block,
	.link=cls_block_link,
	.draw=cls_block_draw_block,
	.update=cls_block_block_update,
	.init=_block_init,
};

void cls_block_make_block(t_block *block)
{
	block->cls=&block_block;
}

void cls_block_make_menu(t_block *block)
{
	block->cls=&block_menu;
}

t_block_class *blocks[] = {
	&block_menu,
	&block_block,
	};

void block_cls_init(t_block *block)
{
	int i;
	int n = sizeof(blocks)/sizeof(blocks[0]);
	int found=0;
	for(i=0;i<n;i++)
	{
		if(is(block->type,blocks[i]->type))
		{
			blocks[i]->make(block);
			found=1;
			break;
		}
	}


	if(!found)printf("[ERROR:cls_block_init] Unknown block type %s\n",block->type);
}

void block_brick_add(t_block *block,t_node *node_brick)
{
	t_brick *brick=node_brick->data;
	brick->block=block;
	brick->geom.block_pos=block->tot_bricks;
	block->tot_bricks++;

	list_add(block->bricks,brick);
}

t_brick *block_brick_get(t_block *block,const char *name)
{
	t_brick *brick;
	t_link *l;

	for(l=block->bricks->first;l;l=l->next)
	{
		brick=l->data;
		if(is(brick->name,name))
		{
			return brick;
		}
	}

	printf("[ERROR block_brick_get] Can't find brick %s\n",name);

	return NULL;
}

t_brick *block_brick_get_by_order(t_block *block, int order)
{
	t_brick *brick;
	t_link *l;

	for(l=block->bricks->first;l;l=l->next)
	{
		brick=l->data;
		if(brick->block_order == order)
		{
			return brick;
		}
	}

	return NULL;
}

void block_brick_init(t_node *node_brick)
{
	t_brick *brick=node_brick->data;
	brick->state.draw_plugs=1;
}

void block_operator_brick_init(t_node *node_brick)
{
	t_brick *brick=node_brick->data;
	brick->state.draw_plugs=1;
	brick->state.draw_outline=0;
	brick->state.draw_name=0;
}

// CLONE

t_block *block_clone(t_block *block)
{
	if(block)
	{
		t_block *clone = block_new(block->name);

		vcp3i(clone->idcol,block->idcol);
		set_name(clone->type,block->type);
		vcp3f(clone->pos,block->pos);
		clone->width = block->width;
		clone->height = block->height;
		clone->state = block->state;
		clone->tot_bricks = block->tot_bricks;
		clone->graph_order = block->graph_order;
		clone->graph_pos = block->graph_pos;
		clone->bricks = lst_clone(block->bricks,dt_brick);

		clone->submenu = NULL;
		clone->selected = NULL; 
		clone->graph = NULL; 
		
		//XXX init cls ???
		clone->cls = block->cls;
		
		return clone;
	}
	else
	{
		return NULL;
	}
}


// INIT

void block_init(t_scene *sc,t_block *block)
{
	block_cls_init(block);
}

// REBIND

t_block *block_rebind(t_scene *sc,void *ptr)
{
	t_block *block=(t_block *)ptr;

	rebind(sc,"block","bricks",(void **)&block->bricks);
	rebind(sc,"block","graph",(void **)&block->graph);
	rebind(sc,"block","set",(void **)&block->set);

	// reset
	block->selected=NULL;
	block->submenu=NULL;

	t_context *C=ctx_get();
	block_init(C->scene,block);

	return block;
}

// MAKE

t_node *block_make(const char *name,const char *type)
{
	t_context *C=ctx_get();

	t_node *n_block=scene_add(C->scene,nt_block,name);
	t_node *n_list=scene_add(C->scene,nt_list,name);

	t_block *block=n_block->data;

	set_name(block->type,type);

	block_init(C->scene,block);

	block->cls->link(block,n_list);

	return n_block;
}

void _block_free(t_block *block)
{
	if(block->bricks) _list_free(block->bricks, dt_brick);
	free(block);
}

// FREE BRICKS

void block_bricks_free(t_block *block)
{
	t_context *C=ctx_get();

	t_scene *sc=C->scene;

	t_link *l;
	t_brick *b;

	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		scene_struct_delete(sc,b);
	}
}

// FREE BLOCK

void block_free(t_block *block)
{
	t_context *C=ctx_get();
	t_scene *sc=C->scene;

	// free bricks
	block_bricks_free(block);

	// free lst
	scene_struct_delete(sc,block->bricks);
}

// NEW

t_block *block_new(const char *name)
{
	t_block *block  = (t_block *)malloc(sizeof(t_block));

	block->id=0;
	block->id_chunk=0;
	set_name(block->name,name);
	block->users=0;
	bzero(block->type,_NAME_);

	block->pos[0]=1;
	block->pos[1]=0;
	block->pos[2]=0;
	block->pos[3]=1;
	block->height=0;

	block->bricks=NULL;

	block->submenu=NULL;
	block->selected=NULL;

	block->state.is_root=0;
	block->state.is_show=0;
	block->state.draw_outline=0;
	block->state.draw_plugs=0;
	block->state.is_mouse_over=0;
	block->state.update_geometry=1;
	block->state.is_moveable = 1;
	block->state.is_a_loop = 0;
	block->state.is_in_graph = 0;

	block->tot_bricks=0;
	block->width=0;
	block->graph_order = -1;
	block->graph_pos = 0;

	block->graph = NULL;
	block->set = NULL;

	return block;
}

