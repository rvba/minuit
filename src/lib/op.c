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
#include "obj.h"
#include "viewport.h"
/*
#include "node.h"
#include "obj.h"
#include "process.h"
#include "engine.h"
#include "ui.h"
#include "list.h"
#include "set.h"
#include "memory.h"
#include "util.h"
*/

// store menu

t_block *menu_node=NULL;

t_block *add_menu_block( t_context *C, const char *name)
{
	t_node *menu = block_make( name, "menu");
	t_block *block = menu->data;
	block->state.is_moveable = 0;
	return block;
}

// menu sketch 

t_block *make_menu_skt( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_sketch");

	add_brick_slider_int(C,block,"line width",&C->skt->line_width);
	add_brick_slider_float(C,block,"point size",&C->skt->point_size);
	add_brick_slider_float(C,block,"intensity",&C->skt->intensity);
	add_brick_slider_int(C,block,"point resolution",&C->skt->point_resolution);
	add_brick_switch(C,block,"point smooth",&C->skt->point_smooth);

	return block;
}

// menu draw

t_block *make_menu_draw( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_draw");

	add_brick_switch(C,block,"face",&C->event->with_face);
	add_brick_switch(C,block,"face outline",&C->event->with_face_outline);
	add_brick_switch(C,block,"points",&C->event->with_point);
	add_brick_switch(C,block,"edges",&C->event->with_edge);
	add_brick_switch(C,block,"edges color",&C->event->with_edge_color);
	add_brick_switch(C,block,"texture",&C->event->with_texture);
	add_brick_switch(C,block,"normal",&C->event->with_normal);
	add_brick_switch(C,block,"light",&C->event->with_light);
	add_brick_switch(C,block,"depth",&C->event->with_depth);
	add_brick_switch(C,block,"blend",&C->event->with_blend);
	add_brick_switch(C,block,"draw lights",&C->draw->draw_lights);

	return block;
}

// menu add object

t_block *make_menu_add_object( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_add_object");

	add_brick_trigger(C,block,"default",op_add_default);
	add_brick_trigger(C,block,"cube",op_add_cube);
	add_brick_trigger(C,block,"light",op_add_light);
	add_brick_trigger(C,block,"camera",op_add_camera);
	add_brick_trigger(C,block,"viewport",op_add_viewport);
	add_brick_trigger(C,block,"object",op_add_empty_object);
	add_brick_trigger(C,block,"mesh",op_add_empty_mesh);
	add_brick_trigger(C,block,"vlstf",op_add_empty_float_vlst);
	add_brick_trigger(C,block,"vlsti",op_add_empty_int_vlst);

	return block;
}

// menu add file

t_block *make_menu_add_file( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_add_geometry");

	add_brick_trigger(C,block,"obj",op_obj_import);
	add_brick_trigger(C,block,"mn",op_add_mn);

	return block;
}

// menu add geometry

t_block *make_menu_add_geometry( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_add_geometry");

	add_brick_trigger(C,block,"geometry",op_add_empty_geometry);
	add_brick_trigger(C,block,"point",op_add_empty_geo_point);
	add_brick_trigger(C,block,"edge",op_add_empty_geo_edge);
	add_brick_trigger(C,block,"array",op_add_geo_array);

	return block;
}

// menu add 

t_block *make_menu_add( t_context *C)
{
	t_block *menu = add_menu_block( C, "menu_mouse");

	t_block *object = make_menu_add_object( C);
	t_block *file = make_menu_add_file( C);
	t_block *geometry = make_menu_add_geometry( C);

	add_brick_submenu( C, menu, object, "object");
	add_brick_submenu( C, menu, file, "file");
	add_brick_submenu( C, menu, geometry, "geometry");

	return menu;
}

// menu node

t_block *make_menu_object( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_object_brick");

	add_brick_trigger(C,block,"pos x",op_brick_add);
	add_brick_trigger(C,block,"pos y",op_brick_add);
	add_brick_trigger(C,block,"pos z",op_brick_add);

	add_brick_trigger(C,block,"rot x",op_brick_add);
	add_brick_trigger(C,block,"rot y",op_brick_add);
	add_brick_trigger(C,block,"rot z",op_brick_add);

	add_brick_trigger(C,block,"scl x",op_brick_add);
	add_brick_trigger(C,block,"scl y",op_brick_add);
	add_brick_trigger(C,block,"scl z",op_brick_add);

	add_brick_trigger(C,block,"red",op_brick_add);
	add_brick_trigger(C,block,"green",op_brick_add);
	add_brick_trigger(C,block,"blue",op_brick_add);
	add_brick_trigger(C,block,"alpha",op_brick_add);

	add_brick_trigger(C,block,"mesh",op_brick_add);

	add_brick_trigger(C,block,"color",op_brick_add);
	add_brick_trigger(C,block,"faces",op_brick_add);

	return block;
}

