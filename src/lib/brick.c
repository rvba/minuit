/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "node.h"
#include "util.h"
#include "context.h"
#include "scene.h"
#include "op.h"
#include "ctx.h"
#include "data.h"
#include "list.h"
#include "block.h"
#include "brick.h"
#include "binding.h"
#include "rhizome.h"
#include "memory.h"

// Split
void brick_rhizome_split(t_brick *brick_x, t_brick *brick_y)
{
	t_block *block_x = brick_x->block;
	t_block *block_y = brick_y->block;

	block_x->block_state.is_root = 0;
	block_y->block_state.is_root = 0;

	rhizome_graph_split(block_x->rhizome, brick_x, brick_y);
}

// Add Block To Rhizome
void brick_rhizome_add(t_brick *brick_x, t_brick *brick_y)
{
	t_context *C = ctx_get();

	t_block *block_x = brick_x->block;
	t_block *block_y = brick_y->block;

	// Each Block Have A Rhizome
	if(block_x->rhizome && block_y->rhizome)
	{
		// Add Dash
		rhizome_graph_link_add(block_x->rhizome, brick_x, brick_y);

		// Merge
		rhizome_merge(block_x->rhizome, block_y->rhizome);
	}
	// Only One Rhizome
	else if(block_x->rhizome)
	{
		// Add Block
		rhizome_block_add(block_x->rhizome, block_y);

		// Add Dash
		rhizome_graph_link_add(block_x->rhizome, brick_x, brick_y);
	}
	// Only One Rhizome
	else if(block_y->rhizome)
	{
		// Add Block
		rhizome_block_add(block_y->rhizome, block_x);

		// Add Dash
		rhizome_graph_link_add(block_y->rhizome, brick_x, brick_y);
	}
	// No Rhizome
	else
	{
		// Build New Rhizome
		scene_store(C->scene,1);
		t_rhizome *rhizome = rhizome_add("rhizome");
		scene_store(C->scene,0);

		// Add Blocks
		rhizome_block_add(rhizome, block_x);
		rhizome_block_add(rhizome, block_y);

		// Add Dash
		rhizome_graph_link_add(block_y->rhizome, brick_x, brick_y);
	}

	// Setup
	rhizome_setup(block_x->rhizome);
}

int brick_is_different(t_brick *dst, t_brick *src)
{
	if(dst->plug_intern.data_type != src->plug_intern.data_type)
		return 1;
	else
		return 0;
}

void brick_copy_data(t_brick *dst, t_brick *src)
{
	if(brick_is_different(dst,src))
		brick_type_change(dst,&src->plug_intern);

	dst->plug_intern.data = src->plug_intern.data;
}

void brick_binding_add(t_brick *brick, t_data_type type, void *data)
{
	t_context *C = ctx_get();
	t_plug *plug_intern = &brick->plug_intern;
	if(!plug_intern->bindings)
	{
		t_node *node_list = scene_add(C->scene,dt_list,"binding");
		t_lst *list = node_list->data;
		plug_intern->bindings = list;
	}

	t_node *node_binding = scene_add(C->scene,dt_binding,"binding");
	t_binding *binding = node_binding->data;

	binding->type = type;
	binding->data = data;

	list_add_data(plug_intern->bindings,binding);
}

// CLONE

void brick_clone_change_name(t_brick *brick)
{
	int n=0;
	char s[10];
	char *name = brick->txt_name.name;
	//char *name = brick->name;
	char old_name[_NAME_LONG_];

	strncpy(old_name,name,_NAME_LONG_);

	s[0] = '[';
	s[1] = '\0';

	n = s_append(name,s,n);
	n = s_append(name,old_name,n);

	s[0] = ' ';
	s[1] = '\0';

	n = s_append(name,s,n);

	sprintf(s,"%d",brick->brick_state.clone);

	n = s_append(name,s,n);

	s[0] = ']';
	s[1] ='\0';

	s_append(name,s,n);
}

t_brick *brick_dupli(t_block *block,t_brick *brick)
{
	t_context *C=ctx_get();

	scene_store(C->scene,1);

	t_plug *plug_intern=&brick->plug_intern;

	t_node *clone_node=brick_make(block,brick->id.name,brick->type,plug_intern->data_type,plug_intern->data);

	t_brick *clone_brick=clone_node->data;

	clone_brick->exe=brick->exe;
	clone_brick->act=brick->act;
	clone_brick->brick_state.draw_name=brick->brick_state.draw_name;
	clone_brick->brick_state.draw_value=brick->brick_state.draw_value;
	clone_brick->brick_state.draw_outline=brick->brick_state.draw_outline;

	scene_store(C->scene,0);

	if(brick->brick_state.clone)
	{
		clone_brick->brick_state.clone = brick->brick_state.clone;
		brick_clone_change_name(clone_brick);
	}
	else
	{
		brick->brick_state.clone = brick->id.id;
		clone_brick->brick_state.clone = brick->id.id;
		brick_clone_change_name(brick);
		brick_clone_change_name(clone_brick);
		brick_build_width(brick);
	}

	return clone_node->data;
}

