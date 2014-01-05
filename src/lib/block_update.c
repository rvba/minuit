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

// [block] brick->menu 		=> pointing to another block-menu 
// [brick] block->submenu 	=> submenu selection
// [brick] block->selected	=> brick selection

void state_block_menu_default( t_block *block, t_event *e);
void state_block_menu_hover_menu( t_block *block, t_event *e);
void state_block_menu_brick_trigger( t_block *block, t_event *e);
void state_block_brick_trigger( t_block *block, t_event *e);


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

void block_menu_hover( t_context *C, t_block *block)
{
	t_brick *brick_hover = ctx_ui_hover_get( C, dt_brick);

	if( brick_hover)
	{
		if( brick_hover->cls->type == bt_menu) BLOCK_SWAP( block, state_block_menu_default);
	}
	else
	{
		BLOCK_SWAP( block, state_block_menu_default);
	}
}

/*	**********************************
	OPERATORS MENU 
	**********************************	*/

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

void block_menu_close( t_block *block)
{
	t_context *C= ctx_get();
	C->ui->show_menu = 0;
	block_unstore( block);
	ctx_event_add( UI_BLOCK_RELEASED);
	BLOCK_SWAP( block, state_block_menu_default);
}

void block_submenu_close( t_block *block)
{
	if( block->selected)
	{
		t_brick *selected = block->selected;

		block->selected->brick_state.show_menu = 0;
		block->selected=NULL;

		t_block *submenu = selected->menu;
		if( submenu) // ????
		{
			block_submenu_close( submenu);
		}
	}
}

void block_submenu_open( t_block *block, t_brick *brick)
{
	brick->brick_state.show_menu=1;
	block->selected=brick;
}

void block_submenu_update( t_block *block)
{
	t_context *C = ctx_get();

	t_brick *brick_hover = NULL;
	t_block *block_hover = NULL;
	int hover = 1;
	int hover_submenu = 0;

	// Get Current Brick
	if( C->scene->hover_type == dt_brick)
	{
		t_node *node = C->scene->hover;
		brick_hover = node->data;
	}
	
	// If Brick Hover
	if( brick_hover)
	{
		block_hover = brick_hover->block;
		if( brick_hover->cls->type == bt_menu) hover_submenu = 1;

		// If Submenu open
		if( block_hover->selected)
		{
			// Check if menu the same
			if( hover_submenu)
			{
				// If not 
				if( brick_hover->id.id != block_hover->selected->id.id) hover = 0;
			}
		}
		// Open it
		else
		{
			block_submenu_open( block_hover, brick_hover);
		}
	}

	// Close Submenu
	if( !hover)
	{
		if( block_hover)
		{
			block_submenu_close( block_hover);
			if( !hover_submenu) BLOCK_SWAP( block, state_block_menu_default);
			else block_submenu_open( block_hover, brick_hover);
		}
	}
	else
	{
		if( block_hover)
		{
			if( !hover_submenu)
			{
				block_submenu_close( block_hover);
				BLOCK_SWAP( block, state_block_menu_default);
			}
		}
	}
}

/*	**********************************
	TRIGGER 
	**********************************	*/

void block_menu_trigger( t_context *C, t_block *block)
{
	t_brick *brick = block_brick_hover( C);
	block->_selected = brick;
	brick->cls->dispatch( brick); 
	BLOCK_SWAP( block, state_block_menu_brick_trigger);
}

void block_brick_trigger( t_context *C, t_block *block)
{
	t_brick *brick = block_brick_hover( C);
	block->_selected = brick;
	brick->cls->dispatch( brick); 
	BLOCK_SWAP( block, state_block_brick_trigger);
}


/*	**********************************
	STATES 
	**********************************	*/

// EXIT

void state_block_exit( t_block *block, t_event *e)
{
	if( e->type == MOUSE_RIGHT_RELEASED)
	{
		block_menu_close( block);
	}
}


/*	**********************************
	STATES DEFAULT
	**********************************	*/


// BRICK TRIGGER

void state_block_brick_trigger( t_block *block, t_event *e)
{
	t_brick *brick = block->_selected;
	if( e->type == UI_BRICK_RELEASED)
	{
		ctx_event_add( UI_BLOCK_RELEASED);
		BLOCK_SWAP( block, state_block_default);
	}
	else
	{
		brick->cls->dispatch( brick); 
	}
}

void state_brick_linking( t_block *block, t_event *e)
{
	switch( e->type)
	{
		case MOUSE_LEFT_RELEASED:
			BLOCK_SWAP( block, state_block_default);
			ctx_event_add( UI_BLOCK_RELEASED);
			break;
	}
}

// DEFAULT

void state_block_default( t_block *block, t_event *e)
{
	t_context *C = ctx_get();

	t_brick *brick = ctx_ui_hover_get( C, dt_brick);
	if( brick)
	{
		if( ctx_mouse_hover_brick_plug_out( C, brick))
		{
			if( e->type == MOUSE_LEFT_PRESSED)
			{
				BLOCK_SWAP( block, state_brick_linking);
			}
		}
		else
		{
			switch( e->type)
			{
				case MOUSE_LEFT_PRESSED:
					block_brick_trigger( C, block);
					break;

				default:
					break;
			}
		}
	}
}

/*	**********************************
	STATES MENU
	**********************************	*/


// MENU BRICK TRIGGER

void state_block_menu_brick_trigger( t_block *block, t_event *e)
{
	t_brick *brick = block->_selected;
	if( e->type == UI_BRICK_RELEASED)
	{
		if( brick->type == bt_trigger) block_menu_close( block);
		else BLOCK_SWAP( block, state_block_menu_default);
	}
	else
	{
		brick->cls->dispatch( brick); 
	}
}

// MENU HOVER BRICK

void state_block_menu_hover_brick( t_block *block, t_event *e)
{
	t_context *C = ctx_get();
	switch( e->type)
	{
		case MOUSE_LEFT_PRESSED:
			block_menu_trigger( C, block);
			break;

		case MOUSE_MOTION_PASSIVE:
			block_menu_hover( C, block);
			break;

		case MOUSE_RIGHT_PRESSED:
			BLOCK_SWAP( block, state_block_exit);
			break;

			
		default: 
			break;
	}
}

// MENU HOVER MENU

void state_block_menu_hover_menu( t_block *block, t_event *e)
{
	switch( e->type)
	{
		case MOUSE_MOTION_PASSIVE:
			block_submenu_update( block);
			break;
		case MOUSE_RIGHT_PRESSED:
			BLOCK_SWAP( block, state_block_exit);
			break;
	}
}

// MENU DEFAULT

void state_block_menu_default( t_block *block, t_event *e)
{
	t_context *C = ctx_get();

	if( e->type == MOUSE_RIGHT_PRESSED)
	{
		BLOCK_SWAP( block, state_block_exit);
	}
	else
	{
		t_brick *brick = ctx_ui_hover_get( C, dt_brick);

		if( brick)
		{
			if(brick->cls->type == bt_menu)
			{
				block_submenu_update( block);
				BLOCK_SWAP( block, state_block_menu_hover_menu);
			}
			else
			{
				BLOCK_SWAP( block, state_block_menu_hover_brick);
			}
		}
	}
}

/*	**********************************
	DISPATCH
	**********************************	*/

void cls_block_dispatch( t_block *block)
{
	t_context *C = ctx_get();
	t_link *l;
	t_event *e;

	for(l=C->event->events_swap->first;l;l=l->next)
	{
		e = l->data;
		block->state( block, e);
	}
}
