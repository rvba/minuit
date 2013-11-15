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
#include "op.h"
#include "action.h"
#include "ctx.h"
#include "app.h"
#include "event.h"
#include "viewport.h"
#include "camera.h"
#include "dict.h"
#include "data.h"
#include "binding.h"
#include "list.h"
#include "ui.h"
#include "block.h"
#include "brick.h"
#include "vector.h"
#include "clock.h"
#include "set.h"
#include "rhizome.h"
#include "geometry.h"

int brick_check_loop(t_brick *brick)
{
	t_context *C = ctx_get();

	int frame = C->app->frame;

	if(brick->state.frame_loop != frame)
	{
	//	brick->state.frame_loop = frame;
		return 1;
	}
	else
	{
		return 0;
	}
}

int brick_pre_check_loop(t_brick *brick)
{
	t_context *C = ctx_get();

	int frame = C->app->frame;

	if(brick->state.frame_loop != frame)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// SET UPDATED

void brick_set_updated(t_brick *brick)
{
	t_plug *plug = &brick->plug_intern;

	// Bindings
	if(plug->bindings)
	{
		t_link *link;
		for(link = plug->bindings->first; link; link = link->next)
		{
			t_binding *binding = link->data;
			binding_update(binding,plug->data);
		}
	}

	// Set Updated
	brick->plug_in.state.is_updated = 1;
	brick->plug_intern.state.is_updated = 1;
	brick->plug_out.state.is_updated = 1;

	// Set Setup
	if(brick->mode == bm_triggering)
	{
		t_block *block = brick->block;
		t_set *set = block->set;
		if(set) set_setup(set);
	}
}

// BRICK ADD

void *op_brick_add(t_brick *brick)
{
	t_context *C = ctx_get();
	t_node *node = NULL;
	t_block *this_block;
	t_brick *this_brick;

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
	else if(is(name,"last?")) 		node = add_switch_custom(C,"last?",NULL,op_is_last);
	else if(is(name,"for")) 		node = add_for(C);
	else if(is(name,"vector")) 		node = add_vector(C);
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

	// Switch Desk
	if(!C->ui->show_sets) show_sets(C);

	// Limit

	if(node)
	{
		this_block = node->data;
		if(this_block)
		{
			t_link *link;
			for(link = this_block->bricks->first; link; link = link->next)
			{
				this_brick = link->data;
				op_limit(this_brick);
			}

			// Frame Based
			if(is(name,"frame") || is(name,"timer") || is(name,"timer low"))
			{
				this_block->state.frame_based  =1;
			}
		}

	}
		
	return NULL;
}


// LIMIT

void *op_limit(t_brick *brick)
{
	if(brick->state.has_limit_low)
	{
		switch(brick->plug_intern.data_type)
		{
			case(dt_float):
				if(drf_float(brick->plug_intern.data) < brick->var.limit_float_low)
					set_float(brick->plug_intern.data,brick->var.limit_float_low);
				break;

			case(dt_int):
				if(drf_int(brick->plug_intern.data) < brick->var.limit_int_low)
					set_int(brick->plug_intern.data,brick->var.limit_int_low);
				break;

			case(dt_uint):
				if(drf_uint(brick->plug_intern.data) < brick->var.limit_int_low)
					set_uint(brick->plug_intern.data,brick->var.limit_int_low);
				break;

			default:
				printf("[op_limit] case not implemented : %s\n",data_name_get(brick->plug_intern.data_type));
				break;
		}
	}

	if(brick->state.has_limit_high)
	{
		switch(brick->plug_intern.data_type)
		{
			case(dt_float):
				if(drf_float(brick->plug_intern.data) > brick->var.limit_float_high)
					set_float(brick->plug_intern.data,brick->var.limit_float_high);
				break;

			case(dt_int):
				if(drf_int(brick->plug_intern.data) > brick->var.limit_int_high)
					set_int(brick->plug_intern.data,brick->var.limit_int_high);
				break;

			case(dt_uint):
				if(drf_uint(brick->plug_intern.data) > brick->var.limit_int_high)
					set_uint(brick->plug_intern.data,brick->var.limit_int_high);
				break;

			default:
				printf("[op_limit] case not implemented : %s\n",data_name_get(brick->plug_intern.data_type));
				break;
		}
	}

	return NULL;
}

// SLIDER

void *op_slider(t_brick *brick)
{
	t_context *C = ctx_get();

	float dx = C->app->mouse->dx;
	int delta_x = C->app->mouse->delta_x;

	t_plug *plug_intern= &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	// trigger
	//if(brick_check_loop(brick) && brick->mode==bm_triggering) // (see:plug_update)
	if(brick->mode==bm_triggering) // (see:plug_update)
	{
		// dragging
		if(brick->state.is_draging)
		{
			// release
			if(C->app->mouse->button_left==button_released)
			{
				brick->state.is_draging=0;
				brick_release(brick);
			}
			else
			{
				if(C->event->loop_step == 0)
				{
					if(brick->plug_intern.data_type==dt_int)
					{
						int *data=brick->plug_intern.data;
						if(delta_x>0) *data 	+=  dx;
						else *data 		-=  dx;
					}
					if(brick->plug_intern.data_type==dt_uint)
					{
						unsigned int *data=brick->plug_intern.data;
						if(delta_x>0) *data 	+=  dx;
						else *data 		-=  dx;
					}
					else if(brick->plug_intern.data_type==dt_float)
					{
						float inc=.1;
						float *data=brick->plug_intern.data;
						if(delta_x>0) *data 	+= inc * dx;
						else *data 		-= inc * dx;
					}
				}
			}
		}
		// simple clic
		else
		{
			// set dragging
			if(
				brick->state.use_dragging
				&&
				(C->app->mouse->button_left == button_pressed && C->app->mouse->is_moving)
				
				)
			{
					brick->state.is_draging=1;
			}
			else
			{
				// exec
				if(!brick->state.is_done)
				{
					brick->state.is_done=1;


					if(brick->plug_intern.data_type==dt_int)
					{
						int *data=brick->plug_intern.data;
						if(brick->state.is_left_pressed) 	*data -= 1; 
						else if(brick->state.is_right_pressed) 	*data += 1; 
					}
					else if(brick->plug_intern.data_type==dt_uint)
					{
						unsigned int *data=brick->plug_intern.data;
						if(brick->state.is_left_pressed) 	*data -= 1; 
						else if(brick->state.is_right_pressed) 	*data += 1; 
					}
					else if(brick->plug_intern.data_type==dt_float)
					{
						float inc=.1;
						float *data=brick->plug_intern.data;
						if(brick->state.is_left_pressed)  	*data -= inc; 
						else if(brick->state.is_right_pressed) 	*data += inc; 
					}
				}
				// release
				else
				{
					if(C->app->mouse->button_left==button_released)
					{
						brick->state.is_done=0;
						brick_release(brick); 
					}
				}
			}
		}

		// Limit

		op_limit(brick);

	}

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

	//t_context *C = ctx_get();
	//printf("%d slider positive %d\n",C->app->frame,drf_int(brick->plug_intern.data));

	// set positive
	int *val = brick->plug_intern.data;
	if(*val < 1) *val=1; 

	return NULL;
}

// SELECTOR

void *op_selector(t_brick *brick)
{
	t_context *C = ctx_get();
	int *target = brick->plug_intern.data;

	if(brick->state.is_mouse_over)
	{
		if(C->app->mouse->button_left==button_pressed)
		{
			// switch state when released
			if(brick->state.is_released)
			{
				brick->state.is_released=0;
				// switch value

				if(brick->var.selector<brick->var.selector_length)
				{
					brick->var.selector++;
					(*target)++;
				}
				else
				{
					brick->var.selector=0;
					*target=0;
				}
			}
		}
	}

	// release
	if(!brick->state.is_released && C->app->mouse->button_left==button_released)
	{
		brick->state.is_released=1;
		brick_release(brick);
	}

	return NULL;
}

// SWITCH

void *op_switch(t_brick *brick)
{
	t_context *C = ctx_get();

	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	if(brick->mode==bm_triggering) 
	{
		int *target = brick->plug_intern.data;

		if(brick->state.is_left_pressed || brick->state.is_right_pressed)
		{
			if(C->app->mouse->button_left==button_pressed)
			{
				// switch state when released
				if(brick->state.is_released)
				{
					brick->state.is_released=0;
					// switch value
					if(*target == 0)  *target=1; 
					else  *target=0; 
				}
			}
		}
	}

	// release
	if(!brick->state.is_released && C->app->mouse->button_left==button_released)
	{
		brick->state.is_released=1;
		brick_release(brick);
	}

	return NULL;
}

// >

void *op_superior(t_brick *brick)
{

	t_block *block=brick->block;

	t_brick *brick_val_1 = block_brick_get(block,"a");
	t_brick *brick_val_2 = block_brick_get(block,"b");
	t_brick *brick_result = brick;

	t_plug *plug_val_1 = &brick_val_1->plug_intern;
	t_plug *plug_val_2 = &brick_val_2->plug_intern;
	t_plug *plug_result = &brick_result->plug_intern;

	float *f1;
	float *f2;

	int *i1;
	int *i2;

	int *result = plug_result->data;

	t_data_type type_1 = plug_val_1->data_type;
	t_data_type type_2 = plug_val_2->data_type;

	t_data_type type_common  = dt_null; 

	if(type_1 == type_2) type_common = type_1;

	if(type_common != dt_null)
	{
		if(type_common == dt_float)
		{
			f1 = plug_val_1->data;
			f2 = plug_val_2->data;

			if(*f1 > *f2) *result = 1;
			else *result = 0;
		}
		else
		{
			i1 = plug_val_1->data;
			i2 = plug_val_2->data;

			if(*i1 > *i2) *result = 1;
			else *result = 0;
		}
	}
	else
	{
		*result = 0;
	}

	return NULL;
}

// <

void *op_inferior(t_brick *brick)
{

	t_block *block=brick->block;

	t_brick *brick_val_1 = block_brick_get(block,"a");
	t_brick *brick_val_2 = block_brick_get(block,"b");
	t_brick *brick_result = brick;

	t_plug *plug_val_1 = &brick_val_1->plug_intern;
	t_plug *plug_val_2 = &brick_val_2->plug_intern;
	t_plug *plug_result = &brick_result->plug_intern;

	float *f1;
	float *f2;

	int *i1;
	int *i2;

	int *result = plug_result->data;

	t_data_type type_1 = plug_val_1->data_type;
	t_data_type type_2 = plug_val_2->data_type;

	t_data_type type_common  = dt_null; 

	if(type_1 == type_2) type_common = type_1;

	if(type_common != dt_null)
	{
		if(type_common == dt_float)
		{
			f1 = plug_val_1->data;
			f2 = plug_val_2->data;

			if(*f1 < *f2) *result = 1;
			else *result = 0;
		}
		else
		{
			i1 = plug_val_1->data;
			i2 = plug_val_2->data;

			if(*i1 < *i2) *result = 1;
			else *result = 0;
		}
	}
	else
	{
		*result = 0;
	}

	return NULL;
}

// EQUAL

void *op_equal(t_brick *brick)
{

	t_block *block=brick->block;

	t_brick *brick_val_1 = block_brick_get(block,"a");
	t_brick *brick_val_2 = block_brick_get(block,"b");
	t_brick *brick_result = brick;

	t_plug *plug_val_1 = &brick_val_1->plug_intern;
	t_plug *plug_val_2 = &brick_val_2->plug_intern;
	t_plug *plug_result = &brick_result->plug_intern;

	float *f1;
	float *f2;

	int *i1;
	int *i2;

	int *result = plug_result->data;

	t_data_type type_1 = plug_val_1->data_type;
	t_data_type type_2 = plug_val_2->data_type;

	t_data_type type_common  = dt_null; 

	if(type_1 == type_2) type_common = type_1;

	if(type_common != dt_null)
	{
		if(type_common == dt_float)
		{
			f1 = plug_val_1->data;
			f2 = plug_val_2->data;

			if(*f1 == *f2) *result = 1;
			else *result = 0;
		}
		else
		{
			i1 = plug_val_1->data;
			i2 = plug_val_2->data;

			if(*i1 == *i2) *result = 1;
			else *result = 0;
		}
	}
	else
	{
		*result = 0;
	}

	return NULL;
}

// MODULO

void *op_mod(t_brick *brick)
{

	t_block *block=brick->block;

	t_brick *brick_val_1 = block_brick_get(block,"a");
	t_brick *brick_val_2 = block_brick_get(block,"b");
	t_brick *brick_result = brick;

	t_plug *plug_val_1 = &brick_val_1->plug_intern;
	t_plug *plug_val_2 = &brick_val_2->plug_intern;
	t_plug *plug_result = &brick_result->plug_intern;

	float *f1;
	float *f2;

	int *i1;
	int *i2;

	int *result = plug_result->data;

	t_data_type type_1 = plug_val_1->data_type;
	t_data_type type_2 = plug_val_2->data_type;

	t_data_type type_common  = dt_null; 

	if(type_1 == type_2) type_common = type_1;

	if(type_common != dt_null)
	{
		if(type_common == dt_float)
		{
			f1 = plug_val_1->data;
			f2 = plug_val_2->data;

			if(*f1 == *f2) *result = 1;
			else *result = 0;
		}
		else
		{
			i1 = plug_val_1->data;
			i2 = plug_val_2->data;

			if(*i2) *result = *i1 % *i2;
		}
	}
	else
	{
		*result = 0;
	}

	return NULL;
}

// ++

void *op_plusplus(t_brick *brick)
{
	op_slider(brick);

	t_plug *plug_intern = &brick->plug_intern;
	t_plug *plug_out=&brick->plug_out;

	int *intern = plug_intern->data;
	int *data;

	if(plug_out->state.is_connected && (*intern == 1))
	{
		t_plug *plug_dst = plug_get_dst(plug_intern);

		switch(plug_dst->data_type)
		{
			case(dt_int):
				data = plug_dst->data;
				(*data)++;

				break;
			default:
				break;
		}
	}

	return NULL;
}


void brick_rhizome_setup(t_brick *brick)
{
	t_block *block = brick->block;
	t_rhizome *rhizome = block->rhizome;
	if(block->rhizome)
	{
		rhizome_setup(rhizome);
	}
}

void exe_remove_brick(t_dict *args)
{
	t_context *C = ctx_get();

	// remove brick

	t_brick *_brick = dict_pop_data(args,"brick");
	t_block *block = _brick->block;

	int remove_connected = _brick->state.remove_connected;

	t_plug *plug_intern = &_brick->plug_intern;
	int *slider = plug_intern->data;

	t_lst *bricks=block->bricks;
	t_link *last=bricks->last;
	t_brick *brick=NULL;
	if(last) brick=last->data;

	if(brick)
	{
		// check if bricks are not connected
		if(brick_delete(brick,remove_connected))
		{
			// remove link from bricks
			lst_link_remove(bricks,last);
			brick_rhizome_setup(brick);
			scene_struct_delete(C->scene,last);
			block->tot_bricks--;
			block->state.update_geometry=1;
		}
		else
		{
			// re-increment slider
			(*slider)++;
		}
	}
}

void exe_add_brick_parent_child(t_dict *args)
{
	t_context *C = ctx_get();

	t_brick *brick = dict_pop_data(args,"brick");

	t_block *block=brick->block;
	t_plug *plug_intern=&brick->plug_intern;

	scene_store(C->scene,1);

	//add brick

	t_node *n=add_part_slider_float(C,block,".",NULL);
	t_brick *b=n->data;
	b->state.is_versatil=1;

	brick_rhizome_setup(b);

	t_plug *p_intern = &b->plug_intern;
	t_plug *p_in = &b->plug_in;
	
	plug_add_parent(p_intern,plug_intern);
	p_in->state.follow_in=0;

	scene_store(C->scene,0);

	block->state.update_geometry=1;
}

void exe_add_brick_child_parent(t_dict *args)
{
	t_context *C = ctx_get();

	t_brick *brick = dict_pop_data(args,"brick");
	t_plug *plug_brick = &brick->plug_intern;
	t_brick *brick_target = dict_pop_data(args,"target");

	t_block *block=brick->block;

	scene_store(C->scene,1);

	t_node *n=add_part_slider_float(C,block,".",NULL);
	t_brick *b=n->data;
	b->state.is_versatil=1;

	brick_rhizome_setup(b);

	t_plug *p_intern = &b->plug_intern;
	t_plug *p_in = &b->plug_in;
	
	t_plug *plug_target = &brick_target->plug_intern;
	plug_add_parent(plug_brick,p_intern);
	plug_add_parent(plug_target,plug_brick);
	p_in->state.follow_in=1;

	scene_store(C->scene,0);

	block->state.update_geometry=1;
}


void add_exe_add_brick(t_brick *brick,t_brick *brick_target,void (* f)(t_dict *))
{
	t_action *action = action_new("action");

	action->act = f;

	t_node *node_dict = dict_add("args");
	t_dict *dict = node_dict->data;
	action->args = dict;

	dict_symbol_add(action->args,"brick",dt_null,brick);
	dict_symbol_add(action->args,"target",dt_null,brick_target);

	exe_add_action(action);
}

void add_exe_remove_brick(t_brick *brick)
{
	t_action *action = action_new("action");

	action->act = exe_remove_brick;

	t_node *node_dict = dict_add("args");
	t_dict *dict = node_dict->data;
	action->args = dict;

	dict_symbol_add(action->args,"brick",dt_null,brick);

	exe_add_action(action);
}

// ADD BRICKS

void op_add_bricks(t_brick *brick,t_brick *brick_target,int offset,t_parent parent)
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
		if(parent == t_parent_child)
			add_exe_add_brick(brick,brick_target,exe_add_brick_parent_child);
		else
			add_exe_add_brick(brick,brick_target,exe_add_brick_child_parent);

	}
	// remove brick
	else if(*slider < tot_bricks - offset)
	{
		add_exe_remove_brick(brick);
	}
}