t_brick *brick_copy(t_block *block,t_brick *brick)
{
	t_context *C=ctx_get();

	scene_store(C->scene,1);

	t_plug *plug_intern=&brick->plug_intern;

	t_data_type data_type = plug_intern->data_type;

	void *data = plug_intern->data;
	void *data_new = data_copy(data_type,data);

	t_node *clone_node=brick_make(block,brick->id.name,brick->type,plug_intern->data_type,data_new);

	t_brick *clone_brick=clone_node->data;

	clone_brick->exe=brick->exe;
	clone_brick->act=brick->act;
	clone_brick->brick_state.draw_name=brick->brick_state.draw_name;
	clone_brick->brick_state.draw_value=brick->brick_state.draw_value;
	clone_brick->brick_state.draw_outline=brick->brick_state.draw_outline;

	scene_store(C->scene,0);

	return clone_node->data;
}

// FREE

void brick_selector_list_free(t_lst *lst)
{
	t_link *l=lst->first;
	char *txt;
	if(l)
	{
		for(;l;l=l->next)
		{
			txt=l->data;
			mem_free( txt, sizeof( t_txt));
		}
	}
	lst_free(lst);
}

// BRICK DELETE

int brick_delete(t_brick *brick,int remove_connected)
{
	t_context *C=ctx_get();

	t_plug *plug_in=&brick->plug_in;
	t_plug *plug_out=&brick->plug_out;

	if(
		(remove_connected && !plug_out->state.is_connected)
		||
		(!plug_in->state.is_connected && !plug_out->state.is_connected)
		)
	{
		scene_delete(C->scene,brick);
		return 1;
	}
	else
	{
		return 0;
	}
}

// CHANGE TYPE

void brick_type_change(t_brick *brick,t_plug *plug)
{
	t_context *C=ctx_get();

	// scene store
	scene_store(C->scene,1);

	t_plug *plug_in=&brick->plug_in;
	t_plug *plug_intern=&brick->plug_intern;
	t_plug *plug_out=&brick->plug_out;

	t_data_type type=plug->data_type;

	// plug init
	plug_in->data_type=type;
	plug_intern->data_type=type;
	plug_out->data_type=type;

	plug_cls_init(plug_in);
	plug_cls_init(plug_intern);
	plug_cls_init(plug_out);

	// build data
	plug_intern->data=data_add(plug_intern->data_type,plug->data);
	plug_intern->data_memory = plug_intern->data;

	// data init
	data_init(plug_intern->data_type, plug_intern->data, plug->data);

	// brick init
	brick_cls_init(brick);

	scene_store(C->scene,0);
}

void brick_change_type_by_name(t_brick *brick,t_data_type type)
{
	t_context *C=ctx_get();

	t_plug *plug_in=&brick->plug_in;
	t_plug *plug_intern=&brick->plug_intern;
	t_plug *plug_out=&brick->plug_out;

	if(plug_intern->data_type!=type)
	{
		scene_store(C->scene,1);

		// plug init

		plug_in->data_type=type;
		plug_intern->data_type=type;
		plug_out->data_type=type;

		plug_cls_init(plug_in);
		plug_cls_init(plug_intern);
		plug_cls_init(plug_out);

		// build data
		plug_intern->data=data_add(plug_intern->data_type,NULL);
		plug_intern->data_memory = plug_intern->data;

		// data init
		data_init(plug_intern->data_type,plug_intern->data, NULL);

		brick_cls_init(brick);

		scene_store(C->scene,0);
	}
}

void plug_color_init(t_plug *plug)
{
	t_context *C=ctx_get();
	int color[3];
	scene_color_get(C->scene,color);
	vseti(plug->idcol,color[0],color[1],color[2]);
}

// PLUG INIT

void plug_init(
			t_plug *plug,
			t_data_type type,
			t_brick *brick,
			void *data_target,
			int duplicate
			)
{
	plug->brick=brick;
	plug->data_type=type;

	if(data_target)
	{
		plug->data=data_target;
		plug->data_memory = plug->data;
	}
	else
	{
		if(duplicate)
		{
			plug->data=data_add(type,data_target);
			plug->data_memory = plug->data;
		}
		else
		{
			plug->data=NULL;
			plug->data_memory = NULL;
		}
	}
}

