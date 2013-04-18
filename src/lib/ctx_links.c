/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"

#define db_main 0

// Step Counter
int loop_step = 0;

// Last Brick
int stop_step = 0;

// All Bricks (from all Blocks) 
t_lst *BRICKS = NULL;

// Stack of Root Bricks
t_lst *ROOTS = NULL;

// Display current Brick
t_brick *brick_current=NULL;

// Terminals
t_term *TERM_ROOT=NULL;
t_term *TERM_BRICKS=NULL;

// Debug

void _show_bricks(void)
{
	t_link *l;
	t_brick *b;

	printf("[BRICKS]\n");

	for(l=BRICKS->first;l;l=l->next)
	{
		b=l->data;
		printf("%s\n",b->name);
	}

	printf("[]\n");
}


void _show_roots(void)
{
	t_link *l;
	t_brick *b;

	printf("[ROOTS]\n");

	for(l=ROOTS->first;l;l=l->next)
	{
		b=l->data;
		printf("%s\n",b->name);
	}

	printf("[]\n");
}

// Show Bricks Stack

void term_bricks_show(void)
{
	t_link *l;
	t_brick *b;

	term_reset(TERM_BRICKS);
	term_echo(TERM_BRICKS,"BRICKS");

	for(l=BRICKS->first;l;l=l->next)
	{
		b=l->data;
		term_echo(TERM_BRICKS,"%s",b->name);
	}
}

// Show Roots Stack

void term_roots_show(void)
{
	t_link *l;
	t_brick *b;

	term_reset(TERM_ROOT);
	term_echo(TERM_ROOT,"ROOTS");

	for(l=ROOTS->first;l;l=l->next)
	{
		b=l->data;
		term_echo(TERM_ROOT,"%s",b->name);
	}
}

// Get Bricks

t_lst *ctx_links_lst_get(void)
{
	return BRICKS;
}

// BUILD TERMS

void ctx_links_build_terms(t_context *C)
{
	if(!TERM_BRICKS)
	{
		TERM_BRICKS = term_new("bricks");
		TERM_BRICKS->init(TERM_BRICKS);
		TERM_BRICKS->loc_x = 200;
		TERM_BRICKS->tot_line = 35;

		lst_add(C->terms,TERM_BRICKS,"bricks");
	}

	if(!TERM_ROOT)
	{
		TERM_ROOT = term_new("root");
		TERM_ROOT->init(TERM_ROOT);
		TERM_ROOT->loc_x = 400;
		TERM_ROOT->tot_line = 35;

		lst_add(C->terms,TERM_ROOT,"root");
	}

	// Reset Main Term
	term_reset(C->term);
}

// GET BRANCH

int is_block_in_list(t_lst *lst, t_block *new_block)
{
	t_link *link;
	t_brick *brick;
	t_block *block;

	if(lst->first)
	{
		// Loop over bricks
		for(link = lst->first; link; link = link->next)
		{
			brick = link->data;
			// get Block
			block = brick->block;

			// If Block  == new_block : return True 
			if(block->id == new_block->id)
			{
				return 1;
			}
		}

		return 0;
	}

	// Default
	return 0;
}

// GET BRANCH

void block_branch_get(t_lst *lst, t_block *block)
{
	t_link *link;
	t_brick *block_brick;
	t_plug *plug_in;

	t_plug *plug_src;
	t_brick *brick_source;
	t_block *block_src;

	// for each brick in block
	for(link = block->bricks->first; link; link = link->next)
	{
		block_brick = link->data;
		plug_in = &block_brick->plug_in;

		// add to list
		lst_add(lst,block_brick,"brick");

		// if connected && follow in 
		if(plug_in->is_connected && plug_in->follow_in)
		{
			plug_src = plug_in->src;
			brick_source = plug_src->brick;
			block_src = brick_source->block;

			// get branch if block not yet in list
			if(!is_block_in_list(lst,block_src))
				block_branch_get(lst,block_src);
		}
	}
}

// GET SRC BRANCH

// Put all Bricks from All Block If Connected with this Block

