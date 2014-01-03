/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "context.h"
#include "ctx.h"
#include "event.h"
#include "block.h"
#include "brick.h"
#include "ui.h"
#include "list.h"
#include "scene.h"
#include "node.h"
#include "app.h"

void block_store(t_block *block,t_brick *brick)
{
	brick->brick_state.show_menu=1;
	block->selected=brick;
	block->submenu=brick; //???
}

void block_unstore(t_block *block)
{
	t_link *link;
	t_brick *brick;

	for(link=block->bricks->first;link;link=link->next)
	{
		brick=link->data;

		if(brick->cls->type==bt_menu)
		{
			// HIDE
			brick->brick_state.show_menu=0;

			// recusrive
			if(brick->menu)
			{
				block_unstore(brick->menu);
			}
		}
	}

	block->submenu=NULL;
	block->selected=NULL;
}

// regular block
void cls_block_block_update(t_block *block)
{
	t_context *C = ctx_get();
	t_brick *brick;
	t_link *link;

	int is_mouse_over=0;

	// loop over all bricks
	for(link=block->bricks->first;link;link=link->next)
	{
		brick=link->data;

		if(!C->event->is_brick_transformed)
		{
			// mouse over
			if(is_mouse_over_brick(C,brick))
			{
				brick->brick_state.is_mouse_over=1;
				is_mouse_over=1;
			}
			else
			{
				brick->brick_state.is_mouse_over=0;
			}
		}

		// update brick 
		brick->cls->update(brick);
	}

	if(is_mouse_over)
	{
		block->block_state.is_mouse_over=1;
	}
	else 
	{
		block->block_state.is_mouse_over=0;
	}
}

// menu block
void cls_block_menu_update(t_block *block)
{
	t_context *C = ctx_get();


	if(C->ui->show_menu)  block->block_state.is_show=1; 
	if(block->block_state.is_show)
	{
		t_brick *brick;
		t_link *link;

		int is_mouse_over=0;

		// hide previous menu
		if(block->block_state.is_show && !C->ui->show_menu)
		{
			block_unstore(block);
			block->selected=NULL;
			block->block_state.is_show=0;
			C->event->is_brick_transformed=0;
		}
		// OR update
		else
		{
			// loop over all bricks
			for(link=block->bricks->first;link;link=link->next)
			{
				brick=link->data;
				if(is_mouse_over_brick(C,brick))
				{
					 brick->brick_state.is_mouse_over=1;
					is_mouse_over=1;
				}
				else 
				{
					brick->brick_state.is_mouse_over=0;
				}

				// UPDATE BRICK 
				brick->cls->update(brick);

				// if BRICK is a MENU 
				if(brick->cls->type == bt_menu)
				{
					// if mouse over
					if(brick->brick_state.is_mouse_over)
					{
						// check STORED menu

						t_brick *brick_menu=block->selected;

						if(brick_menu)
						{
							t_block *block_menu=brick->menu;

							// if a submenu is stored
							if(block->submenu)
							{
								t_brick *brick_submenu=block->submenu;

								if(!is(brick_submenu->id.name,brick->id.name))
								{
									block_unstore(block);
									block_store(block,brick);
								}
							}
							else
							{
								block_store(block,brick);
							}

							// update block_menu
							if(block_menu)
							{
								block_menu->cls->update(block_menu);
							}
								
						}
						// no previous selected_menu
						else
						{
							block_store(block,brick);
						}
					}

					// mouse not over
					else
					{
						// Update Menu
						t_block *m=brick->menu;
						if(m) m->cls->update(m);
					}
				}
			}

			if(is_mouse_over) block->block_state.is_mouse_over=1;
			else block->block_state.is_mouse_over=0;
		}
	}
}


// Screen blocks
void cls_block_generic_update(t_block *block)
{
	t_context *C = ctx_get();

	t_brick *brick;
	t_link *link;

	// loop over all bricks
	for(link=block->bricks->first;link;link=link->next)
	{
		brick=link->data;
		if(is_mouse_over_brick(C,brick))
		{
			 brick->brick_state.is_mouse_over=1;
		}
		else 
		{
			brick->brick_state.is_mouse_over=0;
		}

		// UPDATE BRICK 
		brick->cls->update(brick);
	}
}