void *op_pipe(t_brick *brick)
{
	// add bricks
	op_add_bricks(brick,NULL,2,t_parent_child); //XXX 2

	t_block *block = brick->block;
	t_plug *plug_intern = &brick->plug_intern;
	int tot_bricks = block->tot_bricks;

	// flow
	plug_intern->cls->flow(plug_intern);

	t_brick *brick_state = block_brick_get(block,"state");
	t_plug *plug_state = &brick_state->plug_intern;
	int *state = plug_state->data;

	// update bricks
	if(tot_bricks>0)
	{
		t_link *l;
		t_brick *b;

		for(l = block->bricks->first;l;l = l->next)
		{
			b = l->data;
			if(!is(b->id.name,"clone"))
			{
				t_plug *plug_clone = &b->plug_intern;
				t_plug *plug_in_clone = &b->plug_in;
				t_plug *plug_out_clone = &b->plug_out;

				if(plug_in_clone->state.is_connected)
				{
					t_plug *plug_src = plug_get_src(plug_clone);

					// match current target type
					if(plug_clone->data_type != plug_src->data_type)
					{
						t_brick *brick_clone = plug_clone->brick;
						brick_type_change(brick_clone,plug_src);
					}
				}

				plug_out_clone->state.open_out = *state;

			}
		}
	}

	return NULL;
}


