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
#include "op.h"
#include "action.h"
#include "app.h"
#include "event.h"
#include "viewport.h"
#include "camera.h"
#include "dict.h"
#include "data.h"
#include "list.h"
#include "vlst.h"
#include "ui.h"
#include "block.h"
#include "brick.h"
#include "vector.h"
#include "clock.h"
#include "set.h"
#include "rhizome.h"
#include "geometry.h"
#include "term.h"

// BRICK ADD

void *op_brick_add(t_brick *brick)
{
	t_context *C = ctx_get();
	t_node *node = NULL;
	t_block *block;
	t_set *set;

	// store
	scene_store(C->scene,1);

	char *name=brick->id.name;

	     if(is(name,"frame")) 		node = add_slider_int(C,"frame",&C->app->frame); 
	else if(is(name,"timer"))  		node = add_slider_float(C,"timer",&C->app->timer); 
	else if(is(name,"timer low"))  		node = add_slider_float(C,"timer low",&C->app->timer_add_low); 
	else if(is(name,"=")) 			node = add_operator_double(C,"=");
	else if(is(name,">"))  			node = add_operator_double(C,">"); 
	else if(is(name,"<"))  			node = add_operator_double(C,"<"); 
	else if(is(name,"if")) 			node = add_if(C); 
	else if(is(name,"mod"))  		node = add_operator_double(C,"mod"); 
	else if(is(name,"x")) 			node = add_maths(C,"x");
	else if(is(name,"+"))  			node = add_maths(C,"+"); 
	else if(is(name,"++")) 			node = add_plusplus(C);
	else if(is(name,"switch"))  		node = add_switch(C,"switch",NULL); 
	else if(is(name,"clone"))  		node = add_clone(C); 
	else if(is(name,"pipe")) 		node = add_pipe(C); 
	else if(is(name,"sec")) 		node = add_slider_int(C,"sec",&C->app->clock->sec);
	else if(is(name,"min")) 		node = add_slider_int(C,"min",&C->app->clock->min);
	else if(is(name,"int")) 		node = add_slider_int(C,"int",NULL);
	else if(is(name,"float")) 		node = add_slider_float(C,"float",NULL);
	else if(is(name,"10")) 			node = add_multiplier(C,"10");
	else if(is(name,"100")) 		node = add_multiplier(C,"100");
	else if(is(name,".1")) 			node = add_multiplier(C,".1");
	else if(is(name,".01")) 		node = add_multiplier(C,".01");
	else if(is(name,".001")) 		node = add_multiplier(C,".001");
	else if(is(name,"msec")) 		node = add_slider_int(C,"msec",&C->app->clock->msec);
	else if(is(name,"cos")) 		node = add_operator_single(C,"cos",op_cos);
	else if(is(name,"sin")) 		node = add_operator_single(C,"sin",op_sin);
	else if(is(name,"cam_eye_x")) 		node = add_slider_camera(C,"eye x");
	else if(is(name,"cam_eye_y")) 		node = add_slider_camera(C,"eye y");
	else if(is(name,"cam_eye_z")) 		node = add_slider_camera(C,"eye z");
	else if(is(name,"cam_pos_x")) 		node = add_slider_camera(C,"pos x");
	else if(is(name,"cam_pos_y")) 		node = add_slider_camera(C,"pos y");
	else if(is(name,"cam_pos_z")) 		node = add_slider_camera(C,"pos z");
	else if(is(name,"cam_rot_xy")) 		node = add_slider_float_custom(C,"cam rot xy",op_camera_rotate_xy); 
	else if(is(name,"cam_rot_z")) 		node = add_slider_float_custom(C,"cam rot z",op_camera_rotate_z); 
	else if(is(name,"pos x")) 		node = add_slider_object(C,"pos x");
	else if(is(name,"pos y")) 		node = add_slider_object(C,"pos y");
	else if(is(name,"pos z")) 		node = add_slider_object(C,"pos z");
	else if(is(name,"rot x")) 		node = add_slider_object(C,"rot x");
	else if(is(name,"rot y")) 		node = add_slider_object(C,"rot y");
	else if(is(name,"rot z")) 		node = add_slider_object(C,"rot z");
	else if(is(name,"scl x")) 		node = add_slider_object(C,"scl x");
	else if(is(name,"scl y")) 		node = add_slider_object(C,"scl y");
	else if(is(name,"scl z")) 		node = add_slider_object(C,"scl z");
	else if(is(name,"red")) 		node = add_slider_object(C,"red");
	else if(is(name,"green")) 		node = add_slider_object(C,"green");
	else if(is(name,"blue")) 		node = add_slider_object(C,"blue");
	else if(is(name,"color")) 		node = add_slider_object(C,"color");
	else if(is(name,"faces")) 		node = add_slider_object(C,"faces");
	else if(is(name,"alpha")) 		node = add_slider_object(C,"alpha");
	else if(is(name,"label")) 		node = add_label(C,"label");
	else if(is(name,"get")) 		node = add_get(C);
	else if(is(name,"rewind")) 		node = add_trigger_always(C,"rewind",op_rewind); 
	else if(is(name,"not")) 		node = add_slider_int_custom(C,"not",NULL,op_not); 
	else if(is(name,"mouse_x")) 		node = add_slider_int(C,"mouse x",&C->app->mouse->x); 
	else if(is(name,"mouse_y")) 		node = add_slider_int(C,"mouse y",&C->app->mouse->y); 
	else if(is(name,"keyboard")) 		node = add_slider_char(C,"keyboard",&C->app->keyboard->key_pressed); 
	else if(is(name,"rnd")) 		node = add_slider_int_custom(C,"rnd",NULL,op_rnd);
	else if(is(name,"neg")) 		node = add_slider_int_custom(C,"neg",NULL,op_neg);
	else if(is(name,"abs")) 		node = add_slider_int_custom( C, "abs", NULL, op_abs); 
	else if(is(name,"last?")) 		node = add_switch_custom(C,"last?",NULL,op_is_last);
	else if(is(name,"for")) 		node = add_for(C);
	else if(is(name,"vector 3d")) 		node = add_vector_3d(C);
	else if(is(name,"vector 2d")) 		node = add_vector_2d(C);
	else if(is(name,"bang")) 		node = add_slider_int_custom(C,"bang",NULL,op_bang); 
	else if(is(name,"quit")) 		node = add_trigger_always(C,"quit",op_do_quit); 
	else if(is(name,"const")) 		node = add_const(C); 
	else if(is(name,"and")) 		node = add_maths(C,"and"); 
	else if(is(name,"stack")) 		node = add_stack(C); 
	else if(is(name,"mesh")) 		node = add_slider_object(C,"mesh"); 
	else if(is(name,"vertex")) 		node = add_brick_mesh(C,"vertex"); 
	else if(is(name,"edges")) 		node = add_brick_mesh(C,"edges"); 

	// Store
	scene_store(C->scene,0);

	term_log("+ brick");

	// Switch Desk
	if(!C->ui->show_sets) show_sets(C);

	if( node)
	{
		block = node->data;
		set = block->set;
		set_setup( set);
	}
		
	return NULL;
}