// menu mesh

t_block *make_menu_mesh( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_brick_mesh");

	add_brick_trigger(C,block,"vertex",op_brick_add);
	add_brick_trigger(C,block,"edges",op_brick_add);
	add_brick_trigger(C,block,"faces",op_brick_add);

	return block;
}

// number 

t_block *make_menu_scalar( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_scalar");

	add_brick_trigger(C,block,"int",op_brick_add);
	add_brick_trigger(C,block,"float",op_brick_add);
	add_brick_trigger(C,block,"100",op_brick_add);
	add_brick_trigger(C,block,"10",op_brick_add);
	add_brick_trigger(C,block,".001",op_brick_add);
	add_brick_trigger(C,block,".01",op_brick_add);
	add_brick_trigger(C,block,".1",op_brick_add);
	add_brick_trigger(C,block,"rnd",op_brick_add);
	add_brick_trigger(C,block,"switch",op_brick_add);
	add_brick_trigger(C,block,"const",op_brick_add);

	return block;
}


// logic

t_block *make_menu_logic( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_logic");

	add_brick_trigger(C,block,">",op_brick_add);
	add_brick_trigger(C,block,"<",op_brick_add);
	add_brick_trigger(C,block,"=",op_brick_add);
	add_brick_trigger(C,block,"not",op_brick_add);
	add_brick_trigger(C,block,"and",op_brick_add);
	add_brick_trigger(C,block,"if",op_brick_add);

	return block;
}

// maths

t_block *make_menu_maths( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_maths");

	add_brick_trigger(C,block,"cos",op_brick_add);
	add_brick_trigger(C,block,"sin",op_brick_add);
	add_brick_trigger(C,block,"x",op_brick_add);
	add_brick_trigger(C,block,"+",op_brick_add);
	add_brick_trigger(C,block,"++",op_brick_add);
	add_brick_trigger(C,block,"neg",op_brick_add);
	add_brick_trigger(C,block,"mod",op_brick_add);
	add_brick_trigger(C,block,"abs",op_brick_add);

	return block;
}

// operators

t_block *make_menu_ops( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_operator");

	add_brick_trigger(C,block,"get",op_brick_add);
	add_brick_trigger(C,block,"clone",op_brick_add);
	add_brick_trigger(C,block,"pipe",op_brick_add);
	add_brick_trigger(C,block,"for",op_brick_add);
	add_brick_trigger(C,block,"bang",op_brick_add);
	add_brick_trigger(C,block,"stack",op_brick_add);
	add_brick_trigger(C,block,"quit",op_brick_add);

	return block;
}

// vector

t_block *make_menu_vector( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_vector");

	add_brick_trigger(C,block,"vector 3d",op_brick_add);
	add_brick_trigger(C,block,"vector 2d",op_brick_add);

	return block;
}

// time

t_block *make_menu_time( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_time");

	add_brick_trigger(C,block,"sec",op_brick_add);
	add_brick_trigger(C,block,"msec",op_brick_add);
	add_brick_trigger(C,block,"frame",op_brick_add);

	return block;
}

// camera

t_block *make_menu_cam( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_cam");

	add_brick_trigger(C,block,"cam_pos_x",op_brick_add);
	add_brick_trigger(C,block,"cam_pos_y",op_brick_add);
	add_brick_trigger(C,block,"cam_pos_z",op_brick_add);
	add_brick_trigger(C,block,"cam_rot_xy",op_brick_add);
	add_brick_trigger(C,block,"cam_rot_z",op_brick_add);
	add_brick_trigger(C,block,"cam_eye_x",op_brick_add);
	add_brick_trigger(C,block,"cam_eye_y",op_brick_add);
	add_brick_trigger(C,block,"cam_eye_z",op_brick_add);

	return block;
}

// mouse

t_block *make_submenu_mouse( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_m");

	add_brick_trigger(C,block,"mouse_x",op_brick_add);
	add_brick_trigger(C,block,"mouse_y",op_brick_add);

	return block;
}

// ADD

