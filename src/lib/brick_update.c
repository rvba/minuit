/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "node.h"
#include "scene.h"
#include "op.h"
#include "action.h"
#include "app.h"
#include "event.h"
#include "term.h"
#include "ui.h"
#include "dict.h"
#include "list.h"
#include "event.h"
#include "brick.h"
#include "block.h"
#include "binding.h"
#include "rhizome.h"
#include "set.h"
#include "event.h"

int is_vec_stored=0;
float v[3];
float vec[3];

void state_brick_slider_default( t_brick *brick, t_event *e);
void state_brick_default( t_brick *brick, t_event *e);
void state_brick_edit( t_brick *brick, t_event *e);

typedef void (* State_Func) (struct Brick *, struct Event *);
State_Func brick_state_set_default( t_brick *brick)
{
	switch( brick->type)
	{
		case bt_switch: return 		&state_brick_switch_default; break;
		case bt_slider: return 		&state_brick_slider_default; break;
		default: return 		&state_brick_default; break;
	}
}

int brick_check_viewport( t_brick *brick)
{
	t_context *C=ctx_get();
	if(C->scene->has_generic_viewport) return 1;
	else return 0;
}

/*	**********************************
	:CONNECT
	**********************************	*/

void cls_brick_connect(t_brick *brick_in ,t_brick *brick_out)
{
	t_context *C = ctx_get();
	C->ui->do_connect = 1;
	C->ui->connect_brick_in = brick_in;
	C->ui->connect_brick_out = brick_out;
}

void _cls_brick_connect(t_brick *brick_in ,t_brick *brick_out)
{
	t_plug *plug_brick_in = &brick_in->plug_intern;
	t_plug *plug_brick_out = &brick_out->plug_intern;

	// first Out
	plug_brick_out->cls->connect(mode_out, plug_brick_out, plug_brick_in);
	// Then In
	plug_brick_in->cls->connect(mode_in, plug_brick_in, plug_brick_out);
}

/*	**********************************
	:DISCONNECT
	**********************************	*/

void _cls_brick_disconnect(t_brick *self)
{
	t_brick *brick_in = self;
	t_plug *plug_target = brick_in->plug_in.src;
	if( plug_target)
	{
		t_brick *brick_out = plug_target->brick;
		t_plug *plug_out = &brick_out->plug_intern;
		t_plug *plug_in = &brick_in->plug_intern;

		plug_out->cls->disconnect(mode_out ,plug_out);
		plug_in->cls->disconnect(mode_in , plug_in);

		// Split Graph
		brick_rhizome_split(brick_in, brick_out);
	}
}

void cls_brick_disconnect(t_brick *self)
{
	t_context *C = ctx_get();
	C->ui->do_disconnect = 1;
	C->ui->connect_brick_in = self;
}


void brick_hover_set( t_brick *brick)
{
	t_context *C = ctx_get();
	if( ctx_mouse_hover_brick_left( C, brick))	 	brick->state_pressed = BRICK_LEFT;
	else if( ctx_mouse_hover_brick_right( C, brick)) 	brick->state_pressed = BRICK_RIGHT;
	else if( ctx_mouse_hover_brick_plug_in( C, brick))	brick->state_pressed = BRICK_PLUG_IN;
	else if( ctx_mouse_hover_brick_plug_out( C, brick))	brick->state_pressed = BRICK_PLUG_OUT;
	else							brick->state_pressed = BRICK_NULL;
}

void brick_do_delete( t_brick *brick)
{
	ctx_event_add( BLOCK_DELETE);
	BRICK_SWAP( brick, state_brick_default);
}


/*	**********************************
	:EDIT
	**********************************	*/


void brick_edit_name( t_context *C, t_brick *brick)
{
	t_plug *plug = &brick->plug_intern;

	if(plug->data_type==dt_int)
	{
		int *dt=plug->data;
		*dt=atoi(C->event->buffer_char);
	}
	else if(plug->data_type==dt_float)
	{
		float *dt=plug->data;
		*dt=atof(C->event->buffer_char);
	}
}

void brick_edit_start( t_context *C, t_brick *brick)
{
	set_name_long( C->event->buffer_char, brick->txt_data.name);
	C->event->buffer_char_counter = strlen( brick->txt_data.name);
	C->event->ui.typing_start = 1;
	brick->typing = 1;
	BRICK_SWAP( brick, state_brick_edit);
}

