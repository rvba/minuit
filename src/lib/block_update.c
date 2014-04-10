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

// [block] brick->menu 		=> pointing to another block-menu 
// [brick] block->submenu 	=> submenu selection
// [brick] block->selected	=> brick selection

void state_block_menu_default( t_block *block, t_event *e);
void state_block_menu_hover_menu( t_block *block, t_event *e);
void state_block_menu_hover_brick( t_block *block, t_event *e);
void state_block_menu_brick_trigger( t_block *block, t_event *e);
void state_block_brick_trigger( t_block *block, t_event *e);
void state_block_disconnect( t_block *block, t_event *e);


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
	TRIGGER 
	**********************************	*/


void state_block_brick_trigger( t_block *block, t_event *e)
{
	ctx_ui_log( "block_brick_trigger");
	t_brick *brick = block->selected;
	if( e->type == UI_BRICK_RELEASED)
	{
		ctx_event_add( UI_BLOCK_RELEASED);
		if( block->rhizome) rhizome_setup( block->rhizome);
		BLOCK_SWAP( block, state_block_default);
	}
	else
	{
		brick->cls->dispatch( brick); 
	}
}

void block_menu_trigger( t_context *C, t_block *block)
{
	t_brick *brick = block_brick_hover( C);
	block->selected = brick;
	brick->cls->dispatch( brick); 
	BLOCK_SWAP( block, state_block_menu_brick_trigger);
}

