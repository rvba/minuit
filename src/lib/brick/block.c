/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "ctx.h"
#include "ui.h"
#include "scene.h"
#include "node.h"
#include "base.h"
#include "op.h"
#include "list.h"
#include "block.h"
#include "brick.h"
#include "rhizome.h"
#include "set.h"
#include "graph.h"

void cls_block_link(t_block *self,t_node *target);
void cls_block_make(t_block *block);
void cls_block_make_block(t_block *block);
void cls_block_make_menu(t_block *block);
void cls_block_make_bar(t_block *block);
void cls_block_make_ref(t_block *block);


/*
	**********************************
	BRANCHES
	**********************************
*/

// Get Plug Pointer
t_plug *get_plug( t_brick *brick, int dir)
{
	if( dir == EAST) return &brick->plug_out;
	else return &brick->plug_in;
}

// Get Brick connected to this Brick
t_brick *get_brick_connected( t_brick *brick, int dir)
{
	t_plug *plug = get_plug( brick, dir);
	t_plug *p;
	t_brick *target = NULL;
	if( dir == EAST)
	{
		p = plug->dst;
		if( p) target = p->brick;
	}
	else
	{
		p = plug->src;
		if( p) target = p->brick;
	}

	return target;
}

// Get Block connected to this brick
t_block *get_block_connected( t_brick *brick, int dir)
{
	t_block *block = NULL;
	t_brick *target = get_brick_connected( brick, dir);
	if( target) block = target->block;
	return block;
}

void get_all_block_connected( t_block *block, t_lst *lst, int dir)
{
	t_link *l;
	t_brick *brick;
	for( l = block->bricks->first; l; l= l->next)
	{
		brick = l->data;
		t_block *b = get_block_connected( brick, dir);
		if( b) lst_add( lst, b, "b");
	}
}

int block_in_list( t_block *block, t_lst *lst)
{
	t_link *l;
	t_block *b;
	for( l = lst->first; l; l = l->next)
	{
		b = l->data;
		if( b->id.id == block->id.id)
		{
			return 1;
		}
	}

	return 0;
}

void get_branch( t_block *block, t_lst *lst, int dir)
{
	if( !block_in_list( block, lst))
	{
		lst_add( lst, block, block->id.name);

		t_lst *bricks = block->bricks;
		t_brick *brick;
		t_block *target;
		t_link *l;

		for( l = bricks->first; l; l = l->next)
		{
			brick = l->data;
			target = get_block_connected( brick, dir);
			if( target) get_branch( target, lst, dir);
		}
	}
}

t_lst *block_branch_get( t_block *block, int dir)
{
	t_lst *branch = lst_new( "branch");
	t_lst *bricks = block->bricks;
	t_link *l;
	t_block *target;
	t_brick *brick;

	for( l = bricks->first; l; l = l->next)
	{
		brick = l->data;
		target = get_block_connected( brick, dir);
		if( target) get_branch( target, branch, dir);
	}

	return branch;
}

t_lst *block_leaves_get( t_block *block, int dir)
{
	t_lst *branch = lst_new( "branch");
	t_lst *bricks = block->bricks;
	t_link *l;
	t_block *target;
	t_brick *brick;

	for( l = bricks->first; l; l = l->next)
	{
		brick = l->data;
		target = get_block_connected( brick, dir);
		if( target) lst_add( branch, target, target->id.name);
	}

	return branch;
}

/*
	**********************************
	BRANCHES
	**********************************
*/

#define DIR_ALL 11

t_block *get_nearest( t_block *block, t_lst *lst)
{
	t_link *l;
	t_block *block_target;
	int order = block->rhizome_pos;
	printf("order:%d\n", order);
	for( l = lst->first; l; l = l->next)
	{
		block_target = l->data;
		printf("o: %d\n", block_target->rhizome_pos);
		if( block_target->rhizome_pos == (order - 1))
		{
			return block_target;
		}
	}

	return NULL;
}

void get_bounding_box( t_block *block, float margin, float *box)
{
	float width = block->width;
	float height = block->height;
	float x = block->pos[0];
	float y = block->pos[1];

	box[0] = x - margin;
	box[1] = y - margin;

	box[2] = box[0] + width + margin + margin;
	box[3] = box[1];

	box[4] = box[2];
	box[5] = box[1] +  height + margin + margin;

	box[6] = box[0];
	box[7] = box[5];

	if(0)
	{
	printf("box %s %f %f %f %f %f %f %f %f %f %f\n", 
		block->id.name,
		width, 
		height,
		box[0],
		box[1],
		box[2],
		box[3],
		box[4],
		box[5],
		box[6],
		box[7]
		);
	}

}