// SLIDER

void *op_slider(t_brick *brick)
{
	void *data = brick->plug_intern.data;
	t_data_type type = brick->plug_intern.data_type;
	switch( brick->state_pressed)
	{
		case BRICK_LEFT:
			switch( type)
			{
				case dt_int: 	set_int( data, drf_int( data) - 1); break;
				case dt_float:	set_float( data, drf_float( data) - .1); break;
				default: break;
			}
			break;

		case BRICK_RIGHT:
			switch( type)
			{
				case dt_int: 	set_int( data, drf_int( data) + 1); break;
				case dt_float:	set_float( data, drf_float( data) + .1); break;
				default: break;
			}
			break;
		default:
			break;

	}
	return NULL;
}

void set_odd( void *data, t_data_type type, int sign, int positive, int negative, int strict)
{
	int val;
	int *int_ptr;
	float *float_ptr;
	int exec = 0;

	switch( type)
	{
		case dt_int:
			int_ptr = (int *) data;
			val = *int_ptr;
			exec = 1;
			break;

		case dt_float:
			float_ptr = (float *) data;
			val = (int) *float_ptr;
			exec = 1;
			break;

		default: break;
	}

	if( exec)
	{
		if( val%2 )
		{
			if( type == dt_int)
			{
				if( sign >= 0)
				{
					*int_ptr += 1;
				}
				else
				{
					*int_ptr -= 1;
				}
			}
		}

	}

	if( positive)
	{
		if( type == dt_int)
		{
			if( *int_ptr <= 0)
			{
				if( strict)
				{
					*(int_ptr) = *(int_ptr) + 2;
				}
				else
				{
					*(int_ptr) = 0;
				}
			}
		}
	}
}

void *op_slider_odd(t_brick *brick)
{
	int dir;
	op_slider( brick);

	if( brick->state_pressed == BRICK_RIGHT) dir = 1;
	else dir = -1;

	t_plug *plug = &brick->plug_intern;
	void *data = plug->data;
	set_odd( data, plug->data_type, dir, 0, 0, 0);

	return NULL;
}

void *op_slider_odd_positive_strict(t_brick *brick)
{
	int dir;
	op_slider( brick);

	if( brick->state_pressed == BRICK_RIGHT) dir = 1;
	else dir = -1;

	t_plug *plug = &brick->plug_intern;
	void *data = plug->data;
	set_odd( data, plug->data_type, dir, 1, 0, 1);

	return NULL;
}

// SLIDER POSITIVE

void *op_slider_positive(t_brick *brick)
{
	// slide
	op_slider(brick);

	// set positive
	int *val = brick->plug_intern.data;
	if(*val < 0) *val=0; 

	return NULL;
}

// SLIDER POSITIVE NON ZERO

