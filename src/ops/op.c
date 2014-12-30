/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "ctx.h"
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
#include "base.h"

int brick_check_viewport( t_brick *brick)
{
	t_context *C=ctx_get();
	if(C->scene->has_generic_viewport) return 1;
	else return 0;
}

// store menu

t_block *add_block_block( t_context *C, const char *name)
{
	t_node *menu = block_make( name, "block");
	t_block *block = ( t_block *) menu->data;
	return block;
}

t_block *add_menu_block( t_context *C, const char *name)
{
	t_node *menu = block_make( name, "menu");
	t_block *block = ( t_block *) menu->data;
	return block;
}

t_block *add_bar_block( t_context *C, const char *name)
{
	t_node *menu = block_make( name, "bar");
	t_block *block = ( t_block *) menu->data;
	return block;
}

// menu sketch 

t_block *make_menu_skt( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_sketch");

	add_brick_slider_int(C,block,"line width",&C->skt->line_width,NULL);
	add_brick_slider_float(C,block,"point size",&C->skt->point_size, NULL);
	add_brick_slider_float(C,block,"intensity",&C->skt->intensity, NULL);
	add_brick_slider_int(C,block,"point resolution",&C->skt->point_resolution,NULL);
	add_brick_switch(C,block,"point smooth",&C->skt->point_smooth, NULL);

	return block;
}

// menu draw

t_block *make_menu_draw( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_draw");

	add_brick_switch(C,block,"face",&C->event->with_face, NULL);
	add_brick_switch(C,block,"face outline",&C->event->with_face_outline, NULL);
	add_brick_switch(C,block,"points",&C->event->with_point, NULL);
	add_brick_switch(C,block,"edges",&C->event->with_edge, NULL);
	add_brick_switch(C,block,"edges color",&C->event->with_edge_color, NULL);
	add_brick_switch(C,block,"texture",&C->event->with_texture, NULL);
	add_brick_switch(C,block,"normal",&C->event->with_normal, NULL);
	add_brick_switch(C,block,"light",&C->event->with_light, NULL);
	add_brick_switch(C,block,"depth",&C->event->with_depth, NULL);
	add_brick_switch(C,block,"blend",&C->event->with_blend, NULL);
	add_brick_switch(C,block,"draw lights",&C->draw->draw_lights, NULL);

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
	add_brick_trigger(C,block,"viewport",op_add_screen);
	add_brick_trigger(C,block,"object",op_add_empty_object);
	add_brick_trigger(C,block,"mesh",op_add_empty_mesh);
	add_brick_trigger(C,block,"image",op_add_image);
	add_brick_trigger(C,block,"vlstf",op_add_empty_float_vlst);
	add_brick_trigger(C,block,"vlsti",op_add_empty_int_vlst);

	return block;
}

// menu add file

t_block *make_menu_add_file( t_context *C)
{
	t_block *block = add_menu_block( C, "menu_add_geometry");

	#ifdef HAVE_OBJ
	add_brick_trigger(C,block,"obj",op_obj_import);
	#endif
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
	t_block *menu = add_menu_block( C, "menu_add");

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

	add_brick_trigger(C,block,"case",op_brick_add);
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

	add_brick_trigger(C,block,"min",op_brick_add);
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

// BAR

void make_bar( t_context *C)
{
	char name[64];
	snprintf(name, 64, "%s    %s", "M I N U I T", APP_VERSION);
	t_node *node_block = block_make( "bar", "bar");
	t_block *block = ( t_block *) node_block->data;
	add_brick_bar( C, block, name, op_void_act);
}

void make_screen_selector( t_context *C)
{
	t_block *block = add_bar_block( C, "menu_screen");
	add_brick_slider_int(C,block,"", &C->event->color[0],NULL);
	add_brick_slider_int(C,block,"", &C->event->color[1],NULL);
	add_brick_slider_int(C,block,"", &C->event->color[2],NULL);
}

// MAKE

void op_menu_init(t_context *C)
{
	make_menu_mouse( C);
	make_bar( C);
	make_screen_selector( C);
}

t_block *add_menu( t_context *C, const char *name) 
{
	set_draw_plug=0;

	t_block *block = add_menu_block( C, name); 
	return block;

	set_draw_plug=1;
}

t_block *add_submenu( t_context *C, const char *menu_name, const char *submenu_name) 
{
	set_draw_plug=0;

	t_node *node = scene_node_get( C->scene, dt_block, menu_name);
	if( node)
	{
		char name[_NAME_];
		bzero( name, _NAME_);
		s_cp( name, "menu_", _NAME_);
		s_cat( name, submenu_name, _NAME_);
		t_block *menu = node->data;
		t_block *submenu = add_menu( C, name);
		add_brick_submenu( C, menu, submenu, submenu_name);
		return submenu;
	}
	else
	{
		printf("[WARNING] Add_submenu, can't find menu %s\n", menu_name);
		return NULL;
	}

	set_draw_plug=1;
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



