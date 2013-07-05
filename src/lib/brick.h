/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#ifndef __BRICK_H
#define __BRICK_H

#include "txt.h"
#include "plug.h"
#include "node.h"

struct Block;
struct Brick;
struct Dict;

enum Data_Type;

struct Brick_State;
struct Brick_Geometry;
struct Brick_Var;

// BRICK TYPE

typedef enum Brick_Type
{
	bt_trigger,
	bt_slider,
	bt_menu,
	bt_selector,
	bt_switch,

}t_brick_type;

// BRICK MODE

typedef enum Brick_Mode
{
	bm_idle,
	bm_triggering,
	bm_linking,
	bm_unlinking,
	bm_moving,
	bm_typing,
	bm_cloning,

}t_brick_mode;

// BRICK CLASS

typedef struct BrickClass
{
	char cls_type[_NAME_];
	enum Brick_Type type;
	void (* make)(struct Brick *brick);
	void (* draw)(struct Brick *brick);
	void (* update)(struct Brick *brick);  
	void (* trigger)(struct Brick *brick); 
	void (* init)(struct Brick *brick);
	void (* connect)(struct Brick *self,struct Brick *target);
	void (* disconnect)(struct Brick *self);

}t_brick_class;

// BRICK STATE

typedef struct Brick_State
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

}t_brick_state;

// BRICK GEOMETRY

typedef struct Brick_Geometry
{
	float width;
	float width_txt;
	float height;
	float min_width;
	float min_height;
	int block_pos;			//y position in block
	float margin;

}t_brick_geometry;

// BRICK VAR

typedef struct Brick_Var
{
	float increment;
	int selector;
	int selector_length;
	char selector_list[_LIST_*_NAME_LONG_];
	int limit_int_low;
	int limit_int_high;
	float limit_float_low;
	float limit_float_high;

}t_brick_var;

// BRICK

typedef struct Brick
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];			
	t_brick_class *cls;			// cls

	enum Brick_Type type;
	enum Node_Type context;			// contextual menus

	int idcol_right[3];			// col
	int idcol_left[3];

	int rhizome_order;			// Graph 
	int counter;				// For Loop
	int block_order;			// Internal Block Id

	enum Brick_Mode mode;			// unique mode
	struct Brick_State state;			// multiple states
	struct Brick_Geometry geom;			// geometry
	struct Brick_Var var;			// attributes

	// txts
	struct Txt txt_name;				
	struct Txt txt_data;

	// plugs
	struct Plug plug_intern;
	struct Plug plug_in;
	struct Plug plug_out;

	//blocks
	struct Block *menu; 				// submenu
	struct Block *block; 			// block container

	// action
	void *(* action)(struct Brick *brick);	

}t_brick;


void 		brick_remove(struct Dict *args);

int 		brick_is_different(struct Brick *dst, struct Brick *src);
void 		brick_copy_data(struct Brick *dst, struct Brick *src);
void 		brick_binding_add(struct Brick *brick, enum Data_Type type, void *data);
void 		plug_child_remove_all_parents(struct Plug *child);
struct Brick *	brick_copy(struct Block *block,struct Brick *brick);
struct Brick *	brick_dupli(struct Block *block,struct Brick *brick);
void 		plug_color_init(struct Plug *plug);

void 		plug_init( struct Plug *plug, enum Data_Type type, struct Brick *brick, void *data_target, int duplicate);

void 		brick_release(struct Brick *brick);
void 		brick_free(struct Brick *brick);
int 		brick_delete(struct Brick *brick,int remove_connected);
void 		brick_type_change(struct Brick *brick,struct Plug *plug);
void 		brick_type_reset(struct Brick *brick);
void 		brick_change_type_by_name(struct Brick *brick,enum Data_Type type);

void 		brick_init(struct Scene *sc,struct Brick *brick);
void 		_brick_init(struct Brick *brick);

t_brick *	brick_clone(t_brick *brick);
struct Brick *	brick_new(const char *name);
void *		brick_get_ref(struct Brick *brick, const char *ref);
struct Brick *	brick_rebind(struct Scene *sc,void *ptr);
struct Node 	*brick_make(struct Block *block,const char *name,t_brick_type brick_type,enum Data_Type data_type,void *data_target);

// BRICK_CLS

void 		brick_cls_init(struct Brick *brick);
void 		plug_cls_init(struct Plug *plug);
void 		cls_plug_update(struct Plug *plug);

// BRICK_DRAW

void		brick_draw(struct Brick *brick);
void		cls_brick_build_width(struct Brick *brick);
void 		cls_brick_build_txt(struct Brick *brick);

