/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include "op.h"

#include "ctx.h"
#include "app.h"
#include "node.h"
#include "scene.h"
#include "set.h"
#include "camera.h"
#include "object.h"
#include "light.h"
#include "data.h"
#include "list.h"
#include "vlst.h"
#include "material.h"
#include "viewport.h"
#include "mesh.h"
#include "block.h"
#include "brick.h"
#include "vector.h"
#include "ui.h"
#include "geometry.h"

int set_draw_plug=1;
int current_frame = 0;
int block_pos_offset = 0;
int opt_op_use_random = 0;

// GET LIST

t_set *get_current_set(t_context *C)
{
	t_lst *lst = scene_lst_get( C->scene, dt_set);
	t_set *set = NULL;
	if( lst)
	{
		if( lst->first)
		{
			t_node *node  = ( t_node *) lst->first->data;
			set = ( t_set *) node->data;
		}
	}
	else
	{
		op_new_set("set");
		t_lst *lst = scene_lst_get( C->scene, dt_set);

		t_node *node  = ( t_node *) lst->first->data;
		set = ( t_set *) node->data;
	}
	return set;
}

// GET FIRST BRICK
t_brick *get_block_first_brick(t_node *node)
{
	t_block *block = ( t_block *) node->data;
	t_lst *lst=block->bricks;
	t_brick *brick =  ( t_brick *) lst->first->data;

	return brick;
}

// PLUG DRAW
void set_plug_option(t_brick *brick)
{
	if(set_draw_plug) brick->brick_state.draw_plugs=1;
	else brick->brick_state.draw_plugs=0;
}

// BRICK DATA
void *add_brick_data(t_data_type type,void *data)
{
	if(data)
	{
		return data;
	}
	else
	{
		return data_add(type,data);
	}
}




/**		BLOCK		**/


// NEW BLOCK 

void add_block_offset(t_context *C, t_block *block)
{
	t_link *link=block->bricks->first;
	t_brick *brick =  ( t_brick *) link->data;

	int brick_height=brick->geom.height;
	int tot_brick=block->tot_bricks;

	block_pos_offset+=(brick_height*tot_brick);
}

float _rnd( void)
{
	return( .34 + ( rndf() * .8  * ( rnd_range(0,2) % 2)));
}

t_node *add_block(t_context *C,const char *name)
{
	// Get Set
	t_set *set = get_current_set(C);

	// New Block
	t_node *node_block=block_make(name,"block");
	t_block *block = ( t_block *) node_block->data;

	// Draw Outline
	block->block_state.draw_outline = 1;

	// Add to Set
	set_block_push(set,block);

	// Set Block Pos

	if(current_frame != C->app->frame)
	{
		current_frame = C->app->frame;
		block_pos_offset = 0;
	}

	float p[2];
	ctx_get_mouse_pos(C,p);

	if( opt_op_use_random) vset3f(block->pos,p[0] * _rnd() ,p[1] * _rnd() ,0);
	else vset3f(block->pos,p[0],p[1]+block_pos_offset,0);

	return node_block;
}


/**		BRICK		**/


// BRICK SLIDER INT

t_node *add_brick_slider_int( t_context *C, t_block *block, const char *name, void *data_target, void *(* f)( t_brick *brick))
{
	void *ptr=add_brick_data(dt_int,data_target);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_slider,dt_int,ptr);
	t_brick *brick =  ( t_brick *) node_brick->data;
	
	// ACTION
	if( f)
		brick->act = f;
	else
		brick->act = op_slider;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

t_node *add_brick_slider_int_even( t_context *C, t_block *block, const char *name, void *data_target)
{
	t_node *node = add_brick_slider_int( C, block, name, data_target,NULL);
	t_brick *brick = node->data;
	brick->act = op_slider_even;
	return node;
}

t_node *add_brick_slider_int_even_positive_strict( t_context *C, t_block *block, const char *name, void *data_target)
{
	t_node *node = add_brick_slider_int( C, block, name, data_target,NULL);
	t_brick *brick = node->data;
	brick->act = op_slider_even_positive_strict;
	return node;
}

t_node *add_brick_slider_add_bricks(t_context *C,t_block *block,const char *name,int offset,void *data_target)
{
	t_node *node = add_brick_slider_int( C, block, name, data_target,NULL);
	t_brick *brick = ( t_brick *) node->data;
	brick->act = _op_brick_add;
	brick->var.offset = offset;

	return node;
}

// BRICK SLIDER CHAR

t_node *add_brick_slider_char(t_context *C,t_block *block,const char *name,void *data_target)
{
	void *ptr=add_brick_data(dt_char,data_target);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_slider,dt_char,ptr);
	t_brick *brick = ( t_brick *) node_brick->data;
	
	// ACTION
	brick->act=op_slider;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// NEW BRICK SLIDER FLOAT

t_node *add_brick_slider_float( t_context *C, t_block *block, const char *name, void *data_target, void *(* f)( t_brick *brick))
{
	void *ptr=add_brick_data(dt_float,data_target);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_slider,dt_float,ptr);
	t_brick *brick = ( t_brick *) node_brick->data;
	
	// ACTION
	brick->act=op_slider;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK FLOAT

t_node *add_brick_float(t_context *C,t_block *block,const char *name,void *data_target)
{
	void *ptr=add_brick_data(dt_float,data_target);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_slider,dt_float,ptr);
	t_brick *brick = ( t_brick *) node_brick->data;
	
	// ACTION
	brick->exe=op_float;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK INT