void *op_slider_positive_non_zero(t_brick *brick)
{
	// slide
	op_slider(brick);

	// set positive
	int *val = brick->plug_intern.data;
	if(*val < 1) *val=1; 

	return NULL;
}

// ADD BRICKS

void brick_rhizome_setup(t_brick *brick)
{
	t_block *block = brick->block;
	t_rhizome *rhizome = block->rhizome;
	if(block->rhizome)
	{
		rhizome_setup(rhizome);
	}
}

void exe_remove_brick(t_action *action)
{
	// remove brick

	t_dict *args = action->args;

	t_brick *_brick = dict_pop_data(args,"brick");
	t_block *block = _brick->block;

	int remove_connected = _brick->brick_state.remove_connected;

	t_plug *plug_intern = &_brick->plug_intern;
	int *slider = plug_intern->data;

	t_lst *bricks=block->bricks;
	t_link *first = bricks->first;
	t_brick *brick=NULL;
	if(first) brick = first->data;

	t_context *C = ctx_get();
	scene_store( C->scene, 1);

	if(brick)
	{
		// check if bricks are not connected
		if(brick_delete(brick,remove_connected))
		{
			// remove link from bricks
			list_link_remove( bricks, first);
			brick_rhizome_setup(brick);
			block->tot_bricks--;
			block->block_state.update_geometry=1;
			block_brick_set_order( block);
			block->pos[1] += brick->geom.height;
		}
		else
		{
			// re-increment slider
			(*slider)++;
		}
	}

	scene_store( C->scene, 0);
}


void exe_add_brick(t_action *action)
{
	t_context *C = ctx_get();
	scene_store(C->scene, 1);

	t_dict *args = action->args;

	t_brick *brick = dict_pop_data(args,"brick");

	t_block *block=brick->block;

	t_node *n=add_part_slider_float(C,block,".",NULL);
	t_brick *b=n->data;
	b->brick_state.is_versatil=1;

	lst_lifo( block->bricks);
	block_brick_set_order( block);

	block->pos[1] -= b->geom.height;

	brick_rhizome_setup(b);

	t_plug *p_in = &b->plug_in;
	
	p_in->state.follow_in=1;
	//p_in->state.can_connect = 0;

	block->block_state.update_geometry=1;

	scene_store(C->scene,0);
}

void add_exe_add_brick(t_brick *brick,t_brick *brick_target,void (* f)(t_action *))
{
	t_scene *sc = scene_get();
	scene_store( sc, 1);
	t_action *action = action_new("action");

	action->act = f;

	t_dict *dict = dict_make("args");
	action->args = dict;
	action->brick = brick;

	dict_symbol_add(action->args,"brick",dt_null,brick);
	dict_symbol_add(action->args,"target",dt_null,brick_target);

	exe_add_action(action);

	scene_store( sc, 0);
}

void add_exe_remove_brick(t_brick *brick)
{
	t_scene *sc = scene_get();
	scene_store( sc, 1);
	t_action *action = action_new("action");

	action->act = exe_remove_brick;

	t_dict *dict = dict_make("args");
	action->args = dict;
	action->brick = brick;

	dict_symbol_add(action->args,"brick",dt_null,brick);

	exe_add_action(action);

	scene_store( sc, 0);
}

// ADD BRICKS

void op_add_bricks( t_brick *brick, t_brick *brick_target, int offset)
{
	// slide
	op_slider_positive(brick);

	t_block *block=brick->block;
	t_plug *plug_intern=&brick->plug_intern;
	int tot_bricks=block->tot_bricks;
	int *slider = plug_intern->data;

	if(slider==0)
	{
	}

	// add brick
	else if( *slider > tot_bricks - offset) 
	{
		add_exe_add_brick( brick, brick_target, exe_add_brick);
	}
	// remove brick
	else if(*slider < tot_bricks - offset)
	{
		add_exe_remove_brick(brick);
	}
}

void *_op_brick_add( t_brick *brick)
{
	op_add_bricks( brick, NULL, brick->var.offset);
	return NULL;
}


// SELECTOR

void *op_selector(t_brick *brick)
{
	int *target = brick->plug_intern.data;

	if( brick->var.selector < brick->var.selector_length)
	{
		brick->var.selector++;
		(*target)++;
	}
	else
	{
		brick->var.selector=0;
		*target=0;
	}

	return NULL;
}

// SELECT

void *op_set_selected(t_brick *brick)
{
	t_context *C=ctx_get();
	t_plug *plug=&brick->plug_intern;

	if(plug->data)
	{
		t_id *id = (t_id *) plug->data;
		ctx_scene_set_selected( C, id->node);
	}

	return NULL;
}

// SWITCH

void *op_switch(t_brick *brick)
{
	int *target = brick->plug_intern.data;

	if(*target == 0)  *target=1; 
	else  *target=0; 

	return NULL;
}

// VOID

void *op_void_act(t_brick *brick)
{
	return NULL;
}
