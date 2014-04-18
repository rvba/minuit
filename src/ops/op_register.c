/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "context.h"
#include "scene.h"
#include "block.h"
#include "brick.h"
#include "app.h"
#include "op.h"
#include "dict.h"
#include "event.h"
#include "sketch.h"
#include "draw.h"
#include "clock.h"
#include "viewport.h"
#ifdef HAVE_OBJ
#include "obj.h"
#endif

// REGISTER GET

void *find_register(const char *target,const char *name)
{
	t_context *C=ctx_get();

	t_node *node_dict=scene_node_get(C->scene,"dict",target);

	if(node_dict)
	{
		t_dict *dict_register = ( t_dict *) node_dict->data;
		t_symbol *symbol=dict_pop(dict_register,name);
		if(symbol)
		{
			return symbol->data;
		}
		else
		{
			printf("[ERROR app_data_get] Can't get symbol %s\n",name);
		}
	}
	else
	{
		printf("[ERROR app_data_get] Can't get dict %s\n",target);
	}

	printf("[ERROR app_data_get] Unknown target %s\n",name);
	return NULL;
}

void op_add_register(t_context *C,t_dict *dict,const char *name,void *ptr)
{
	scene_store(C->scene,1);
	scene_add_data(C->scene,"app_data", dict->id.name, name, ptr);
	scene_store(C->scene,0);

	dict_symbol_add(dict,name,dt_pointer,ptr);
}

// REGISTER

