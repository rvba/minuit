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
		brick_dispatch( brick);
	}
}

void block_brick_trigger( t_context *C, t_block *block)
{
	t_brick *brick = block_brick_hover( C);
	block->selected = brick;
	brick_dispatch( brick);

	BLOCK_SWAP( block, state_block_brick_trigger);
}

/*	**********************************
	:CONNECT
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

void block_linking_swap( t_context *C, t_block *block, t_block *block_target)
{
	C->scene->selection_swap = block_target->id.node;

	block->block_state.connecting = 0;

//	C->ui->draw_link = 0;

	BLOCK_SWAP( block, state_block_default);
	t_brick *brick = block->selected;
	if( brick) brick->brick_state.connecting = 0;
	block->selected = NULL;
	ctx_event_add( UI_BLOCK_SWAP);

}

void block_connect( t_context *C, t_block *block, t_brick *brick)
{
	if( ctx_mouse_hover_brick_plug_in( C, brick))
	{
		brick->cls->connect( brick, block->selected);
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

		brick->cls->disconnect( brick);

		block->block_state.connecting = 0;


		t_brick *brick_target = plug_target->brick;
		t_block *block_target = brick_target->block;

		block_linking_swap( C, block, block_target);

		block_connect_start( C, block_target, brick_target, e);
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
		if( !block->state)
		{
			block->state = state_block_default;
		}
		block->state( block, e);
	}
}