// CLONE

void *op_clone(t_brick *brick)
{
	t_block *block = brick->block;
	int tot_bricks = block->tot_bricks;

	// Get Brick
	t_brick *brick_brick = block_brick_get(block,"brick");
	t_plug *plug_brick = &brick_brick->plug_intern;
	t_plug *plug_out_brick = &brick_brick->plug_out;
	t_plug *plug_in_brick = &brick_brick->plug_in;

	// Add New Bricks
	if(brick_pre_check_loop(brick)) 
	{
		op_add_bricks(brick,brick_brick,2,t_parent_child);
	}

	// Update Clones
	if(tot_bricks>0)
	{
		t_link *l;
		t_brick *b;

		for(l = block->bricks->first;l;l = l->next)
		{
			b = l->data;
			if(!is(b->id.name,"brick") && !is(b->id.name,"clone"))
			{
				t_plug *plug_clone = &b->plug_intern;
				t_plug *plug_in_clone = &b->plug_in;

				// Change Type 
				if(plug_brick->data_type != plug_clone->data_type)
				{
					brick_type_change(b,plug_brick);
				}

				// Connect Clone
				plug_in_clone->src=plug_out_brick;

				if(plug_out_brick->dst || plug_in_brick->dst)
				plug_in_clone->state.is_connected = 1;
				else
				plug_in_clone->state.is_connected = 0;

				// Flow
				plug_clone->cls->flow(plug_clone);

				// Disconnect Clone
				plug_in_clone->src=NULL;
				plug_in_clone->state.is_connected = 0;
			}
		}
	}

	return NULL;
}


