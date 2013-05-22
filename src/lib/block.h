/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#ifndef __BLOCK_H
#define __BLOCK_H

#define BRICK_MIN_WIDTH 130

#include "minuit.h"
#include "txt.h"

typedef struct Block t_block;
typedef struct Block_Class t_block_class;
typedef struct Block_State t_block_state;

typedef struct Brick t_brick;
typedef struct BrickClass t_brick_class;
typedef struct Brick_State t_brick_state;
typedef struct Brick_Geometry t_brick_geometry;
typedef struct Brick_Var t_brick_var;
typedef enum Brick_Mode t_brick_mode;
typedef enum Brick_Type t_brick_type;

typedef struct Plug t_plug;
typedef struct Plug_Class t_plug_class;

typedef enum Plug_Mode
{
	mode_in,
	mode_out

}t_plug_mode;


typedef enum Type_Operator
{
	ot_null,
	ot_for,
	ot_get,
	ot_const,

}t_operator;

typedef enum Type_Parent
{
	t_parent_child,
	t_child_parent
}t_parent;

typedef enum Type_Operation 
{
	t_op_add,
	t_op_mult

}t_operation;

/***		BLOCK		***/

struct Block_Class
{
	char cls_type[_NAME_];
	char type[_NAME_];
	void (* make)(t_block *self);
	void (* link)(t_block *self,t_node *target);
	void (* draw)(t_block *self);
	void (* update)(t_block *self);
	void (* init)(t_block *self);
};

struct Block_State
{	
	int is_root:1;
	int is_show:1;	
	int draw_outline:1;
	int draw_plugs:1;
	int is_mouse_over:1;
	int update_geometry:1;
};

struct Block
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	t_block_class *cls;

	int idcol[3];
	char type[_NAME_];

	float pos[3];				// block position in space
	int cursor; 				// for second menu

	float width;				// block width
	float height;				// block height

	t_block_state state;			// block state

	int tot_bricks; 			// drawing plugs
	int graph_order;			// unset > -1

	t_lst *bricks;
	t_brick *submenu;
	t_brick *selected;			// submenu
};

/***		PLUG		***/

struct Plug_Class
{
	char cls_type[_NAME_];
	t_data_type type;
	void (* make)(t_plug *plug);
	void (* flow)(t_plug *plug);
	void (* connect)(t_plug_mode mode, t_plug *self, t_plug *dst);
	void (* disconnect)(t_plug_mode mode, t_plug *plug);
};

// PLUG

struct Plug
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	t_plug_class *cls;

	int idcol[3];

	int pos; 			// height (for drawing line)
	int is_connected;
	int is_updated;
	int is_init;
	int is_versatil;		// change plug type if connected
	int store_data;			// for rebind
	int is_eval;
	int is_volatil;			// stored data is volatil
	int is_state_volatil;		// plug ports can change state when connected
	int is_a_loop;
	int is_in_loop;
	int close_flow_in;		// will close target's flow in
	int use_flow;
	int is_parent;

	int flow_in;
	int flow_out;
	int follow_in;
	int follow_out;
	int open_in;
	int open_out;

	int bang;
	int last_bang;

	t_plug *src;
	t_plug *dst;

	t_lst *bindings;
	t_lst *parents;
	t_plug *child;
	t_brick *brick;			// self
	t_data_type data_type;
	t_operator operator_type;

	void *data;
	void *data_memory;
};

/***		BRICK		***/

// BRICK TYPE

enum Brick_Type
{
	bt_trigger,
	bt_slider,
	bt_menu,
	bt_selector,
	bt_switch,
};

// BRICK MODE

enum Brick_Mode
{
	bm_idle,
	bm_triggering,
	bm_linking,
	bm_unlinking,
	bm_moving,
	bm_typing,
	bm_cloning,
};

// BRICK CLASS

struct BrickClass
{
	char cls_type[_NAME_];
	t_brick_type type;
	void (* make)(t_brick *brick);
	void (* draw)(t_brick *brick);
	void (* update)(t_brick *brick);  
	void (* trigger)(t_brick *brick); 
	void (* init)(t_brick *brick);
	void (* connect)(t_brick *self,t_brick *target);
	void (* disconnect)(t_brick *self);
};

// BRICK STATE

struct Brick_State
{
	// ON

	int is_idle:1;
	int is_released:1;
	int draw_outline:1;
	int draw_name:1;
	int is_clicable:1;
	int use_min_width:1;
	int use_block_width:1;
	int is_mouse_mode:1;
	int draw:1;