t_node *add_brick_int(t_context *C,t_block *block,const char *name,void *data_target)
{
	void *ptr=add_brick_data(dt_int,data_target);

	// NEW BRICK
	t_node *node_brick=add_brick_slider_int(C,block,name,ptr,NULL);
	t_brick *brick = ( t_brick *) node_brick->data;
	
	// ACTION
	brick->exe=op_int;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK LABEL

t_node *add_brick_label(t_context *C,t_block *block,const char *name)
{
	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_trigger,dt_string,NULL);
	t_brick *brick = ( t_brick *) node_brick->data;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK VECTOR

t_node *add_brick_vector(t_context *C,t_block *block,const char *name)
{
	void *ptr=add_brick_data(dt_vector,NULL);
	t_vector *vector = (t_vector *) ptr;
	set_name(vector->id.name,name);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_trigger,dt_vector,ptr);
	t_brick *brick = ( t_brick *) node_brick->data;
	brick->exe=op_vector;

	brick->brick_state.always_trigger = 1;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK SWITCH

t_node *add_brick_switch(t_context *C,t_block *block,const char *name,void *data_target, void *(* f)(t_brick *brick))
{
	void *ptr=add_brick_data(dt_int,data_target);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_switch,dt_int,ptr);
	t_brick *brick = ( t_brick *) node_brick->data;

	// Dont Draw Value
	brick->brick_state.draw_value = 0;
	
	// ACTION
	if( f) brick->act = f;
	else brick->act=op_switch;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK MULTISWITCH

t_node *add_brick_multiswitch( t_context *C, t_block *block, const char *name, void *data_target, void *(* f)(t_brick *brick))
{
	t_node *node = add_brick_switch( C, block, name, data_target, f);
	t_brick *brick = node->data;

	// ACTION
	if( f) brick->act = f;
	else brick->act = op_multiswitch;

	return node;
}

// BRICK TRIGGER

t_node *add_brick_trigger(t_context *C,t_block *block,const char *name,void *f(t_brick *b))
{
	void *ptr=add_brick_data(dt_int,NULL);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_trigger,dt_int,ptr);
	t_brick *brick = ( t_brick *) node_brick->data;
	// Dont Draw Value
	brick->brick_state.draw_value = 0;

	// ACTION
	if(f)
	{
		brick->act = f;
	}

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK BAR

t_node *add_brick_bar( t_context *C, t_block *block, const char *name, void *f(t_brick *b))
{
	void *ptr=add_brick_data(dt_int,NULL);

	// NEW BRICK
	t_node *node_brick = brick_make( block, name, bt_bar, dt_int, ptr);
	t_brick *brick = ( t_brick *) node_brick->data;
	// Dont Draw Value
	brick->brick_state.draw_value = 0;

	// ACTION
	if(f)  brick->act = f; 

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK TRIGGER TYPE

t_node *add_brick_trigger_type(t_context *C,t_block *block,const char *name,void *f(t_brick *b),t_data_type type)
{
	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_trigger,type,NULL);
	t_brick *brick = ( t_brick *) node_brick->data;
	
	// ACTION
	if(f)
	{
		brick->act = f;
	}

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

//  BRICK OPERAND

t_node *add_brick_operand( t_context *C, t_block *block, const char *name)
{
	t_node *node_brick = add_brick_slider_float( C, block, name, NULL, NULL);
	t_brick *brick = ( t_brick *) node_brick->data;
	brick->brick_state.draw_plugs=1;
	brick->brick_state.draw_outline=0;
	brick->brick_state.draw_name=1;
	brick->brick_state.draw_value = 0;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

//  BRICK OPERATOR

t_node *add_brick_operator(t_context *C,t_block *block,const char *name)
{
	t_node *node_brick=add_brick_slider_int(C,block,name,NULL,NULL);
	t_brick *brick = ( t_brick *) node_brick->data;

	brick->brick_state.draw_name=1;
	brick->brick_state.draw_outline=0;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK SUBMENU

t_block *add_default_menu(t_context *C, const char *name)
{
	t_node *menu_root = scene_get_node_by_type_name( C->scene, dt_block, "menu_mouse");
	t_block *block_root = ( t_block *) menu_root->data;

	t_node *menu = block_make(name,"menu");
	t_block *block = ( t_block *) menu->data;

	add_brick_submenu(C, block_root, block, name);

	return block;
}

t_node *add_brick_submenu( t_context *C, t_block *menu, t_block *submenu, const char *name)
{
	t_node *node=brick_make( menu, name, bt_menu, dt_string, NULL);
	t_brick *brick = ( t_brick *) node->data;

	submenu->block_state.is_root=0;
	brick->menu = submenu;

	// PLUG
	set_plug_option(brick);

	return node;
}

// BRICK SUBMENU CONTEXTUAL

t_node *add_brick_submenu_contextual( t_context *C, t_block *menu, t_block *submenu, const char *name, t_data_type context)
{
	t_node *node = add_brick_submenu( C, menu, submenu, name);
	t_brick *brick = ( t_brick *) node->data;

	brick->brick_state.is_contextual=1;
	brick->context=context;

	// PLUG
	set_plug_option(brick);

	return node;
}

t_node *add_brick_submenu_poll( t_context *C, t_block *menu, t_block *submenu, const char *name, int (* f)(t_brick *brick))
{
	t_node *node = add_brick_submenu( C, menu, submenu, name);
	t_brick *brick = ( t_brick *) node->data;

	brick->brick_state.is_contextual=1;
	brick->brick_state.poll = 1;
	brick->poll = f;

	// PLUG
	set_plug_option(brick);

	return node;
}

// BRICK SELECTOR

t_node *add_brick_selector(t_context *C,t_block *block,const char *name,void *data_target,int length)
{
	t_node *node=brick_make(block,name,bt_selector,dt_selector,data_target);
	t_brick *brick = ( t_brick *) node->data;
	
	brick->act=op_selector;
	brick->var.selector_length=length;

	// PLUG
	set_plug_option(brick);
	
	return node;
}

// BRICK OBJECT

t_node *add_brick_object(t_context *C,t_block *block,const char *name,void *data_target, t_data_type type)
{
	t_node *node=brick_make(block,name,bt_trigger,type,data_target);
	t_brick *brick = ( t_brick *) node->data;
	
	brick->exe=op_void_exe;

	// PLUG
	set_plug_option(brick);
	
	return node;
}

// BRICK GEO POINT

t_node *add_brick_geo( t_context *C, t_block *block, const char *name, void *data_target, t_data_type type)
{
	t_node *node = brick_make( block, name, bt_trigger, type, data_target);
	t_brick *brick = ( t_brick *) node->data;

	if(type == dt_geo) brick->exe = op_geo;
	else if(type == dt_geo_point) brick->exe = op_geo_point;
	else if(type == dt_geo_edge) brick->exe = op_geo_edge;
	else if(type == dt_geo_array) brick->exe = op_geo_array;
	
	// PLUG
	set_plug_option(brick);

	brick->brick_state.always_trigger = 1;
	brick->plug_intern.state.store_data = 1;
	
	return node;
}

// BRICK MESH

t_node *_add_brick_mesh(t_context *C,t_block *block,const char *name,void *data_target, t_data_type type)
{
	t_node *node=brick_make(block,name,bt_trigger,type,data_target);
	t_brick *brick = ( t_brick *) node->data;
	
	brick->exe=op_void_exe;

	// PLUG
	set_plug_option(brick);
	
	return node;
}

// BRICK VLST

t_node *add_brick_vlst(t_context *C,t_block *block,const char *name,t_data_type type,void *pointer)
{
	t_node *node=brick_make(block,name,bt_trigger,dt_vlst,pointer);
	t_brick *brick = ( t_brick *) node->data;
	
	brick->exe=op_pointer;
	brick->brick_state.draw_value=1;
	brick->brick_state.always_trigger = 1;
	brick->brick_state.use_loops = 0;

	// PLUG
	set_plug_option(brick);
	
	return node;
}

// BRICK LST

t_node *add_brick_lst(t_context *C,t_block *block,const char *name,t_data_type type,void *pointer)
{
	t_node *node=brick_make(block,name,bt_trigger,dt_lst,pointer);
	t_brick *brick = ( t_brick *)  node->data;
	
	brick->exe=op_pointer;
	brick->brick_state.draw_value=1;

	// PLUG
	set_plug_option(brick);
	
	return node;
}

// BRICK POINTER

t_node *add_brick_pointer(t_context *C,t_block *block,const char *name,t_data_type type,void *pointer)
{
	t_node *node = NULL;
	if(type==dt_vertex) node=brick_make(block,name,bt_trigger,dt_vertex,pointer);
	else if(type==dt_face) node=brick_make(block,name,bt_trigger,dt_face,pointer);
	else if(type==dt_mesh) node=brick_make(block,name,bt_trigger,dt_mesh,pointer);
	else printf("[ERROR add_brick_pointer]\n");

	t_brick *brick = ( t_brick *) node->data;
	
	brick->exe=op_pointer;
	brick->brick_state.always_trigger = 1;

	// PLUG
	set_plug_option(brick);
	
	return node;
}

/**		PART		**/

// PART LABEL

t_node *add_part_label(t_context *C,t_block *block,const char *name)
{
	t_node *node=brick_make( block, name, bt_label, dt_string, NULL);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;
	return node;
}

t_node *add_part_label_custom(t_context *C,t_block *block,const char *name, void *(* f)(struct Brick *brick))
{
	t_node *node = add_part_label( C, block, name);
	t_brick *brick = ( t_brick *) node->data;
	// ACTION
	brick->act=f;
	return node;
}

// PART TRIGGER 

t_node *add_part_trigger(t_context *C,t_block *block,const char *name,void *f(t_brick *b))
{
	t_node *node=add_brick_trigger(C,block,name,f);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;
	return node;
}

t_node *add_part_vector(t_context *C,t_block *block,const char *name)
{
	t_node *node = add_brick_vector(C,block,name);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline = 0;
	return node;
}

// PART TRIGGER TYPE

t_node *add_part_trigger_type(t_context *C,t_block *block,const char *name,void *f(t_brick *b),t_data_type type)
{
	t_node *node=add_brick_trigger_type(C,block,name,f,type);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;
	return node;
}

// PART OBJECT

t_node *add_part_object(t_context *C,t_block *block,const char *name,void *data,t_data_type type)
{
	t_node *node=add_brick_object(C,block,name,data,type);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;
	return node;
}

// PART GEO POINT

t_node *add_part_geo(t_context *C, t_block *block, const char *name, void *data, t_data_type type)
{
	t_node *node = add_brick_geo( C, block, name, data, type);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline = 0;
	return node;
}

// PART OBJECT

t_node *add_part_mesh(t_context *C,t_block *block,const char *name,void *data,t_data_type type)
{
	t_node *node = _add_brick_mesh( C, block, name, data, type);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline = 0;
	return node;
}

// PART SLIDER FLOAT

t_node *add_part_slider_float(t_context *C,t_block *block,const char *name,void *data_target)
{
	t_node *node = add_brick_slider_float( C, block, name, data_target, NULL);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;
	return node;
}

t_node *add_part_float(t_context *C,t_block *block,const char *name,void *data_target)
{
	t_node *node=add_brick_float(C,block,name,data_target);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;
	return node;
}

// PART SLIDER INT

t_node *add_part_slider_int(t_context *C,t_block *block,const char *name,void *data_target)
{
	t_node *node=add_brick_slider_int(C,block,name,data_target,NULL);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;
	return node;
}

t_node *add_part_slider_add_bricks(t_context *C,t_block *block,const char *name, int offset, void *data_target)
{
	t_node *node = add_brick_slider_add_bricks(C,block,name, offset, data_target);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;
	return node;
}

// PART SLIDER ADD

t_node *add_part_slider_add( t_context *C, t_block *block, const char *name, void *data_target)
{
	t_node *node = add_brick_slider_int( C, block, name, data_target,NULL);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline = 0;
	brick->plug_out.state.flow_out = 0;
	brick->plug_out.state.open_out = 0;
	brick->plug_in.state.open_in = 0;
	brick->plug_in.state.flow_in = 0;
	return node;
}

t_node *add_part_int(t_context *C,t_block *block,const char *name,void *data_target)
{
	t_node *node=add_brick_int(C,block,name,data_target);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;
	return node;
}

// PART SLIDER INT POSITIVE

t_node *add_part_slider_int_positive(t_context *C,t_block *block,const char *name,void *data_target)
{
	t_node *node=add_brick_slider_int(C,block,name,data_target,op_slider_positive);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;
	return node;
}

// PART VLST

t_node *add_part_vlst(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr)
{
	t_node *node;

	node=add_brick_vlst(C,block,name,type,ptr);

	t_vlst *vlst = ( t_vlst *) ptr;

	scene_add_ref(C->scene,"struct_ref","vlst","count",&vlst->count_new,vlst);

	t_node *node_count=add_part_slider_int_positive(C,block,"count:",&vlst->count_new);
	t_brick *brick_count = ( t_brick *) node_count->data;

	brick_count->brick_state.use_loops = 0;
	brick_count->plug_intern.data_memory = NULL;
	brick_count->exe=op_set_vlst;
	brick_count->brick_state.singular=1;
	brick_count->brick_state.use_hold = 1;

	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;

	return node;
}

// PART LST

t_node *add_part_lst(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr)
{
	t_node *node=add_brick_lst(C,block,name,type,ptr);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline=0;

	return node;
}

// PART SELECTOR

t_node *add_part_selector(t_context *C, t_block *block, const char *name, t_node *node, t_data_type type)
{
	t_node *node_brick=add_brick_trigger_type(C,block,name,op_set_selected,type);
	t_brick *brick = ( t_brick *) node_brick->data;
	brick->brick_state.draw_outline=0;
	brick->brick_state.draw_value=1;
	brick->brick_state.always_trigger = 1;
	t_plug *plug=&brick->plug_intern;
	plug->data=node->data;
	plug->state.store_data = 1;

	return node_brick;
}

// PART POINTER

// mesh 
t_node *add_part_pointer(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr)
{
	t_node *node = add_brick_pointer(C,block,name,type,ptr);
	t_brick *brick = ( t_brick *) node->data;
	brick->brick_state.draw_outline = 0;
	if(type == dt_mesh)
	{
		brick->brick_state.draw_value = 1;
		brick->brick_state.draw_name = 1;
	}
	else
	{
		brick->brick_state.draw_value = 1;
		brick->brick_state.draw_name = 0;
	}

	return node;
}


//  TRIGGER

t_node *add_trigger(t_context *C,const char *name,void*(* f)(t_brick *brick))
{
	// BLOCK
	t_node *node_block = add_block(C,"trigger");
	t_block *block = ( t_block *) node_block->data;

	// LABEL
	add_brick_trigger(C,block,name,f);

	return node_block;
}

//  TRIGGER ALWAYS

t_node *add_trigger_always(t_context *C,const char *name,void*(* f)(t_brick *brick))
{
	// BLOCK
	t_node *node_block = add_block(C,"trigger");
	t_block *block = ( t_block *) node_block->data;

	// LABEL
	t_node *node_brick=add_brick_trigger(C,block,name,f);
	t_brick *brick = ( t_brick *) node_brick->data;

	brick->brick_state.always_trigger=1;

	return node_block;
}

//  LABEL

t_node *add_label(t_context *C,const char *name)
{
	// BLOCK
	t_node *node_block = add_block(C,"label");
	t_block *block = ( t_block *) node_block->data;

	// LABEL
	add_brick_label(C,block,name);

	return node_block;
}

//  SWITCH 

t_node *add_switch(t_context *C,const char *name,void *data, void *(* f)( t_brick *brick))
{
	// BLOCK
	t_node *node_block = add_block(C,"switch");
	t_block *block = ( t_block *) node_block->data;

	// BRICK SWICH
	add_brick_switch(C,block,name,data, f);

	return node_block;
}

// MULTIPLIER 

t_node *add_multiplier( t_context *C, const char *type)
{
	// BLOCK
	t_node *node_block = add_block(C, type);
	t_block *block = ( t_block *) node_block->data;

	t_node *node_brick = add_brick_slider_float( C, block, type, NULL, NULL);
	t_brick *brick = ( t_brick *) node_brick->data;

	if(iseq(type,"10"))  brick->var.increment=10; 
	else if(iseq(type,"100"))  brick->var.increment=100; 
	else if(iseq(type,".1"))  brick->var.increment=.1; 
	else if(iseq(type,".01"))  brick->var.increment=.01; 
	else if(iseq(type,".001"))  brick->var.increment=.001; 

	return node_block;
}

//  OPERATOR DOUBLE

t_node *add_operator_double(t_context *C,const char *type)
{
	// BLOCK
	t_node *node_block = add_block(C,type);
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline=1;

	// OPERANDS
	t_node *node_op_1 = add_brick_operand(C,block,"a");
	t_node *node_op_2 = add_brick_operand(C,block,"b"); 
	t_brick *brick_op_1 = ( t_brick *) node_op_1->data;
	t_brick *brick_op_2 = ( t_brick *) node_op_2->data;
	brick_op_1->brick_state.is_versatil=1;
	brick_op_2->brick_state.is_versatil=1;

	// RESULT
	t_node *brick_result=add_brick_operator(C,block,type);
	t_brick *result = ( t_brick *) brick_result->data;
	
	// ACTIONS
	if(iseq(type,">"))  			result->exe=op_superior;
	else if(iseq(type,"<"))  			result->exe=op_inferior;
	else if(iseq(type,"="))  			result->exe=op_equal;
	else if(iseq(type,"mod"))  		result->exe=op_mod;

	return node_block;
}

//  OPERATOR SINGLE

t_node *add_operator_single(t_context *C,const char *type,void *f(t_brick *brick))
{
	// BLOCK
	t_node *node_block=add_slider_float(C,type,NULL);
	t_brick *brick=get_block_first_brick(node_block);

	// ACTION
	brick->exe=f;

	return node_block;
}


//  SLIDER INT   

t_node *add_slider_int(t_context *C,const char *name,void *target_data)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block = ( t_block *) node_block->data;

	// NEW BRICK
	t_node *node_brick=add_part_slider_int(C,block,name,target_data);
	t_brick *brick = ( t_brick *) node_brick->data;

	// SET ACTION
	brick->act=op_slider;

	return node_block;
}


// SLIDER CHAR

t_node *add_slider_char(t_context *C,const char *name,void *target_data)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block = ( t_block *) node_block->data;

	// NEW BRICK
	t_node *node_brick=add_brick_slider_char(C,block,name,target_data);
	t_brick *brick = ( t_brick *) node_brick->data;

	// SET ACTION
	brick->exe=op_slider;

	return node_block;
}

// SLIDER INT CUSTOM

t_node *add_slider_int_custom(t_context *C,const char *name,void *target_data,void*(* f)(t_brick *b))
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block = ( t_block *) node_block->data;

	// NEW BRICK
	t_node *node_brick=add_brick_slider_int(C,block,name,target_data,NULL);
	t_brick *brick = ( t_brick *) node_brick->data;
 
	// SET ACTION
	brick->exe=f;
	brick->brick_state.is_versatil=1;

	return node_block;
}

//  SLIDER FLOAT   

t_node *add_slider_float(t_context *C,const char *name,void *target_data)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline = 1;

	// NEW BRICK
	t_node *node_brick=add_part_slider_float(C,block,name,target_data);
	t_brick *brick = ( t_brick *) node_brick->data;

	// SET ACTION
	brick->act=op_slider;

	return node_block;
}