t_block *make_menu_brick_add( t_context *C)
{
	t_block *menu = add_menu_block( C, "menu_brick_add");

	add_brick_submenu( C, menu, make_menu_scalar( C), "scalar");
	add_brick_submenu( C, menu, make_menu_vector( C), "vector");
	add_brick_submenu( C, menu, make_menu_logic( C), "logic");
	add_brick_submenu( C, menu, make_menu_maths( C), "maths");
	add_brick_submenu( C, menu, make_menu_ops( C), "operator");
	add_brick_submenu( C, menu, make_menu_time( C), "time");
	add_brick_submenu_poll( C, menu, make_menu_cam( C), "camera", brick_check_viewport);
	add_brick_submenu( C, menu, make_submenu_mouse( C), "mouse");
	add_brick_submenu_contextual( C, menu, make_menu_object( C), "object", dt_object);
	add_brick_submenu_contextual(C, menu, make_menu_mesh( C),"mesh", dt_mesh);

	return menu;
}

// MOUSE

void make_menu_mouse( t_context *C) 
{
	t_block *menu = add_menu_block( C, "menu_mouse"); 

	t_block *add = make_menu_add( C);
	t_block *brick = make_menu_brick_add( C);
	t_block *skt = make_menu_skt( C);
	t_block *draw = make_menu_draw( C);

	add_brick_submenu(C, menu, add, "add");
	add_brick_submenu(C, menu, brick, "brick");
	add_brick_submenu(C, menu, skt, "sketch");
	add_brick_submenu(C, menu, draw, "draw");
}

// MAKE

void op_menu_init(t_context *C)
{
	make_menu_mouse( C);
}

// REGISTER GET