	// OFF

	int draw_plugs:1;
	int draw_value:1;
	int show_menu:1;
	int is_pressed:1;
	int is_current:1;
	int is_mouse_over:1;		
	int is_mouse_over_plug_out:1;		
	int is_mouse_over_plug_in:1;		
	int is_moving:1;
	int is_linking:1; 		// start linking state
	int is_draging:1;		// clic & drag number but
	int is_done:1;			// action (number) is done
	int is_left_pressed:1;
	int is_right_pressed:1;
	int use_brick_blocking:1;	// one clic
	int use_global_blocking:1;	// one clic per screen
	int is_plug_update:1;
	int always_trigger:1;
	int is_contextual:1;
	int is_versatil:1;
	int has_ref:1;
	int use_dragging:1;
	int use_loops:1;
	int remove_connected:1;
	int is_root:1;
	int debug:1;
	int has_components:1;
	int clone;
	int frame_loop;
	int has_limit_low:1;
	int has_limit_high:1;
};

// BRICK GEOMETRY

struct Brick_Geometry
{
	float width;
	float height;
	float min_width;
	float min_height;
	int block_pos;			//y position in block
	float margin;
};

// BRICK VAR

struct Brick_Var
{
	float increment;
	int selector;
	int selector_length;
	char selector_list[_LIST_*_NAME_LONG_];
	int limit_int_low;
	int limit_int_high;
	float limit_float_low;
	float limit_float_high;
};

// BRICK

struct Brick
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];			
	t_brick_class *cls;			// cls

	t_brick_type type;
	t_node_type context;			// contextual menus

	int idcol_right[3];			// col
	int idcol_left[3];

	int graph_order;			// Graph 
	int counter;				// For Loop
	int block_order;			// Internal Block Id

	t_brick_mode mode;			// unique mode
	t_brick_state state;			// multiple states
	t_brick_geometry geom;			// geometry
	t_brick_var var;			// attributes

	// txts
	t_txt txt_name;				
	t_txt txt_data;

	// plugs
	t_plug plug_intern;
	t_plug plug_in;
	t_plug plug_out;

	//blocks
	t_block *menu; 				// submenu
	t_block *block; 			// block container

	// action
	void *(* action)(t_brick *brick);	
};



/***		BLOCK		***/




// BLOCK

t_brick *	block_brick_get_by_order(t_block *block, int order);
void 		block_set_graph_order(t_block *block, int order);
int 		block_is_connected(const char *gate, t_block *block);
t_lst 		*block_get_connections(const char *gate,t_block *block);
t_block *	block_copy(t_block *block);
t_block *	block_clone(t_block *block);
t_brick*	block_brick_get(t_block *block,const char *name);
void 		block_brick_add(t_block *block,t_node *node_brick);
t_node*		block_create(const char *name);
t_node*		block_make(const char *name,const char *type);
void 		block_free(t_block *block);
t_block *	block_new(const char *name);
t_block *	block_rebind(t_scene *sc,void *ptr);


// BLOCK DRAW

int 		brick_get_width(t_brick *brick);
void 		block_draw(t_block *block);
void		cls_block_draw_generic(t_block *self);
void		cls_block_draw_menu(t_block *self);
void		cls_block_draw_ref(t_block *self);
void		cls_block_draw_block(t_block *self);

// BLOCK_UDPATE

void		ctx_block_unstore(void);
void 		context_set_block(void);
void 		cls_block_update(t_block *block);
void		cls_block_generic_update(t_block *self);
void		cls_block_ref_update(t_block *self);
void		cls_block_menu_update(t_block *self);
void		cls_block_block_update(t_block *self);
void		block_unstore(t_block *block);



/***		BRICK		***/


// BRICK

int 		brick_is_different(t_brick *dst, t_brick *src);
void 		brick_copy_data(t_brick *dst, t_brick *src);
void 		brick_binding_add(t_brick *brick, t_data_type type, void *data);
void 		plug_child_remove_all_parents(t_plug *child);
t_brick *	brick_copy(t_block *block,t_brick *brick);
t_brick *	brick_clone(t_block *block,t_brick *brick);
void 		plug_color_init(t_plug *plug);

void 		plug_init( t_plug *plug, t_data_type type, t_brick *brick, void *data_target, int duplicate);