//  SLIDER INT POSITIVE  

t_node *add_slider_int_positive(t_context *C,const char *name,void *target_data)
{
	// BLOCK
	t_node *node_block=add_slider_int(C,name,target_data);
	t_brick *brick=get_block_first_brick(node_block);

	// SET ACTION
	brick->act=op_slider_positive;

	return node_block;
}

//  SLIDER FLOAT CUSTOM

t_node *add_slider_float_custom(t_context *C,const char *name,void *f(t_brick *brick))
{
	// NEW SLIDER
	t_node *node_block=add_slider_float(C,name,NULL);
	t_brick *brick=get_block_first_brick(node_block);

	// SET ACTION
	brick->exe = f;

	return node_block;
}

//  SLIDER FLOAT SPECIAL

t_node *add_slider_float_special(t_context *C,const char *name,void *f(t_brick *brick))
{
	// NEW SLIDER
	t_node *node_block=add_slider_float(C,name,NULL);
	t_brick *brick=get_block_first_brick(node_block);

	// SET ACTION
	brick->act=f;

	return node_block;
}

// SLIDER INT SPECIAL

t_node *add_slider_int_special(t_context *C,const char *name,void *f(t_brick *brick))
{
	// NEW SLIDER
	t_node *node_block=add_slider_int(C,name,NULL);
	t_brick *brick=get_block_first_brick(node_block);

	// SET ACTION
	brick->act=f;

	return node_block;
}