void cls_block_state_block_default( t_block *block, t_event *e)
{
}

void cls_block_dispatch_block( t_block *block)
{
}


void cls_block_state_menu_default( t_block *block, t_event *e);
void cls_block_state_menu_hover_menu( t_block *block, t_event *e);


// [block] brick->menu 		=> pointing to another block-menu 
// [brick] block->submenu 	=> 
// [brick] block->selected	=>

void cls_block_menu_close( t_block *block)
{
	if( block->selected)
	{
		t_brick *selected = block->selected;

		block->selected->brick_state.show_menu = 0;
		block->selected=NULL;

		t_block *submenu = selected->menu;
		if( submenu) // ????
		{
			cls_block_menu_close( submenu);
		}
	}
}

void cls_block_menu_open( t_block *block, t_brick *brick)
{
	brick->brick_state.show_menu=1;
	block->selected=brick;
}

t_brick *block_brick_hover( t_context *C)
{
	if( C->scene->hover_type == dt_brick)
	{
		t_node *node = C->scene->hover;
		t_brick *brick = node->data;
		return brick;
	}
	else
	{
		return NULL;
	}
}

void cls_block_submenu_update( t_block *block)
{
	t_context *C = ctx_get();

	t_brick *brick_hover = NULL;
	t_block *block_hover = NULL;
	int hover = 1;
	int hover_submenu = 0;

	if( C->scene->hover_type == dt_brick)
	{
		t_node *node = C->scene->hover;
		brick_hover = node->data;
	}
	
	if( brick_hover)
	{
		block_hover = brick_hover->block;
		if( brick_hover->cls->type == bt_menu) hover_submenu = 1;
		if( block_hover->selected)
		{
			if( hover_submenu)
			{
				if( brick_hover->id.id != block_hover->selected->id.id) hover = 0;
			}
		}
		else
		{
			cls_block_menu_open( block_hover, brick_hover);
		}
	}

	if( !hover)
	{
		if( block_hover)
		{
			cls_block_menu_close( block_hover);
			if( !hover_submenu) BLOCK_TRANS( block, cls_block_state_menu_default);
			else cls_block_menu_open( block_hover, brick_hover);
		}
	}
	else
	{
		if( block_hover)
		{
			if( !hover_submenu)
			{
				cls_block_menu_close( block_hover);
				BLOCK_TRANS( block, cls_block_state_menu_default);
			}
		}
	}
}

void cls_block_state_menu_hover_menu( t_block *block, t_event *e)
{
	switch( e->type)
	{
		case MOUSE_MOTION_PASSIVE:
			cls_block_submenu_update( block);
			break;
	}
}

void cls_block_state_menu_hover_brick( t_block *block, t_event *e)
{
	t_context *C = ctx_get();
	t_brick *brick;
	switch( e->type)
	{
		case MOUSE_LEFT_PRESSED:
			brick = block_brick_hover( C);
			printf("%s\n",brick->id.name);
			break;
		case MOUSE_MOTION_PASSIVE:
			cls_block_submenu_update( block);
			break;
			
		default:
			break;
	}
}

void cls_block_state_menu_default( t_block *block, t_event *e)
{
	t_context *C = ctx_get();
	if( C->scene->hover_type == dt_brick)
	{
		t_node *node = C->scene->hover;
		t_brick *brick = node->data;
		if(brick->cls->type == bt_menu)
		{
			printf("menu\n");
			cls_block_submenu_update( block);
			BLOCK_TRANS( block, cls_block_state_menu_hover_menu);
		}
		else
		{
			printf("brick\n");
			BLOCK_TRANS( block, cls_block_state_menu_hover_brick);
		}
	}
}

void cls_block_dispatch_menu( t_block *block)
{
	t_context *C = ctx_get();
	t_link *l;
	t_event *e;
	for(l=C->event->events->first;l;l=l->next)
	{
		e = l->data;
		block->state( block, e);
	}
}
