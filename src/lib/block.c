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
#include "rhizome.h"
#include "set.h"
#include "graph.h"
#include "memory.h"

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
	t_rhizome *rhizome = block->rhizome;
	if(rhizome)
	{
		// Set Loop On
		if(state == 1)
		{
			rhizome->start_loop = 1;
			rhizome->end_loop = 0;
		}
		// Set Loop Off
		else if(state == 2)
		{
			rhizome->start_loop = 0;
			rhizome->end_loop = 1;
		}
		else 
		{
			rhizome->start_loop = 0;
			rhizome->end_loop = 0;
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
		if(b->id.id == block->id.id) return 1;
	}
	return 0;
}

void block_set_rhizome_order(t_block *block, int order)
{
	t_link *link;
	t_brick *brick;

	block->rhizome_order = order;

	for(link = block->bricks->first; link; link = link->next)
	{
		brick = link->data;
		brick->rhizome_order = order;
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
			if(block_src->id.id == block_dst->id.id)
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

	list_add_data(list, block);
}

t_block *block_dupli(t_block *block)
{
	t_context *C=ctx_get();
	scene_store(C->scene,1);

	t_node *clone_node=add_block(C,block->id.name);
	t_block *clone_block=clone_node->data;

	vcp(clone_block->pos,block->pos);

	t_link *l;
	t_brick *b;

	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		brick_dupli(clone_block,b);
	}

	scene_store(C->scene,0);

	return clone_block;
}

t_block *block_copy(t_block *block)
{
	t_context *C=ctx_get();
	scene_store(C->scene,1);

	t_node *clone_node = block_make(block->id.name,block->type);
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

	scene_store(C->scene,0);

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
	if(target->type==dt_list)
	{
		t_lst *lst=target->data;
		self->bricks=lst;
		target->users++;
	}

	else printf("[ERROR:cls_block_link] Unknown node type %s",data_name_get(target->type));
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

	list_add_data(block->bricks,brick);
}

t_brick *block_brick_get(t_block *block,const char *name)
{
	t_brick *brick;
	t_link *l;

	for(l=block->bricks->first;l;l=l->next)
	{
		brick=l->data;
		if(is(brick->id.name,name))
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
		t_block *clone = block_new(block->id.name);

		vcp3i(clone->idcol,block->idcol);
		set_name(clone->type,block->type);
		vcp3f(clone->pos,block->pos);
		clone->width = block->width;
		clone->height = block->height;
		clone->state = block->state;
		clone->tot_bricks = block->tot_bricks;
		clone->rhizome_order = block->rhizome_order;
		clone->rhizome_pos = block->rhizome_pos;
		clone->bricks = list_clone(block->bricks,dt_brick);

		clone->submenu = NULL;
		clone->selected = NULL; 
		clone->rhizome = NULL; 
		
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
	rebind(sc,"block","rhizome",(void **)&block->rhizome);
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

	t_node *n_block=scene_add(C->scene,dt_block,name);
	t_node *n_list=scene_add(C->scene,dt_list,name);

	t_block *block=n_block->data;

	set_name(block->type,type);

	block_init(C->scene,block);

	block->cls->link(block,n_list);

	return n_block;
}

void _block_free(t_block *block)
{
	if(block->bricks) list_free_data(block->bricks, dt_brick);
	mem_free( block, sizeof( t_block));
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
	t_block *block  = (t_block *)mem_malloc(sizeof(t_block));

	id_init(&block->id, name);

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
	block->state.is_in_rhizome = 0;
	block->state.frame_based = 0;

	block->tot_bricks=0;
	block->width=0;
	block->rhizome_order = -1;
	block->rhizome_pos = 0;

	block->rhizome = NULL;
	block->set = NULL;

	return block;
}