// PLUG_RESET

void plug_reset(t_plug *plug,const char *name)
{
	id_init(&plug->id, name);

	plug->src=NULL;
	plug->dst=NULL;
	plug->pos=0; 
	plug->data=NULL;
	plug->data_memory = NULL;
	plug->brick=NULL;
	plug->data_type=dt_null;
	plug->operator_type = ot_null;
	vseti(plug->idcol,0,0,0);
	plug->bindings = NULL;

	plug->state.is_connected=0;
	plug->state.is_updated=0;
	plug->state.is_versatil=0;
	plug->state.store_data = 1;
	plug->state.store_data_memory = 1;
	plug->state.bang = 0;
	plug->state.last_bang = 0;
	plug->state.is_init=0;
	plug->state.is_state_volatil = 1;
	plug->state.is_a_loop = 0;
	plug->state.close_flow_in = 0;
	plug->state.use_flow = 1;
	plug->state.flow_in = 1;
	plug->state.flow_out = 0;
	plug->state.follow_in=1;
	plug->state.follow_out=1;
	plug->state.open_in = 0;
	plug->state.open_out = 1;
	plug->state.is_eval = 0;
	plug->state.is_volatil = 0;
	plug->state.bang = 0;
	plug->state.is_init = 0;

	plug->state.swap_flow = 0;
}

// REF

void *brick_get_ref(t_brick *brick, const char *ref)
{
	void *p;

	if(is(ref,"in"))  p=&brick->plug_in; 
	else if(is(ref,"intern"))  p=&brick->plug_intern; 
	else if(is(ref,"out"))  p=&brick->plug_out; 
	else
	{
		printf("[ERROR berick_get_ref] Unknown ref %s \n",ref);
		return NULL;
	}

	return p;
}

// REBIND

t_brick *brick_rebind(t_scene *sc,void *ptr)
{
	t_brick *brick=(t_brick *)ptr;

	rebind(sc,"brick","menu",(void **)&brick->menu);
	rebind(sc,"brick","block",(void **)&brick->block);
	rebind(sc,"brick","plug_in_src",(void **)&brick->plug_in.src);

	if(brick->plug_intern.state.store_data) 
		rebind(sc,"brick",brick->id.name,(void **)&brick->plug_intern.data);
	else 	
		brick->plug_intern.data = NULL;

	if(brick->plug_intern.state.store_data_memory)
	{
	if(brick->plug_intern.data_memory) 
	{
		rebind(sc,"brick","plug_intern_data_memory",(void **)&brick->plug_intern.data_memory);
		if(brick->plug_intern.data == NULL)
			brick->plug_intern.data = brick->plug_intern.data_memory;
	}
	}


	rebind(sc,"brick","plug_out_dst",(void **)&brick->plug_out.dst);
	rebind(sc,"brick exe",brick->id.name,(void **)&brick->exe);
	rebind(sc,"brick act",brick->id.name,(void **)&brick->act);

	rebind(sc,"brick","plug binding",(void **)&brick->plug_intern.bindings);

	txt_init(&brick->txt_name,brick->id.name);
	txt_init(&brick->txt_data,NULL);

	if(brick->brick_state.clone)
	{
		brick_clone_change_name(brick);
		brick_build_width(brick);
	}

	return brick;
}

// INIT

void brick_init(t_scene *sc,t_brick *brick)
{
	int col[3];

	// brick color
	scene_color_get(sc,col);
	vseti(brick->idcol_right,col[0],col[1],col[2]);
	scene_color_get(sc,col);
	vseti(brick->idcol_left,col[0],col[1],col[2]);


	// plug cls
	plug_cls_init(&brick->plug_in);
	plug_cls_init(&brick->plug_intern);
	plug_cls_init(&brick->plug_out);

	plug_color_init(&brick->plug_in);
	plug_color_init(&brick->plug_intern);
	plug_color_init(&brick->plug_out);

	t_plug *plug_in=&brick->plug_in;
	t_plug *plug_intern=&brick->plug_intern;
	t_plug *plug_out=&brick->plug_out;

	plug_in->brick = brick;
	plug_intern->brick = brick;
	plug_out->brick = brick;

	// link plugs
	plug_in->dst = plug_intern;
	plug_intern->src = plug_in;
	plug_intern->dst = plug_out;
	plug_out->src = plug_intern;

	// State
	brick->state = state_brick_default;
}


// MAKE