// SIN

void *op_sin(t_brick *brick)
{
	t_plug *plug_intern= &brick->plug_intern;
	float *data = brick->plug_intern.data;

	// flow
	plug_intern->cls->flow(plug_intern);

	// compute
	t_plug *plug_in=&brick->plug_in;
	if(plug_in->state.is_connected) *data=sin(*data);

	// release
	if(brick->mode==bm_triggering) brick_release(brick);

	return NULL;
}

// COS

void *op_cos(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;
	float *data = brick->plug_intern.data;

	// flow
	plug_intern->cls->flow(plug_intern);

	// compute
	t_plug *plug_in=&brick->plug_in;
	if(plug_in->state.is_connected) *data=cos(*data);

	// release
	if(brick->mode==bm_triggering) brick_release(brick);

	return NULL;
}

// WINDOW

void *op_window(t_brick *brick)
{
	op_slider(brick);

	t_app *app = app_get();
	app->window->viewport_width=app->window->width;
	app->window->viewport_height=app->window->height;
	glutReshapeWindow(app->window->width,app->window->height);
	return NULL;
}

// CAMERA ROTATE XY

void *op_camera_rotate_xy(t_brick *brick)
{
	t_context *C=ctx_get();

	op_slider(brick);

	float *data=brick->plug_intern.data;

	// Get default Viewport
	t_node *node_viewport = scene_node_get(C->scene,"viewport","viewport");
	t_viewport *viewport;
	t_camera *camera;

	if(node_viewport)
	{
		viewport = node_viewport->data;
		camera = viewport->camera;
		op_camera_rotate(C,camera,*data,0);	
	}

	return NULL;
}

