/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "node.h"
#include "context.h"
#include "scene.h"
#include "action.h"
#include "app.h"
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
#include "action.h"
#include "dict.h"
#include "set.h"
#include "object.h"
#include "camera.h"
#include "viewport.h"

#define UI_EVENT_MAX 10

t_lst *EXE=NULL;
int UI_EVENT;

/*	**********************************
	DECLARATIONS
	*********************************	*/

void state_ui_block_trigger( t_context *C, t_event *e);
void state_ui_default( t_context *C, t_event *e);
void state_ui_motion( t_context *C, t_event *e);

void ctx_ui_space_rotate( t_context *C, t_event *e);

void ctx_ui_block_trigger( t_context *C);

void op_debug_all(t_context *C)
{
	// OFF
	if(C->event->debug_all)
	{
		C->event->debug_all = 0;
		C->ui->show_term = 0;
		C->ui->show_rhizome_bounding_box = 0;
		C->ui->show_rhizome_order = 0;
		C->scene->debug_all = 0;
		PLUG_DEBUG = 0;
	}
	// ON
	else
	{
		C->event->debug_all = 1;
		C->ui->show_term = 1;
		C->ui->show_rhizome_bounding_box = 1;
		C->ui->show_rhizome_order = 1;
		C->scene->debug_all = 1;
		PLUG_DEBUG = 1;
	}
}

/*	**********************************
	:UI_EVENT
	*********************************	*/

void ctx_ui_event_init( t_context *C)
{
	UI_EVENT = UI_EVENT_NULL;
}

void ctx_ui_event_add( int ui_event)
{
	UI_EVENT = ui_event;
}

/*	**********************************
	:DELETE
	*********************************	*/

void block_delete(t_action *action)
{
	t_context *C = ctx_get();
	t_dict *args = action->args;

	t_brick *brick = ( t_brick *) dict_pop_data(args,"brick");
	t_block *block = brick->block;

	if(
		   !block_is_connected("in",block)
		&& !block_is_connected("out",block))
	{
		t_set *set = get_current_set(C);
		t_lst *lst = set->blocks;

		list_remove_by_ptr(lst,block);
		scene_delete(C->scene,block);
	}
}

void block_do_delete( t_block *block)
{
	t_action *action = action_new("action");
	t_brick *brick = block->selected;

	action->act = block_delete;

	t_dict *dict = dict_make("args");
	action->args = dict;
	action->brick = brick;

	dict_symbol_add( action->args, "brick", dt_null, brick);

	exe_add_action(action);
}


int action_check( t_action *action)
{
	t_brick *brick = action->brick;
	t_set *set = brick->block->set;
	if(set->processing) return 0;
	else return 1;
}

void ctx_ui_exe(t_context *C)
{
	t_link *l;
	t_action *action;

	t_lst *tmp = lst_copy( EXE);
	lst_cleanup(EXE);

	for(l=tmp->first;l;l=l->next)
	{
		action = ( t_action *) l->data;
		if( action_check( action))
		{
			action->act(action);
			action_free(action);
		}
		else
		{
			exe_add_action( action);
		}
	}

	lst_free( tmp);
}

void exe_init( void)
{
	EXE=lst_new("exe");
}

void exe_add_action(t_action *action)
{
	lst_add(EXE,action,"action");
}

void ctx_ui_buffer_clear( t_context *C)
{
	bzero(&C->event->buffer_char[0],_NAME_LONG_);
	C->event->buffer_char_counter=0;
}

void ctx_ui_log( const char *name)
{
	char msg[128];

	t_context *C = ctx_get();
	t_term *term = term_get( "term_state");

	snprintf( msg, 128,"%d %s", C->app->frame, name);

	if( term) term_print( term, msg);
	else printf("[CTX_LOG] Can't find term\n");
}

void ctx_ui_freeze(t_context *C)
{
	if(C->ui->update_links) C->ui->update_links = 0;
	else C->ui->update_links = 1;
}

void ctx_ui_switch_show_states(t_context *C)
{
	if(C->ui->show_states)
		C->ui->show_states = 0;
	else
		C->ui->show_states = 1;
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
	int color[] = { 255, 255, 255};
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
			t_brick *brick = ( t_brick *) node->data;
			brick->brick_state.is_mouse_over = 0;
			brick->block->block_state.is_mouse_over = 0;
		}
		else if( node->cls->type == dt_object)
		{
			t_object *object = ( t_object *) node->data;
			object->hover = 0;
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
		node = ctx_scene_hover( C, dt_object);
		if( node)
		{
			C->scene->hover = node;
			C->scene->hover_type = dt_object;
		}
		else
		{
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
		}
	}
}

/*	**********************************
	:OPERATORS
	*********************************	*/