// REF

void add_slider_ref(t_context *C,t_object *object,const char *name)
{
	t_mesh *mesh=NULL;
	t_material *mat=NULL;
	
	if(object->mesh) mesh=object->mesh;
	if(mesh)
	{
		 if(mesh->material) mat=mesh->material;
	}

	if(iseq(name,"pos x")) 		scene_add_ref(C->scene,"struct_ref","object","loc_x",&object->loc[0],object);
	else if(iseq(name,"pos y"))	scene_add_ref(C->scene,"struct_ref","object","loc_y",&object->loc[1],object);
	else if(iseq(name,"pos z"))	scene_add_ref(C->scene,"struct_ref","object","loc_z",&object->loc[2],object);
	else if(iseq(name,"rot x"))	scene_add_ref(C->scene,"struct_ref","object","rot_x",&object->rot[0],object);
	else if(iseq(name,"rot y"))	scene_add_ref(C->scene,"struct_ref","object","rot_y",&object->rot[1],object);
	else if(iseq(name,"rot z"))	scene_add_ref(C->scene,"struct_ref","object","rot_z",&object->rot[2],object);
	else if(iseq(name,"scl x"))	scene_add_ref(C->scene,"struct_ref","object","scl_x",&object->size[0],object);
	else if(iseq(name,"scl y"))	scene_add_ref(C->scene,"struct_ref","object","scl_y",&object->size[1],object);
	else if(iseq(name,"scl z"))	scene_add_ref(C->scene,"struct_ref","object","scl_z",&object->size[2],object);
	else if(iseq(name,"red") && mat)	scene_add_ref(C->scene,"struct_ref","material","red",&mat->color[0],mat);
	else if(iseq(name,"green") && mat)	scene_add_ref(C->scene,"struct_ref","material","green",&mat->color[1],mat);
	else if(iseq(name,"blue") && mat)		scene_add_ref(C->scene,"struct_ref","material","blue",&mat->color[2],mat);
	else if(iseq(name,"alpha") && mat)	scene_add_ref(C->scene,"struct_ref","material","alpha",&mat->color[3],mat);
	else if(iseq(name,"quad face") && mesh)	scene_add_ref(C->scene,"struct_ref","mesh","quad_face",&mesh->quad_face,mesh);
	else if(iseq(name,"mesh") && mesh)	scene_add_ref(C->scene,"struct_ref","mesh","mesh",mesh,mesh);

}

