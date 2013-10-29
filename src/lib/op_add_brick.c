/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "op.h"

#include "context.h"
#include "app.h"
#include "node.h"
#include "scene.h"
#include "ctx.h"
#include "set.h"
#include "camera.h"
#include "object.h"
#include "light.h"
#include "data.h"
#include "list.h"
#include "viewport.h"
#include "ctx.h"
#include "mesh.h"
#include "block.h"
#include "brick.h"

int set_draw_plug=1;
int current_frame = 0;
int block_pos_offset = 0;

// GET LIST

t_set *get_current_set(t_context *C)
{
	if(C->scene->sets->first)
	{
		t_node *node  = C->scene->sets->first->data;
		t_set  *set = node->data;
		return set;
	}
	else
	{
		op_new_set("set");

		t_node *node  = C->scene->sets->first->data;
		t_set  *set = node->data;
		return set;
	}
}

// GET FIRST BRICK
t_brick *get_block_first_brick(t_node *node)
{
	t_block *block=node->data;
	t_lst *lst=block->bricks;
	t_brick *brick=lst->first->data;

	return brick;
}

// PLUG DRAW
void set_plug_option(t_brick *brick)
{
	if(set_draw_plug) brick->state.draw_plugs=1;
	else brick->state.draw_plugs=0;
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
	t_brick *brick=link->data;

	int brick_height=brick->geom.height;
	int tot_brick=block->tot_bricks;

	block_pos_offset+=(brick_height*tot_brick);
}

t_node *add_block(t_context *C,const char *name)
{
	// Get Set
	t_set *set = get_current_set(C);

	// New Block
	t_node *node_block=block_make(name,"block");
	t_block *block=node_block->data;

	// Draw Outline
	block->state.draw_outline = 1;

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
	vset3f(block->pos,p[0],p[1]+block_pos_offset,0);

	return node_block;
}


/**		BRICK		**/


// BRICK SLIDER INT

t_node *add_brick_slider_int(t_context *C,t_block *block,const char *name,void *data_target)
{
	void *ptr=add_brick_data(dt_int,data_target);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_slider,dt_int,ptr);
	t_brick *brick=node_brick->data;
	
	// ACTION
	brick->action=op_slider;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK SLIDER CHAR

t_node *add_brick_slider_char(t_context *C,t_block *block,const char *name,void *data_target)
{
	void *ptr=add_brick_data(dt_char,data_target);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_slider,dt_char,ptr);
	t_brick *brick=node_brick->data;
	
	// ACTION
	brick->action=op_slider;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// NEW BRICK SLIDER FLOAT

t_node *add_brick_slider_float(t_context *C,t_block *block,const char *name,void *data_target)
{
	void *ptr=add_brick_data(dt_float,data_target);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_slider,dt_float,ptr);
	t_brick *brick=node_brick->data;
	
	// ACTION
	brick->action=op_slider;

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
	t_brick *brick=node_brick->data;
	
	// ACTION
	brick->action=op_float;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK SLIDER FLOAT CUSTOM