// BRICK_ACT



// OP_ADD_OBJECT

void *		op_add_viewport(t_brick *brick);
void *		op_add_set(struct Brick *brick);
void *		op_new_set(const char *name);
void *		op_new_camera(const char *name);
struct Node *	op_new_cube(const char *name);
struct Node *	op_add_new_wire_cube(struct Node *node);
void 		add_mn(void);
void *		op_add_mn(struct Brick *brick);
void *		op_add_camera(struct Brick *brick);
void *	 	op_add_camera_main(void);
void *		op_add_default(struct Brick *brick);
void *		op_add_light(struct Brick *brick);
void *		op_add_cube(struct Brick *brick);
void *		op_add_wire_cube(struct Brick *brick);
void *		op_add_uv_cube(struct Brick *brick);
void *		op_add_uv_plane(struct Brick *brick);
void *		op_add_triangle(struct Brick *brick);
void *		op_add_quad(struct Brick *brick);


void *		op_camera_rotate_xy(struct Brick *brick);
void *		op_camera_rotate_z(struct Brick *brick);



void *		op_brick_add(struct Brick *brick);
void *		op_limit(struct Brick *brick);
void *		op_selector(struct Brick *brick);
void *		op_brick_node_action(struct Brick *brick);
void *		op_void(struct Brick *brick);
void *		op_clone(struct Brick *brick);
void *		op_loop(struct Brick *brick);
void *		op_loop_get(struct Brick *brick);
void *		op_switch(struct Brick *brick);
void *		op_trigger(struct Brick *brick);
void *		op_sin(struct Brick *brick);
void *		op_cos(struct Brick *brick);
void *		op_divide(struct Brick *brick);
void *		op_add(struct Brick *brick);
void *		op_superior(struct Brick *brick);
void *		op_inferior(struct Brick *brick);
void *		op_plusplus(struct Brick *brick);
void *		op_mult(struct Brick *brick);
void * 		op_and(struct Brick *brick);
void *		op_slider(struct Brick *brick);
void *		op_slider_positive(struct Brick *brick);
void *		op_slider_positive_non_zero(struct Brick *brick);
void *		op_if(struct Brick *brick);
void *		op_equal(struct Brick *brick);
void *		op_while(struct Brick *brick);
void *		op_in_multiplicator(struct Brick *brick);
void *		op_window(struct Brick *brick);
void *		op_set_selected(struct Brick *brick);
void *		op_pointer(struct Brick *brick);
void *		op_delete(struct Brick *brick);
void *		op_pipe(struct Brick *brick);
void *		op_rewind(struct Brick *brick);
void *		op_not(struct Brick *brick);
void *		op_set_vlst(struct Brick *brick);
void *		op_set_colors(struct Brick *brick);
void *		op_rnd(struct Brick *brick);
void *		op_neg(struct Brick *brick);
void *		op_is_last(struct Brick *brick);
void *		op_operator(struct Brick *brick);
void *		op_vector(struct Brick *brick);
void *		op_bang(struct Brick *brick);
void *		op_do_quit(struct Brick *brick);
void *		op_const(struct Brick *brick);
void *		op_mod(struct Brick *brick);
void *		op_float(struct Brick *brick);
void *		op_int(struct Brick *brick);
void *		op_stack(struct Brick *brick);

void 		cls_brick_trigger_connections(struct Brick *brick);

void 		brick_set_updated(struct Brick *brick);

// BRICK_UPDATE

void 		cls_brick_trigger_vlst(struct Brick *brick);
void		cls_brick_trigger_lst(struct Brick *brick);
void 		cls_brick_trigger_selector(struct Brick *brick);
void 		cls_brick_trigger_number(struct Brick *brick);
void 		cls_brick_trigger_switch(struct Brick *brick);
void 		cls_brick_trigger_menu(struct Brick *brick);
void 		cls_brick_trigger_generic(struct Brick *brick);
void 		cls_brick_trigger_operator(struct Brick *brick);
void 		cls_brick_update(struct Brick *brick);
void 		cls_brick_trigger_action_default(struct Brick *brick);

void 		cls_brick_connect(struct Brick *self,struct Brick *target);
void 		cls_brick_disconnect(struct Brick *self);

void 		_cls_brick_connect(struct Brick *self,struct Brick *target);
void 		_cls_brick_disconnect(struct Brick *self);

void 		brick_build_width(struct Brick *brick);
void 		brick_build_txt(struct Brick *brick);

struct Plug *	brick_get_src(struct Brick *brick);

int 		brick_get_width(struct Brick *brick);
void _brick_free(t_brick *brick);

#endif
