/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "obj.h"

// store menu

t_block *menu_node=NULL;

float global_pos=1;

void op_add_global(t_context *C,t_block *block)
{
	t_link *link=block->bricks->first;
	t_brick *brick=link->data;

	int brick_height=brick->geom.height;
	int tot_brick=block->tot_bricks;

	block->pos[1]=global_pos;

	global_pos+=(brick_height*tot_brick);

	t_lst *set_lst = get_target_list(C);

	list_add_global(set_lst,block);

	//list_add_global(C->scene->global,block);
}

// menu process

t_node *add_menu_process(void)
{
	t_context *C=ctx_get();
	t_node *menu_main=scene_node_get(C->scene,"block","menu_set");
	t_node *menu=block_make("menu_process","menu");
	t_block *block=menu->data;

	add_brick_switch(C,block,"debug process",&C->event->debug_process);
	add_brick_slider_float(C,block,"limit",&C->engine->global_limit);
	add_brick_slider_float(C,block,"freq",&C->engine->global_freq);
	add_brick_switch(C,block,"limit process",&C->engine->with_global_limit);

	add_brick_submenu(C,menu_main,menu,"process");

	return menu;
}

// menu debug

t_node *make_menu_debug(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_debug","menu");
	t_block *block=menu->data;

	add_brick_switch(C,block,"show terminal",&C->ui->show_term);
	add_brick_switch(C,block,"debug keyboard",&C->app->debug_keyboard);
	add_brick_switch(C,block,"debug select",&C->event->debug_select);
	add_brick_switch(C,block,"debug console",&C->event->debug_console);
	add_brick_switch(C,block,"debug terminal",&C->event->debug_terminal);
	add_brick_switch(C,block,"debug loop",&C->event->debug_loop);

	return menu;
}

// menu view

t_node *make_menu_view(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_view","menu");
	t_block *block=menu->data;

	add_brick_slider_int_custom(C,block,"win width",&C->app->window->width,op_window);
	add_brick_slider_int_custom(C,block,"win height",&C->app->window->height,op_window);
	add_brick_slider_int(C,block,"vp width",&C->app->window->viewport_width);

	return menu;
}

// menu ui

t_node *make_menu_ui(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_ui","menu");
	t_block *block=menu->data;

	add_brick_switch(C,block,"update links",&C->ui->update_links);
	add_brick_switch(C,block,"fixed menu",&C->ui->fixed_menu);
	add_brick_switch(C,block,"flow brick",&C->ui->flow_brick);
	add_brick_switch(C,block,"selection",&C->ui->object_selection);
	add_brick_switch(C,block,"bitmap font",&C->ui->use_bitmap_font);
	add_brick_switch(C,block,"mouse mode",&C->ui->mouse_mode);
	add_brick_switch(C,block,"show states",&C->ui->show_states);
	add_brick_switch(C,block,"show step",&C->ui->show_step);
	add_brick_switch(C,block,"show brick step",&C->ui->show_brick_step);
	add_brick_switch(C,block,"video build",&C->app->video_build);
	add_brick_slider_int(C,block,"video offset",&C->app->video_offset);
	add_brick_slider_int(C,block,"mouse size",&C->ui->mouse_size);
	add_brick_slider_int(C,block,"draw plug state",&C->ui->draw_plug_state);
	add_brick_switch(C,block,"use threading",&C->ui->use_threading);

	return menu;
}

// menu app

t_node *make_menu_app(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_app","menu");
	t_block *block=menu->data;

	add_brick_switch(C,block,"loop",&C->app->loop);
	add_brick_switch(C,block,"rec save",&C->event->rec_save);

	return menu;
}

// menu set

t_node *make_menu_set(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_set","menu");

	t_node *menu_view=make_menu_view();
	add_brick_submenu(C,menu,menu_view,"view");

	t_node *menu_debug=make_menu_debug();
	add_brick_submenu(C,menu,menu_debug,"debug");

	t_node *menu_ui=make_menu_ui();
	add_brick_submenu(C,menu,menu_ui,"ui");

	t_node *menu_app=make_menu_app();
	add_brick_submenu(C,menu,menu_app,"app");

	return menu;
}

// menu sketch 

t_node *make_menu_skt(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_sketch","menu");
	t_block *block=menu->data;

	add_brick_slider_int(C,block,"line width",&C->skt->line_width);
	add_brick_slider_float(C,block,"point size",&C->skt->point_size);
	add_brick_switch(C,block,"with scale",&C->event->sketch_with_scale);
	add_brick_slider_float(C,block,"intensity",&C->skt->intensity);

	return menu;
}