void *find_register(const char *target,const char *name)
{
	t_context *C=ctx_get();

	t_node *node_dict=scene_node_get(C->scene,"dict",target);

	if(node_dict)
	{
		t_dict *dict_register=node_dict->data;
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
	scene_add_data(C->scene,"app_data","func","op_slider",			op_slider);
	scene_add_data(C->scene,"app_data","func","op_slider_positive",		op_slider_positive);
	scene_add_data(C->scene,"app_data","func","op_slider_positive_non_zero",		op_slider_positive_non_zero);
	scene_add_data(C->scene,"app_data","func","op_cos",			op_cos);
	scene_add_data(C->scene,"app_data","func","op_sin",			op_sin);
	scene_add_data(C->scene,"app_data","func","op_add",			op_add);
	scene_add_data(C->scene,"app_data","func","op_mult",			op_mult);
	scene_add_data(C->scene,"app_data","func","op_equal",			op_equal);
	scene_add_data(C->scene,"app_data","func","op_superior",		op_superior);
	scene_add_data(C->scene,"app_data","func","op_inferior",		op_inferior);
	scene_add_data(C->scene,"app_data","func","op_clone",			op_clone);
	scene_add_data(C->scene,"app_data","func","op_plusplus",		op_plusplus);
	scene_add_data(C->scene,"app_data","func","op_switch",			op_switch);
	scene_add_data(C->scene,"app_data","func","op_pointer",			op_pointer);
	scene_add_data(C->scene,"app_data","func","op_camera_rotate_z",		op_camera_rotate_z);
	scene_add_data(C->scene,"app_data","func","op_camera_rotate_xy",	op_camera_rotate_xy);
	scene_add_data(C->scene,"app_data","func","op_void",			op_void);
	scene_add_data(C->scene,"app_data","func","op_set_selected",		op_set_selected);
	scene_add_data(C->scene,"app_data","func","op_rewind",			op_rewind);
	scene_add_data(C->scene,"app_data","func","op_pipe",			op_pipe);
	scene_add_data(C->scene,"app_data","func","op_not",			op_not);
	scene_add_data(C->scene,"app_data","func","op_set_vlst",		op_set_vlst);
	scene_add_data(C->scene,"app_data","func","op_rnd",			op_rnd);
	scene_add_data(C->scene,"app_data","func","op_neg",			op_neg);
	scene_add_data(C->scene,"app_data","func","op_is_last",			op_is_last);
	scene_add_data(C->scene,"app_data","func","op_vector",			op_vector);
	scene_add_data(C->scene,"app_data","func","op_bang",			op_bang);
	scene_add_data(C->scene,"app_data","func","op_do_quit",			op_do_quit);
	scene_add_data(C->scene,"app_data","func","op_operator",		op_operator);
	scene_add_data(C->scene,"app_data","func","op_float",			op_float);
	scene_add_data(C->scene,"app_data","func","op_int",			op_int);
	scene_add_data(C->scene,"app_data","func","op_const",			op_const);
	scene_add_data(C->scene,"app_data","func","op_and",			op_and);
	scene_add_data(C->scene,"app_data","func","op_mod",			op_mod);
	scene_add_data(C->scene,"app_data","func","op_stack",			op_stack);
	scene_add_data(C->scene,"app_data","func","op_if",			op_if);
	scene_add_data(C->scene,"app_data","func","op_geo_point",		op_geo_point);
	scene_add_data(C->scene,"app_data","func","op_geo_edge",		op_geo_edge);
	scene_add_data(C->scene,"app_data","func","op_geo_array",		op_geo_array);
	scene_add_data(C->scene,"app_data","func","op_geometry",		op_geometry);
	scene_add_data(C->scene,"app_data","func","op_geo",			op_geo);

	scene_add_data(C->scene,"app_data","func","viewport_draw_scene",	viewport_draw_scene);


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

	dict_symbol_add(dict_func,"op_slider",dt_pointer,op_slider);
	dict_symbol_add(dict_func,"op_slider_positive",dt_pointer,op_slider_positive);
	dict_symbol_add(dict_func,"op_slider_positive_non_zero",dt_pointer,op_slider_positive_non_zero);
	dict_symbol_add(dict_func,"op_cos",dt_pointer,op_cos);
	dict_symbol_add(dict_func,"op_sin",dt_pointer,op_sin);
	dict_symbol_add(dict_func,"op_add",dt_pointer,op_add);
	dict_symbol_add(dict_func,"op_mult",dt_pointer,op_mult);
	dict_symbol_add(dict_func,"op_equal",dt_pointer,op_equal);
	dict_symbol_add(dict_func,"op_superior",dt_pointer,op_superior);
	dict_symbol_add(dict_func,"op_inferior",dt_pointer,op_inferior);
	dict_symbol_add(dict_func,"op_clone",dt_pointer,op_clone);
	dict_symbol_add(dict_func,"op_plusplus",dt_pointer,op_plusplus);
	dict_symbol_add(dict_func,"op_switch",dt_pointer,op_switch);
	dict_symbol_add(dict_func,"op_pointer",dt_pointer,op_pointer);
	dict_symbol_add(dict_func,"op_camera_rotate_z",dt_pointer,op_camera_rotate_z);
	dict_symbol_add(dict_func,"op_camera_rotate_xy",dt_pointer,op_camera_rotate_xy);
	dict_symbol_add(dict_func,"op_void",dt_pointer,op_void);
	dict_symbol_add(dict_func,"op_set_selected",dt_pointer,op_set_selected);
	dict_symbol_add(dict_func,"op_rewind",dt_pointer,op_rewind);
	dict_symbol_add(dict_func,"op_pipe",dt_pointer,op_pipe);
	dict_symbol_add(dict_func,"op_not",dt_pointer,op_not);
	dict_symbol_add(dict_func,"op_set_vlst",dt_pointer,op_set_vlst);
	dict_symbol_add(dict_func,"op_rnd",dt_pointer,op_rnd);
	dict_symbol_add(dict_func,"op_neg",dt_pointer,op_neg);
	dict_symbol_add(dict_func,"op_is_last",dt_pointer,op_is_last);
	dict_symbol_add(dict_func,"op_vector",dt_pointer,op_vector);
	dict_symbol_add(dict_func,"op_bang",dt_pointer,op_bang);
	dict_symbol_add(dict_func,"op_operator",dt_pointer,op_operator);
	dict_symbol_add(dict_func,"op_float",dt_pointer,op_float);
	dict_symbol_add(dict_func,"op_int",dt_pointer,op_int);
	dict_symbol_add(dict_func,"op_do_quit",dt_pointer,op_do_quit);
	dict_symbol_add(dict_func,"op_const",dt_pointer,op_const);
	dict_symbol_add(dict_func,"op_and",dt_pointer,op_and);
	dict_symbol_add(dict_func,"op_mod",dt_pointer,op_mod);
	dict_symbol_add(dict_func,"op_stack",dt_pointer,op_stack);
	dict_symbol_add(dict_func,"op_if",dt_pointer,op_if);
	dict_symbol_add(dict_func,"op_geo_point",dt_pointer,op_geo_point);
	dict_symbol_add(dict_func,"op_geo_edge",dt_pointer,op_geo_edge);
	dict_symbol_add(dict_func,"op_geo_array",dt_pointer,op_geo_array);
	dict_symbol_add(dict_func,"op_geometry",dt_pointer,op_geometry);
	dict_symbol_add(dict_func,"op_geo",dt_pointer,op_geo);

	dict_symbol_add(dict_func,"viewport_draw_scene",dt_pointer,viewport_draw_scene);


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

// INIT

void op_init(t_context *C)
{
	set_draw_plug=0;

	// create menus
	op_menu_init(C);

	// register
	register_set(C);

	set_draw_plug=1;
}