void ctx_clock_test( t_context *C)
{
	int udelta = clock_get_delta_usec( C->ui->clock);

	if( udelta >= 700000)
	{
		ctx_event_add( MOUSE_LEFT_LONG);
		clock_time_set( C->ui->clock);
	}
}

void ctx_events_swap( t_context *C)
{
	ctx_event_cleanup( C->event->events_swap);
	lst_dupli( C->event->events_swap, C->event->events);
}

void ctx_ui_mouse_store( t_context *C, t_event *e)
{
	vset3f(C->ui->last_clic, (float) e->x, (float) e->y, 0);
}

void ctx_ui_mouse( t_context *C)
{
	t_link *l;
	t_event *e;
	t_ui *ui = C->ui;

	for(l=C->event->events->first;l;l=l->next)
	{
		e = ( t_event *) l->data;
		switch( e->type)
		{
			case MOUSE_RIGHT_PRESSED:
				ui->mouse_right_pressed = 1;
				ctx_ui_mouse_store( C, e);
				break;

			case MOUSE_RIGHT_RELEASED:
				ui->mouse_right_pressed = 0;
				break;

			case MOUSE_LEFT_PRESSED:
				clock_time_set( C->ui->clock);
				ctx_ui_mouse_store( C, e);
				ui->mouse_left_pressed = 1;
				break;

			case MOUSE_LEFT_RELEASED:
				ui->mouse_left_pressed = 0;
				break;
		}
	}

	if( ui->mouse_left_pressed)
	{
		ctx_clock_test( C);
	}
}

void ctx_ui_reset( t_context *C)
{
	C->ui->key_shift = 0;
	C->ui->key_alt = 0; 
	C->ui->key_ctrl= 0; 
	C->app->keyboard->shift =0;
	C->app->keyboard->modifier = 0;
	C->app->mouse->wheel = wheel_idle;
}

void ctx_ui_menu_show( t_context *C)
{
	t_node *node = scene_node_get( C->scene, "block", "menu_mouse");
	t_block *block = ( t_block *) node->data;

	block->pos[0] = C->app->mouse->x + 10;
	block->pos[1] = C->app->mouse->y + 10;

	C->ui->show_menu = 1; 

	ctx_ui_selection_set( C, node);
}

void ctx_ui_browser( t_context *C)
{
	t_node *node = scene_node_get( C->scene, "block", "block_browser");
	if( node)
	{
		ctx_ui_selection_set( C, node);
		UI_EVENT = UI_EVENT_NULL;
		UI_SWAP( C, state_ui_block_trigger); 
	}
	else
	{
	}
}

void ctx_ui_block_select( t_context *C)
{
	t_brick *brick = ( t_brick *) ctx_ui_hover_get( C, dt_brick);
	if( brick)
	{
		t_block *block = brick->block;
		t_node *node = block->id.node;

		ctx_ui_selection_set( C, node);
	}
}

void ctx_ui_menu_hide( t_context *C)
{
	t_node *node = scene_node_get( C->scene, "block", "menu_mouse");
	t_block *block = ( t_block *) node->data;

	C->ui->show_menu = 0;
	block_unstore( block);
}

void ctx_ui_intro_stop( t_context *C)
{
	C->ui->show_intro=0;
}

/*	**********************************
	:TRIGGER
	*********************************	*/
void ctx_ui_deselect( t_context *C, t_event *e)
{
	t_node *node = C->scene->selected;
	if( node)
	{
		ctx_scene_selection( C, node, 0);
		C->scene->selected = NULL;
	}
}

void state_ui_block_trigger( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_block_trigger");
	if( e->type == UI_BLOCK_RELEASED)
	{
		ctx_ui_selection_release( C);
		UI_SWAP( C, state_ui_default);
	}
	else if( e->type == BLOCK_DELETE)
	{
		t_block *block = ( t_block *) ctx_ui_selection_get( C, dt_block);
		block_do_delete( block);
		ctx_ui_selection_release( C);
		UI_SWAP( C, state_ui_default);
	}
	else
	{
		t_block *block = ( t_block *) ctx_ui_selection_get( C, dt_block);
		block->cls->dispatch( block);
	}
}

void state_ui_object_trigger( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_object_trigger");
	UI_SWAP( C, state_ui_default);
}

void ctx_ui_block_trigger( t_context *C)
{
	ctx_ui_block_select( C);
	t_block *block = ( t_block *) ctx_ui_selection_get( C, dt_block);
	block->cls->dispatch( block);
	UI_SWAP( C, state_ui_block_trigger);
}

void ctx_ui_object_trigger( t_context *C)
{
	t_object *object = ( t_object *) ctx_ui_hover_get( C, dt_object);
	if( object)
	{
		ctx_scene_set_selected( C, object->id.node);
		UI_SWAP( C, state_ui_object_trigger);
	}
}