void 		brick_release(t_brick *brick);
void 		brick_free(t_brick *brick);
int 		brick_delete(t_brick *brick,int remove_connected);
void 		brick_type_change(t_brick *brick,t_plug *plug);
void 		brick_type_reset(t_brick *brick);
void 		brick_change_type_by_name(t_brick *brick,t_data_type type);

void 		brick_init(t_scene *sc,t_brick *brick);
void 		_brick_init(t_brick *brick);

t_brick *	brick_new(const char *name);
void *		brick_get_ref(t_brick *brick, const char *ref);
t_brick *	brick_rebind(t_scene *sc,void *ptr);
t_node 		*brick_make(t_block *block,const char *name,t_brick_type brick_type,t_data_type data_type,void *data_target);

// BRICK_CLS

void 		brick_cls_init(t_brick *brick);
void 		plug_cls_init(t_plug *plug);
void 		cls_plug_update(t_plug *plug);

// BRICK_DRAW

void		brick_draw(t_brick *brick);
void		cls_brick_build_width(t_brick *brick);
void 		cls_brick_build_txt(t_brick *brick);

// BRICK_ACT

void *		op_limit(t_brick *brick);
void *		op_selector(t_brick *brick);
void *		op_brick_node_action(t_brick *brick);
void *		op_void(t_brick *brick);
void *		op_clone(t_brick *brick);
void *		op_loop(t_brick *brick);
void *		op_loop_get(t_brick *brick);
void *		op_switch(t_brick *brick);
void *		op_trigger(t_brick *brick);
void *		op_sin(t_brick *brick);
void *		op_cos(t_brick *brick);
void *		op_divide(t_brick *brick);
void *		op_add(t_brick *brick);
void *		op_superior(t_brick *brick);
void *		op_inferior(t_brick *brick);
void *		op_plusplus(t_brick *brick);
void *		op_mult(t_brick *brick);
void * 		op_and(t_brick *brick);
void *		op_slider(t_brick *brick);
void *		op_slider_positive(t_brick *brick);
void *		op_slider_positive_non_zero(t_brick *brick);
void *		op_if(t_brick *brick);
void *		op_equal(t_brick *brick);
void *		op_while(t_brick *brick);
void *		op_in_multiplicator(t_brick *brick);
void *		op_window(t_brick *brick);
void *		op_set_selected(t_brick *brick);
void *		op_pointer(t_brick *brick);
void *		op_delete(t_brick *brick);
void *		op_pipe(t_brick *brick);
void *		op_rewind(t_brick *brick);
void *		op_not(t_brick *brick);
void *		op_set_vlst(t_brick *brick);
void *		op_set_colors(t_brick *brick);
void *		op_rnd(t_brick *brick);
void *		op_neg(t_brick *brick);
void *		op_is_last(t_brick *brick);
void *		op_operator(t_brick *brick);
void *		op_vector(t_brick *brick);
void *		op_bang(t_brick *brick);
void *		op_do_quit(t_brick *brick);

// BRICK_UPDATE

void 		plug_get_data(t_plug *plug);
void 		cls_brick_trigger_vlst(t_brick *brick);
void		cls_brick_trigger_lst(t_brick *brick);
void 		cls_brick_trigger_selector(t_brick *brick);
void 		cls_brick_trigger_number(t_brick *brick);
void 		cls_brick_trigger_switch(t_brick *brick);
void 		cls_brick_trigger_menu(t_brick *brick);
void 		cls_brick_trigger_generic(t_brick *brick);
void 		cls_brick_trigger_operator(t_brick *brick);
void 		cls_brick_update(t_brick *brick);
void 		cls_brick_trigger_action_default(t_brick *brick);

void 		cls_brick_connect(t_brick *self,t_brick *target);
void 		cls_brick_disconnect(t_brick *self);

void 		brick_build_width(t_brick *brick);
void 		brick_build_txt(t_brick *brick);


t_plug *	brick_get_src(t_brick *brick);
t_plug *	plug_get_src(t_plug *plug);

t_plug *	plug_get_dst(t_plug *plug);

void 		cls_brick_trigger_connections(t_brick *brick);

void 		brick_set_updated(t_brick *brick);

void 		plug_data_reset(t_plug *plug);
void 		plug_data_negate(t_plug *plug);
void 		plug_data_set(t_plug *plug,t_data_type type,void *data);



void 		plug_add_parent(t_plug *plug,t_plug *parent);
void 		plug_remove_parent(t_plug *plug);

void 		*op_stack(t_brick *brick);

void 		set_for_loop(t_block *block ,int state);
void 		plug_debug(t_plug *plug);

#endif