// menu draw

t_node *make_menu_draw(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_draw","menu");
	t_block *block=menu->data;

	add_brick_switch(C,block,"face",&C->event->with_face);
	add_brick_switch(C,block,"points",&C->event->with_point);
	add_brick_switch(C,block,"texture",&C->event->with_texture);
	add_brick_switch(C,block,"normal",&C->event->with_normal);
	add_brick_switch(C,block,"light",&C->event->with_light);
	add_brick_switch(C,block,"depth",&C->event->with_depth);
	add_brick_switch(C,block,"blend",&C->event->with_blend);
	add_brick_switch(C,block,"restrict",&C->draw->with_restrict_matrix);
	add_brick_switch(C,block,"direct",&C->draw->mode_direct);
	add_brick_switch(C,block,"draw lights",&C->draw->draw_lights);

	return menu;
}

// menu object

/*
t_node *make_menu_brick_object(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_brick_object","menu");
	t_block *block=menu->data;

	add_brick_trigger(C,block,"delete",op_delete);
	
	return menu;
}
*/

// menu add object


t_node *make_menu_node_add(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_node_add","menu");
	t_block *block=menu->data;

	add_brick_trigger(C,block,"default",op_add_default);
	add_brick_trigger(C,block,"cube",op_add_cube);
	add_brick_trigger(C,block,"uv cube",op_add_uv_cube);
	add_brick_trigger(C,block,"light",op_add_light);
	add_brick_trigger(C,block,"obj",op_obj_import);
	add_brick_trigger(C,block,"mn",op_add_mn);
	add_brick_trigger(C,block,"camera",op_add_camera);

	return menu;
}

// menu node

t_node *make_menu_object(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_brick_node","menu");
	t_block *block=menu->data;

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

	return menu;
}

t_node *make_menu_mesh(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_brick_mesh","menu");
	t_block *block=menu->data;

	add_brick_trigger(C,block,"color",op_brick_add);

	return menu;
}

// number 

t_node *make_menu_num(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_num","menu");
	t_block *block=menu->data;

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

	return menu;
}

// lst

t_node *make_menu_lst(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_lst","menu");
	t_block *block=menu->data;

	add_brick_trigger(C,block,"last?",op_brick_add);
	add_brick_trigger(C,block,"rewind",op_brick_add);

	return menu;
}

// logic

t_node *make_menu_logic(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_logic","menu");
	t_block *block=menu->data;

	add_brick_trigger(C,block,">",op_brick_add);
	add_brick_trigger(C,block,"<",op_brick_add);
	add_brick_trigger(C,block,"=",op_brick_add);
	add_brick_trigger(C,block,"not",op_brick_add);
	add_brick_trigger(C,block,"and",op_brick_add);

	return menu;
}

// maths

t_node *make_menu_maths(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_logic","menu");
	t_block *block=menu->data;

	add_brick_trigger(C,block,"cos",op_brick_add);
	add_brick_trigger(C,block,"sin",op_brick_add);
	add_brick_trigger(C,block,"x",op_brick_add);
	add_brick_trigger(C,block,"+",op_brick_add);
	add_brick_trigger(C,block,"++",op_brick_add);
	add_brick_trigger(C,block,"neg",op_brick_add);
	add_brick_trigger(C,block,"mod",op_brick_add);

	return menu;
}

// ops

t_node *make_menu_ops(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_logic","menu");
	t_block *block=menu->data;

	add_brick_trigger(C,block,"get",op_brick_add);
	add_brick_trigger(C,block,"clone",op_brick_add);
	add_brick_trigger(C,block,"pipe",op_brick_add);
	add_brick_trigger(C,block,"for",op_brick_add);
	add_brick_trigger(C,block,"vector",op_brick_add);
	add_brick_trigger(C,block,"bang",op_brick_add);
	add_brick_trigger(C,block,"stack",op_brick_add);
	add_brick_trigger(C,block,"quit",op_brick_add);

	return menu;
}

// operator

t_node *make_menu_operators(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_operators","menu");


	t_node *menu_lst=make_menu_lst();
	add_brick_submenu(C,menu,menu_lst,"lst");

	t_node *menu_logic=make_menu_logic();
	add_brick_submenu(C,menu,menu_logic,"logic");

	t_node *menu_maths=make_menu_maths();
	add_brick_submenu(C,menu,menu_maths,"maths");

	t_node *menu_ops=make_menu_ops();
	add_brick_submenu(C,menu,menu_ops,"ops");

	return menu;
}

// time