t_node *brick_make(t_block *block,const char *name,t_brick_type brick_type,t_data_type data_type,void *data_target)
{
	t_context *C=ctx_get();

	// NEW BRICK
	t_node *node_brick = scene_add(C->scene,dt_brick,name);
	t_brick *brick = node_brick->data;

	// TYPE
	brick->type=brick_type;

	// brick cls
	brick_cls_init(brick);

	// ADD TO BLOCK 
	block_brick_add(block,node_brick);

	// DEFAULT ACTION
	brick->exe=op_void_exe;
	brick->act=op_void_act;

	// PLUG INIT 
	plug_init(&brick->plug_intern,data_type,brick,data_target,0);
	plug_init(&brick->plug_in,data_type,brick,NULL,0);
	plug_init(&brick->plug_out,data_type,brick,NULL,0);

	// ADD PLUG REFs
	if(C->scene->store)
	{
		scene_add_ref(C->scene,"struct_ref","plug","in",&brick->plug_in,brick);
		scene_add_ref(C->scene,"struct_ref","plug","out",&brick->plug_out,brick);
		scene_add_ref(C->scene,"struct_ref","plug","intern",&brick->plug_intern,brick);
		brick->brick_state.has_ref=1;
	}

	// BRICK INIT
	//brick_init(C->scene,brick);
	brick->cls->init( brick);

	// Frame Based
	if(is(name,"frame"))
	{
		block->block_state.frame_based  =1;
	}

	return node_brick;
}

// CLONE

t_brick *brick_clone(t_brick *brick)
{
	if(brick)
	{
		t_brick *clone = (t_brick *) mem_malloc(sizeof(t_brick));
		memcpy(clone,brick,sizeof(t_brick));
		return clone;
	}
	else
	{
		return NULL;
	}
}

void _brick_free(t_brick *brick)
{
	// that simple !
	mem_free(brick, sizeof( t_brick));
}

// FREE

void brick_free(t_brick *brick)
{
	t_context *C=ctx_get();
	t_scene *sc=C->scene;

	t_plug *plug_intern = &brick->plug_intern;

	if(plug_intern->bindings)
	{
		scene_delete(C->scene, plug_intern->bindings);
	}

	if(brick->brick_state.has_ref)
	{
		scene_remove_ref(sc,&brick->plug_in);
		scene_remove_ref(sc,&brick->plug_out);
		scene_remove_ref(sc,&brick->plug_intern);
	}
}

// NEW

t_brick *brick_new(const char *name)
{
	t_brick *brick = (t_brick *)mem_malloc(sizeof(t_brick));

	id_init(&brick->id, name);

	txt_init(&brick->txt_name,name);
	txt_init(&brick->txt_data,NULL);

	brick->context=dt_null;

	bzero(&brick->brick_state,sizeof(t_brick_state));

	brick->brick_state.is_idle=1;
	brick->brick_state.is_released=1;
	brick->brick_state.draw_outline=1;
	brick->brick_state.draw_plugs=1;
	brick->brick_state.draw_name=1;
	brick->brick_state.is_clicable=1;
	brick->brick_state.use_min_width=1;
	brick->brick_state.use_block_width=1;
	brick->brick_state.is_mouse_mode=1;
	brick->brick_state.is_contextual=0;
	brick->brick_state.poll=0;
	brick->brick_state.is_versatil=0;
	brick->brick_state.has_ref=0;
	brick->brick_state.is_current=0;
	brick->brick_state.always_trigger=0;
	brick->brick_state.use_dragging = 1;
	brick->brick_state.use_loops = 1;
	brick->brick_state.frame_loop = 0;
	brick->brick_state.remove_connected = 0;
	brick->brick_state.clone = 0;
	brick->brick_state.draw_value = 1;
	brick->brick_state.is_root = 0;
	brick->brick_state.debug = 0;
	brick->brick_state.draw = 1;
	brick->brick_state.has_components = 0;

	brick->geom.block_pos=0;
	brick->geom.height=20;
	brick->geom.margin=10;
	brick->geom.min_width=BRICK_MIN_WIDTH;

	brick->var.increment=0;
	brick->var.selector=0;
	brick->var.selector_length=0;

	brick->cls=NULL;
	brick->exe=NULL;
	brick->act=NULL;
	brick->poll = NULL;
	brick->menu=NULL;

	brick->rhizome_order = -1;
	brick->block_order = -1;

	//brick->mode=bm_idle;

	plug_reset(&brick->plug_in,name);
	plug_reset(&brick->plug_intern,name);
	plug_reset(&brick->plug_out,name);

	bzero(&brick->var.selector_list,_LIST_*_NAME_LONG_);

	brick->counter = 0;
	brick->state = NULL;
	//brick->state_pressed = BRICK_NULL;
	brick->typing = 0;

	return brick;
}

