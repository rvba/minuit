/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#ifndef __PLUG_H
#define __PLUG_H

#include "data.h"

struct Brick;
struct Lst;

enum Type_Operator;

typedef struct Plug t_plug;
typedef struct Plug_State t_plug_state;
typedef struct Plug_Class t_plug_class;

typedef enum Type_Operator
{
	ot_null,
	ot_for,
	ot_get,
	ot_const,

}t_operator;

typedef enum Plug_Mode
{
	mode_in,
	mode_out

}t_plug_mode;


struct Plug_Class
{
	char cls_type[_NAME_];
	t_data_type type;
	void (* make)(t_plug *plug);
	void (* flow)(t_plug *plug);
	void (* connect)(t_plug_mode mode, t_plug *self, t_plug *dst);
	void (* disconnect)(t_plug_mode mode, t_plug *plug);
};

struct Plug_State
{
	int is_connected:1;
	int is_updated:1;
	int is_init:1;
	int is_versatil:1;		// change plug type if connected
	int store_data:1;		// for rebind
	int is_eval:1;
	int is_volatil:1;		// stored data is volatil
	int is_state_volatil:1;		// plug ports can change state when connected
	int is_a_loop:1;
	int close_flow_in:1;		// will close target's flow in
	int use_flow:1;
	int is_parent:1;
	int swap_flow:1;

	int flow_in:1;
	int flow_out:1;
	int follow_in:1;
	int follow_out:1;
	int open_in:1;
	int open_out:1;

	int bang:1;
	int last_bang:1;
};

// PLUG

struct Plug
{
	t_id id;
	t_plug_class *cls;

	int idcol[3];

	t_data_type data_type;
	//t_operator operator_type;
	enum Type_Operator operator_type;
	t_plug_state state;

	struct Brick *brick;			

	int pos; 			// height (for drawing line)

	t_plug *src;
	t_plug *dst;
	t_plug *child;

	struct Lst *parents;
	struct Lst *bindings;

	void *data;
	void *data_memory;
};

void 		plug_get_data(t_plug *plug);

void 		plug_data_reset(t_plug *plug);
void 		plug_data_negate(t_plug *plug);
void 		plug_data_set(t_plug *plug,t_data_type type,void *data);

void 		plug_debug(t_plug *plug);

void 		plug_add_parent(t_plug *plug,t_plug *parent);
void 		plug_remove_parent(t_plug *plug);

t_plug *	plug_get_dst(t_plug *plug);
t_plug *	plug_get_src(t_plug *plug);

#endif
