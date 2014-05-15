/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "ctx.h"
#include "event.h"
#include "block.h"
#include "brick.h"
#include "ui.h"
#include "list.h"
#include "scene.h"
#include "node.h"
#include "app.h"
#include "action.h"
#include "dict.h"
#include "set.h"
#include "op.h"
#include "rhizome.h"

void state_block_menu_brick_trigger( t_block *block, t_event *e);

t_block *dupli = NULL;

void state_block_dupli( t_block *block, t_event *e)
{
	t_context *C = ctx_get();
	ctx_ui_log( "state_block_dupli");
	switch( e->type)
	{
		case MOUSE_LEFT_RELEASED:

			if( dupli)
			{
				t_block *clone = block_dupli( dupli);
				clone->pos[0] = C->app->mouse->x;
				clone->pos[1] = C->app->mouse->y;
				block_cls_change( clone, "block");
				if(!C->ui->show_sets) show_sets(C);
			}

			BLOCK_SWAP( block, state_block_exit);
			break;
	}
}

void ctx_block_dupli( t_block *block, t_event *e)
{
	t_context *C = ctx_get();
	t_node *node = C->scene->hover;
	if( node)
	{
		if( node->type ==dt_brick)
		{
			t_brick *brick = node->data;
			t_block *b = brick->block;

			dupli = b;
			BLOCK_SWAP( block, state_block_dupli);
			cls_block_dispatch( block);
		}
	}
}



/*	**********************************
	OPERATORS MENU 
	**********************************	*/

void block_store(t_block *block,t_brick *brick)
{
	brick->brick_state.show_menu=1;
	block->hover=brick;
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
	block->hover=NULL;
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
	if( block->hover)
	{
		t_brick *hover = block->hover;

		block->hover->brick_state.show_menu = 0;
		block->hover=NULL;

		t_block *submenu = hover->menu;
		if( submenu) // ????
		{
			block_submenu_close( submenu);
		}
	}
}

void block_submenu_open( t_block *block, t_brick *brick)
{
	brick->brick_state.show_menu=1;
	block->hover=brick;
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
		if( block_hover->hover)
		{
			// Check if menu the same
			if( hover_submenu)
			{
				// If not 
				if( brick_hover->id.id != block_hover->hover->id.id) hover = 0;
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
	:EXIT 
	**********************************	*/

void state_block_exit( t_block *block, t_event *e)
{
	if( e->type == MOUSE_RIGHT_RELEASED)
	{
		block_menu_close( block);
	}
}


void block_menu_trigger( t_context *C, t_block *block)
{
	t_brick *brick = block_brick_hover( C);
	block->selected = brick;
	brick_dispatch( brick);
	BLOCK_SWAP( block, state_block_menu_brick_trigger);
}



/*	*******************************************************************************************
	:MENU
	******************************************************************************************* */

// MENU BRICK TRIGGER

void state_block_menu_brick_trigger_fixed( t_block *block, t_event *e)
{
	ctx_ui_log( "block_menu_brick_trigger_fixed");
	t_brick *brick = block->selected;
	if( e->type == UI_BRICK_RELEASED)
	{
		BLOCK_SWAP( block, state_block_menu_default);
	}
	else if( e->type == MOUSE_RIGHT_PRESSED)
	{
		BLOCK_SWAP( block, state_block_exit);
	}
	else if( e->type == SHIFTKEY)
	{
		BLOCK_SWAP( block, state_block_menu_brick_trigger_fixed);
	}
	else
	{
		brick_dispatch( brick);
	}
}

void state_block_menu_brick_trigger( t_block *block, t_event *e)
{
	ctx_ui_log( "block_menu_brick_trigger");
	t_brick *brick = block->selected;
	t_context *C = ctx_get();
	if( e->type == UI_BRICK_RELEASED)
	{
		if( brick->type == bt_trigger && !C->app->keyboard->shift) block_menu_close( block);
		else BLOCK_SWAP( block, state_block_menu_default);
	}
	else if( e->type == MOUSE_RIGHT_PRESSED)
	{
		BLOCK_SWAP( block, state_block_exit);
	}
	else if( e->type == SHIFTKEY)
	{
		BLOCK_SWAP( block, state_block_menu_brick_trigger_fixed);
	}
	else if( e->type == CTRLKEY)
	{
		ctx_block_dupli( block, e);
	}
	else 
	{
		brick_dispatch( brick);
	}
}

// MENU HOVER BRICK


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

void state_block_menu_hover_brick( t_block *block, t_event *e)
{
	ctx_ui_log( "block_menu_hover_brick");
	t_context *C = ctx_get();
	switch( e->type)
	{
		case MOUSE_LEFT_PRESSED: block_menu_trigger( C, block); break;
		case MOUSE_MOTION_PASSIVE: block_menu_hover( C, block); break;
		case MOUSE_RIGHT_PRESSED: BLOCK_SWAP( block, state_block_exit); break;
		default: 
			break;
	}
}

// MENU HOVER MENU

void state_block_menu_hover_menu( t_block *block, t_event *e)
{
	ctx_ui_log( "block_menu_hover_menu");
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


void block_hover_brick( t_context *C, t_block *block, t_brick *brick, t_event *e)
{
		block->selected = brick;
		switch( e->type)
		{
			case MOUSE_LEFT_PRESSED:
				brick_dispatch( brick);
				BLOCK_SWAP( block, state_block_menu_brick_trigger);
				break;
		}
}

// MENU DEFAULT

void state_block_menu_default( t_block *block, t_event *e)
{
	t_context *C = ctx_get();
	ctx_ui_log( "block_menu_default");

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
				block_hover_brick( C, block, brick, e);
			}
		}
	}
}