t_node *add_brick_slider_float_custom(t_context *C,t_block *block,const char *name,void *data_target,void *f(t_brick *b))
{
	void *ptr=add_brick_data(dt_float,data_target);

	// NEW BRICK
	t_node *node_brick=add_brick_slider_float(C,block,name,ptr);
	t_brick *brick=node_brick->data;
	
	// ACTION
	brick->action=f;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK SLIDER INT CUSTOM

t_node *add_brick_slider_int_custom(t_context *C,t_block *block,const char *name,void *data_target,void *f(t_brick *b))
{
	void *ptr=add_brick_data(dt_int,data_target);

	// NEW BRICK
	t_node *node_brick=add_brick_slider_int(C,block,name,ptr);
	t_brick *brick=node_brick->data;
	
	// ACTION
	brick->action=f;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK INT

t_node *add_brick_int(t_context *C,t_block *block,const char *name,void *data_target)
{
	void *ptr=add_brick_data(dt_int,data_target);

	// NEW BRICK
	t_node *node_brick=add_brick_slider_int(C,block,name,ptr);
	t_brick *brick=node_brick->data;
	
	// ACTION
	brick->action=op_int;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK LABEL

t_node *add_brick_label(t_context *C,t_block *block,const char *name)
{
	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_trigger,dt_string,NULL);
	t_brick *brick=node_brick->data;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK VECTOR

t_node *add_brick_vector(t_context *C,t_block *block,const char *name)
{
	void *ptr=add_brick_data(dt_vector,NULL);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_trigger,dt_vector,ptr);
	t_brick *brick=node_brick->data;
	brick->action=op_vector;

	brick->state.always_trigger = 1;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK SWITCH

t_node *add_brick_switch(t_context *C,t_block *block,const char *name,void *data_target)
{
	void *ptr=add_brick_data(dt_int,data_target);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_switch,dt_int,ptr);
	t_brick *brick=node_brick->data;

	// Dont Draw Value
	brick->state.draw_value = 0;
	
	// ACTION
	brick->action=op_switch;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK SWITCH CUSTOM

t_node *add_brick_switch_custom(t_context *C,t_block *block,const char *name,void *data_target,void *(* f)(t_brick *brick))
{
	void *ptr=add_brick_data(dt_int,data_target);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_switch,dt_int,ptr);
	t_brick *brick=node_brick->data;
	
	// ACTION
	brick->action=f;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK TRIGGER

t_node *add_brick_trigger(t_context *C,t_block *block,const char *name,void *f(t_brick *b))
{
	void *ptr=add_brick_data(dt_int,NULL);

	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_trigger,dt_int,ptr);
	t_brick *brick=node_brick->data;
	// Dont Draw Value
	brick->state.draw_value = 0;

	// ACTION
	brick->action=f;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK TRIGGER TYPE

t_node *add_brick_trigger_type(t_context *C,t_block *block,const char *name,void *f(t_brick *b),t_data_type type)
{
	// NEW BRICK
	t_node *node_brick=brick_make(block,name,bt_trigger,type,NULL);
	t_brick *brick=node_brick->data;
	
	// ACTION
	brick->action=f;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

//  BRICK OPERAND

t_node *add_brick_operand(t_context *C,t_block *block,const char *name)
{
	t_node *node_brick=add_brick_slider_float(C,block,name,NULL);
	t_brick *brick=node_brick->data;
	brick->state.draw_plugs=1;
	brick->state.draw_outline=0;
	brick->state.draw_name=1;
	brick->state.draw_value = 0;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

//  BRICK OPERATOR

t_node *add_brick_operator(t_context *C,t_block *block,const char *name)
{
	t_node *node_brick=add_brick_slider_int(C,block,name,NULL);
	t_brick *brick=node_brick->data;

	brick->state.is_clicable=0;
	brick->state.draw_name=1;
	brick->state.draw_outline=0;

	// PLUG
	set_plug_option(brick);

	return node_brick;
}

// BRICK SUBMENU

t_block *add_default_menu(t_context *C, const char *name)
{
	t_node *menu_root = scene_node_get(C->scene,"block","menu_mouse");

	t_node *menu = block_make(name,"menu");
	t_block *block = menu->data;

	add_brick_submenu(C, menu_root, menu, name);

	return block;
}

t_node *add_brick_submenu(t_context *C,t_node *menu,t_node *submenu,const char *name)
{
	t_block *block=menu->data;
	t_node *node=brick_make(block,name,bt_menu,dt_string,NULL);
	t_brick *brick=node->data;

	t_block *m_target = submenu->data;
	m_target->state.is_root=0;
	brick->menu=m_target;

	// PLUG
	set_plug_option(brick);

	return node;
}

// BRICK SUBMENU CONTEXTUAL

t_node *add_brick_submenu_contextual(t_context *C,t_node *menu,t_node *submenu,const char *name,t_node_type context)
{
	t_node *node=add_brick_submenu(C,menu,submenu,name);
	t_brick *brick=node->data;

	brick->state.is_contextual=1;
	brick->context=context;

	// PLUG
	set_plug_option(brick);

	return node;
}

// BRICK SELECTOR

t_node *add_brick_selector(t_context *C,t_block *block,const char *name,void *data_target,int length)
{
	t_node *node=brick_make(block,name,bt_selector,dt_selector,data_target);
	t_brick *brick=node->data;
	
	brick->action=op_selector;
	brick->var.selector_length=length;

	// PLUG
	set_plug_option(brick);
	
	return node;
}

// BRICK VLST

t_node *add_brick_vlst(t_context *C,t_block *block,const char *name,t_data_type type,void *pointer)
{
	t_node *node=brick_make(block,name,bt_trigger,dt_vlst,pointer);
	t_brick *brick=node->data;
	
	brick->action=op_pointer;
	brick->state.draw_value=1;
	brick->state.always_trigger = 1;
	brick->state.use_loops = 0;

	// PLUG
	set_plug_option(brick);
	
	return node;
}

// BRICK LST

t_node *add_brick_lst(t_context *C,t_block *block,const char *name,t_data_type type,void *pointer)
{
	t_node *node=brick_make(block,name,bt_trigger,dt_lst,pointer);
	t_brick *brick=node->data;
	
	brick->action=op_pointer;
	brick->state.draw_value=1;

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

	t_brick *brick=node->data;
	
	brick->action=op_pointer;
	brick->state.always_trigger = 1;

	// PLUG
	set_plug_option(brick);
	
	return node;
}

/**		PART		**/

// PART LABEL

t_node *add_part_label(t_context *C,t_block *block,const char *name)
{
	t_node *node=brick_make(block,name,bt_trigger,dt_int,NULL);
	t_brick *brick=node->data;
	brick->state.draw_outline=0;
	return node;
}

// PART TRIGGER 

t_node *add_part_trigger(t_context *C,t_block *block,const char *name,void *f(t_brick *b))
{
	t_node *node=add_brick_trigger(C,block,name,f);
	t_brick *brick=node->data;
	brick->state.draw_outline=0;
	return node;
}

t_node *add_part_vector(t_context *C,t_block *block,const char *name)
{
	t_node *node = add_brick_vector(C,block,name);
	t_brick *brick = node->data;
	brick->state.draw_outline = 0;
	return node;
}

// PART TRIGGER TYPE

t_node *add_part_trigger_type(t_context *C,t_block *block,const char *name,void *f(t_brick *b),t_data_type type)
{
	t_node *node=add_brick_trigger_type(C,block,name,f,type);
	t_brick *brick=node->data;
	brick->state.draw_outline=0;
	return node;
}

// PART SLIDER FLOAT

t_node *add_part_slider_float(t_context *C,t_block *block,const char *name,void *data_target)
{
	t_node *node=add_brick_slider_float(C,block,name,data_target);
	t_brick *brick=node->data;
	brick->state.draw_outline=0;
	return node;
}

t_node *add_part_float(t_context *C,t_block *block,const char *name,void *data_target)
{
	t_node *node=add_brick_float(C,block,name,data_target);
	t_brick *brick=node->data;
	brick->state.draw_outline=0;
	return node;
}

// PART SLIDER INT

t_node *add_part_slider_int(t_context *C,t_block *block,const char *name,void *data_target)
{
	t_node *node=add_brick_slider_int(C,block,name,data_target);
	t_brick *brick=node->data;
	brick->state.draw_outline=0;
	return node;
}

t_node *add_part_int(t_context *C,t_block *block,const char *name,void *data_target)
{
	t_node *node=add_brick_int(C,block,name,data_target);
	t_brick *brick=node->data;
	brick->state.draw_outline=0;
	return node;
}

// PART SLIDER INT POSITIVE

t_node *add_part_slider_int_positive(t_context *C,t_block *block,const char *name,void *data_target)
{
	t_node *node=add_brick_slider_int_custom(C,block,name,data_target,op_slider_positive);
	t_brick *brick=node->data;
	brick->state.draw_outline=0;
	return node;
}

// PART VLST

t_node *add_part_vlst(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr)
{
	t_node *node;

	node=add_brick_vlst(C,block,name,type,ptr);

	t_vlst *vlst=ptr;

	scene_add_ref(C->scene,"struct_ref","vlst","count",&vlst->count_new,vlst);

	t_node *node_count=add_part_slider_int_positive(C,block,"count:",&vlst->count_new);
	t_brick *brick_count=node_count->data;

	brick_count->state.use_loops = 0;
	brick_count->plug_intern.data_memory = NULL;
	brick_count->action=op_set_vlst;

	t_brick *brick=node->data;
	brick->state.draw_outline=0;

	return node;
}

// PART LST

t_node *add_part_lst(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr)
{
	t_node *node=add_brick_lst(C,block,name,type,ptr);
	t_brick *brick=node->data;
	brick->state.draw_outline=0;

	return node;
}

// PART SELECTOR

// object
t_node *add_part_selector(t_context *C,t_block *block,const char *name,t_node *node)
{
	t_node *node_brick=add_brick_trigger_type(C,block,name,op_set_selected,dt_object);
	t_brick *brick=node_brick->data;
	brick->state.draw_outline=0;
	brick->state.draw_value=1;
	brick->state.always_trigger = 1;
	t_plug *plug=&brick->plug_intern;
	plug->data=node->data;

	return node_brick;
}

// PART POINTER

// mesh 
t_node *add_part_pointer(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr)
{
	t_node *node=add_brick_pointer(C,block,name,type,ptr);
	t_brick *brick=node->data;
	brick->state.draw_outline=0;
	if(type==dt_mesh)
	{
	brick->state.draw_value=0;
	brick->state.draw_name=1;
	}
	else
	{
	brick->state.draw_value=1;
	brick->state.draw_name=0;
	}

	return node;
}


//  TRIGGER

t_node *add_trigger(t_context *C,const char *name,void*(* f)(t_brick *brick))
{
	// BLOCK
	t_node *node_block = add_block(C,"trigger");
	t_block *block=node_block->data;

	// LABEL
	add_brick_trigger(C,block,name,f);

	return node_block;
}

//  TRIGGER ALWAYS

t_node *add_trigger_always(t_context *C,const char *name,void*(* f)(t_brick *brick))
{
	// BLOCK
	t_node *node_block = add_block(C,"trigger");
	t_block *block=node_block->data;

	// LABEL
	t_node *node_brick=add_brick_trigger(C,block,name,f);
	t_brick *brick=node_brick->data;

	brick->state.always_trigger=1;

	return node_block;
}

//  LABEL

t_node *add_label(t_context *C,const char *name)
{
	// BLOCK
	t_node *node_block = add_block(C,"label");
	t_block *block=node_block->data;

	// LABEL
	add_brick_label(C,block,name);

	return node_block;
}

//  SWITCH 

t_node *add_switch(t_context *C,const char *name,void *data)
{
	// BLOCK
	t_node *node_block = add_block(C,"switch");
	t_block *block=node_block->data;

	// BRICK SWICH
	add_brick_switch(C,block,name,data);

	return node_block;
}

// SWITCH CUSTOM

t_node *add_switch_custom(t_context *C,const char *name,void *data,void *(* f)(t_brick *brick))
{
	// BLOCK
	t_node *node_block = add_block(C,"switch");
	t_block *block=node_block->data;

	// BRICK SWICH
	t_node *node_switch = add_brick_switch_custom(C,block,name,data,f);
	t_brick *brick_switch = node_switch->data;

	brick_switch->state.always_trigger = 1;

	return node_block;
}

// MULTIPLIER 

t_node *add_multiplier(t_context *C,const char *type)
{
	// BLOCK
	t_node *node_block = add_block(C,"mulitplier");
	t_block *block=node_block->data;

	t_node *node_brick=add_brick_slider_float(C,block,"multiplier",NULL);
	t_brick *brick=node_brick->data;

	if(is(type,"10"))  brick->var.increment=10; 
	else if(is(type,"100"))  brick->var.increment=100; 
	else if(is(type,".1"))  brick->var.increment=.1; 
	else if(is(type,".01"))  brick->var.increment=.01; 
	else if(is(type,".001"))  brick->var.increment=.001; 

	return node_block;
}

//  OPERATOR DOUBLE

t_node *add_operator_double(t_context *C,const char *type)
{
	// BLOCK
	t_node *node_block = add_block(C,type);
	t_block *block=node_block->data;
	block->state.draw_outline=1;

	t_node *node_op_2 = add_brick_operand(C,block,"b"); //XXX name ! -> op_add

	// RESULT
	t_node *brick_result=add_brick_operator(C,block,type);
	t_brick *result=brick_result->data;
	
	// OPERANDS
	t_node *node_op_1 = add_brick_operand(C,block,"a");

	t_brick *brick_op_1=node_op_1->data;
	t_brick *brick_op_2=node_op_2->data;

	brick_op_1->state.is_versatil=1;
	brick_op_2->state.is_versatil=1;

	// ACTIONS
	if(is(type,">"))  			result->action=op_superior;
	else if(is(type,"<"))  			result->action=op_inferior;
	else if(is(type,"="))  			result->action=op_equal;
	else if(is(type,"mod"))  		result->action=op_mod;

	plug_add_parent(&result->plug_intern,&brick_op_1->plug_intern);
	plug_add_parent(&result->plug_intern,&brick_op_2->plug_intern);

	return node_block;
}

//  OPERATOR SINGLE

t_node *add_operator_single(t_context *C,const char *type,void *f(t_brick *brick))
{
	// BLOCK
	t_node *node_block=add_slider_float(C,type,NULL);
	t_brick *brick=get_block_first_brick(node_block);

	// ACTION
	brick->action=f;

	return node_block;
}


//  SLIDER INT   

t_node *add_slider_int(t_context *C,const char *name,void *target_data)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block=node_block->data;

	// NEW BRICK
	t_node *node_brick=add_part_slider_int(C,block,name,target_data);
	t_brick *brick=node_brick->data;

	// SET ACTION
	brick->action=op_slider;

	return node_block;
}


// SLIDER CHAR

t_node *add_slider_char(t_context *C,const char *name,void *target_data)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block=node_block->data;

	// NEW BRICK
	t_node *node_brick=add_brick_slider_char(C,block,name,target_data);
	t_brick *brick=node_brick->data;

	// SET ACTION
	brick->action=op_slider;

	return node_block;
}

// SLIDER INT CUSTOM

t_node *add_slider_int_custom(t_context *C,const char *name,void *target_data,void*(* f)(t_brick *b))
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block=node_block->data;

	// NEW BRICK
	t_node *node_brick=add_brick_slider_int(C,block,name,target_data);
	t_brick *brick=node_brick->data;

	// SET ACTION
	brick->action=f;
	brick->state.is_versatil=1;

	return node_block;
}

//  SLIDER FLOAT   

t_node *add_slider_float(t_context *C,const char *name,void *target_data)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block=node_block->data;
	block->state.draw_outline = 1;

	// NEW BRICK
	t_node *node_brick=add_part_slider_float(C,block,name,target_data);
	t_brick *brick=node_brick->data;

	// SET ACTION
	brick->action=op_slider;

	return node_block;
}

//  SLIDER INT POSITIVE  

t_node *add_slider_int_positive(t_context *C,const char *name,void *target_data)
{
	// BLOCK
	t_node *node_block=add_slider_int(C,name,target_data);
	t_brick *brick=get_block_first_brick(node_block);

	// SET ACTION
	brick->action=op_slider_positive;

	return node_block;
}

//  SLIDER FLOAT CUSTOM

t_node *add_slider_float_custom(t_context *C,const char *name,void *f(t_brick *brick))
{
	// NEW SLIDER
	t_node *node_block=add_slider_float(C,name,NULL);
	t_brick *brick=get_block_first_brick(node_block);

	// SET ACTION
	brick->action=f;

	return node_block;
}

//  SLIDER FLOAT SPECIAL

t_node *add_slider_float_special(t_context *C,const char *name,void *f(t_brick *brick))
{
	// NEW SLIDER
	t_node *node_block=add_slider_float(C,name,NULL);
	t_brick *brick=get_block_first_brick(node_block);

	// SET ACTION
	brick->action=f;

	return node_block;
}

// SLIDER INT SPECIAL

t_node *add_slider_int_special(t_context *C,const char *name,void *f(t_brick *brick))
{
	// NEW SLIDER
	t_node *node_block=add_slider_int(C,name,NULL);
	t_brick *brick=get_block_first_brick(node_block);

	// SET ACTION
	brick->action=f;

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

	if(is(name,"pos x")) 		scene_add_ref(C->scene,"struct_ref","object","loc_x",&object->loc[0],object);
	else if(is(name,"pos y"))	scene_add_ref(C->scene,"struct_ref","object","loc_y",&object->loc[1],object);
	else if(is(name,"pos z"))	scene_add_ref(C->scene,"struct_ref","object","loc_z",&object->loc[2],object);
	else if(is(name,"rot x"))	scene_add_ref(C->scene,"struct_ref","object","rot_x",&object->rot[0],object);
	else if(is(name,"rot y"))	scene_add_ref(C->scene,"struct_ref","object","rot_y",&object->rot[1],object);
	else if(is(name,"rot z"))	scene_add_ref(C->scene,"struct_ref","object","rot_z",&object->rot[2],object);
	else if(is(name,"scl x"))	scene_add_ref(C->scene,"struct_ref","object","scl_x",&object->size[0],object);
	else if(is(name,"scl y"))	scene_add_ref(C->scene,"struct_ref","object","scl_y",&object->size[1],object);
	else if(is(name,"scl z"))	scene_add_ref(C->scene,"struct_ref","object","scl_z",&object->size[2],object);
	else if(is(name,"red") && mat)	scene_add_ref(C->scene,"struct_ref","material","red",&mat->color[0],mat);
	else if(is(name,"green") && mat)	scene_add_ref(C->scene,"struct_ref","material","green",&mat->color[1],mat);
	else if(is(name,"blue") && mat)		scene_add_ref(C->scene,"struct_ref","material","blue",&mat->color[2],mat);
	else if(is(name,"alpha") && mat)	scene_add_ref(C->scene,"struct_ref","material","alpha",&mat->color[3],mat);
	else if(is(name,"quad face") && mesh)	scene_add_ref(C->scene,"struct_ref","mesh","quad_face",&mesh->quad_face,mesh);

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

	if(is(name,"pos x")) 		add_part_slider_float(C,block,"pos x",&object->loc[0]);
	else if(is(name,"pos y")) 	add_part_slider_float(C,block,"pos y",&object->loc[1]);
	else if(is(name,"pos z")) 	add_part_slider_float(C,block,"pos z",&object->loc[2]);
	else if(is(name,"rot x")) 	add_part_slider_float(C,block,"rot x",&object->rot[0]);
	else if(is(name,"rot y")) 	add_part_slider_float(C,block,"rot y",&object->rot[1]);
	else if(is(name,"rot z")) 	add_part_slider_float(C,block,"rot z",&object->rot[2]);
	else if(is(name,"scl x")) 	add_part_slider_float(C,block,"scl x",&object->size[0]);
	else if(is(name,"scl y")) 	add_part_slider_float(C,block,"scl y",&object->size[1]);
	else if(is(name,"scl z")) 	add_part_slider_float(C,block,"scl z",&object->size[2]);
	else if(is(name,"red") && material) 	add_part_slider_float(C,block,"red",&material->color[0]);
	else if(is(name,"green") && material) 	add_part_slider_float(C,block,"green",&material->color[1]);
	else if(is(name,"blue") && material) 	add_part_slider_float(C,block,"blue",&material->color[2]);
	else if(is(name,"alpha") && material) 	add_part_slider_float(C,block,"alpha",&material->color[3]);
	else if(is(name,"color") && mesh) 	mesh_add_default_color(mesh); 
	else if(is(name,"faces") && mesh) 	mesh_add_brick_faces(mesh); 

	block->state.update_geometry = 1;

}

// SLIDER OBJECT

t_node *add_slider_object(t_context *C,const char *name)
{
	t_node *selected=C->scene->selected;

	if(selected)
	{
		t_object *object=selected->data; 

		add_slider_ref(C,object,name);
		add_slider_target(C,object,name);
	}

	return NULL;
}

// SLIDER CAMERA

t_node *add_slider_camera(t_context *C,const char *name)
{
	// Get default Viewport
	t_node *node_viewport = scene_node_get(C->scene,"viewport","viewport");
	t_viewport *viewport = NULL;
	t_camera *camera = NULL;

	if(node_viewport)
	{
		viewport = node_viewport->data;
		camera = viewport->camera;
	}

	t_node *node = NULL;

	if(camera)
	{
		if(is(name,"pos x"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","pos_x",&camera->pos[0],camera);
			node = add_slider_float(C,"cam pos x",&camera->pos[0]);
		}
		else if(is(name,"pos y"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","pos_y",&camera->pos[1],camera);
			node = add_slider_float(C,"cam pos y",&camera->pos[1]);
		}
		else if(is(name,"pos z"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","pos_z",&camera->pos[2],camera);
			node = add_slider_float(C,"cam pos z",&camera->pos[2]);
		}
		else if(is(name,"eye x"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","eye_x",&camera->eye[1],camera);
			node = add_slider_float(C,"cam eye x",&camera->eye[1]);
		}
		else if(is(name,"eye y"))
		{
			scene_add_ref(C->scene,"struct_ref","camera","eye_y",&camera->eye[2],camera);
			node = add_slider_float(C,"cam eye y",&camera->eye[2]);
		}
		else if(is(name,"eye z"))
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
	t_block *block=node_block->data;
	block->state.draw_outline=1;

	// Add Clone
	t_node *node_brick_clone=add_part_slider_int(C,block,"clone",NULL);
	t_brick *brick_clone=node_brick_clone->data;

	brick_clone->action=op_clone;
	brick_clone->state.use_dragging = 0;

	// Add Brick
	t_node *node_brick = add_part_slider_float(C,block,"brick",NULL);
	t_brick *brick_brick = node_brick->data;
	brick_brick->state.is_versatil = 1;
	t_plug *plug_brick = &brick_brick->plug_intern;

	plug_brick->state.swap_flow = 1;

	t_plug *plug_clone = &brick_clone->plug_intern;
	plug_add_parent(plug_clone,plug_brick);

	return node_block;
}


// PIPE

t_node *add_pipe(t_context *C)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"pipe");
	t_block *block=node_block->data;
	block->state.draw_outline=1;

	// CLONE

	t_node *node_brick_clone=add_part_slider_int(C,block,"pipe",NULL);
	t_brick *brick_clone=node_brick_clone->data;

	add_part_slider_int(C,block,"state",NULL);

	// re-init to float (old int data used by plug_out)
	plug_init(&brick_clone->plug_in,dt_float,brick_clone,NULL,0);

	brick_clone->action=op_pipe;
	brick_clone->plug_out.state.flow_out=0;
	brick_clone->plug_in.state.flow_in=0;
	brick_clone->plug_out.state.open_out=0;

	return node_block;
}

// MATHS

t_node *add_maths(t_context *C,const char *name)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,name);
	t_block *block=node_block->data;
	block->state.draw_outline = 1;

	// CLONE
	t_node *node_brick_clone = add_part_slider_int( C, block, name, NULL);
	t_brick *brick_clone=node_brick_clone->data;

	// Brick Result
	add_part_slider_int(C,block,"result",NULL);

	brick_clone->plug_out.state.flow_out=0;
	brick_clone->plug_out.state.open_out=0;
	brick_clone->state.use_dragging = 0;

	if(is(name,"+"))
		brick_clone->action=op_add;
	else if(is(name,"x"))
		brick_clone->action=op_mult;
	else if(is(name,"and"))
		brick_clone->action=op_and;
	else
		printf("[WARNING add_maths] Unknown type %s\n",name);

	return node_block;
}

// GET

t_node *add_get(t_context *C)
{
	// new block
	t_node *node_block = add_block(C,"get");
	t_block *block=node_block->data;
	block->state.draw_outline=1;

	// get
	t_node *node_get = add_part_trigger_type(C,block,"get",op_operator,dt_operator);
	t_brick *brick_get = node_get->data;
	t_plug *plug_get = &brick_get->plug_intern;
	brick_get->state.always_trigger = 1;
	brick_get->plug_intern.operator_type = ot_get;

	// result
	t_node *node_result = add_part_trigger_type(C,block,"result",op_operator,dt_int);
	t_brick *brick_result = node_result->data;

	brick_result->state.is_versatil = 1;
	t_plug *plug_result = &brick_result->plug_intern;
	plug_result->state.is_volatil = 1;

	// indice
	t_node *node_indice = add_part_slider_int_positive(C,block,"indice",NULL);
	t_brick *brick_indice = node_indice->data;
	t_plug *plug_indice = &brick_indice->plug_intern;

	plug_add_parent(plug_result,plug_get);
	plug_add_parent(plug_get,plug_indice);

	return node_block;
}

// FOR

t_node *add_for(t_context *C)
{
	// new block
	t_node *node_block = add_block(C,"for");
	t_block *block=node_block->data;
	block->state.draw_outline=1;

	// add for
	t_node *node_for = add_part_trigger_type(C,block,"for",op_operator,dt_operator);
	t_brick *brick_for = node_for->data;
	brick_for->state.always_trigger = 1;
	t_plug *plug_for = &brick_for->plug_intern;
	brick_for->plug_intern.operator_type = ot_for;
	plug_for->state.is_a_loop = 1;

	// indice
	t_node *node_indice = add_part_int(C,block,"indice",NULL);
	t_brick *brick_indice = node_indice->data;
	t_plug *plug_indice = &brick_indice->plug_intern;
	plug_indice->state.is_a_loop = 1;
	brick_indice->plug_in.state.flow_in = 0;

	// vector
	t_node *node_vector = add_part_vector(C,block,"vector");
	t_brick *brick_vector = node_vector->data;
	t_plug *plug_vector = &brick_vector->plug_intern;
	plug_vector->state.is_volatil = 1;
	plug_vector->state.is_a_loop = 1;
	plug_vector->state.is_parent = 1;
	brick_vector->state.draw_value = 0;
	brick_vector->plug_in.state.flow_in = 0;
	brick_vector->plug_in.state.open_in = 1;
	brick_vector->plug_intern.state.is_state_volatil = 0;

	// Parent
	plug_add_parent(plug_for,plug_vector);

	block->state.is_a_loop = 1;

	return node_block;
}

// VECTOR

t_node *parent_brick_vector(t_plug *plug_vector, t_node *node, int order)
{
	t_brick *brick = node->data;
	brick->block_order = order;
	brick->state.draw = 0;
	t_plug *plug_intern = &brick->plug_intern;
	plug_intern->state.store_data = 0;
	plug_add_parent(plug_vector,plug_intern);

	return node;
}

// VECTOR

t_node *add_vector(t_context *C)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"vector");
	t_block *block=node_block->data;
	block->state.draw_outline=1;

	// ADD VECTOR

	t_node *node_vector = add_part_vector(C,block,"vector");
	t_brick *brick_vector = node_vector->data;
	brick_vector->state.draw_value = 0;
	brick_vector->state.has_components = 1;
	t_plug *plug_vector = &brick_vector->plug_intern;

	// ADD X Y Z

	parent_brick_vector(plug_vector, add_part_slider_float(C,block,"x",NULL),0);
	parent_brick_vector(plug_vector, add_part_slider_float(C,block,"y",NULL),1);
	parent_brick_vector(plug_vector, add_part_slider_float(C,block,"z",NULL),2);
	parent_brick_vector(plug_vector, add_part_slider_float(C,block,"w",NULL),3);

	return node_block;
}