t_lst *block_branch_src_get(t_context *C,t_block *block)
{
	t_lst *lst = lst_new("branch");

	t_link *l;
	t_brick *b;
	t_plug *p;
	t_plug *s;

	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_in;

		// if is connected
		if(p->is_connected)
		{
			s=p->src;
			t_brick *t=s->brick;

			// Get Block Target
			t_block *blk = t->block;

			// Get Block Branch 
			block_branch_get(lst,blk);
		}
	}

	return lst;
}

// STORE ROOTS

void ctx_links_store_roots(t_lst *lst, t_brick *brick)
{
	t_plug *plug_in = &brick->plug_in;
	t_plug *plug_intern = &brick->plug_intern;
	t_plug *plug_target_out = plug_in->src;
	t_plug *plug_target = plug_target_out->src;

	if(!plug_intern->is_in_loop)
	{
		// if follow in
		if(plug_in->follow_in)
		{
			// and target is updated
			//XXX
			//if(plug_target->is_updated || plug_target->is_in_loop)
			if(plug_target->is_updated)
			{
				// this plug is root
				brick->state.is_root = 1;
				lst_add(lst, brick, brick->name);
			}
		}
		// else is no follow
		else
		{
			// this plug is root
			brick->state.is_root = 1;
			lst_add(lst , brick, brick->name);
		}
	}
}

// CHECK PARETNTS

int ctx_links_check_parents(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;
	t_plug *plug_parent;
	t_link *link;

	int state = 1;

	// check if parents are updated 

	if(plug_intern->parents)
	{
		for(link = plug_intern->parents->first; link; link = link->next)
		{
			plug_parent = link->data;

			if(plug_parent->is_in_loop)
			{
				state = 1;
			}
			else
			{
				if(!plug_parent->is_updated)
				{
					state = 0;
					break;
				}
			}
		}
	}

	return state;
}

// SET CURRENT 

void set_brick_current(t_context *C,t_brick *b)
{
	if(brick_current)
	{
		brick_current->state.is_current = 0;
		brick_current=NULL;
	}

	if(b) b->state.is_current=1;

	brick_current=b;
}

// REMOVE LOOP

void ctx_links_remove_loop(t_context *C)
{
	t_lst *lst = BRICKS;

	// Remove Loop Locked Bricks

	if(lst->first)
	{
		t_lst *tmp = lst_new("tmp");

		t_link *link;
		t_brick *brick;
		t_plug *plug;

		for(link = lst->first; link; link = link->next)
		{
			brick = link->data;
			plug = &brick->plug_intern;

			// discard is_in_loop
			if(plug->is_in_loop)
			{
				// set updated 
				plug->is_updated = 1;
			}
			else
			{
				lst_add(tmp,brick,"brick");
			}
		}

		// If Free Bricks
		if(tmp->first)
		{
			// cleanup Bricks
			lst_cleanup(lst);

			// Add regular bricks
			for(link = tmp->first; link; link = link->next)
			{
				brick = link->data;
				lst_add(lst,brick,"brick");
			}

			lst_free(tmp);
		}
		else
		{
			// cleanup Bricks
			lst_cleanup(lst);
		}
	}
}

// GET NEXT

void ctx_links_get_next(t_lst *lst, int order)
{
	t_link *link;
	t_block *block;
	t_lst *tmp = NULL;
	t_lst *connections;

	// Loop over Blocks
	for(link = lst->first; link; link = link->next)
	{
		block = link->data;

		// Set Order
		//block->graph_order = order;
		block_set_graph_order(block,order);

		// Get Out Connections
		connections = block_get_connections("out",block);

		// If Connections
		if(connections)
		{
			// Build Tmp
			if(!tmp) tmp = lst_new("tmp");

			// Merge to tmp
			lst_add_lst(tmp,connections);

			// Free Connections
			lst_free(connections);
		}
	}

	// If Connections
	if(tmp)
	{
		// Cleanup Lst
		lst_cleanup(lst);

		// Remove Doubles
		lst_remove_doubles(tmp);

		// Add to Main
		lst_add_lst(lst,tmp);

		// Free Tmp
		lst_free(tmp);

		// Go Recursive
		ctx_links_get_next(lst, order+1);
	}
}

