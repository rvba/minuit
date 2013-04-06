/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"

// STEP

// All Bricks (from all Blocks) from Desk
t_lst *BRICKS = NULL;

// Stack of Root Bricks
t_lst *ROOTS = NULL;

// Display current Brick
t_brick *brick_current=NULL;

t_lst *ctx_links_lst_get(void)
{
	return BRICKS;
}

t_term *TERM_ROOT=NULL;

// GET BRANCH

int block_branch_done(t_lst *lst, t_block *new_block)
{
	t_link *link;
	t_brick *brick;
	t_block *block;

	if(lst->first)
	{
		for(link = lst->first; link; link = link->next)
		{
			brick = link->data;
			block = brick->block;

			// return True if Same Block
			if(block->id == new_block->id)
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

			// get branch if new block
			if(!block_branch_done(lst,block_src))
				block_branch_get(lst,block_src);
		}
	}
}

// GET BRANCH

// Put all Bricks from All Block If Connected with this Block
// Get N-1

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

			// get target
			t_block *blk = t->block;

			// get branch
			block_branch_get(lst,blk);
		}
	}

	return lst;
}

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
			if(plug_target->is_updated || plug_target->is_in_loop)
			{
				brick->state.is_root = 1;
				// this plug is root
				lst_add(lst, brick, brick->name);
			}
		}
		// else is no follow
		else
		{
			brick->state.is_root = 1;
			// this plug is root
			lst_add(lst , brick, brick->name);
		}
	}
}