// VECTOR 3D

t_node *add_vector_3d(t_context *C)
{
	t_node *node_block = add_vector(C);
	t_block *block = node_block->data;
	
	int i;
	for(i = 0; i < 3; i++)
	{
		t_brick *brick = block_brick_get_by_order(block, i);
		brick->state.draw = 1;
	}

	return node_block;
}

// VECTOR 2D

t_node *add_vector_2d(t_context *C)
{
	t_node *node_block = add_vector(C);
	t_block *block = node_block->data;
	
	int i;
	for(i = 0; i < 2; i++)
	{
		t_brick *brick = block_brick_get_by_order(block, i);
		brick->state.draw = 1;
	}

	return node_block;
}

// CONST

t_node *add_const(t_context *C)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"vector");
	t_block *block=node_block->data;
	block->state.draw_outline=1;

	t_node *node_const = add_part_slider_int(C,block,"const",NULL);
	t_brick *brick_const = node_const->data;

	brick_const->action = op_const;
	brick_const->state.always_trigger=1;

	add_part_slider_int(C,block,"i",NULL);

	return node_block;
}

// STACK

t_node *add_stack(t_context *C)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"stack");
	t_block *block=node_block->data;
	block->state.draw_outline=1;

	// counter
	t_node *node_counter = add_part_slider_int(C,block,"counter",NULL);
	t_brick *brick_counter = node_counter->data;
	t_plug *plug_counter = &brick_counter->plug_intern;

	// i
	t_node *node_i = add_part_slider_int(C,block,"i",NULL);
	t_brick *brick_i = node_i->data;
	t_plug *plug_i = &brick_i->plug_intern;

	// limit
	t_node *node_limit = add_part_slider_int(C,block,"limit",NULL);
	t_brick *brick_limit = node_limit->data;
	t_plug *plug_limit = &brick_limit->plug_intern;

	brick_limit->state.use_loops = 0;

	// ++

	t_node *node_plus = add_part_slider_int(C,block,"++",NULL);
	t_brick *brick_plus = node_plus->data;
	t_plug *plug_plus = &brick_plus->plug_intern;

	brick_plus->action = op_stack;

	plug_add_parent(plug_i,plug_plus);
	plug_add_parent(plug_counter,plug_plus);
	plug_add_parent(plug_limit,plug_plus);

	return node_block;
}