t_node *make_menu_time(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_time","menu");
	t_block *block=menu->data;

	add_brick_trigger(C,block,"sec",op_brick_add);
	add_brick_trigger(C,block,"msec",op_brick_add);
	add_brick_trigger(C,block,"frame",op_brick_add);
	add_brick_trigger(C,block,"timer",op_brick_add);
	add_brick_trigger(C,block,"timer low",op_brick_add);

	return menu;
}

// camera

t_node *make_menu_cam(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_cam","menu");
	t_block *block=menu->data;

	add_brick_trigger(C,block,"cam_pos_x",op_brick_add);
	add_brick_trigger(C,block,"cam_pos_y",op_brick_add);
	add_brick_trigger(C,block,"cam_pos_z",op_brick_add);
	add_brick_trigger(C,block,"cam_rot_xy",op_brick_add);
	add_brick_trigger(C,block,"cam_rot_z",op_brick_add);
	add_brick_trigger(C,block,"cam_eye_x",op_brick_add);
	add_brick_trigger(C,block,"cam_eye_y",op_brick_add);
	add_brick_trigger(C,block,"cam_eye_z",op_brick_add);

	return menu;
}

// mouse

t_node *make_submenu_mouse(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_mouse","menu");
	t_block *block=menu->data;

	add_brick_trigger(C,block,"mouse_x",op_brick_add);
	add_brick_trigger(C,block,"mouse_y",op_brick_add);
	add_brick_trigger(C,block,"keyboard",op_brick_add);

	return menu;
}

// ADD

t_node *make_menu_brick_add(void)
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_brick_add","menu");

	t_node *menu_num=make_menu_num();
	add_brick_submenu(C,menu,menu_num,"number");

	t_node *menu_operators=make_menu_operators();
	add_brick_submenu(C,menu,menu_operators,"operators");

	t_node *menu_time=make_menu_time();
	add_brick_submenu(C,menu,menu_time,"time");
	
	t_node *menu_cam=make_menu_cam();
	add_brick_submenu(C,menu,menu_cam,"camera");

	t_node *menu_mouse=make_submenu_mouse();
	add_brick_submenu(C,menu,menu_mouse,"mouse");

	t_node *menu_object=make_menu_object();
	add_brick_submenu_contextual(C,menu,menu_object,"object",nt_object);

	t_node *menu_mesh=make_menu_mesh();
	add_brick_submenu_contextual(C,menu,menu_mesh,"mesh",nt_mesh);

	return menu;
}

// MOUSE

void make_menu_mouse(void) 
{
	t_context *C=ctx_get();
	t_node *menu=block_make("menu_mouse","menu");

	t_node *add = make_menu_node_add();
	t_node *brick = make_menu_brick_add();
	t_node *set = make_menu_set();
	t_node *skt = make_menu_skt();
	t_node *draw = make_menu_draw();

	add_brick_submenu(C,menu,add,"add");
	add_brick_submenu(C,menu,brick,"brick");
	add_brick_submenu(C,menu,set,"set");
	add_brick_submenu(C,menu,skt,"sketch");
	add_brick_submenu(C,menu,draw,"draw");
}

// MAKE

void op_menu_init(t_context *C)
{
	make_menu_mouse();
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
	C->scene->store=1;
	scene_add_data(C->scene,"app_data",dict->name,name,ptr);
	C->scene->store=0;

	dict_symbol_add(dict,name,dt_pointer,ptr);
}

// REGISTER

void register_set(t_context *C)
{
	//store
	C->scene->store=1;

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
	scene_add_data(C->scene,"app_data","func","op_set_colors",		op_set_colors);
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
	C->scene->store=0;


	// FUNC

	t_node *node_dict_func=dict_add("func");
	t_dict *dict_func=node_dict_func->data;

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
	dict_symbol_add(dict_func,"op_set_colors",dt_pointer,op_set_colors);
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


	// APP

	t_node *node_dict_app=dict_add("app");
	t_dict *dict_app=node_dict_app->data;

	dict_symbol_add(dict_app,"frame",dt_pointer,&C->app->frame);
	dict_symbol_add(dict_app,"timer",dt_pointer,&C->app->timer);
	dict_symbol_add(dict_app,"timer low",dt_pointer,&C->app->timer_add_low);
	dict_symbol_add(dict_app,"sec",dt_pointer,&C->app->clock->sec);
	dict_symbol_add(dict_app,"msec",dt_pointer,&C->app->clock->msec);
	dict_symbol_add(dict_app,"loaded_file",dt_pointer,&C->app->loaded_file);

	// MOUSE

	t_node *node_mouse=dict_add("mouse");
	t_dict *dict_mouse=node_mouse->data;

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