// SLIDER TARGET

void add_slider_target(t_context *C,t_object *object,const char *name)
{
	t_mesh *mesh=NULL;
	t_material *material=NULL;
	t_block *block=object->ref;
	
	if(object->mesh) mesh=object->mesh;
	if(mesh)
	{
		 if(mesh->material) material=mesh->material;
	}

	if(iseq(name,"pos x")) 		add_part_slider_float(C,block,"pos x",&object->loc[0]);
	else if(iseq(name,"pos y")) 	add_part_slider_float(C,block,"pos y",&object->loc[1]);
	else if(iseq(name,"pos z")) 	add_part_slider_float(C,block,"pos z",&object->loc[2]);
	else if(iseq(name,"rot x")) 	add_part_slider_float(C,block,"rot x",&object->rot[0]);
	else if(iseq(name,"rot y")) 	add_part_slider_float(C,block,"rot y",&object->rot[1]);
	else if(iseq(name,"rot z")) 	add_part_slider_float(C,block,"rot z",&object->rot[2]);
	else if(iseq(name,"scl x")) 	add_part_slider_float(C,block,"scl x",&object->size[0]);
	else if(iseq(name,"scl y")) 	add_part_slider_float(C,block,"scl y",&object->size[1]);
	else if(iseq(name,"scl z")) 	add_part_slider_float(C,block,"scl z",&object->size[2]);
	else if(iseq(name,"red") && material) 	add_part_slider_float(C,block,"red",&material->color[0]);
	else if(iseq(name,"green") && material) 	add_part_slider_float(C,block,"green",&material->color[1]);
	else if(iseq(name,"blue") && material) 	add_part_slider_float(C,block,"blue",&material->color[2]);
	else if(iseq(name,"alpha") && material) 	add_part_slider_float(C,block,"alpha",&material->color[3]);
	else if(iseq(name,"color") && mesh) 	mesh_add_default_color(mesh); 
	else if(iseq(name,"faces") && mesh) 	mesh_add_brick_faces(mesh); 
	else if(iseq(name,"mesh")) 		add_part_object(C,block,"mesh",object,dt_object); 

	block->block_state.update_geometry = 1;

}