// SET BLOCK ORDER

void ctx_links_set_order(t_context *C)
{
	t_lst *lst = C->scene->global;
	t_lst *blocks = lst_new("blocks");
	t_link *link;
	t_block *block;

	// Get Roots
	for(link = lst->first; link; link = link->next)
	{
		block = link->data;

		// IF No In Connections
		if(!block_is_connected("in",block))
		{
			// Add to Blocks
			lst_add(blocks,block,"block");
		}
	}

	// Get Next
	if(blocks->first)
	{
		// Set Order Next
		ctx_links_get_next(blocks,0);

		// Free Blocks Lst
		lst_free(blocks);
	}
}

int ctx_links_get_order_lower(t_context *C, t_lst *lst)
{
	t_link *link;
	t_block *brick;
	int order = -1;

	for(link = lst->first; link; link = link->next)
	{
		brick = link->data; 

		//Set First Order
		if(order == -1)
		{
			order = brick->graph_order;
		}
		else
		{

		// Get Lower
		if(brick->graph_order < order)
			order = brick->graph_order;
		}
	}

	return order;
}

void ctx_links_fill_order(t_context *C,int order, t_lst *roots, t_lst *lst)
{
	t_lst *tmp = NULL;
	t_link *link;
	t_brick *brick;

	if(roots->first)
	{
		for(link = roots->first; link; link = link->next)
		{
			brick = link->data;

			if(brick->graph_order == order)
			{
				lst_add(lst,brick,"brick");
			}
			else
			{
				if(!tmp) tmp = lst_new("tmp");
				lst_add(tmp,brick,"brick");
			}
		}

		if(tmp)
		{
			lst_cleanup(roots);
			lst_add_lst(roots,tmp);
			lst_free(tmp);
			ctx_links_fill_order(C, order+1, roots, lst);
		}
	}
}

void ctx_links_set_roots_order(t_context *C)
{
	t_lst *roots = ROOTS;
	t_lst *lst = lst_new("lst");

	int order_lower = ctx_links_get_order_lower(C,roots);

	ctx_links_fill_order(C,order_lower,roots,lst);

	lst_cleanup(roots);
	lst_add_lst(roots,lst);
	lst_free(lst);
}


// GET ROOTS

void ctx_links_get_roots(t_context *C)
{
	t_link *l;
	t_brick *b;
	t_plug *plug_in;
	t_plug *plug_intern;

	t_lst *lst = BRICKS;
	t_lst *roots = ROOTS;

	// Remove is_in_loop bricks
	ctx_links_remove_loop(C);

	// loop over bricks 
	for(l=lst->first;l;l=l->next)
	{
		b=l->data;
		plug_in = &b->plug_in;
		plug_intern = &b->plug_intern;

		int go = 1;

		if(plug_intern->is_a_loop)
		{
			t_block *block = b->block;
			// get branch (all bricks)
			t_lst *lst = block_branch_src_get(C,block);

			t_link *l;
			t_brick *b;
			t_plug *p;

			for(l=lst->first;l;l=l->next)
			{
				b=l->data;
				p=&b->plug_intern;
				if(!p->is_updated)
				{
					go = 0;
					break;
				}
			}

			lst_free(lst);

		}


		// if plug_in is connected
		if(plug_in->is_connected && go)
		{
			int all_updated = ctx_links_check_parents(b);

			// check parents 
			if(all_updated)
			{
				// check connections
				ctx_links_store_roots(roots,b);
			}
		}
		// else if plug_in is not connected
		else if (go)
		{
			int all_updated = ctx_links_check_parents(b);

			// check parents
			if(all_updated)
			{
				t_plug *plug_intern = &b->plug_intern;
				if(!plug_intern->is_in_loop)
				{
					// this plug is root
					b->state.is_root = 1;
					lst_add(roots,b,b->name);
				}
			}
		}
	}

	// Set Order

	ctx_links_set_roots_order(C);

}

// RELOOP