void register_set(t_context *C)
{
	//store
	scene_store(C->scene,1);

	// func
	scene_add_data(C->scene,"app_data","func","op_slider",			(void *) op_slider);
	scene_add_data(C->scene,"app_data","func","op_slider_positive",		(void *) op_slider_positive);
	scene_add_data(C->scene,"app_data","func","op_slider_positive_non_zero",		(void *) op_slider_positive_non_zero);
	scene_add_data(C->scene,"app_data","func","op_slider_odd_positive_strict",		(void *) op_slider_odd_positive_strict);
	scene_add_data(C->scene,"app_data","func","op_cos",			(void *) op_cos);
	scene_add_data(C->scene,"app_data","func","op_sin",			(void *) op_sin);
	scene_add_data(C->scene,"app_data","func","op_add",			(void *) op_add);
	scene_add_data(C->scene,"app_data","func","op_mult",			(void *) op_mult);
	scene_add_data(C->scene,"app_data","func","op_equal",			(void *) op_equal);
	scene_add_data(C->scene,"app_data","func","op_superior",		(void *) op_superior);
	scene_add_data(C->scene,"app_data","func","op_inferior",		(void *) op_inferior);
	scene_add_data(C->scene,"app_data","func","op_clone",			(void *) op_clone);
	scene_add_data(C->scene,"app_data","func","op_plusplus",		(void *) op_plusplus);
	scene_add_data(C->scene,"app_data","func","op_switch",			(void *) op_switch);
	scene_add_data(C->scene,"app_data","func","op_pointer",			(void *) op_pointer);
	scene_add_data(C->scene,"app_data","func","op_camera_rotate_z",		(void *) op_camera_rotate_z);
	scene_add_data(C->scene,"app_data","func","op_camera_rotate_xy",	(void *) op_camera_rotate_xy);
	scene_add_data(C->scene,"app_data","func","op_void_exe",		(void *) op_void_exe);
	scene_add_data(C->scene,"app_data","func","op_void_act",		(void *) op_void_act);
	scene_add_data(C->scene,"app_data","func","op_set_selected",		(void *) op_set_selected);
	scene_add_data(C->scene,"app_data","func","op_rewind",			(void *) op_rewind);
	scene_add_data(C->scene,"app_data","func","op_pipe",			(void *) op_pipe);
	scene_add_data(C->scene,"app_data","func","op_not",			(void *) op_not);
	scene_add_data(C->scene,"app_data","func","op_set_vlst",		(void *) op_set_vlst);
	scene_add_data(C->scene,"app_data","func","op_rnd",			(void *) op_rnd);
	scene_add_data(C->scene,"app_data","func","op_neg",			(void *) op_neg);
	scene_add_data(C->scene,"app_data","func","op_is_last",			(void *) op_is_last);
	scene_add_data(C->scene,"app_data","func","op_vector",			(void *) op_vector);
	scene_add_data(C->scene,"app_data","func","op_bang",			(void *) op_bang);
	scene_add_data(C->scene,"app_data","func","op_do_quit",			(void *) op_do_quit);
	scene_add_data(C->scene,"app_data","func","op_operator",		(void *) op_operator);
	scene_add_data(C->scene,"app_data","func","op_float",			(void *) op_float);
	scene_add_data(C->scene,"app_data","func","op_int",			(void *) op_int);
	scene_add_data(C->scene,"app_data","func","op_const",			(void *) op_const);
	scene_add_data(C->scene,"app_data","func","op_and",			(void *) op_and);
	scene_add_data(C->scene,"app_data","func","op_mod",			(void *) op_mod);
	scene_add_data(C->scene,"app_data","func","op_stack",			(void *) op_stack);
	scene_add_data(C->scene,"app_data","func","op_if",			(void *) op_if);
	scene_add_data(C->scene,"app_data","func","op_geo_point",		(void *) op_geo_point);
	scene_add_data(C->scene,"app_data","func","op_geo_edge",		(void *) op_geo_edge);
	scene_add_data(C->scene,"app_data","func","op_geo_array",		(void *) op_geo_array);
	scene_add_data(C->scene,"app_data","func","op_geometry",		(void *) op_geometry);
	scene_add_data(C->scene,"app_data","func","op_geo",			(void *) op_geo);
	scene_add_data(C->scene,"app_data","func","_op_brick_add",		(void *) _op_brick_add);
	scene_add_data(C->scene,"app_data","func","op_viewport_set_dimensions",		(void *) op_viewport_set_dimensions);

//	scene_add_data(C->scene,"app_data","func","viewport_draw_scene",	(void *) viewport_draw_scene);


	// data
	scene_add_data(C->scene,"app_data","app","frame",			&C->app->frame);
	scene_add_data(C->scene,"app_data","app","timer",			&C->app->timer);
	scene_add_data(C->scene,"app_data","app","timer low",			&C->app->timer_add_low);
	scene_add_data(C->scene,"app_data","app","sec",				&C->app->clock->sec);
	scene_add_data(C->scene,"app_data","app","msec",			&C->app->clock->msec);
	scene_add_data(C->scene,"app_data","app","loaded_file",			&C->app->loaded_file);

	// struct

	scene_add_data(C->scene,"app_struct","block","menu_node",		menu_node);

	//store
	scene_store(C->scene,0);


	// FUNC

	t_dict *dict_func = dict_make( "func");

	dict_symbol_add(dict_func,"op_slider",dt_pointer, (void *) op_slider);
	dict_symbol_add(dict_func,"op_slider_positive",dt_pointer, (void *) op_slider_positive);
	dict_symbol_add(dict_func,"op_slider_positive_non_zero",dt_pointer, (void *) op_slider_positive_non_zero);
	dict_symbol_add(dict_func,"op_slider_positive_non_zero",dt_pointer, (void *) op_slider_positive_non_zero);
	dict_symbol_add(dict_func,"op_slider_odd_positive_strict",dt_pointer, (void *) op_slider_odd_positive_strict);
	dict_symbol_add(dict_func,"op_cos",dt_pointer, (void *) op_cos);
	dict_symbol_add(dict_func,"op_sin",dt_pointer, (void *) op_sin);
	dict_symbol_add(dict_func,"op_add",dt_pointer, (void *) op_add);
	dict_symbol_add(dict_func,"op_mult",dt_pointer, (void *) op_mult);
	dict_symbol_add(dict_func,"op_equal",dt_pointer,(void *)  op_equal);
	dict_symbol_add(dict_func,"op_superior",dt_pointer, (void *) op_superior);
	dict_symbol_add(dict_func,"op_inferior",dt_pointer, (void *) op_inferior);
	dict_symbol_add(dict_func,"op_clone",dt_pointer, (void *) op_clone);
	dict_symbol_add(dict_func,"op_plusplus",dt_pointer, (void *) op_plusplus);
	dict_symbol_add(dict_func,"op_switch",dt_pointer, (void *) op_switch);
	dict_symbol_add(dict_func,"op_pointer",dt_pointer, (void *) op_pointer);
	dict_symbol_add(dict_func,"op_camera_rotate_z",dt_pointer, (void *) op_camera_rotate_z);
	dict_symbol_add(dict_func,"op_camera_rotate_xy",dt_pointer,(void *) op_camera_rotate_xy);
	dict_symbol_add(dict_func,"op_void_exe",dt_pointer, (void *) op_void_exe);
	dict_symbol_add(dict_func,"op_void_act",dt_pointer, (void *) op_void_act);
	dict_symbol_add(dict_func,"op_set_selected",dt_pointer, (void *) op_set_selected);
	dict_symbol_add(dict_func,"op_rewind",dt_pointer, (void *) op_rewind);
	dict_symbol_add(dict_func,"op_pipe",dt_pointer, (void *) op_pipe);
	dict_symbol_add(dict_func,"op_not",dt_pointer, (void *) op_not);
	dict_symbol_add(dict_func,"op_set_vlst",dt_pointer, (void *) op_set_vlst);
	dict_symbol_add(dict_func,"op_rnd",dt_pointer, (void *) op_rnd);
	dict_symbol_add(dict_func,"op_neg",dt_pointer, (void *) op_neg);
	dict_symbol_add(dict_func,"op_is_last",dt_pointer, (void *) op_is_last);
	dict_symbol_add(dict_func,"op_vector",dt_pointer,(void *)  op_vector);
	dict_symbol_add(dict_func,"op_bang",dt_pointer, (void *) op_bang);
	dict_symbol_add(dict_func,"op_operator",dt_pointer, (void *) op_operator);
	dict_symbol_add(dict_func,"op_float",dt_pointer, (void *) op_float);
	dict_symbol_add(dict_func,"op_int",dt_pointer,(void *) op_int);
	dict_symbol_add(dict_func,"op_do_quit",dt_pointer, (void *) op_do_quit);
	dict_symbol_add(dict_func,"op_const",dt_pointer,(void *) op_const);
	dict_symbol_add(dict_func,"op_and",dt_pointer, (void *) op_and);
	dict_symbol_add(dict_func,"op_mod",dt_pointer, (void *) op_mod);
	dict_symbol_add(dict_func,"op_stack",dt_pointer, (void *) op_stack);
	dict_symbol_add(dict_func,"op_if",dt_pointer, (void *) op_if);
	dict_symbol_add(dict_func,"op_geo_point",dt_pointer, (void *) op_geo_point);
	dict_symbol_add(dict_func,"op_geo_edge",dt_pointer, (void *) op_geo_edge);
	dict_symbol_add(dict_func,"op_geo_array",dt_pointer, (void *) op_geo_array);
	dict_symbol_add(dict_func,"op_geometry",dt_pointer, (void *) op_geometry);
	dict_symbol_add(dict_func,"op_geo",dt_pointer, (void *) op_geo);
	dict_symbol_add(dict_func,"_op_brick_add",dt_pointer, (void *) _op_brick_add);
	dict_symbol_add(dict_func,"op_viewport_set_dimensions",dt_pointer, (void *) op_viewport_set_dimensions);
	//dict_symbol_add(dict_func,"viewport_draw_scene",dt_pointer, (void *) viewport_draw_scene);


	// APP

	t_dict *dict_app = dict_make( "app");

	dict_symbol_add(dict_app,"frame",dt_pointer,&C->app->frame);
	dict_symbol_add(dict_app,"timer",dt_pointer,&C->app->timer);
	dict_symbol_add(dict_app,"timer low",dt_pointer,&C->app->timer_add_low);
	dict_symbol_add(dict_app,"sec",dt_pointer,&C->app->clock->sec);
	dict_symbol_add(dict_app,"msec",dt_pointer,&C->app->clock->msec);
	dict_symbol_add(dict_app,"loaded_file",dt_pointer,&C->app->loaded_file);

	// MOUSE

	t_dict *dict_mouse = dict_make( "mouse");

	t_mouse *mouse=C->app->mouse;

	op_add_register(C,dict_mouse,"mouse_x",&mouse->x);
	op_add_register(C,dict_mouse,"mouse_y",&mouse->y);

	t_keyboard *keyboard=C->app->keyboard;

	op_add_register(C,dict_mouse,"keyboard",&keyboard->key_pressed);


}