int is_inside( float *box, float x, float y)
{
	float box_lower_x = box[0];
	float box_lower_y = box[1];
	float box_upper_x = box[4];
	float box_upper_y = box[5];

	return( ( x >= box_lower_x) && ( x <= box_upper_x) && ( y >= box_lower_y) && ( y <= box_upper_y));
}

int test_bounding_box( float *b1, float *b2) // big small
{

	int a = is_inside( b1, b2[0], b2[1]);
	int b = is_inside( b1, b2[2], b2[3]);
	int c = is_inside( b1, b2[4], b2[5]);
	int d = is_inside( b1, b2[6], b2[7]);

	//printf("%d %d %d %d\n", a, b, c, d);

	if( !a && !b && !c && !d)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


void displace( t_block *src, t_block *dst, int dir)
{
	float *src_pos = src->pos;
	float *dst_pos = dst->pos;
	float d = 2;

	switch( dir)
	{
		case DIR_ALL:
			if( dst_pos[0] <= src_pos[0]) { dst_pos[0] -=2; }
			else { dst_pos[0] +=2; }

			if( dst_pos[1] <= src_pos[1]) { dst_pos[1] -=2; }
			else { dst_pos[1] +=2; }

			break;
		case WEST:
			dst_pos[0] -= d;
			break;
		case EAST:
			dst_pos[0] += d;
			break;

		case NORTH:
			dst_pos[1] += d;
			break;

		case SOUTH:
			dst_pos[1] -= d;
			break;

		default:
			break;
	}
}

void displace_tree( t_lst *lst, int dir)
{
	float delta;
	int indice;
	switch( dir)
	{
		case( NORTH): delta = 2; indice = 2; break;
		default: printf("displace_tree ????\n");
	}

	t_link *l;
	t_block *block;

	for( l = lst->first; l; l = l->next)
	{
		block = l->data;
		block->pos[indice] += delta;
	}
}
	
void drive_away( t_block *block, t_block *block_nearest, int dir)
{
	float box_block[8];
	float box_nearest[8];
	float margin = 5;

	get_bounding_box( block, margin ,box_block);
	get_bounding_box( block_nearest, margin, box_nearest);

	float *big;
	float *small;

	if( block->width >= block_nearest->width && block->height >= block_nearest->height)
	{
		big = box_block;
		small = box_nearest;
	}
	else
	{
		big = box_nearest;
		small = box_block;
	}

	int test = test_bounding_box( big, small);

	if( test)
	{
		displace( block, block_nearest, dir);
		drive_away( block, block_nearest, dir);
	}
}

void justify( t_block *block, t_block *block_nearest, int dir)
{
	float x = block_nearest->pos[0];
	float y = block_nearest->pos[1];
	float m = 60;
	switch( dir)
	{
		case WEST:
			if( x + block_nearest->width + m >= block->pos[0])
			{
				displace( block, block_nearest, dir);
				justify( block, block_nearest, dir);
			}
			break;
		case EAST:
			if( x <= block->pos[0] +block->width + m)
			{
				displace( block, block_nearest, dir);
				justify( block, block_nearest, dir);
			}
			break;

		case NORTH:
			if( y <= block->pos[1] + block->height + m)
			{
				displace( block, block_nearest, dir);
				justify( block, block_nearest, dir);
			}
			break;

		case SOUTH:
			if( y <= block->pos[1] + block->height + m)
			{
				displace( block, block_nearest, dir);
				justify( block, block_nearest, dir);
			}
			break;
	}
}

void get_bigger_box( float *origin, float *test)
{
	if( test[0] < origin[0]) origin[0] = test[0];
	if( test[1] < origin[1]) origin[1] = test[1];
	if( test[2] > origin[2]) origin[2] = test[2];
	if( test[3] < origin[3]) origin[3] = test[3];
	if( test[4] > origin[4]) origin[4] = test[4];
	if( test[5] > origin[5]) origin[5] = test[5];
	if( test[6] < origin[6]) origin[6] = test[6];
	if( test[7] > origin[7]) origin[7] = test[7];
}

void get_branch_bounding_box( t_block *block, t_lst *lst, float *box_tree, int dir)
{
	t_link *l;
	t_block *block_leaf;
	float box_leaf[8];
	float margin = 0;
	for( l = lst->first; l; l = l->next)
	{
		block_leaf = l->data;
		get_bounding_box( block_leaf, margin, box_leaf);
		get_bigger_box( box_tree, box_leaf);
	}
}

void do_justify_tree( t_block *block, t_lst *tree, float *box_previous, float *box_current, int dir)
{
	float previous_y = box_previous[5];
	float current_y = box_current[5];
	switch( dir)
	{
		case NORTH:
			if( current_y <= previous_y)
			{
				displace_tree( tree, dir);
				get_branch_bounding_box( block, tree, box_current, dir);
				do_justify_tree( block, tree, box_previous, box_current, dir);
			}
			break;
		default:
			printf("do_justify_tree ???\n");
			break;
	}
}

void justify_tree( t_block *block_previous, t_block *block_current, int dir)
{
	float box_previous[8];
	float box_current[8];

	bzero( box_previous, 8);
	bzero( box_current, 8);

	t_lst *lst_previous = block_branch_get( block_previous, dir);
	t_lst *lst_current = block_branch_get( block_current, dir);

	get_branch_bounding_box( block_previous, lst_previous, box_previous, dir);
	get_branch_bounding_box( block_current, lst_current, box_current, dir);

	do_justify_tree( block_current, lst_current, box_previous, box_current, dir);

	lst_free( lst_previous);
	lst_free( lst_current);
}

void block_arrange( t_block *block)
{
	t_lst *lst = block_leaves_get( block, WEST);

	t_link *l;
	for( l = lst->first; l; l = l->next)
	{
		t_block *block_nearest = l->data;
		drive_away( block, block_nearest, WEST);
		block_arrange( block_nearest);
	}

	lst_free( lst);
}

void block_justify( t_block *block, int dir)
{
	t_lst *lst = block_leaves_get( block, dir);

	t_link *l;
	t_block *previous = NULL;
	for( l = lst->first; l; l = l->next)
	{
		t_block *block_nearest = l->data;
		justify( block, block_nearest, dir);

		if( previous)
		{
			if( previous->id.id != block_nearest->id.id)
			{
				justify( previous, block_nearest, NORTH);
				//justify_tree( previous, block_nearest, NORTH);
			}
		}

		// Go Recursive
		block_justify( block_nearest, dir);

		previous = block_nearest;
	}

	lst_free( lst);
}


/*
	**********************************
	RHIZOME
	**********************************
*/


void block_set_setup( t_block *block)
{
	t_set *set = block->set;
	if( set) set_set_process( set);
}

void block_get_pos_plug_out( t_block *block, t_brick *brick, float *v)
{
	float pos_x = block->pos[0];
	float pos_y = block->pos[1];

	float plug_width = brick->geom.height;
	float brick_width = brick->geom.width;
	int pos = brick->geom.block_pos;

	float x = pos_x + brick_width ;
	float y = pos_y + ((pos + 1) * plug_width) - ( plug_width / 2);
	float z = 0;

	v[0] = x;
	v[1] = y;
	v[2] = z;
}

void block_get_center( t_block *block, float *v)
{
	float width = block->width;
	float height = block->height;
	vset( v, width / 2, height / 2, 0);
}

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

	// Reset State
	block_reset(block);

	// Trigger
	for(link=block->bricks->first;link;link=link->next)
	{
		brick = link->data;
		if( !brick->brick_state.hold)
		{
			brick->exe( brick);
			if( brick->brick_state.singular && brick->brick_state.use_hold)
			{
				brick->brick_state.hold = 1;
			}
		}
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

void block_brick_set_order( t_block *block)
{
	t_link *l;
	t_brick *brick;
	int pos = 0;
	for(l=block->bricks->first;l;l=l->next)
	{
		brick = l->data;
		brick->geom.block_pos = pos;
		pos++;
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

	clone_block->pos[0]+=10;
	clone_block->pos[1]+=10;

	clone_block->block_state.draw_clone_link = 1;
	t_link *l;
	t_brick *b;

	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		brick_dupli(clone_block,b);
	}

	if( block->clone)
	{
		clone_block->clone = block->clone;
	}
	else
	{
		clone_block->clone = block;
	}

	t_block *block_parent = clone_block->clone;

	block_parent->clones+=1;

	scene_store(C->scene,0);

	return clone_block;
}

t_block *block_copy(t_block *block)
{
	t_context *C=ctx_get();
	scene_store(C->scene,1);

	t_node *clone_node = block_make(block->id.name,block->type);
	t_block *clone_block = clone_node->data;

	clone_block->block_state.draw_outline = block->block_state.draw_outline;

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

// menu
t_block_class block_menu =
{
	.cls_type="block",
	.type="menu",
	.make=cls_block_make_menu,
	.link=cls_block_link,
	.draw=cls_block_draw_menu,
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
	.init=_block_init,
};

// bar
t_block_class block_bar =
{
	.cls_type="block",
	.type="bar",
	.make=cls_block_make_bar,
	.link=cls_block_link,
	.draw=cls_block_draw_bar,
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

void cls_block_make_bar(t_block *block)
{
	block->cls=&block_bar;
}

t_block_class *blocks[] = {
	&block_menu,
	&block_block,
	&block_bar,
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

	if( is(block->cls->type, "menu"))
	{
		block->state = state_block_menu_default;
	}
	else
	{
		block->state = state_block_default;
	}

	if(!found)printf("[ERROR:cls_block_init] Unknown block type %s\n",block->type);

	block->block_state.connecting = 0;
}


void block_cls_change( t_block *block, const char *name)
{
	set_name( block->type, name);
	block_cls_init( block); 
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

t_brick *block_brick_get_by_position(t_block *block, int pos)
{
	t_brick *brick;
	t_link *l;
	int i=0;

	for(l=block->bricks->first;l;l=l->next)
	{
		brick=l->data;
		if( i == pos )
		{
			return brick;
		}
		i++;
	}

	return NULL;
}

void block_brick_init(t_node *node_brick)
{
	t_brick *brick=node_brick->data;
	brick->brick_state.draw_plugs=1;
}

void block_operator_brick_init(t_node *node_brick)
{
	t_brick *brick=node_brick->data;
	brick->brick_state.draw_plugs=1;
	brick->brick_state.draw_outline=0;
	brick->brick_state.draw_name=0;
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
		clone->block_state = block->block_state;
		clone->tot_bricks = block->tot_bricks;
		clone->rhizome_order = block->rhizome_order;
		clone->rhizome_pos = block->rhizome_pos;
		clone->bricks = list_clone(block->bricks,dt_brick);

		clone->submenu = NULL;
		clone->hover = NULL; 
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

void block_rebind(t_scene *sc,void *ptr)
{
	t_block *block=(t_block *)ptr;

	rebind(sc,"block","bricks",(void **)&block->bricks);
	rebind(sc,"block","rhizome",(void **)&block->rhizome);
	rebind(sc,"block","set",(void **)&block->set);
	rebind(sc,"block","clone",(void **)&block->clone);

	// reset
	block->hover=NULL;
	block->submenu=NULL;

	t_context *C=ctx_get();
	block_init(C->scene,block);
}

// MAKE

t_node *block_make(const char *name,const char *type)
{
	t_context *C=ctx_get();

	t_node *n_block = scene_add(C->scene,dt_block,name);
	t_node *n_list = scene_add(C->scene,dt_list,name);

	t_block *block = n_block->data;

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
	t_scene *sc = scene_get();

	t_link *l;
	t_brick *b;

	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		scene_delete(sc,b);
	}
}

// FREE 

void block_free( t_block *block)
{
	t_scene *sc = scene_get();

	// free bricks
	block_bricks_free( block);

	// free lst
	scene_delete( sc, block->bricks);
}

// NEW

void *block_new(const char *name)
{
	t_block *block  = (t_block *) mem_malloc( sizeof( t_block));

	block->cls = NULL;

	vset3i( block->idcol, 0, 0, 0);

	id_init(&block->id, name);
	bzero(block->type,_NAME_);
	
	vset3f( block->pos, 0, 0, 0);
	block->width = 0;
	block->height = 0;

	block->block_state.is_root=0;
	block->block_state.draw_outline=0;
	block->block_state.draw_plugs=0;
	block->block_state.is_mouse_over=0;
	block->block_state.update_geometry=1;
	block->block_state.is_a_loop = 0;
	block->block_state.is_in_rhizome = 0;
	block->block_state.frame_based = 0;
	block->block_state.connecting = 0;
	block->block_state.draw_clone_link = 0;

	block->tot_bricks=0;
	block->rhizome_order = -1;
	block->rhizome_pos = 0;

	block->bricks=NULL;
	block->submenu = NULL;
	block->hover = NULL;
	block->selected = NULL;
	block->rhizome = NULL;
	block->set = NULL;

	block->state = NULL;
	block->clone = NULL;

	block->up = 1;
	block->clones = 0;

	return block;
}