void ctx_links_reloop(t_context *C)
{
	if(C->ui->show_step)
	{
		if(!BRICKS->last)
		{
			term_echo(C->term,"[Last Brick]");
			stop_step = 1;
		}
	}
	else
	{
		// Step ++
		loop_step++;
		C->event->loop_step++;

		// Loop While Bricks
		if(BRICKS->last)
		{
			// Cleanup Roots
			lst_cleanup(ROOTS);
			// Loop Again
			ctx_links_loop(C);
		}
	}
}

void ctx_links_trigger(t_context *C)
{
	t_link *l;
	t_brick *b;

	if(ROOTS->first)
	{
		for(l=ROOTS->first;l;l=l->next)
		{
			b=l->data;

			if(C->ui->show_step) term_echo(C->term,"trigger %s",b->name);

			// *** Trigger
			b->cls->trigger(b);

			// Reset state
			b->state.is_root = 0;

			// *** Remove from BRICKS
			lst_remove_by_id(BRICKS,b->id);

			// [STEP] 
			if(C->ui->show_step)
			{
				// *** Remove from ROOTS
				lst_remove_by_id(ROOTS,b->id);

				// set current
				set_brick_current(C,b);

				term_bricks_show();
				term_roots_show();

				// Exit
				break;
			}
		}
	}
}

// FIND ROOTS

void ctx_links_find_roots(t_context *C)
{
	// Get Roots
	if(C->ui->show_step)
	{
		if(!ROOTS->last && BRICKS->last) 
		{
			term_echo(C->term,"[GET ROOTS]");
			ctx_links_get_roots(C);
			term_roots_show();
		}
	}
	else
	{
		ctx_links_get_roots(C);

		if(db_main)
		{
			printf("ctx_links_find_roots\n");
			_show_roots();
		}
	}
}

// Main Loop

void ctx_links_loop(t_context *C)
{
	if(!C->ui->show_step && db_main) printf("%d ctx_links_loop\n",loop_step);

	// Get roots
	ctx_links_find_roots(C);

	// Trigger roots
	ctx_links_trigger(C);

	// Loop BRICKS
	ctx_links_reloop(C);
}

// BUILD BRICKS

t_lst *ctx_links_build(t_context *C)
{
	t_lst *lst = lst_new("lst");
	t_link *l;

	// Get All Bricks from All Blocks
	for(l=C->scene->global->first;l;l=l->next)
	{
		t_link *link;
		t_block *block = l->data;

		// Reset Graph Pos
		block->graph_order = -1;

		for(link=block->bricks->first;link;link=link->next)
		{
			t_brick *brick = link->data;
			lst_add(lst,brick,brick->name);
		}
	}

	return lst;
}

// RESET Loop

void ctx_links_reset_loop(t_context *C, t_lst *lst)
{
	t_link *l;
	t_brick *b;
	t_plug *p_intern;

	// Set is_in_loop : 1
	// Set for counter : 0

	for(l=lst->first;l;l=l->next)
	{
		b=l->data;
		p_intern=&b->plug_intern;

		// is a loop
		if(p_intern->is_a_loop)
		{
			t_block *block = b->block;
			set_for_loop(block,1);

			b->counter = 0;
			p_intern->is_init = 0;
		}
	}
}

// RESET 

void ctx_links_reset(t_context *C,t_lst *lst)
{
	t_link *l;
	t_brick *b;
	t_plug *p_in;
	t_plug *p_intern;
	t_plug *p_out;

	// Set is_updated : 0
	// Set is_root : 0
	// Set is_current : 0

	for(l=lst->first;l;l=l->next)
	{
		b=l->data;
		p_in=&b->plug_in;
		p_intern=&b->plug_intern;
		p_out = &b->plug_out;

		p_in->is_updated = 0;
		p_intern->is_updated = 0;
		p_out->is_updated = 0;

		b->state.is_root = 0;
		b->state.is_current = 0;
	}

	// Set Order
	ctx_links_set_order(C);
}