/*	**********************************
	:MIDDLE
	**********************************	*/


t_camera *ctx_ui_camera_get( t_context *C)
{
	t_camera *camera = NULL;

	if(C->scene->has_generic_viewport)
	{
		// Get default Viewport
		t_node *node_viewport = scene_node_get(C->scene,"viewport","viewport");
		t_viewport *viewport = NULL;

		if(node_viewport)
		{
			viewport = ( t_viewport *) node_viewport->data;
			camera = viewport->camera;
		}
	}

	return camera;
}


void ctx_ui_camera_rotate( t_context *C, t_camera *camera, t_event *e)
{
	float dx = (float) -C->ui->mouse_dx * .1;
	float dy = (float) C->ui->mouse_dy * .1;

	op_camera_rotate(C, camera, (float)dx,(float)dy);
}

void ctx_ui_camera_translate( t_context *C, t_camera *camera, t_event *e)
{
	op_camera_translate(C, camera);
}

void ctx_ui_camera_zoom( t_context *C, t_camera *camera, t_event *e)
{
	if(C->app->keyboard->shift) C->ui->zoom-=.1;
	if (camera->type == camera_frustum)
	{
		switch( e->type)
		{
			case MOUSE_WHEEL_UP: 	op_camera_zoom(C, camera, -1); break;
			case MOUSE_WHEEL_DOWN: 	op_camera_zoom(C, camera, 1); break;
		}
	}
	else
	{
		switch( e->type)
		{
			case MOUSE_WHEEL_UP:	op_camera_set_ortho_zoom(C, camera,-1); 
			case MOUSE_WHEEL_DOWN:	op_camera_set_ortho_zoom(C, camera,1); 
		}
	}
}

void state_ui_space_zoom( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_space_zoom");
	t_camera *camera = ctx_ui_camera_get( C);
	if( camera)
	{
		op_camera_change_speed(camera);
		ctx_ui_camera_zoom(C, camera, e);
	}

	UI_SWAP( C, state_ui_default);

}

void state_ui_space_rotate( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_space_rotate");
	t_camera *camera = ctx_ui_camera_get( C);
	if( camera)
	{
		op_camera_change_speed(camera);
		ctx_ui_camera_rotate(C, camera, e);
	}

	switch( e->type)
	{
		case MOUSE_MIDDLE_RELEASED:
		case MOUSE_RIGHT_RELEASED:
			UI_SWAP( C, state_ui_default);
			break;
	}
}

void state_ui_space_translate( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_space_translate");
	t_camera *camera = ctx_ui_camera_get( C);
	if( camera)
	{
		op_camera_change_speed(camera);
		ctx_ui_camera_translate(C, camera, e);
	}

	switch( e->type)
	{
		case MOUSE_RIGHT_RELEASED:
			UI_SWAP( C, state_ui_default);
			break;
	}
}

void state_ui_mouse_middle( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_mouse_middle");

	switch( e->type)
	{
		case MOUSE_MIDDLE_PRESSED: ctx_ui_space_rotate( C, e); break;
		case MOUSE_WHEEL_UP:
		case MOUSE_WHEEL_DOWN:
				UI_SWAP( C, state_ui_space_zoom);
				C->ui->state( C, e);
				break;
	}
}

void ctx_ui_middle( t_context *C, t_event *e)
{
	UI_SWAP( C, state_ui_mouse_middle);
	C->ui->state( C, e);
}

void ctx_ui_space_rotate( t_context *C, t_event *e)
{
	UI_SWAP( C, state_ui_space_rotate);
	C->ui->state( C, e);
}

/*	**********************************
	:LEFT
	**********************************	*/


void state_ui_mouse_left( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_mouse_left");

	switch( C->scene->hover_type)
	{
		case dt_brick: ctx_ui_block_trigger( C); break;
		case dt_object: ctx_ui_object_trigger( C); break;
		default: 
			UI_SWAP( C, state_ui_default);
			ctx_ui_deselect( C, e); 
			break;
	}
}

void ctx_ui_left( t_context *C, t_event *e)
{
	UI_SWAP( C, state_ui_mouse_left);
	C->ui->state( C, e);
}

void ctx_ui_pan_set( t_context *C, t_event *e)
{
	float *a = C->ui->last_clic;
	float b[3] = { e->x, e->y, 0};
	float *p = C->ui->last_pan;
	float z[3];
	float r[3];

	vsub(z, b, a);
	vadd(r, p, z);

	C->ui->pan_x = r[0];
	C->ui->pan_y = r[1];
} 

void ctx_ui_zoom_set( t_context *C, t_event *e)
{

	C->ui->zoom += ( C->ui->mouse_dy * 0.002);
} 