void brick_edit_stop( t_context *C, t_brick *brick)
{
	C->event->ui.typing_start = 0;
	C->event->is_typing=0;
	ctx_ui_buffer_clear( C);

	C->event->ui.typing_start = 0;
	brick->state = brick_state_set_default( brick);
	brick->typing = 0;
	ctx_event_add( UI_BRICK_RELEASED);
}

void state_brick_edit( t_brick *brick, t_event *e)
{
	ctx_ui_log( "brick_edit");
	t_context *C = ctx_get();
	switch( e->type)
	{
		case RETURNKEY:
		case MOUSE_LEFT_PRESSED:
		case MOUSE_RIGHT_PRESSED:
			brick_edit_stop( C, brick);
			break;
		case SHIFTKEY:
			break;
		default:
			brick_edit_name( C, brick);
			break;
	}
}


/*	**********************************
	SLIDER
	**********************************	*/

void state_brick_slider_drag( t_brick *brick, t_event *e)
{
	ctx_ui_log( "brick_slider_drag");
	t_context *C = ctx_get();
	if( e->type == MOUSE_LEFT_RELEASED)
	{
		ctx_event_add( UI_BRICK_RELEASED);
		BRICK_SWAP( brick, state_brick_slider_default);
	}
	else
	{
		if( C->ui->mouse_delta_x >= 0) brick->state_pressed = BRICK_RIGHT;
		else brick->state_pressed = BRICK_LEFT;
		brick->act( brick);
	}
}

void state_brick_slider_trigger( t_brick *brick, t_event *e)
{
	ctx_ui_log( "brick_slider_trigger");
	t_context *C = ctx_get();
	switch( e->type)
	{
		case MOUSE_LEFT_RELEASED:
			brick->act( brick);
			ctx_event_add( UI_BRICK_RELEASED);
			BRICK_SWAP( brick, state_brick_slider_default);
			break;

		case MOUSE_MOTION:
			BRICK_SWAP( brick, state_brick_slider_drag);
			break;

		case MOUSE_LEFT_LONG:
			brick_edit_start( C, brick);
			break;
	}
}

void state_brick_slider_default( t_brick *brick, t_event *e)
{
	ctx_ui_log( "brick_slider_default");
	switch( e->type)
	{
		case MOUSE_LEFT_PRESSED:
			brick_hover_set( brick);
			if( !brick->plug_in.src)
			{
				BRICK_SWAP( brick, state_brick_slider_trigger);
				brick->cls->dispatch( brick);
			}
			else
			{
				ctx_event_add( UI_BRICK_RELEASED);
				BRICK_SWAP( brick, state_brick_slider_default);
			}

			break;

		case MOUSE_MIDDLE_PRESSED:
			brick_hover_set( brick);
			BRICK_SWAP( brick, state_brick_slider_trigger);
			break;
		case DKEY:
			brick_do_delete( brick);
			break;
	}
}

/*	**********************************
	SWITCH
	**********************************	*/


void state_brick_switch_default( t_brick *brick, t_event *e)
{
	ctx_ui_log( "brick_switch_default");
	switch( e->type)
	{
		case MOUSE_LEFT_PRESSED:
			brick->act( brick);
			break;

		case MOUSE_LEFT_RELEASED:
			ctx_event_add( UI_BRICK_RELEASED); 
			BRICK_SWAP( brick, state_brick_switch_default);
			break;
	}
}

void state_brick_trigger( t_brick *brick, t_event *e)
{
	ctx_ui_log( "brick_trigger");
	switch( e->type)
	{
		case MOUSE_LEFT_RELEASED:
			brick->act( brick);
			ctx_event_add( UI_BRICK_RELEASED); 
			BRICK_SWAP( brick, state_brick_default);
			break;
	}
}

					
void state_brick_default( t_brick *brick, t_event *e)
{
	ctx_ui_log( "brick_default");
	switch( e->type)
	{
		case MOUSE_LEFT_PRESSED:
			BRICK_SWAP( brick, state_brick_trigger);
			break;
	}
}

/*	**********************************
	DISPATCH
	**********************************	*/

void cls_brick_dispatch( t_brick *brick)
{
	t_context *C = ctx_get();
	t_link *l;
	t_event *e;
	for(l=C->event->events_swap->first;l;l=l->next)
	{
		e = l->data;
		brick->state( brick, e);
	}
}