// CAMERA ROTATE Z

void *op_camera_rotate_z(t_brick *brick)
{
	t_context *C=ctx_get();

	// Get default Viewport
	t_node *node_viewport = scene_node_get(C->scene,"viewport","viewport");
	t_viewport *viewport;
	t_camera *camera;

	if(node_viewport)
	{
		viewport = node_viewport->data;
		camera = viewport->camera;

		op_slider(brick);
		float *data=brick->plug_intern.data;
		op_camera_rotate(C,camera,0,*data);	
	}

	return NULL;
}

// DELETE

void *op_delete(t_brick *brick)
{
	t_context *C = ctx_get();
	if(C->scene->selected)
	{
		t_node *node=C->scene->selected;

		node->cls->del(node);

		ctx_ui_menu_hide(C);
		brick_release(brick);
		C->scene->selected=NULL;
	}

	return NULL;
}

// MENU

void *op_menu(t_brick *brick)
{
	brick_release(brick);

	return NULL;
}

// VOID

void *op_void(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;
	// flow
	plug_intern->cls->flow(plug_intern);

	brick_release(brick);

	return NULL;
}

// POINTER

void *op_pointer(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}

// MATHS

void op_maths_plug(t_operation operation,t_plug *dst,t_plug *src)
{
	t_context *C=ctx_get();

	t_data_type type_dst=dst->data_type;
	t_data_type type_src=src->data_type;

	int *dst_int;
	float *dst_float;
	t_vlst *dst_vlst;

	void *data_src=src->data;

	int count;
	int length; 
	int i,j;

	float *data_dst;
	float *data = NULL;
	t_data_type type=dt_null;

	t_vector *vector_src;
	t_vector *vector_dst;

	t_plug *plug_src;

	switch(type_dst)
	{
		case(dt_int):

			dst_int=dst->data;

			switch(type_src)
			{
				case(dt_int):

					switch(operation)
					{
						case (t_op_add): 	*dst_int += drf_int(data_src); break;
						case (t_op_mult): 	*dst_int *= drf_int(data_src); break;
					}

					break;

				case(dt_float):
					
					switch(operation)
					{
						case(t_op_add): 	*dst_int += (int)drf_float(data_src); break;
						case(t_op_mult): 	*dst_int *= (int)drf_float(data_src); break;
					}

					break;

				default:
					break;
			}

			break;

		case(dt_float):

			dst_float=dst->data;

			switch(type_src)
			{

				case(dt_int):

					switch(operation)
					{
						case(t_op_add):		*dst_float += (float)drf_int(data_src); break;
						case(t_op_mult):	*dst_float *= (float)drf_int(data_src); break;
					}

					break;

				case(dt_float):
				
					switch(operation)
					{
						case(t_op_add):		*dst_float += drf_float(data_src); break;
						case(t_op_mult):	*dst_float *= drf_float(data_src); break;
					}
					
					break;

				default:
					break;
			}

			break;

		case(dt_vlst):

			dst_vlst=dst->data;

			count=dst_vlst->count;
			length=dst_vlst->length; 

			data_dst=dst_vlst->data;

			plug_src=src->src;

			for(i=0;i<count;i++)
			{
				for(j=0;j<length;j++)
				{
					if(plug_src->state.is_connected)
					{

						C->event->update_connections=1;

						t_plug *plug_source=plug_get_src(src);
						t_brick *brick_source=plug_source->brick;

						brick_source->cls->trigger(brick_source);

						data=plug_source->data;
						type=plug_source->data_type;

						C->event->update_connections=0;
					}

					switch(type)
					{
						case(dt_int):

							switch(operation)
							{
								case(t_op_add):	 	*data_dst += (float)drf_int(data); break;
								case(t_op_mult):	*data_dst *= (float)drf_int(data); break;
							}

							break;

						case(dt_float):

							switch(operation)
							{
								case(t_op_add):	 	*data_dst += drf_float(data); break;
								case(t_op_mult):	*data_dst *= drf_float(data); break;
							}

							break;
							
						default:
							break;
					}

					data_dst++;
				}
			}


			break;

		case(dt_vector):

			switch(type_src)
			{
				case(dt_vector):

					switch(operation)
					{
						case(t_op_add): 

							vector_src = src->data;
							vector_dst = dst->data;

							vector_op_add(vector_dst,vector_src);

							break;
						default:
							break;
					}

					break;

				default:
					break;

			}

			break;


		default:printf("[?]\n");break;

	}
}