// SLIDER OBJECT

t_node *add_slider_object(t_context *C,const char *name)
{
	t_node *selected=C->scene->selected;

	if(selected)
	{
		t_object *object = ( t_object *) selected->data; 

		add_slider_ref(C,object,name);
		add_slider_target(C,object,name);
	}

	return NULL;
}

// MESH

void add_target_mesh( t_context *C, t_mesh *mesh, const char *name)
{
	t_node *selected=C->scene->selected;

	if(selected)
	{
		t_mesh *mesh =  ( t_mesh *) selected->data;
		t_block *block = mesh->ref;
		if( iseq( name, "vertex")) 	add_part_mesh( C, block, "vertex", mesh, dt_mesh);
		else if( iseq( name, "edges")) 	add_part_mesh( C, block, "edges", mesh, dt_mesh);
		block->block_state.update_geometry = 1;
	}
}

void add_mesh_ref( t_context *C, t_mesh *mesh, const char *name)
{
}

t_node *add_brick_mesh( t_context *C, const char *name)
{
	t_node *selected = C->scene->selected;

	if( selected)
	{
		t_mesh *mesh = ( t_mesh *) selected->data; 

		add_mesh_ref( C, mesh, name);
		add_target_mesh( C, mesh, name);
	}

	return NULL;
}

// SLIDER CAMERA

t_node *add_slider_camera(t_context *C,const char *name)
{
	t_camera *camera = op_camera_get_current( C);
	t_node *node = NULL;

	if(camera)
	{
		if(iseq(name,"pos x"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","pos_x",&camera->pos[0],camera);
			node = add_slider_float(C,"cam pos x",&camera->pos[0]);
		}
		else if(iseq(name,"pos y"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","pos_y",&camera->pos[1],camera);
			node = add_slider_float(C,"cam pos y",&camera->pos[1]);
		}
		else if(iseq(name,"pos z"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","pos_z",&camera->pos[2],camera);
			node = add_slider_float(C,"cam pos z",&camera->pos[2]);
		}
		else if(iseq(name,"eye x"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","eye_x",&camera->eye[1],camera);
			node = add_slider_float(C,"cam eye x",&camera->eye[1]);
		}
		else if(iseq(name,"eye y"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","eye_y",&camera->eye[2],camera);
			node = add_slider_float(C,"cam eye y",&camera->eye[2]);
		}
		else if(iseq(name,"eye z"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","eye_z",&camera->eye[3],camera);
			node = add_slider_float(C,"cam eye z",&camera->eye[3]);
		}
	}

	return node;
}

// CLONE

t_node *add_clone(t_context *C)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"clone");
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline=1;

	// Add Brick
	t_node *node_brick = add_part_slider_float(C,block,"brick",NULL);
	t_brick *brick_brick = ( t_brick *) node_brick->data;
	brick_brick->brick_state.is_versatil = 1;
	t_plug *plug_brick = &brick_brick->plug_intern;
	plug_brick->state.swap_flow = 1;

	// Add Clone
	//t_node *node_brick_clone=add_part_slider_add( C, block, "clone", NULL);
	t_node *node_brick_clone = add_part_slider_add_bricks( C, block, "clone", 2, NULL);
	t_brick *brick_clone = ( t_brick *) node_brick_clone->data;

	brick_clone->exe=op_clone;

	return node_block;
}

t_node *add_case(t_context *C, const char *name)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"clone");
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline=1;

	// Add Clone
	t_node *node_brick_clone = add_part_slider_add_bricks( C, block, "clone", 3, NULL);
	t_brick *brick_clone = ( t_brick *) node_brick_clone->data;
	brick_clone->exe=op_case;

	// Add Result
	add_part_slider_float(C,block,"result",NULL);

	// Add Case
	add_part_slider_int(C,block,"case",NULL);

	return node_block;
}

// PIPE

t_node *add_pipe(t_context *C)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"pipe");
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline=1;

	// INT
	add_part_slider_int(C,block,"state",NULL);

	// CLONE
	//t_node *node_brick_clone=add_part_slider_add( C, block, "pipe", NULL);
	t_node *node_brick_clone = add_part_slider_add_bricks( C, block, "pipe", 2, NULL);
	t_brick *brick_clone = ( t_brick *) node_brick_clone->data;

	// re-init to float (old int data used by plug_out)
	plug_init(&brick_clone->plug_in,dt_float,brick_clone,NULL,0);

	brick_clone->exe=op_pipe;

	return node_block;
}