void block_brick_trigger( t_context *C, t_block *block)
{
	t_brick *brick = block_brick_hover( C);
	block->selected = brick;
	brick->cls->dispatch( brick); 

	BLOCK_SWAP( block, state_block_brick_trigger);
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

/*	**********************************
	:LINKING
	**********************************	*/


void block_linking_stop( t_context *C, t_block *block)
{
	block->block_state.connecting = 0;
	C->ui->draw_link = 0;
	BLOCK_SWAP( block, state_block_default);
	t_brick *brick = block->selected;
	if( brick) brick->brick_state.connecting = 0;
	block->selected = NULL;
	ctx_event_add( UI_BLOCK_RELEASED);
}

void block_connect( t_context *C, t_block *block, t_brick *brick)
{
	if( ctx_mouse_hover_brick_plug_in( C, brick))
	{
		_cls_brick_connect( brick, block->selected);
	}

}

void state_brick_linking( t_block *block, t_event *e)
{
	ctx_ui_log( "block_brick_linking");
	t_context *C = ctx_get();
	t_brick *brick = ctx_ui_hover_get( C, dt_brick);

	C->event->end_x = e->x;
	C->event->end_y = e->y;

	if( brick)
	{
		switch( e->type)
		{
			case MOUSE_LEFT_RELEASED:
				block_connect( C, block, brick);
				block_linking_stop( C, block);
				break;
		}
	}
	else
	{
		switch( e->type)
		{
			case MOUSE_LEFT_RELEASED:
				block_linking_stop( C, block);
				break;
		}
	}
}

void block_linking_start( t_context *C, t_block *block, t_brick *brick)
{
	block->selected = brick;
	block->block_state.connecting = 1;
	brick->brick_state.connecting = 1;
	BLOCK_SWAP( block, state_brick_linking);
}

void block_connect_start( t_context *C, t_block *block, t_brick *brick, t_event *e)
{
	if( !brick->plug_out.dst)
	{
		C->ui->draw_link = 1;

		float v[3];
		block_get_pos_plug_out( block, brick, v);

		float dd[3] = { C->ui->pan_x, C->ui->pan_y, 0};
		vadd( v, v, dd);

		C->event->end_x = v[0];
		C->event->end_y = v[1];

		C->event->start_x = v[0];
		C->event->start_y = v[1];

		block_linking_start( C, block, brick);
	}
}

/*	**********************************
	:DISCONNECT
	**********************************	*/

void state_block_disconnect( t_block *block, t_event *e)
{
	ctx_ui_log( "block_disconnect");

	t_context *C = ctx_get();

	switch( e->type)
	{
		case MOUSE_LEFT_RELEASED:
			block_linking_stop( C, block);
			break;
	}
}

void block_disconnect( t_context *C, t_block *block, t_brick *brick, t_event *e)
{
	t_plug *plug_target = brick->plug_in.src;
	if( plug_target)
	{
		C->ui->draw_link = 1;

		_cls_brick_disconnect( brick);

		block->block_state.connecting = 0;

		block_linking_stop( C, block);
	}
}

void block_hover_brick( t_context *C, t_block *block, t_brick *brick, t_event *e)
{
		block->selected = brick;
		switch( e->type)
		{
			case MOUSE_LEFT_PRESSED:
				brick->cls->dispatch( brick);
				BLOCK_SWAP( block, state_block_menu_brick_trigger);
				break;
		}
}

/*	**********************************
	:MOVE
	**********************************	*/

void state_block_move( t_block *block, t_event *e)
{
	ctx_ui_log( "block_move");

	float mouse[3];
	float center[3];
	float vector[3];

	t_context *C = ctx_get();

	ctx_get_mouse_pos(C,mouse);
	block_get_center( block, center);

	vsub( vector, mouse, center);

	vcp( block->pos, vector);

	switch( e->type)
	{
		case MOUSE_RIGHT_RELEASED:
			BLOCK_SWAP( block, state_block_default);
			ctx_event_add( UI_BLOCK_RELEASED);
			break;
	}
}

void ctx_ui_plug_out( t_context *C, t_block *block, t_brick *brick, t_event *e)
{
	t_plug *plug = &brick->plug_out;
	switch( e->type)
	{
		case	OKEY:
		    	plug->state.flow_out = switch_int(plug->state.flow_out);
			BLOCK_SWAP( block, state_block_default);
			ctx_event_add( UI_BLOCK_RELEASED);
			break;
	}

}

/*	**********************************
	:DEFAULT
	**********************************	*/

void block_brick_delete( t_block *block, t_brick *brick)
{
	t_context *C = ctx_get();

	block->selected = brick;
	 block_brick_trigger( C, block);
}

void state_block_default( t_block *block, t_event *e)
{
	ctx_ui_log( "block_default");
	t_context *C = ctx_get();

	if( e->type == MOUSE_RIGHT_PRESSED)
	{
		BLOCK_SWAP( block, state_block_move);
	}
	else if( e->type == UI_BLOCK_MOVE)
	{
		BLOCK_SWAP( block, state_block_move);
	}
	else
	{
		t_brick *brick = ctx_ui_hover_get( C, dt_brick);
		if( brick)
		{
			if( e->type == DKEY)
			{
				block_brick_delete( block, brick);
			}

			if( e->type == MOUSE_RIGHT_PRESSED)
			{
				BLOCK_SWAP( block, state_block_move);
			}
			else
			{
				if( ctx_mouse_hover_brick_plug_out( C, brick))
				{
					if(  e->type == MOUSE_LEFT_PRESSED)
					{
						block_connect_start( C, block, brick, e);
					}

					if( EVENT_KEYBOARD( e->type))
					{
						ctx_ui_plug_out( C, block, brick, e);
					}
				}
				else if( ctx_mouse_hover_brick_plug_in( C, brick))
				{
					if(  e->type == MOUSE_LEFT_PRESSED)
					{
						if( brick->plug_in.src)
						{
							block_disconnect( C, block, brick, e);
						}
					}
				}
				else
				{
					switch( e->type)
					{
						case MOUSE_LEFT_PRESSED:
							block_brick_trigger( C, block);
							break;

						case MOUSE_MIDDLE_PRESSED:
							block_brick_trigger( C, block);
							break;

						default:
							break;
					}
				}
			}
		}
	}
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
		brick->cls->dispatch( brick); 
	}
}

void state_block_menu_brick_trigger( t_block *block, t_event *e)
{
	ctx_ui_log( "block_menu_brick_trigger");
	t_brick *brick = block->selected;
	if( e->type == UI_BRICK_RELEASED)
	{
		if( brick->type == bt_trigger) block_menu_close( block);
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
	else 
	{
		brick->cls->dispatch( brick); 
	}
}

// MENU HOVER BRICK

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