/*	**********************************
	:RIGHT
	**********************************	*/

void state_ui_pan( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_pan");

	switch( e->type)
	{
		case MOUSE_RIGHT_RELEASED:
			vset3f( C->ui->last_pan,  C->ui->pan_x, C->ui->pan_y, 0);
			UI_SWAP( C, state_ui_default);
			break;
		default:
			ctx_ui_pan_set( C, e);
			break;
	}
}

void state_ui_zoom( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_zoom");

	switch( e->type)
	{
		case MOUSE_RIGHT_RELEASED:
			UI_SWAP( C, state_ui_default);
			break;
		default:
			ctx_ui_zoom_set( C, e);
			break;
	}
}

void state_ui_mouse_right_motion( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_mouse_right_motion");

	if( C->scene->hover_type == dt_brick)
	{
		ctx_event_add( UI_BLOCK_MOVE);
		ctx_ui_block_trigger( C);
	}
	else
	{
		switch( e->type)
		{
			case MOUSE_RIGHT_RELEASED:
				UI_SWAP( C, state_ui_default);
				break;

			case SHIFTKEY:
				UI_SWAP( C, state_ui_pan);
				break;

			case CTRLKEY:
				UI_SWAP( C, state_ui_zoom);
				break;
		}
	}

}

void state_ui_mouse_right( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_mouse_right");

	int D = 30;

	if( C->scene->hover_type == dt_brick)
	{
		ctx_ui_block_trigger( C);
	}
	else
	{
		switch( e->type)
		{
			case SHIFTKEY: UI_SWAP( C, state_ui_space_rotate); break;
			case CTRLKEY: UI_SWAP( C, state_ui_space_translate); break;
			case MOUSE_MOTION: 
				if( ( abs( C->ui->mouse_delta_x) > D) || ( abs( C->ui->mouse_delta_y) > D))
				UI_SWAP( C, state_ui_mouse_right_motion);
				break;
			case MOUSE_RIGHT_RELEASED:
				ctx_ui_menu_show( C);
				UI_SWAP( C, state_ui_block_trigger); 
				break;
		}
				
	}
}

void ctx_ui_right( t_context *C, t_event *e)
{
	UI_SWAP( C, state_ui_mouse_right);
	C->ui->state( C, e);
}

/*	**********************************
	:INTRO
	**********************************	*/

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
			ctx_ui_menu_show( C);
			UI_SWAP( C, state_ui_block_trigger); 
			break;
	}
}

/*	**********************************
	:KEYBOARD
	**********************************	*/

void ctx_ui_keyboard( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_keyboard");
	if( C->scene->hover_type == dt_brick)
	{
		ctx_ui_block_trigger( C);
	}

}

/*	**********************************
	:DEFAULT
	*********************************	*/

void state_ui_default( t_context *C, t_event *e)
{
	ctx_ui_log( "ui_default");

	switch( UI_EVENT)
	{
		case UI_EVENT_BROWSER_SHOW:
						ctx_ui_browser( C);
						break;
		default:
			break;
	}

	switch( e->type)
	{
		case MOUSE_RIGHT_PRESSED: 	ctx_ui_right( C, e); break;
		case MOUSE_LEFT_PRESSED: 	ctx_ui_left( C, e); break;

		case MOUSE_MIDDLE_PRESSED:
		case MOUSE_WHEEL_UP:
		case MOUSE_WHEEL_DOWN:
		 				ctx_ui_middle( C, e); 
						break;

		default: break;
	}

	if( EVENT_KEYBOARD( e->type))
	{
		ctx_ui_keyboard( C, e);
	}
}

/*	**********************************
	:DISPATCH
	*********************************	*/

void ctx_ui_dispatch( t_context *C)
{
	t_link *l;
	t_event *e;

	ctx_events_swap( C);
	lst_cleanup( C->event->events);

	for(l=C->event->events_swap->first;l;l=l->next)
	{
		e = ( t_event *) l->data;
		event_log( e);
		C->ui->state( C, e);
	}

	if( UI_EVENT != UI_EVENT_NULL)
	{
		//event_ui_log( UI_EVENT);
		//C->ui->state( C, NULL);
	}
}

/*	**********************************
	:CTX_UI
	*********************************	*/

void ctx_ui(t_context *C)
{
	ctx_ui_mouse( C);
	ctx_ui_hover( C);
	ctx_ui_dispatch( C);
	ctx_ui_exe( C);

	if(C->event->color_transition_use && C->event->color_transition)
	{
		op_set_color(C,2);
	}

	ctx_ui_reset( C);
}

void ctx_ui_init( t_context *C)
{
	C->ui->state = state_ui_intro;
	exe_init();
	ctx_ui_event_init( C);
}


	