// MATHS

t_node *add_maths(t_context *C,const char *name)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline = 1;

	// Brick Result
	add_part_slider_int(C,block,"result",NULL);
	//add_part_slider_add_bricks(C,block,"result", 2,NULL);

	// CLONE
	//t_node *node_brick_clone = add_part_slider_add( C, block, name, NULL);
	t_node *node_brick_clone = add_part_slider_add_bricks( C, block, name, 2, NULL);
	t_brick *brick_clone = ( t_brick *) node_brick_clone->data;

	if(iseq(name,"+")) 	brick_clone->exe = op_add;
	else if(iseq(name,"x")) 	brick_clone->exe = op_mult;
	else if(iseq(name,"and")) brick_clone->exe = op_and;
	else printf("[WARNING add_maths] Unknown type %s\n",name);

	return node_block;
}

// :GEOMETRY

t_node *add_brick_geometry(t_context *C,const char *name, void *data)
{
	t_geo *geo = (t_geo *) data;

	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline = 1;


	// GEO 
	add_part_geo(C, block, name, data, dt_geo);

	// LISTS
	t_node *node_points = add_part_lst( C, block, dt_lst, "points", geo->points);
	t_brick *points = ( t_brick *) node_points->data;
	points->plug_intern.state.store_data = 0;
	points->plug_intern.state.store_data_memory = 0;
	t_node *node_edges = add_part_lst( C, block, dt_lst, "edges", geo->edges);
	t_brick *edges = ( t_brick *) node_edges->data;
	edges->plug_intern.state.store_data = 0;
	edges->plug_intern.state.store_data_memory = 0;


	// CLONE
	//t_node *node_brick_clone = add_part_slider_int( C, block, "geometry", NULL);
	t_node *node_brick_clone = add_part_slider_add_bricks( C, block, "geometry", 4,NULL);
	t_brick *brick_clone = ( t_brick *) node_brick_clone->data;
	brick_clone->plug_out.state.flow_out=0;
	brick_clone->plug_out.state.open_out=0;

	brick_clone->exe=op_geometry;

	return node_block;
}

void add_brick_geo_point_bare(t_context *C,t_block *block, const char *name, void *data, int order)
{
	t_node *node = add_part_geo( C, block, name, data, dt_geo_point);
	t_brick *brick = ( t_brick *) node->data;
	brick->exe = op_void_exe;
	brick->block_order = order;
}

t_node *add_brick_geo_edge(t_context *C,const char *name, void *data)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline = 1;

	// GEO POINT
	add_brick_geo_point_bare(C,block,"point",NULL,1);
	add_brick_geo_point_bare(C,block,"point",NULL,2);

	// GEO EDGE
	add_part_geo(C,block,"edge",data,dt_geo_edge);

	return node_block;
}

t_node *add_brick_geo_array( t_context *C, const char *name , void *data)
{
	t_geo_array *array = (t_geo_array *) data;

	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline = 1;

	// VECTOR
	C->ui->add_bricks = 0;
	add_part_vector( C, block, "vector");
	C->ui->add_bricks = 1;

	// TARGET
	t_node *node_target = add_part_slider_float( C, block, "target", NULL);
	t_brick *brick_target = ( t_brick *) node_target->data;
	brick_target->brick_state.is_versatil = 1;

	// COUNT
	add_part_slider_int( C, block, "count", &array->count);
	scene_add_ref( C->scene, "struct_ref", "geo_array", "count", &array->count, array);

	// GEO ARRAY
	add_part_geo( C, block, "array", data, dt_geo_array);

	return node_block;
}

t_node *add_brick_geo_point(t_context *C,const char *name, void *data)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline = 1;

	// ADD X Y Z
	add_part_slider_float(C,block,"x",NULL);
	add_part_slider_float(C,block,"y",NULL);
	add_part_slider_float(C,block,"z",NULL);

	// ADD VECTOR
	C->ui->add_bricks = 0;
	t_node *node_vector = add_part_vector(C,block,"vector");
	t_brick *brick_vector = ( t_brick *) node_vector->data;
	brick_vector->brick_state.draw_value = 0;
	brick_vector->brick_state.has_components = 1;
	C->ui->add_bricks = 1;

	// GEO POINT
	add_part_geo(C,block,name,data,dt_geo_point);

	return node_block;
}

// GET