// ++

t_node *add_plusplus(t_context *C)
{
	t_node *node_block = add_slider_int_special(C,"++",op_plusplus); 
	t_block *block = node_block->data;
	t_brick *brick = block_brick_get(block,"++");
	brick->plug_out.state.open_out = 0;
	brick->plug_out.state.open_in = 0;

	return node_block;
}

t_node *add_if(t_context *C)
{
	// NEW BLOCK
	t_node *node_block = add_block(C,"if");
	t_block *block=node_block->data;
	block->state.draw_outline=1;

	// if
	t_node *node_if = add_part_slider_int(C,block,"if",NULL);
	t_brick *brick_if = node_if->data;
	t_plug *plug_if = &brick_if->plug_intern;

	// true
	t_node *node_true = add_part_slider_int(C,block,"true",NULL);
	t_brick *brick_true = node_true->data;
	brick_true->state.is_versatil = 1;
	t_plug *plug_true = &brick_true->plug_intern;

	// false
	t_node *node_false = add_part_slider_int(C,block,"false",NULL);
	t_brick *brick_false = node_false->data;
	brick_false->state.is_versatil = 1;
	t_plug *plug_false = &brick_false->plug_intern;

	// result
	t_node *node_result = add_part_slider_int(C,block,"result",NULL);
	t_brick *brick_result = node_result->data;
	t_plug *plug_result = &brick_result->plug_intern;

	brick_if->action = op_if;

	plug_add_parent(plug_true,plug_if);
	plug_add_parent(plug_false,plug_if);
	plug_add_parent(plug_result,plug_if);

	return node_block;
}

t_block *get_menu(t_context *C,const char *name)
{
	t_node *node;
	t_block *block = NULL;

	if(is(name,"add"))
	{
		node = scene_node_get(C->scene,"block","menu_node_add");
		block = node->data;
	}

	return block;
}


