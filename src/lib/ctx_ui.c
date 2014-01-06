/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "node.h"
#include "context.h"
#include "scene.h"
#include "action.h"
#include "ctx.h"
#include "app.h"
#include "process.h"
#include "engine.h"
#include "event.h"
#include "term.h"
#include "draw.h"
#include "ui.h"
#include "block.h"
#include "brick.h"
#include "list.h"
#include "data.h"
#include "plug.h"
#include "set.h"
#include "screen.h"
#include "rhizome.h"
#include "op.h"

int _is_vec_stored=0;
float v[3];
float vec[3];

/*	**********************************
	DECLARATIONS
	*********************************	*/


void state_ui_block_trigger( t_context *C, t_event *e);
void state_ui_default( t_context *C, t_event *e);
void state_ui_motion( t_context *C, t_event *e);

void ctx_ui_buffer_clear( t_context *C)
{
	bzero(&C->event->buffer_char[0],20);
	C->event->buffer_char_counter=0;
	C->event->ui.typing_start = 0;
	C->event->ui.typing_end = 0;
}

void ctx_ui_log( const char *name)
{
	char msg[128];

	t_context *C = ctx_get();
	t_term *term = term_get( "term_state");

	snprintf( msg, 128,"%d %s", C->app->frame, name);

	if( term)
	{
		term_print( term, msg);
	}
}

/*	**********************************
	SELECTION
	*********************************	*/

int ctx_ui_selection_set( t_context *C, t_node *node)
{
	if ( C->scene->selection == NULL)
	{
		C->scene->selection = node;
		return 1;
	}
	else 
	{
		printf("[SELECTION SET] SELECTION NOT EMPTY\n");
		return 0;
	}
}

void ctx_ui_selection_release( t_context *C)
{
	if( C->scene->selection) C->scene->selection = NULL;
	else printf("[SELECTION RELEASE] NULL SELECTION\n");
}

void *ctx_ui_selection_get( t_context *C, t_data_type type)
{
	if( C->scene->selection)
	{
		t_node *node = C->scene->selection;
		if( node->type == type)
		{	
			return node->data;
		}
		else
		{
			printf("[SELECTION GET] WRONG TYPE\n");
			return NULL;
		}
	}
	else
	{
		printf("[SELECTION GET] NULL SELECTION\n");
		return NULL;
	}
}

/*	**********************************
	HOVER
	*********************************	*/

void *ctx_ui_hover_get( t_context *C, t_data_type type)
{
	if( C->scene->hover_type == type) return C->scene->hover->data;
	else return NULL;
}

t_node *ctx_ui_hover_get_node( t_context *C, t_data_type type)
{
	if( C->scene->hover_type == type) return C->scene->hover;
	else return NULL;
}

void ctx_ui_hover_set_selection( t_context *C, t_data_type type)
{
	t_node *node = ctx_ui_hover_get_node( C, type);
	if(node)
	{
		if( !ctx_ui_selection_set( C, node)) printf("[HOVER SELECTION] ERR\n");
	}
	else
	{
		printf("[HOVER SELECTION] NULL NODE\n");
	}
}

int ctx_ui_hover_background( t_context *C)
{
	int color[3] = { 255, 255, 255};
	int *c = color;
	return COLOR_MATCH( C->event->color, c);
}

void ctx_ui_hover_reset( t_context *C)
{
	t_node *node = NULL;
	if( C->scene->hover)
	{
		node = C->scene->hover;
		if( node->cls->type == dt_brick)
		{
			t_brick *brick = node->data;
			brick->brick_state.is_mouse_over = 0;
		}
	}
};

void ctx_ui_hover( t_context *C)
{
	ctx_ui_hover_reset( C);

	if( ctx_ui_hover_background( C))
	{
		C->scene->hover = NULL;
		C->scene->hover_type = dt_null;
	}
	else
	{
		t_node *node = NULL;
		/*
		node = ctx_scene_hover( C, dt_object);
		if( node)
		{
			C->scene->hover = node;
			C->scene->hover_type = dt_object;
		}
		else
		{
			*/
			node = ctx_scene_hover( C, dt_brick);
			if( node)
			{
				C->scene->hover = node;
				C->scene->hover_type = dt_brick;
			}
			else
			{
				C->scene->hover = NULL;
				C->scene->hover_type = dt_null;
			}
			/*
		}
		*/
	}
}

/*	**********************************
	OPERATORS
	*********************************	*/


void ctx_ui_brick_move( t_context *C, t_brick *brick)
{
	t_block *block = brick->block;
	float mouse_pos[2];

	float *block_pos=block->pos;
	ctx_get_mouse_pos(C,mouse_pos);

	if(!_is_vec_stored)
	{
		_is_vec_stored=1;
		vsub(vec,block_pos,mouse_pos);
	}
	else
	{
		vadd(v,mouse_pos,vec);
		vcp(block_pos,v);
	}

	vadd(v,mouse_pos,vec);
	vcp(block_pos,v);
}

void CTX_ui_menu_show( t_context *C)
{
	t_node *node = scene_node_get( C->scene, "block", "menu_mouse");
	t_block *block = node->data;

	block->pos[0] = C->app->mouse->x + 10;
	block->pos[1] = C->app->mouse->y + 10;

	C->ui->show_menu = 1; 

	ctx_ui_selection_set( C, node);
}