t_node *add_get(t_context *C)
{
	// new block
	t_node *node_block = add_block(C,"get");
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline=1;

	// get
	t_node *node_get = add_part_trigger_type(C,block,"get",op_operator,dt_operator);
	t_brick *brick_get = ( t_brick *) node_get->data;
	brick_get->brick_state.always_trigger = 1;
	brick_get->plug_intern.operator_type = ot_get;

	// result
	t_node *node_result = add_part_trigger_type(C,block,"result",op_operator,dt_int);
	t_brick *brick_result = ( t_brick *) node_result->data;

	brick_result->brick_state.is_versatil = 1;
	t_plug *plug_result = &brick_result->plug_intern;
	plug_result->state.is_volatil = 1;

	// indice
	add_part_slider_int_positive(C,block,"indice",NULL);

	return node_block;
}

// FOR

t_node *add_for(t_context *C)
{
	// new block
	t_node *node_block = add_block(C,"for");
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline=1;

	// add for
	t_node *node_for = add_part_trigger_type(C,block,"for",op_operator,dt_operator);
	t_brick *brick_for = ( t_brick *) node_for->data;
	brick_for->brick_state.always_trigger = 1;
	t_plug *plug_for = &brick_for->plug_intern;
	brick_for->plug_intern.operator_type = ot_for;
	plug_for->state.is_a_loop = 1;

	// indice
	t_node *node_indice = add_part_int(C,block,"indice",NULL);
	t_brick *brick_indice = ( t_brick *) node_indice->data;
	t_plug *plug_indice = &brick_indice->plug_intern;
	plug_indice->state.is_a_loop = 1;
	brick_indice->plug_in.state.flow_in = 0;

	// vector
	C->ui->add_bricks = 0;
	t_node *node_vector = add_part_vector(C,block,"vector");
	t_brick *brick_vector = ( t_brick *) node_vector->data;
	t_plug *plug_vector = &brick_vector->plug_intern;
	plug_vector->state.is_volatil = 1;
	plug_vector->state.is_a_loop = 1;
	brick_vector->brick_state.draw_value = 0;
	brick_vector->plug_in.state.flow_in = 0;
	brick_vector->plug_in.state.open_in = 1;
	brick_vector->plug_intern.state.is_state_volatil = 0;

	C->ui->add_bricks = 1;

	block->block_state.is_a_loop = 1;

	return node_block;
}


// VECTOR

t_node *add_vector(t_context *C, int length)
{
	C->ui->add_bricks = 0;

	// NEW BLOCK
	t_node *node_block = add_block(C,"vector");
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline=1;


	// ADD X Y Z

	if(length >= 2)
	{
		add_part_slider_float(C,block,"x",NULL);
		add_part_slider_float(C,block,"y",NULL);
	}
	if(length >=3 )
	{
		add_part_slider_float(C,block,"z",NULL);
	}
	if(length >= 4)
	{
		add_part_slider_float(C,block,"w",NULL);
	}

	// ADD VECTOR

	t_node *node_vector = add_part_vector(C,block,"vector");
	t_brick *brick_vector = ( t_brick *) node_vector->data;
	brick_vector->brick_state.draw_value = 0;
	brick_vector->brick_state.has_components = 1;
	t_plug *plug_vector = &brick_vector->plug_intern;
	t_vector *vector = ( t_vector *) plug_vector->data;
	vector->length = length;

	C->ui->add_bricks = 1;

	return node_block;
}

t_node *add_vector_3d(t_context *C)
{
	t_node *node_block = add_vector(C, 3);
	return node_block;
}

t_node *add_vector_2d(t_context *C)
{
	t_node *node_block = add_vector(C, 2);
	return node_block;
}

// CONST

t_node *add_const(t_context *C)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"vector");
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline=1;

	// i
	add_part_slider_int(C,block,"i",NULL);

	// const
	t_node *node_const = add_part_slider_int(C,block,"const",NULL);
	t_brick *brick_const = ( t_brick *) node_const->data;
	brick_const->exe = op_const;
	brick_const->brick_state.always_trigger=1;

	return node_block;
}

// STACK

t_node *add_stack(t_context *C)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"stack");
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline=1;

	// counter
	add_part_slider_int(C,block,"counter",NULL);

	// i
	add_part_slider_int(C,block,"i",NULL);

	// limit
	t_node *node_limit = add_part_slider_int(C,block,"limit",NULL);
	t_brick *brick_limit = ( t_brick *) node_limit->data;
	brick_limit->brick_state.use_loops = 0;

	// ++
	t_node *node_plus = add_part_slider_int(C,block,"++",NULL);
	t_brick *brick_plus = ( t_brick *) node_plus->data;
	brick_plus->exe = op_stack;

	return node_block;
}

// ++

t_node *add_plusplus(t_context *C)
{
	t_node *node_block = add_slider_int_special(C,"++",op_plusplus); 
	t_block *block = ( t_block *) node_block->data;
	t_brick *brick = block_brick_get(block,"++");
	brick->plug_out.state.open_out = 0;
	brick->plug_out.state.open_in = 0;

	return node_block;
}

// IF

t_node *add_if(t_context *C)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"if");
	t_block *block = ( t_block *) node_block->data;
	block->block_state.draw_outline=1;

	// true
	t_node *node_true = add_part_slider_int(C,block,"true",NULL);
	t_brick *brick_true = ( t_brick *) node_true->data;
	brick_true->brick_state.is_versatil = 1;

	// false
	t_node *node_false = add_part_slider_int(C,block,"false",NULL);
	t_brick *brick_false = ( t_brick *) node_false->data;
	brick_false->brick_state.is_versatil = 1;

	// result
	add_part_slider_int(C,block,"result",NULL);

	// if
	t_node *node_if = add_part_slider_int(C,block,"if",NULL);
	t_brick *brick_if = ( t_brick *) node_if->data;
	brick_if->exe = op_if;


	return node_block;
}