int ctx_links_check_parents(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;
	t_plug *plug_parent;
	t_link *link;

	int state = 1;

	// check parents state
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

// Remove all NONE Root Bricks from ROOT
// NONE Root = No PARENT or PARENT is UPDATED
void ctx_links_get_roots(t_context *C, t_lst *lst, t_lst *roots)
{
	t_link *l;
	t_brick *b;
	t_plug *plug_in;

	// LOOP over list
	for(l=lst->first;l;l=l->next)
	{
		b=l->data;
		plug_in = &b->plug_in;

		// if plug_in is connected
		if(plug_in->is_connected)
		{
			int all_updated = ctx_links_check_parents(b);

			// check parents 
			if(all_updated)
			{
				// check conections
				ctx_links_store_roots(roots,b);
			}
		}
		// else if plug_in is not connected
		else
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
}

void ctx_links_reset(t_context *C,t_lst *lst)
{
	// RESET plug_out STATES to 0

	t_link *l;
	t_brick *b;
	t_plug *p_in;
	t_plug *p_intern;
	t_plug *p_out;

	for(l=lst->first;l;l=l->next)
	{
		b=l->data;
		p_in=&b->plug_in;
		p_intern=&b->plug_intern;
		p_out = &b->plug_out;

		p_in->is_updated = 0;
		p_intern->is_updated = 0;
		p_out->is_updated = 0;

		p_intern->is_eval = 0;

	}
}

void ctx_links_unreset(t_context *C,t_lst *lst)
{
	// RESET plug_out STATES to 1

	t_link *l;
	t_brick *b;
	t_plug *p_in;
	t_plug *p_intern;
	t_plug *p_out;

	for(l=lst->first;l;l=l->next)
	{
		b=l->data;
		p_in=&b->plug_in;
		p_intern=&b->plug_intern;
		p_out = &b->plug_out;

		p_in->is_updated = 1;
		p_intern->is_updated = 1;
		p_out->is_updated = 1;
	}
}

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

void echo_root(t_context *C)
{
	t_link *l;
	t_brick *b;

	term_echo(TERM_ROOT,"%d NEW ROOTS",C->app->frame);

	for(l = ROOTS->first; l; l = l->next)
	{
		b = l->data;
		term_echo(TERM_ROOT,"%s",b->name);
	}
}
	
int ctx_links_loop(t_context *C)
{
	t_link *l;
	t_brick *b;

	// [STEP]
	if(C->ui->show_step)
	{
		if(ROOTS->last)
		{
			// continue
		}
		else
		{
			// get roots
			if(BRICKS->last)
			{
				term_log("[GET ROOTS]");
				lst_cleanup(ROOTS);
				ctx_links_get_roots(C, BRICKS, ROOTS);
				echo_root(C);
			}
			// FINISH
			else
			{
				term_log("[END] no root + no bricks");
				return 1;
			}
		}
		
	}
	else
	{
		// get roots
		ctx_links_get_roots(C, BRICKS, ROOTS);
	}

	// TRIGGER ROOTS
	for(l=ROOTS->first;l;l=l->next)
	{
		b=l->data;

		// TRIGGER BRICK
		b->cls->trigger(b);
		b->state.is_root = 0;

		// REMOVE from LIST
		lst_remove_by_id(BRICKS,b->id);

		// [STEP] REMOVE LIST
		if(C->ui->show_step)
		{
			term_log("trigger %s",b->name);
			// remove from ROOTS
			lst_remove_by_id(ROOTS,b->id);

			// set current
			set_brick_current(C,b);

			return 0;
		}
	}

	// LOOP if bricks left
	if(BRICKS->last)
	{
		if(C->ui->show_step)
		{
			// pass ?
			return 0;
		}
		else
		{
			lst_cleanup(ROOTS);
			ctx_links_loop(C);
		}
	}
	
	return 0;
}

t_lst *ctx_links_build(t_context *C)
{
	t_lst *lst = lst_new("lst");
	t_link *l;

	// From Global Get All Bricks from All Blocks
	for(l=C->scene->global->first;l;l=l->next)
	{
		t_link *link;
		t_block *block = l->data;

		for(link=block->bricks->first;link;link=link->next)
		{
			t_brick *brick = link->data;
			lst_add(lst,brick,brick->name);
		}
	}

	return lst;
}

int reset = 0;

void ctx_links_reset_for(t_lst *lst)
{
	t_link *l;
	t_brick *b;

	for(l=lst->first;l;l=l->next)
	{
		b = l->data;
		if(is(b->name,"for"))
		{
			b->counter = 0;
		}
	}
}
		 
/*
void ctx_links_init_loops(t_context *C)
{
	t_link *l;
	t_brick *b;
	t_plug *p;

	for(l=BRICKS->first;l;l=l->next)
	{
		b = l->data;
		p = &b->plug_intern;

		if(p->operator_type == ot_for)
		{
			if(C->ui->show_step) term_log("init for");

			t_block *_block = b->block;
			t_brick *brick_vector_for = block_brick_get(_block,"vector"); 
			t_brick *brick_indice = block_brick_get(_block,"indice");
			t_plug *plug_indice = &brick_indice->plug_intern;
			int *indice =plug_indice->data;
			*indice = 0;
			t_plug *plug_vector_for = &brick_vector_for->plug_intern;
			if(plug_vector_for->data)
			{
				t_plug *plug_vector = plug_get_src(plug_vector_for);

				if(plug_vector)
				{
					plug_vector->cls->flow(plug_vector);
				}
			}
		}
	}
}
*/

void ctx_links_update(t_context *C)
{
	// UPDATE
	if(C->ui->update_links)
	{
		// STEP 
		if(C->ui->show_step)
		{
			// RESET
			if(C->ui->step_reset)
			{
				term_log("[RESET]");

				// reset RESET
				C->ui->step_reset = 0;

				// reset BRICKS
				if(BRICKS)
				{
					// reset for indice
					ctx_links_reset_for(BRICKS);
					lst_free(BRICKS);
				}

				BRICKS = NULL;
				ROOTS = NULL;

				// SET CURRENT
				set_brick_current(C,NULL); 

			}

			// INIT
			if(!BRICKS)
			{
				term_log("[INIT]");

				// build second term
				TERM_ROOT = term_new("root");
				TERM_ROOT->init(TERM_ROOT);
				TERM_ROOT->loc_x = 200;
				TERM_ROOT->tot_line = 35;

				lst_add(C->terms,TERM_ROOT,"root");

				// BUILD LIST
				BRICKS=ctx_links_build(C);
				ROOTS=lst_new("root");

				// RESET
				ctx_links_reset(C,BRICKS);
				//ctx_links_init_loops(C);
			}

			// STEP
			if(C->ui->step)
			{
				// RESET, wait for key pressed
				C->ui->step = 0;
	
				// LOOP
				// IF LAST BRICK : RESET

				// END LOOP
				if(reset)
				{
					// BUILD new list
					t_lst *__BRICKS=ctx_links_build(C);

					// RESET bricks
					ctx_links_reset(C,__BRICKS);

					// free
					lst_free(__BRICKS);
					lst_free(BRICKS);

					// Remove Root Terminal
					lst_link_delete_by_name(C->terms,"root");
					term_free(TERM_ROOT);

					BRICKS=NULL;
					ROOTS=NULL;

					set_brick_current(C,NULL); 

					reset = 0;
				}
				else
				{
					// STEP IN LOOP
					int is_last = ctx_links_loop(C);

					// SET LAST
					if(is_last)
					{
						reset = 1;
					}
				}
			}

		}

		// CONTINUOUS
		else
		{
			// get desk bricks
			BRICKS = ctx_links_build(C);

			// reset
			ctx_links_reset(C,BRICKS);

			// add ROOT
			ROOTS = lst_new("root");
			
			// loop
			ctx_links_loop(C);

			//ctx_links_init_loops(C);

			// free
			lst_free(BRICKS);
			lst_free(ROOTS);

			// reset
			BRICKS=NULL;
			ROOTS=NULL;

		}
	}
}


