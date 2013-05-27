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

enum Data_Type;

typedef struct Brick t_brick;
typedef struct BrickClass t_brick_class;
typedef struct Brick_State t_brick_state;
typedef struct Brick_Geometry t_brick_geometry;
typedef struct Brick_Var t_brick_var;
typedef enum Brick_Mode t_brick_mode;
typedef enum Brick_Type t_brick_type;

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
	struct Txt txt_name;				
	struct Txt txt_data;

	// plugs
	struct Plug plug_intern;
	struct Plug plug_in;
	struct Plug plug_out;

	//blocks
	t_block *menu; 				// submenu
	t_block *block; 			// block container

	// action
	void *(* action)(t_brick *brick);	
};


void 		brick_remove(struct Dict *args);

int 		brick_is_different(t_brick *dst, t_brick *src);
void 		brick_copy_data(t_brick *dst, t_brick *src);
void 		brick_binding_add(t_brick *brick, enum Data_Type type, void *data);
void 		plug_child_remove_all_parents(struct Plug *child);
t_brick *	brick_copy(t_block *block,t_brick *brick);
t_brick *	brick_clone(t_block *block,t_brick *brick);
void 		plug_color_init(struct Plug *plug);

void 		plug_init( struct Plug *plug, enum Data_Type type, t_brick *brick, void *data_target, int duplicate);

void 		brick_release(t_brick *brick);
void 		brick_free(t_brick *brick);
int 		brick_delete(t_brick *brick,int remove_connected);
void 		brick_type_change(t_brick *brick,struct Plug *plug);
void 		brick_type_reset(t_brick *brick);
void 		brick_change_type_by_name(t_brick *brick,enum Data_Type type);

void 		brick_init(struct Scene *sc,t_brick *brick);
void 		_brick_init(t_brick *brick);

t_brick *	brick_new(const char *name);
void *		brick_get_ref(t_brick *brick, const char *ref);
t_brick *	brick_rebind(struct Scene *sc,void *ptr);
struct Node 		*brick_make(t_block *block,const char *name,t_brick_type brick_type,enum Data_Type data_type,void *data_target);

// BRICK_CLS

void 		brick_cls_init(t_brick *brick);
void 		plug_cls_init(struct Plug *plug);
void 		cls_plug_update(struct Plug *plug);

// BRICK_DRAW

void		brick_draw(t_brick *brick);
void		cls_brick_build_width(t_brick *brick);
void 		cls_brick_build_txt(t_brick *brick);

// BRICK_ACT



// OP_ADD_OBJECT

void *		op_add_set(t_brick *brick);
void *		op_new_set(const char *name);
void *		op_new_camera(const char *name);
struct Node *	op_new_cube(const char *name);
struct Node *	op_add_new_wire_cube(struct Node *node);
void 		add_mn(void);
void *		op_add_mn(t_brick *brick);
void *		op_add_camera(t_brick *brick);
void *	 	op_add_camera_main(void);
void *		op_add_default(t_brick *brick);
void *		op_add_light(t_brick *brick);
void *		op_add_cube(t_brick *brick);
void *		op_add_wire_cube(t_brick *brick);
void *		op_add_uv_cube(t_brick *brick);
void *		op_add_uv_plane(t_brick *brick);
void *		op_add_triangle(t_brick *brick);
void *		op_add_quad(t_brick *brick);


void *		op_camera_rotate_xy(t_brick *brick);
void *		op_camera_rotate_z(t_brick *brick);



void *		op_brick_add(t_brick *brick);
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
void *		op_const(t_brick *brick);
void *		op_mod(t_brick *brick);
void *		op_float(t_brick *brick);
void *		op_int(t_brick *brick);
void *		op_stack(t_brick *brick);

void 		cls_brick_trigger_connections(t_brick *brick);

void 		brick_set_updated(t_brick *brick);

// BRICK_UPDATE

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

struct Plug *	brick_get_src(t_brick *brick);

int 		brick_get_width(t_brick *brick);

#endif
