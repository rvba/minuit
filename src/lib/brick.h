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

#ifdef __cplusplus
extern "C" {
#endif

#define BRICK_SWAP( brick, st) ((brick->state) = &(st)) 

struct Block;
struct Brick;
struct Dict;

enum Data_Type;

struct Brick_State;
struct Brick_Geometry;
struct Brick_Var;
struct Action;
struct Event;

// BRICK TYPE

typedef enum Brick_Type
{
	bt_trigger,
	bt_slider,
	bt_menu,
	bt_selector,
	bt_switch,
	bt_bar,

}t_brick_type;

// BRICK CLASS

typedef struct BrickClass
{
	char cls_type[_NAME_];
	enum Brick_Type type;
	void (* make)(struct Brick *brick);
	void (* draw)(struct Brick *brick);
	void (* init)(struct Brick *brick);
	void (* connect)(struct Brick *self,struct Brick *target);
	void (* disconnect)(struct Brick *self);
	void (* dispatch)(struct Brick *brick);

}t_brick_class;

// BRICK STATE

typedef struct Brick_State
{
	// ON

	int draw_outline:1;
	int draw_name:1;
	int use_min_width:1;
	int use_block_width:1;
	int draw:1;

	// OFF

	int draw_plugs:1;
	int draw_value:1;
	int show_menu:1;
	int is_current:1;
	int is_mouse_over:1;		
	int is_mouse_over_plug_out:1;		
	int is_mouse_over_plug_in:1;		
	int always_trigger:1;
	int is_contextual:1;
	int poll:1;
	int is_versatil:1;
	int has_ref:1;
	int use_loops:1;
	int remove_connected:1;
	int is_root:1;
	int debug:1;
	int has_components:1;
	int connecting:1;
	int clone;
	int frame_loop;

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
	int offset;

}t_brick_var;


// BRICK

typedef struct Brick
{
	t_id id;
	t_brick_class *cls;			// cls

	enum Brick_Type type;
	t_data_type context;			// contextual menus
	int state_pressed;

	int typing;

	int idcol_right[3];			// col
	int idcol_left[3];

	int rhizome_order;			// Graph 
	int counter;				// For Loop
	int block_order;			// Internal Block Id

	struct Brick_State brick_state;			// multiple states
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
	void 	*(* exe)(struct Brick *brick);	
	void 	*(* act)(struct Brick *brick);	
	int 	(* poll)(struct Brick *brick);	

	void (* state)( struct Brick *brick, struct Event *e);

	// data
	void *data;

}t_brick;


void 		brick_rhizome_split(t_brick *brick_x, t_brick *brick_y);
void 		brick_rhizome_add(t_brick *brick_x, t_brick *brick_y);
void 		brick_remove(struct Action *action);

int 		brick_is_different(struct Brick *dst, struct Brick *src);
void 		brick_copy_data(struct Brick *dst, struct Brick *src);
void 		brick_binding_add(struct Brick *brick, enum Data_Type type, void *data);
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
void*		brick_new(const char *name);
void *		brick_get_ref(struct Brick *brick, const char *ref);
void 		brick_rebind(struct Scene *sc,void *ptr);
struct Node 	*brick_make(struct Block *block,const char *name,t_brick_type brick_type,enum Data_Type data_type,void *data_target);

// BRICK_CLS

void 		brick_cls_init(struct Brick *brick);
void 		plug_cls_init(struct Plug *plug);
void 		cls_plug_update(struct Plug *plug);

// BRICK_DRAW

void		brick_draw(struct Brick *brick);
void 		brick_draw_bar(t_brick *brick);
void		cls_brick_build_width(struct Brick *brick);
void 		cls_brick_build_txt(struct Brick *brick);


void 		cls_brick_trigger_connections(struct Brick *brick);

void 		brick_set_updated(struct Brick *brick);

// BRICK_UPDATE

int brick_check_viewport( t_brick *brick);

void 		cls_brick_trigger_selector(struct Brick *brick);
void 		cls_brick_trigger_slider(struct Brick *brick);
void 		cls_brick_trigger_switch(struct Brick *brick);
void 		cls_brick_trigger_menu(struct Brick *brick);
void 		cls_brick_trigger_trigger(struct Brick *brick);

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

void *_op_brick_add( struct Brick *brick);

void state_brick_default( t_brick *brick, struct Event *e);
void cls_brick_dispatch( t_brick *brick);

void state_brick_switch_default( t_brick *brick, struct Event *e);
void state_brick_slider_default( t_brick *brick, struct Event *e);;

#ifdef __cplusplus
}
#endif

#endif