void ctx_ui_block_select( t_context *C)
{
	t_brick *brick = ctx_ui_hover_get( C, dt_brick);
	if( brick)
	{
		t_block *block = brick->block;
		t_node *node = block->id.node;

		ctx_ui_selection_set( C, node);
	}
}

void CTX_ui_menu_hide( t_context *C)
{
	t_node *node = scene_node_get( C->scene, "block", "menu_mouse");
	t_block *block = node->data;

	C->ui->show_menu = 0;
	block_unstore( block);
}

void ctx_ui_intro_stop( t_context *C)
{
	C->ui->show_intro=0;
}

/*	**********************************
	STATES
	*********************************	*/



// BLOCK MOTION

void state_ui_block_motion( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_block_motion");
	t_brick *brick = ctx_ui_selection_get( C, dt_brick);

	switch( e->type)
	{
		case MOUSE_MOTION:
			ctx_ui_brick_move( C, brick);
			break;
		case MOUSE_RIGHT_RELEASED:
			ctx_ui_selection_release( C);
			_is_vec_stored = 0;
			UI_SWAP( C, state_ui_default); 
			break;
	}
}

// TRIGGER

void state_ui_block_trigger( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_block_trigger");
	if( e->type == UI_BLOCK_RELEASED)
	{
		ctx_ui_selection_release( C);
		UI_SWAP( C, state_ui_default);
	}
	else
	{
		t_block *block = ctx_ui_selection_get( C, dt_block);
		block->cls->dispatch( block);
	}
}

/*	**********************************
	:LEFT
	**********************************	*/

void ctx_ui_brick_trigger( t_context *C)
{
	ctx_ui_block_select( C);
	t_block *block = ctx_ui_selection_get( C, dt_block);
	block->cls->dispatch( block);
	UI_SWAP( C, state_ui_block_trigger);
}

void state_ui_mouse_left( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_mouse_left");
	if( !C->scene->selection)
	{
		switch( C->scene->hover_type)
		{
			case dt_brick: ctx_ui_brick_trigger( C); break;
			default: UI_SWAP( C, state_ui_default); break;
		}
	}
}

void ctx_ui_left( t_context *C, t_event *e)
{
	UI_SWAP( C, state_ui_mouse_left);
	C->ui->state( C, e);
}


/*	**********************************
	:RIGHT
	**********************************	*/


void state_ui_mouse_right_motion( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_mouse_right_motion");

	if( C->scene->hover_type == dt_brick)
	{
		ctx_ui_hover_set_selection( C, dt_brick);
		UI_SWAP( C, state_ui_block_motion);
	}
	else
	{
		switch( e->type)
		{
			case MOUSE_RIGHT_RELEASED:
				UI_SWAP( C, state_ui_default);
				break;
		}
	}
}

// MOUSE RIGHT

void state_ui_mouse_right( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_mouse_right");
	switch( e->type)
	{
		case MOUSE_MOTION: 
			UI_SWAP( C, state_ui_mouse_right_motion);
			break;

		case MOUSE_RIGHT_RELEASED: 
			CTX_ui_menu_show( C);
			UI_SWAP( C, state_ui_block_trigger);
			break;
	}
}

void ctx_ui_right( t_context *C, t_event *e)
{
	UI_SWAP( C, state_ui_mouse_right);
	C->ui->state( C, e);
}





// MOUSE MIDDLE

void state_ui_mouse_middle( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_mouse_middle");
	switch( C->scene->hover_type)
	{
		case dt_brick:
			if( !C->scene->selection)
			{
				ctx_ui_block_select( C);
				UI_SWAP( C, state_ui_block_trigger);
			}
			break;

		default:
			UI_SWAP( C, state_ui_default);
			break;
	}
}

// INTRO

void state_ui_intro( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_intro");
	switch( e->type)
	{
		case MOUSE_LEFT_PRESSED:
			ctx_ui_intro_stop( C);
			UI_SWAP( C, state_ui_default); 
			break;
		case MOUSE_RIGHT_PRESSED:
			ctx_ui_intro_stop( C);
			CTX_ui_menu_show( C);
			UI_SWAP( C, state_ui_block_trigger); 
			break;
	}
}


// DEFAULT

void state_ui_default( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_default");
	switch( e->type)
	{
		case MOUSE_RIGHT_PRESSED: 
			//UI_SWAP( C, state_ui_mouse_right);
			ctx_ui_right( C, e);
			break;

		case MOUSE_LEFT_PRESSED: 
			ctx_ui_left( C, e);
			break;

		case MOUSE_MIDDLE_PRESSED: 
			UI_SWAP( C, state_ui_mouse_middle);
			break;

		default: break;
	}
}


/*	**********************************
	DISPATCH
	*********************************	*/


void ctx_events_swap( t_context *C)
{
	ctx_event_cleanup( C->event->events_swap);
	lst_dupli( C->event->events_swap, C->event->events);
}


void ctx_ui_dispatch( t_context *C)
{
	t_link *l;
	t_event *e;

	ctx_events_swap( C);
	lst_cleanup( C->event->events);

	for(l=C->event->events_swap->first;l;l=l->next)
	{
		e = l->data;
		event_log( e);
		C->ui->state( C, e);
	}
}

void ctx_ui(t_context *C)
{
	ctx_ui_hover( C);
	ctx_ui_dispatch( C);
}

void ctx_ui_init( t_context *C)
{
	C->ui->state = state_ui_intro;
}


	