t_vlst *get_first_vlst(t_block *block)
{
	t_link *l;
	t_brick *b;
	t_plug *p;

	// offset first two bricks

	l=block->bricks->first;
	l=l->next;
	l=l->next;

	for(;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		switch(p->data_type)
		{
			case(dt_vlst):return p->data ;break;
			default:break;
		}
	}

	return NULL;
}

int get_first_vlst_pos(t_block *block)
{
	t_link *l;
	t_brick *b;
	t_plug *p;

	// offset first two bricks

	l=block->bricks->first;
	l=l->next;
	l=l->next;

	int pos=2;

	for(;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		switch(p->data_type)
		{
			case(dt_vlst):return pos;break;
			default:break;
		}
		pos++;
	}

	return pos;
}

// OP MATHS COMPUTE

void op_maths_exe(t_operation operation,t_brick *brick)
{
	t_link *l;
	t_brick *b;
	t_plug *p;
	t_block *block = brick->block;

	t_brick *brick_result=block_brick_get(block,"result");
	t_plug *plug_result=&brick_result->plug_intern;

	int pos=0;

	// Get Pos
	pos=get_first_vlst_pos(block);

	// Offset first two bricks operator, result
	l=block->bricks->first;
	l=l->next;
	l=l->next;

	// get type
	int this_pos=2;
	for(;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		// don't evaluate result plug
		if(this_pos != pos) 
			// compute
			op_maths_plug(operation,plug_result,p);
		this_pos++;
	}
}

// OP MATHS SET TYPE 

void *op_maths(t_operation operation,t_brick *brick)
{
	// get result type

	int is_int=0;
	int is_float=0;
	int is_vlst=0;
	int is_vector=0;

	t_link *l;
	t_brick *b;
	t_plug *p;

	t_block *block=brick->block;
	t_brick *brick_result=block_brick_get(block,"result");
	t_plug *plug_result=&brick_result->plug_intern;

	// add bricks
	op_add_bricks(brick,brick_result,2,t_child_parent);

	// offset first two bricks
	l=block->bricks->first;
	l=l->next;
	l=l->next;

	// get type
	for(;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		switch(p->data_type)
		{
			case(dt_int):is_int=1;break;
			case(dt_float):is_float=1;break;
			case(dt_vlst):is_vlst=1;break;
			case(dt_vector):is_vector=1;break;
			default:break;
		}
	}

	// Change Type && Init Result

	if(is_vlst)
	{
		brick_change_type_by_name(brick_result,dt_vlst);
		t_vlst *v=get_first_vlst(block);
		if(v) brick_result->plug_intern.data=v;
		else printf("[ERROR] Can't get vlst\n");
	}
	else if(is_float)
	{
		brick_change_type_by_name(brick_result,dt_float);
		float *r=plug_result->data;
		switch(operation)
		{
			case(t_op_add): *r=0; break;
			case(t_op_mult): *r=1; break;
		}
	}
	else if(is_int)
	{
		brick_change_type_by_name(brick_result,dt_int);
		int *r=plug_result->data;

		switch(operation)
		{
			case(t_op_add): *r=0; break;
			case(t_op_mult): *r=1; break;
		}
	}
	else if(is_vector)
	{
		brick_change_type_by_name(brick_result,dt_vector);

		t_plug *plug_vector = &brick_result->plug_intern;
		t_vector *vector = plug_vector->data;
		t_vlst *vlst = vector->vector;
		float *v = vlst->data;
		
		vset3f(v,0,0,0);
	}

	return NULL;
}

// :GEOMETRY


void *op_geo_array( t_brick *brick)
{
	t_plug *plug_self = &brick->plug_intern;
	t_geo_array *array = plug_self->data;
	t_block *block = brick->block;
	t_brick *brick_target = block_brick_get( block, "target");
	t_brick *brick_vector = block_brick_get( block, "vector");
	t_plug *plug_target = &brick_target->plug_intern;
	t_plug *plug_vector = &brick_vector->plug_intern;
	void *target = plug_target->data;
	t_vector *vector = plug_vector->data;
	t_data_type type = plug_target->cls->type;

	array->type_element = type;
	array->element = target;
	array->vector = vector;

	geo_array_build( array, type, vector, target); 

	return NULL;
}