void ctx_links_step_reset(t_context *C)
{
	if(C->ui->step_reset)
	{
		term_log("[RESET]");

		// Reset States 
			if(BRICKS)
			{
				// ** Reset For Loops  
				ctx_links_reset_loop(C,BRICKS);

				// *** Remove BRICKS
				lst_free(BRICKS);

				// free roots
				lst_free(ROOTS);
			}

		// Set To Init State
			BRICKS = NULL;
			ROOTS = NULL;

		// Reset current position
			set_brick_current(C,NULL); 

		// Reset Terminals 
			term_reset(C->term);
			if(TERM_ROOT)
			term_reset(TERM_ROOT);
			if(TERM_BRICKS)
			term_reset(TERM_BRICKS);
	}
}

// STEP CLEANUP

void ctx_links_step_cleanup(t_context *C)
{
	// Reset Terminals

	if(TERM_BRICKS)
	{
		term_free(TERM_BRICKS);
		TERM_BRICKS = NULL;
		lst_link_delete_by_name(C->terms,"bricks");
	}

	if(TERM_ROOT)
	{
		term_free(TERM_ROOT);
		TERM_ROOT = NULL;
		lst_link_delete_by_name(C->terms,"root");
	}

	// Reset main term
	term_reset(C->term);
}


// STEP INIT

void ctx_links_step_init(t_context *C)
{
	// Init if no BRICKS
	if(!BRICKS)
	{
		term_log("[INIT]");

		// Reset step counter
		loop_step = 0;

		// Build list
		BRICKS=ctx_links_build(C);
		ROOTS=lst_new("root");

		// Reset
		ctx_links_reset(C,BRICKS);

		// Reset for 
		ctx_links_reset_loop(C,BRICKS);
	}

	if(!TERM_BRICKS)
		// Build Terms
		ctx_links_build_terms(C);
}

// STEP STOP

void ctx_links_step_stop(t_context *C)
{
	// reset for next time
	stop_step = 0;

	// step ~= frame
	loop_step++;

	// reset current position
	set_brick_current(C,NULL); 

	// Reset Terminal
	term_reset(C->term);

	// Reset brick states

		// re-build BRICKS
		t_lst *bricks=ctx_links_build(C);

		// reset bricks
		ctx_links_reset(C,bricks);

		// free
		lst_free(bricks);

	// free BRICKS
	if(BRICKS) lst_free(BRICKS);

	BRICKS=NULL;
	ROOTS=NULL;
}

// INIT 

void ctx_links_init(t_context *C)
{
	if(C->ui->show_step)
	{
		// Init first time
		ctx_links_step_init(C);
	}
	else
	{
		// Reset States
		ctx_links_reset(C,BRICKS);
	}
}

// STEP

void ctx_links_step(t_context *C)
{
	if (C->ui->step_reset)
	{
		// Reset states on demand
		ctx_links_step_reset(C);
	}
	// Step
	if(C->ui->step)
	{
		// Stop if Last Bricks
		if(stop_step)
		{
			ctx_links_step_stop(C);
		}
		// or Next Step
		else
		{
			// Init
			ctx_links_init(C);

			// Next Step in graph
			ctx_links_loop(C);

			// Stop Step
			C->ui->step = 0;
		}
	}

	if(C->ui->step_reset)
	{

		// Reset reset
		C->ui->step_reset = 0;
		C->ui->step = 1;
	}
}

// CONTINUOUS

void ctx_links_continuous(t_context *C)
{
	// 1) Init
		// Get All Bricks
		BRICKS = ctx_links_build(C);
		// Build New List
		ROOTS = lst_new("root");

	// 2) Reset States updated,root,current
		ctx_links_init(C);

	// 3) Loop
		ctx_links_loop(C);

	// 4) Free
		lst_free(BRICKS);
		lst_free(ROOTS);
		BRICKS=NULL;
		ROOTS=NULL;
}

// MAIN

void ctx_links_update(t_context *C)
{
	// Freeze 
	if(C->ui->update_links)
	{
		// Step 
		if(C->ui->show_step)
		{
			ctx_links_step(C);
		}
		// Continuous
		else
		{
			ctx_links_continuous(C);
		}
	}
}