void op_geo_exe(t_brick *brick)
{
	t_link *l;
	t_brick *b;
	t_plug *p;
	t_geo_point *point;
	t_geo_edge *edge;
	t_geo_array *array;

	t_block *block=brick->block;
	t_brick *brick_geometry = block_brick_get(block,"data");
	t_geo *geo = brick_geometry->plug_intern.data;

	t_lst *points = lst_new("lst");
	t_lst *edges = lst_new("lst");

	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		if(p->cls->type == dt_geo_point)
		{
			point = p->data;
			lst_add(points,point,"point");
		}
		else if(p->cls->type == dt_geo_edge)
		{
			edge = p->data;
			if(edge->a && edge->b)
			{
				lst_add(edges,edge,"edge");
			}
		}
		else if( p->cls->type == dt_geo_array)
		{
			array = p->data;

			if( array->elements->count > 0)
			{
				geo_array_get_points( array, points);
				geo_array_get_edges( array, edges);
			}
		}
	}

	// Reset

	int count_points = points->count;
	int count_edges = edges->count;

	if( count_points || count_edges)
	{
		geo_reset( geo);

		if(points->count > 0) geo_data_set(geo, dt_geo_point, points);
		if(edges->count > 0) geo_data_set(geo, dt_geo_edge, edges);
	}

	// Free
	lst_free(points);
	lst_free(edges);
}

void *op_geo_brick_add(t_brick *brick)
{
	t_block *block=brick->block;
	t_brick *brick_result=block_brick_get(block,"data");

	// add bricks
	op_add_bricks(brick,brick_result,4,t_child_parent);

	return NULL;
}

void *op_geometry(t_brick *brick)
{
	op_geo_brick_add(brick);
	op_geo_exe(brick);
	return NULL;
}

void *op_geo(t_brick *brick)
{
	return NULL;
}

void *op_geo_point(t_brick *brick)
{
	t_plug *plug = &brick->plug_intern;
	// flow
	plug->cls->flow(plug);

	t_geo_point *point = brick->plug_intern.data;
	t_block *block = brick->block;
	t_brick *brick_vector = block_brick_get(block,"vector");
	t_vector *vector = brick_vector->plug_intern.data;

	geo_point_vector_update(point,vector);

	return NULL;
}

void *op_geo_edge(t_brick *brick)
{
	t_geo_edge *edge = brick->plug_intern.data;
	t_block *block = brick->block;
	t_brick *brick_point_1 = block_brick_get_by_order(block,1);
	t_brick *brick_point_2 = block_brick_get_by_order(block,2);
	t_geo_point *point_1 = brick_point_1->plug_intern.data;
	t_geo_point *point_2 = brick_point_2->plug_intern.data;

	edge->a = point_1;
	edge->b = point_2;

	return NULL;
}

void *op_add(t_brick *brick)
{
	op_maths(t_op_add,brick);
	op_maths_exe(t_op_add,brick);
	return NULL;
}

void *op_mult(t_brick *brick)
{
	op_maths(t_op_mult,brick);
	op_maths_exe(t_op_mult,brick);
	return NULL;
}

void *op_and(t_brick *brick)
{
	t_link *l;
	t_brick *b;
	t_plug *p;

	t_block *block=brick->block;
	t_brick *brick_result=block_brick_get(block,"result");
	t_plug *plug_result=&brick_result->plug_intern;
	int *result = plug_result->data;

	// add bricks
	op_add_bricks(brick,brick_result,2,t_child_parent);

	int pos=0;

	pos=get_first_vlst_pos(block);

	// offset first two bricks operator, result
	l=block->bricks->first;
	l=l->next;
	l=l->next;

	int r=1;

	// get type
	int this_pos=2;
	for(;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		// don't evaluate result plug
		if(this_pos != pos)
		{
			int *_r = p->data;
			if(*_r == 0) r=0;

		}
		this_pos++;
	}

	*result = r;

	return NULL;

}

// SELECT

void *op_set_selected(t_brick *brick)
{
	t_context *C=ctx_get();
	t_plug *plug=&brick->plug_intern;

	// flow
	plug->cls->flow(plug);

	if(brick->mode==bm_triggering) 
	{
		if(plug->data)
		{
			t_id *id = (t_id *) plug->data;
			t_node *node  =scene_node_get_by_id_global(C->scene, id->id);
			
			ctx_scene_set_selected(C,node);
		}
	}

	return NULL;
}

// REWIND

void *op_rewind(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}

// NOT

void *op_not(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;
	t_plug *plug_in=&brick->plug_in;

	// flow
	plug_intern->cls->flow(plug_intern);

	int *data=plug_intern->data;

	switch(plug_intern->data_type)
	{
		case(dt_int):

			if(plug_in->state.is_connected)
			{
				if(*data == 0) *data = 1;
				else *data =0;
			}

			break;
		default:printf("op_not need an int\n");break;
	}

	return NULL;
}

// SET VLST

int _pressed=0;
int _released=0;

void *op_set_vlst(t_brick *brick)
{
	t_context *C=ctx_get();

	op_slider_positive_non_zero(brick);

	t_block *block=brick->block;

	t_brick *brick_vlst = NULL;

	if(is(block->id.name,"vertex"))
		brick_vlst=block_brick_get(block,"vertex");
	else if(is(block->id.name,"color"))
		brick_vlst=block_brick_get(block,"colors");
	else if(is(block->id.name,"faces"))
		brick_vlst=block_brick_get(block,"quads");
	else if(is(block->id.name,"vlst"))
		brick_vlst=block_brick_get(block,"vlst");
	else 
		printf("err!\n");

	if(brick_vlst)
	{

		t_vlst *vlst=brick_vlst->plug_intern.data;

		if(vlst)
		{

		if(C->app->mouse->button_left == button_pressed)
		{
			_pressed=1;
		}
		
		if(_pressed== 1 && C->app->mouse->button_left == button_released)
		{
			_pressed=0;
			_released=1;
		}
		
		if(C->app->mouse->button_left==button_released)
		{
			if(vlst->count != vlst->count_new) 
			{
				if(_released)
				{
					_released=0;
					vlst_update_data(vlst,NULL);
				}
			}
		}
		}
	}

	return NULL;
}

// RND

void *op_rnd(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;
	int *data=plug_intern->data;
	*data=rnd_range(0,100);

	return NULL;
}

// NEG

void *op_neg(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;
	t_plug *plug_in=&brick->plug_in;

	// flow
	plug_intern->cls->flow(plug_intern);

	// negate
	if(plug_in->state.is_connected)
		plug_data_negate(plug_intern);

	return NULL;
}

// IS_LAST

void *op_is_last(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;
	t_plug *plug_src=plug_get_src(plug_intern);

	int *data = plug_intern->data;

	if(plug_src)
	{
		if(plug_src->data_type==dt_lst)
		{
			t_lst *lst = plug_src->data;

			if(lst->current && lst->last)
			{
				if(lst->current == lst->last)
				{
					*data = 1;
				}
				else
				{
					*data = 0;
				}
			}
			else
			{
				*data = 0;
			}
		}
	}
	else
	{
		*data = 0;
	}

	return NULL;
}

// FOR

void *op_for(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}

// OPERATOR

void *op_operator(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}


// VECTOR

void *op_vector(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}

// BANG

void *op_bang(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	int *state = plug_intern->data;

	// [TRUE] : IN (true) + BANG (false) = BANG (true)
	// let true pass throw
	if(*state == 1 && !(plug_intern->state.bang))
	{
		plug_intern->state.bang = 1;
	}
	// [FALSE] IN (false) && BANG (true) = BANG (flase)
	else if(*state == 0 && plug_intern->state.bang)
	{
		plug_intern->state.bang=0;
	}
	// ELSE [FALSE]
	else
	{
		*state=0;
	}

	return NULL;
}

void *op_float(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	if(brick->mode == bm_triggering)	brick_release(brick);
	return NULL;
}


void *op_int(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	if(brick->mode == bm_triggering)	brick_release(brick);
	return NULL;
}

void *op_do_quit(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	int i = drf_int(plug_intern->data);

	if(i) op_quit(NULL);

	return NULL;
}

void *op_const(t_brick *brick)
{
	t_block *block = brick->block;

	t_brick *brick_i = block_brick_get(block,"i");

	t_plug *plug_const = &brick->plug_intern;
	t_plug *plug_i = &brick_i->plug_intern;

	int *_const = plug_const->data;
	int *_i = plug_i->data;

	// flow
	plug_const->cls->flow(plug_const);

	/*
	if(!plug_const->state.is_eval)
	{
		plug_const->state.is_eval = 1;

		*_const = *_i;
	}
	*/
	
	t_context *C = ctx_get();

	if(brick->state.frame_loop != C->app->frame)
	{
		brick->state.frame_loop = C->app->frame;
		*_const = *_i;
	}

	if(brick->mode == bm_triggering)
		brick_release(brick);

	return NULL;
}


void *op_stack(t_brick *brick)
{
	t_context *C = ctx_get();

	op_slider(brick);


	t_block *block = brick->block;

	t_brick *brick_i = block_brick_get(block,"i");
	t_brick *brick_counter = block_brick_get(block,"counter");
	t_brick *brick_limit = block_brick_get(block,"limit");

	int *i = brick_i->plug_intern.data;
	int *counter = brick_counter->plug_intern.data;
	int *limit = brick_limit->plug_intern.data;

	// reset by frame

	if(brick->state.frame_loop != C->app->frame)
	{
		brick->state.frame_loop = C->app->frame;
		*counter = 0;
		*i = 0;
	}
	else
	{
		(*counter)++;
	}

	// test

	if(*counter > *limit)
	{
		*counter = 0;
		(*i)++;
	}

	if(brick->mode == bm_triggering) brick_release(brick);

	return NULL;
}

void *op_if(t_brick *brick)
{
	t_block *block = brick->block;
	t_brick *brick_result = block_brick_get(block,"result");
	t_brick *brick_true = block_brick_get(block,"true");
	t_brick *brick_false = block_brick_get(block,"false");

	// Flow
	t_plug *plug = &brick->plug_intern;
	plug->cls->flow(plug);

	if(drf_int(brick->plug_intern.data) == 1)
	{
		brick_copy_data(brick_result,brick_true);
	}
	else
	{
		brick_copy_data(brick_result,brick_false);
	}

	if(brick->mode == bm_triggering) brick_release(brick);

	return NULL;
}

