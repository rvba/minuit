/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "node.h"
#include "scene.h"

#include "op.h"
#include "ctx.h"
#include "event.h"
#include "term.h"
#include "camera.h"
#include "object.h"
#include "list.h"
#include "vector.h"
#include "data.h"
#include "ui.h"

#include "block.h"
#include "brick.h"

#include "mesh.h"
#include "geometry.h"
#include "vlst.h"

int PLUG_DEBUG;

void cls_plug_make_float(t_plug *plug);
void cls_plug_make_int(t_plug *plug);
void cls_plug_make_uint(t_plug *plug);
void cls_plug_make_string(t_plug *plug);
void cls_plug_make_pointer(t_plug *plug);
void cls_plug_make_mesh(t_plug *plug);
void cls_plug_make_vertex(t_plug *plug);
void cls_plug_make_face(t_plug *plug);
void cls_plug_make_vlst(t_plug *plug);
void cls_plug_make_lst(t_plug *plug);
void cls_plug_make_camera(t_plug *plug);
void cls_plug_make_char(t_plug *plug);
void cls_plug_make_object(t_plug *plug);
void cls_plug_make_selector(t_plug *plug);
void cls_plug_make_trigger(t_plug *plug);
void cls_plug_make_operator(t_plug *plug);
void cls_plug_make_vector(t_plug *plug);
void cls_plug_make_geo(t_plug *plug);
void cls_plug_make_geo_point(t_plug *plug);
void cls_plug_make_geo_edge(t_plug *plug);
void cls_plug_make_geo_array(t_plug *plug);

void cls_plug_connect_general(t_plug_mode mode, t_plug *self, t_plug *dst);
void cls_plug_disconnect_general(t_plug_mode mode, t_plug *self);


void close_vector(t_brick *brick, int open);


void object_show(t_object *object)
{
	term_log("object");
}

void mesh_show(t_mesh *mesh)
{

	//term_log("mesh");
	if(mesh->vertex)
	{
		vlst_show(mesh->vertex);
	}
	else
	{
		printf("MESH no vlst\n");
	}
}


void plug_debug(t_plug *plug)
{
	t_data_type type = plug->data_type;
	void *data = plug->data;

	switch(type)
	{
		case dt_int: term_log("%d", drf_int(data)); break;
		case dt_float: term_log("%f", drf_float(data)); break;
		case dt_vector: vector_show(data); break;
		case dt_vlst: vlst_show(data); break;
		case dt_object: object_show(data);break;
		case dt_mesh: mesh_show(data);break;
		default:
			term_log("unknown type %s", data_name_get(type));
			break;
	}
}


void plug_data_reset(t_plug *plug)
{
	int *zero;
	zero=0;

	switch (plug->data_type)
	{
		case (dt_int): flow_int_int(plug->data,zero); break;
		case (dt_float): flow_float_int(plug->data,zero); break;
		default: break;
	}
}

void plug_data_set(t_plug *plug,t_data_type type,void *data)
{
	switch (plug->data_type)
	{
		case (dt_int):

			switch(type)
			{
				case(dt_int): flow_int_int(plug->data,data); break;
				case(dt_float): flow_int_float(plug->data,data); break;
				default: break;
			}

			break;

		case (dt_float):

			switch(type)
			{
				case(dt_int): flow_float_int(plug->data,data); break;
				case(dt_float): flow_float_float(plug->data,data); break;
				default: break;
			}

			break;

		default:
			break;
	}
}

void plug_data_negate(t_plug *plug)
{
	switch (plug->data_type)
	{
		case (dt_int): negate_int(plug->data); break;
		case (dt_float): negate_float(plug->data); break;
		default: break;
	}
}

// input: 	plug_intern 
// return: 	plug_intern
t_plug *plug_get_src(t_plug *plug)
{
	// get plug_in
	t_plug *plug_src=plug->src;
	// get plug_out
	t_plug *plug_src_out=plug_src->src;

	if(plug_src_out)
	{
		//get plug_intern of out
		t_plug *plug_source=plug_src_out->src;

		// return target's plug intern
		return plug_source;
	}
	else
	{
		return NULL;
	}
}

t_plug *plug_get_dst(t_plug *plug)
{
	// get plug_out
	t_plug *plug_dst = plug->dst;
	// get dst
	t_plug *plug_dst_out = plug_dst->dst;

	if(plug_dst_out)
	{
		// get plug_intern
		t_plug *plug_distant = plug_dst_out->dst;

		return plug_distant;
	}
	else
	{
		return NULL;
	}
}

void plug_warning(t_plug *dst_plug,t_plug *src_plug)
{
	t_context *C=ctx_get();
	char msg[40];
	char *src_plug_type=data_name_get(src_plug->data_type);
	char *dst_plug_type=data_name_get(dst_plug->data_type);

	sprintf(msg,"(%s)(%s)-(%s)(%s)",src_plug->id.name,src_plug_type,dst_plug->id.name,dst_plug_type);
	term_print(C->term,msg);
}

void plug_debug_connect(const char *name, t_plug *dst_plug, t_plug *src_plug)
{
	if(PLUG_DEBUG)
	{
		t_context *C=ctx_get();
		char msg[40];
		char *src_plug_type=data_name_get(src_plug->data_type);
		char *dst_plug_type=data_name_get(dst_plug->data_type);

		sprintf(msg,"%s %s-%s",name,dst_plug_type,src_plug_type);
		term_print(C->term,msg);
	}
}

// FLOW

void _cls_flow_(t_plug *plug,void (* f)(t_plug_mode mode,t_plug *p1,t_plug *p2))
{
	t_brick *brick=plug->brick;

	t_plug *plug_in=&brick->plug_in;
	t_plug *plug_out=&brick->plug_out;

	// IN

	if(plug_in->state.is_connected)
	{
		t_plug *plug_origin = plug_in->src;

		if(plug_origin->state.open_out)
		{
			t_plug *src_plug=plug_get_src(plug);

			if(plug_in->state.flow_in)
			{
				f(mode_in,plug,src_plug);
			}
		}
	}
	else
	{
		f(mode_in,plug,NULL);
	}

	// OUT

	if(plug_out->state.is_connected)
	{
		t_plug *plug_origin = plug_out->dst;

		if(plug_origin->state.open_in)
		{
			t_plug *src_plug=plug_get_dst(plug);

			if(plug_out->state.flow_out)
			{
				f(mode_out,plug,src_plug);
			}
		}
	}
	else
	{
		f(mode_out,plug,NULL);
	}
}


// DISCONNECT FOR

void cls_plug_disconnect_operator(t_plug_mode mode, t_plug *plug)
{
	t_brick *brick = plug->brick;
	t_block *block = brick->block;

	// General
	cls_plug_disconnect_general(mode,plug);

	// Operator For 
	if(brick->plug_intern.operator_type == ot_for)
	{
		t_brick *brick_vector = block_brick_get(block,"vector");
		t_plug *plug_vector = &brick_vector->plug_intern;
		t_plug *plug_in_vector = &brick_vector->plug_in;
		t_vector *vector = plug_vector->data;
		vector->pointer = NULL;

		if(plug_in_vector->state.is_connected)
		{
			t_plug *plug_dst = plug_in_vector->src;
			if(plug_dst->data_type == dt_vector)
			{
				t_brick *brick_dst = plug_dst->brick;
				t_plug *plug_dst_vector = &brick_dst->plug_intern;
				t_vector *vector = plug_dst_vector->data;
				t_block *block_vector = brick_dst->block;
				t_vlst *vlst_dst = vector->vector;

				if(brick_dst->state.has_components)
				{
					// For Vector Length
					int i;
					t_brick *brick_component;
					t_plug *plug_intern_component;
					t_plug *plug_in_component;
					for(i=0; i < vector->length; i++)
					{
						// Get Brick Component
						//brick_component = block_brick_get_by_order(block_vector,i);
						brick_component = block_brick_get_by_position(block_vector,i);

						// Get Plugs
						plug_intern_component = &brick_component->plug_intern;
						plug_in_component = &brick_component->plug_in;

						// Set Component Pointer
						plug_intern_component->data = vlst_get_pointer(vlst_dst,i);

						// Don't Store Pointer 
						plug_intern_component->state.store_data  = 0;

						// Open Flow In
						plug_in_component->state.flow_in = 0;
					}
				}
			}
		}
	}
}

// CONNECT GENERAL

void cls_plug_connect_general(t_plug_mode mode, t_plug *self, t_plug *dst)
{
	t_brick *brick = self->brick;
	t_plug *plug_in = &brick->plug_in;
	t_plug *plug_out = &brick->plug_out;

	t_brick *brick_dst = dst->brick;
	t_plug *plug_dst_in = &brick_dst->plug_in;
	t_plug *plug_dst_out = &brick_dst->plug_out;


	// Mode In
	if(mode == mode_in)
	{

		plug_debug_connect("connect",dst,self);

		// Connect Plugs
		plug_in->src = plug_dst_out;
		plug_in->state.is_connected = 1;

		// Add To Rhizome
		brick_rhizome_add(brick, brick_dst);

		// If Versatil
		if(brick->state.is_versatil)
		{
			if(self->data_type != dst->data_type)
			{
				// Change Type
				brick_type_change(brick,dst);
			}
		}
	}
	// Mode Out
	else
	{
		// Connect Plugs
		plug_out->dst = plug_dst_in;
		plug_out->state.is_connected = 1;
	}

 	// Swap Flow
 	if(self->state.swap_flow && dst)
 	{
		if(mode == mode_out)
		{
			plug_out->state.flow_out = 1;
			plug_dst_in->state.flow_in = 0;
			plug_dst_in->state.open_in = 1;

			// If Versatil
			if(brick->state.is_versatil)
			{
				if(self->data_type != dst->data_type)
				{
					// Change Type
					brick_type_change(brick,dst);
				}
			}
		}
 	}
}

// DISCONNECT GENERAL

void cls_plug_disconnect_general(t_plug_mode mode, t_plug *self)
{
	t_brick *brick = self->brick;
	t_plug *plug_in = &brick->plug_in;
	t_plug *plug_out = &brick->plug_out;
	t_plug *plug_src = plug_in->src;
	t_plug *dst = NULL;
	t_brick *brick_src;
	if(plug_src)
	{
		brick_src = plug_src->brick;
		dst = &brick_src->plug_intern;
	}

 	// Swap Flow
 	if(self->state.swap_flow && (mode==mode_out))
 	{
		t_plug *dst = plug_out->dst;
		t_brick *brick_dst = dst->brick;
		t_plug *plug_dst_in = &brick_dst->plug_in;

		plug_out->state.flow_out = 0;
		plug_dst_in->state.flow_in = 1;
		plug_dst_in->state.open_in = 0;
 	}

	// Mode In
	if(mode == mode_in)
	{
		plug_debug_connect("disconnect",dst,self);

		// Disconnect
		plug_in->src = NULL;
		plug_in->state.is_connected = 0;
	}
	// Mode Out
	else
	{
		// Disconnect
		plug_out->dst = NULL;
		plug_out->state.is_connected = 0;
	}


}

// CONNECT VECTOR

void cls_plug_connect_vector(t_plug_mode mode, t_plug *self, t_plug *dst)
{
	t_context *C = ctx_get();

	t_brick *brick = self->brick;
	t_block *block = brick->block;
	t_plug *plug_in = &brick->plug_in;
	t_plug *plug_out = &brick->plug_out;

	if(dst->data_type == dt_vector)
	{
		t_vector *vector_src = self->data;
		t_vector *vector_dst = dst->data;

		// If Vectors from same type
		if(vector_src->length == vector_dst->length)
		{
			// Connect General
			cls_plug_connect_general(mode,self,dst);

			// **********************************************
			// Connect To **For Pointer**
			// **********************************************

			// If Dst Is Volatil [OUT] 
			if(dst->state.is_volatil && mode == mode_out)
			{
				// If Is State Volatil (=> For Vector is Not)
				if(self->state.is_state_volatil) 
				{
					// *****************
					// {A} Reverse Flow 
					// ****************
					// Close Vector Flow In 
					plug_in->state.flow_in = 0;
					plug_in->state.open_in = 1;

					// Open Dst Flow In (For Vector)
					plug_out->state.open_out = 0;
					plug_out->state.flow_out = 1;
					plug_out->state.open_in = 1;
				}
					
				// IF Vector Has Components
				if(brick->state.has_components)
				{
					t_brick *brick_component;
					t_plug *plug_in_component;
					t_plug *plug_intern_component;

					// For 4 (xyzw) XX
					int i;
					//for(i = 0; i < 4; i++)
					for(i = 0; i < vector_src->length; i++)
					{
						// Get Component
						//brick_component = block_brick_get_by_order(block, i); 
						brick_component = block_brick_get_by_position(block, i); 
						plug_in_component = &brick_component->plug_in;
						plug_intern_component = &brick_component->plug_intern;

						// **********************
						// {B] Wait for For Loop
						// **********************

						// Close Flow In
						plug_in_component->state.flow_in = 0;
						// Reset Internal Pointer
						plug_intern_component->data = NULL;
						// Don't Store Pointer
						plug_intern_component->state.store_data = 0;
					}
				}
			}

			// ***************************
			// Reverse Parenting
			// ***************************

			// Normal: Vector is Child Of XYZ Components
			// Pointer: XYZ are childs of Vector
			if(
				// If Vector Has Components
				brick->state.has_components
				&&
				(
				// And Dst is For Pointer [OUT] 
					dst->state.is_parent 
				||
				// Or Dst is Vector [IN] 
					(
						(dst->data_type == dt_vector)
						&&
						(mode == mode_in)
					)
				)
			)
			{
				// **************************
				// Remove Vectors' XYZ Parents 
				// *************************
				plug_child_remove_all_parents(self);

				// Store
				scene_store(C->scene,1);

				t_brick *brick_component;
				t_plug *plug_intern_component;
				t_vector *vector_dst = dst->data;
				int i;

				// For All Components 
				for(i = 0; i < 4; i++)
				{
					//brick_component = block_brick_get_by_order(block, i); 
					brick_component = block_brick_get_by_position(block, i); 
					plug_intern_component = &brick_component->plug_intern;

					// If i < Vector Length
					if(i < vector_dst->length)

						// **************************
						// Parent Component To Vector
						// **************************
						plug_add_parent(plug_intern_component,self);

				}

				scene_store(C->scene,0);
			}
		}
	}
}

void cls_plug_disconnect_vector(t_plug_mode mode, t_plug *plug)
{
	t_context *C = ctx_get();

	t_brick *brick = plug->brick;
	t_block *block = brick->block;
	t_plug *plug_in = &brick->plug_in;
	t_plug *plug_out = &brick->plug_out;
	t_vector *vector = plug->data;

	// Disconnect General
	cls_plug_disconnect_general(mode,plug);

	// **************
	// Reverse Flow
	// *************

	// Regular Vector
	if(plug->state.is_state_volatil)
	{
		plug_in->state.flow_in = 1;
		plug_in->state.open_in = 0;
		plug_out->state.open_out = 1;
		plug_out->state.flow_out = 0;
		plug_out->state.open_in = 0;
	}

	// For Vector
	else
	{
		plug_in->state.flow_in = 0;
		plug_in->state.open_in = 1;
	}

	// ******************
	// Reverse Parenting
	// ******************

	if(brick->state.has_components)
	{

		t_brick *brick_component;
		t_plug *plug_intern_component;

		int i;

		// For All Components
		for(i = 0; i < 4; i++)
		{
			//brick_component = block_brick_get_by_order(block, i);
			brick_component = block_brick_get_by_position(block, i);
			plug_intern_component = &brick_component->plug_intern;

			plug_intern_component->data = plug_intern_component->data_memory;
			
			// Show Value
			brick_component->state.draw_value = 1;

			// Remove Parenting
			plug_child_remove_all_parents(plug_intern_component);

			// Store
			scene_store(C->scene,1);

			// If i < Length
			if(i < vector->length)
			{
				// Reverse Parenting
				plug_add_parent(plug, plug_intern_component);
			}

			scene_store(C->scene,0);
		}
	}
}

void close_vector(t_brick *brick, int open)
{
	t_block *block = brick->block;
	t_plug *plug_intern = &brick->plug_intern;

	t_vector *vector = plug_intern->data;

	t_brick *brick_component;
	t_plug *plug_in_component;

	// Open
	if(open)
	{
		if(brick->state.has_components)
		{
			// For Vector Length
			int i;
			for(i=0; i < vector->length; i++)
			{
				// Get Brick Component
				//brick_component = block_brick_get_by_order(block,i);
				brick_component = block_brick_get_by_position(block,i);

				// Get Plugs
				plug_in_component = &brick_component->plug_in;

				// Open Flow In
				plug_in_component->state.flow_in = 1;
			}
		}
	}
	// Close
	else
	{
		if(brick->state.has_components)
		{
			// For Vector Length
			int i;
			for(i=0; i < vector->length; i++)
			{
				// Get Brick Component
				//brick_component = block_brick_get_by_order(block,i);
				brick_component = block_brick_get_by_position(block,i);

				// Get Plugs
				plug_in_component = &brick_component->plug_in;

				// Close Flow In
				plug_in_component->state.flow_in = 0;
			}
		}
	}
}


// FOR

void __cls_plug_flow_operator_for(t_plug_mode mode,t_plug *plug,t_plug *plug_src)
{
	t_context *C =ctx_get();

	t_plug *plug_in = plug->src;
	t_brick *brick = plug->brick;

	// get bricks
	t_block *block = brick->block;
	t_brick *brick_indice = block_brick_get(block,"indice");
	t_brick *brick_vector = block_brick_get(block,"vector");

	t_plug *plug_indice = &brick_indice->plug_intern;
	t_plug *plug_vector = &brick_vector->plug_intern;
	t_plug *plug_vector_in = &brick_vector->plug_in;

	int *data_indice = plug_indice->data;
	t_vector *vector = plug_vector->data;

	// IN
	if(mode == mode_in)
	{
		// reset vector
		vector->pointer = NULL;

		// if for connected
		if(plug_in->state.is_connected)
		{
			// get src
			t_plug *src_plug = plug_get_src(plug);
			t_data_type src_type = src_plug->data_type;

			t_vlst *vlst=NULL;

			switch(src_type)
			{
				// + vlst
				case dt_vlst:

					// set vlst
					vlst = src_plug->data;

					// Set vector
					if(vlst)
					{
						vector->pointer = vlst->data;
						vector->type = vlst->type;
						vector->length = vlst->length;
					}

					break;
					
				default:
					plug_warning(plug,src_plug);
					break;
			}

			// If Vector is Connected
			if(plug_vector_in->state.is_connected)
			{
				// Open Vector
				close_vector(brick_vector,1);

				// And Vlst is Connected
				if(vlst)
				{
					if(brick->counter < vlst->count)
					{
						// Open Vector
						close_vector(brick_vector,1);

						if(C->ui->show_step) term_log("[for][%d]",brick->counter);
						//printf("[for][%d]\n",brick->counter);

						// Set Vector Pointer
						vector->pointer = vlst_get_pointer(vlst, vlst->length * brick->counter);

						// Set Indice
						*data_indice=brick->counter;

						// Star Loop
						block_set_loop_state(block,1);

						// Counter ++
						brick->counter++;
					}
					// Last Loop : counter > vlst
					else
					{
						if(C->ui->show_step)  term_log("[for][%d] END LOOP\n",brick->counter); 
						//printf("[for][end loop %d]\n",brick->counter);

						// Close Vector
						close_vector(brick_vector,0);

						// reset counter
						brick->counter = 0;

						// reset init
						plug->state.is_init = 0;

						// reset indice
						*data_indice = 0;

						// Set Loop State On
						block_set_loop_state(block,2);
					}
				}
				// no vlst
				else
				{
					// Close Vector
					close_vector(brick_vector,0);
					block_set_loop_state(block,3);
				}
			}
			// plug vector in not connected
			else
			{
				// Close Vector
				close_vector(brick_vector,0);
				block_set_loop_state(block,3);
			}
		}
		// IF For not connected
		else
		{
			// Close Vector
			close_vector(brick_vector,0);
			block_set_loop_state(block,3);
		}
	}
}

// GET

void __cls_plug_flow_operator_get(t_plug_mode mode,t_plug *plug,t_plug *plug_src)
{
	t_brick *brick = plug->brick;

	t_block *block = brick->block;

	t_brick *brick_indice = block_brick_get(block,"indice");
	t_brick *brick_result = block_brick_get(block,"result");

	t_plug *plug_result = &brick_result->plug_intern;
	t_plug *plug_indice = &brick_indice->plug_intern;

	t_lst *lst;
	t_vlst *vlst;

	// IN
	if(plug_src && mode == mode_in)
	{
		switch(plug_src->data_type)
		{
			// + LST
			case (dt_lst):
				
				// get lst
				lst = plug_src->data;

				// if lst
				if(lst)
				{
					if(!lst->current)
					{
						// set first
						if(lst->first)
						{
							lst->current = lst->first;
						}
					}
					else
					{
						t_link *link = lst->current;

						if(link->next)
						{
							lst->current = link->next;
						}
						else
						{
							//
						}

						//XXX change type

						plug_result->data_type=dt_camera;
						t_camera *camera=link->data;
						plug_result->data=camera;

					}
				}

				break;

			// +VLST

			case(dt_vlst):
				
				// get vlst
				vlst = plug_src->data;

				if(vlst)
				{
					t_plug _plug;
					t_vector vector_new;
					t_vector *vector;
					vector_new.type = vlst->type;
					vector_new.length = vlst->length;
					_plug.data_type = dt_vector;
					_plug.data = &vector_new;

					// Change Vector Type
					if(brick_result->plug_intern.data_type == dt_vector)
					{
						t_vector *vector_dst = brick_result->plug_intern.data;

						if(vector_is_different(vector_dst, &vector_new))
							brick_type_change(brick_result,&_plug);
					}
					else
					{
						brick_type_change(brick_result,&_plug);
					}

					// Get Indice
					int i = drf_int(plug_indice->data);

					// Limit Indice
					if(i >= vlst->count) set_int(plug_indice->data,vlst->count-1);

					// Set Pointer
					vector = plug_result->data;
					vector->pointer = vlst_get_pointer(vlst, vlst->length * i);
				}

				break;
				

			default:
				//printf(">> %s\n",data_name_get(plug_src->data_type));
				break;
		}
	}
}

// OPERATOR


void cls_plug_flow_operator(t_plug *plug)
{
	switch(plug->operator_type)
	{
		case(ot_null):
			break;
		case(ot_for):
			_cls_flow_(plug,__cls_plug_flow_operator_for);
			break;
		case(ot_get):

			_cls_flow_(plug,__cls_plug_flow_operator_get);
			break;
		default:
			break;
	}
}

// VECTOR

void __cls_plug_flow_vector(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	t_context *C = ctx_get();

	t_brick *brick = plug->brick;
	t_block *block = brick->block;

	t_vector *vector_self = plug->data;
	t_vector *vector_src;

	// If Src or Dst
	if(src_plug)
	{
		t_data_type type = src_plug->data_type;

		switch(type)
		{
			// case VECTOR
			case dt_vector:

				// Get Vector Src
				vector_src = src_plug->data;

				// ************************************************
				// {A} 	If Vector With Pointer [IN/OUT]
				//	Components points to Distant Pointer
				if(vector_src->pointer)
				{
					// Copy Pointer
					vector_copy_pointer(vector_self, vector_src);

					// If Brick Vector Has Components
					if(brick->state.has_components)
					{
						int i;
						t_brick *brick_component;
						t_plug *plug_intern_component;
						t_plug *plug_in_component;

						// For Vector Length
						for(i=0; i < vector_self->length; i++)
						{
							// Get Brick Component
							//brick_component = block_brick_get_by_order(block,i);
							brick_component = block_brick_get_by_position(block,i);

							// Get Plugs
							plug_intern_component = &brick_component->plug_intern;
							plug_in_component = &brick_component->plug_in;

							// Set Component Pointer
							plug_intern_component->data = vector_get_pointer(vector_self,i);

							// Open Flow In
							plug_in_component->state.flow_in = 1;
						}
					}

					if(C->ui->show_step) term_log("[VECTOR] set pointer %p",vector_self->pointer);
				}

				// ***********************************************
				// {B}	If Vector Without Pointer [IN] 
				// 	Components Poinst To Internal List
				else if(mode == mode_in)
				{
					// Get Internal Vector List
					t_vlst *vlst_src = vector_src->vector;
					t_vlst *vlst_dst = vector_self->vector;

					// Copy Vector List Datas
					vlst_copy(vlst_dst, vlst_src);

					// If Vector Has Components
					if(brick->state.has_components)
					{
						int i;
						t_brick *brick_component;
						t_plug *plug_intern_component;

						// For Vector Length
						for(i=0; i < vector_self->length; i++)
						{
							// Get Component
							//brick_component = block_brick_get_by_order(block,i);
							brick_component = block_brick_get_by_position(block,i);
							plug_intern_component = &brick_component->plug_intern;

							// Set Component Pointer
							plug_intern_component->data = vlst_get_pointer(vlst_dst,i);

							// Don't Store Pointer 
							plug_intern_component->state.store_data  = 0;
						}
					}
				}


				break;
				
			default:
				break;
		}
	}
	else
	{
		// *************************************************
		// {C} No Vector IN
		// Internal List points to Components
		if(mode == mode_in)
		{
			// If Vector Has Components
			if(brick->state.has_components)
			{
				int i;
				t_brick *brick_component;
				t_plug *plug_intern_component;

				// Get Vector Internal List
				t_vlst *vlst = vector_self->vector;

				// For Vector Lenght
				for(i=0; i < vector_self->length; i++)
				{
					// Get Componenent
					//brick_component = block_brick_get_by_order(block,i);
					brick_component = block_brick_get_by_position(block,i);
					plug_intern_component = &brick_component->plug_intern;

					// If Componenent Has Data (Scalar)
					if(plug_intern_component->data) 

						// Set vector Internal
						vlst_set_data(vlst,plug_intern_component->data,i);
				}
			}
		}
	}
}

void cls_plug_flow_vector(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_vector);
}

// :GEO

void __cls_plug_flow_geo(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
}

void cls_plug_flow_geo(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_geo);
}

void __cls_plug_flow_geo_array(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
}

void cls_plug_flow_geo_array(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_geo_array);
}

// GEO POINT

void __cls_plug_flow_geo_point(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	//t_context *C = ctx_get();
	//t_brick *brick = plug->brick;
	//t_block *block = brick->block;
	//t_geo_point *point = plug->data;

	// If Src or Dst
	if(src_plug)
	{
		t_data_type type = src_plug->data_type;

		switch(type)
		{
			// case VECTOR
			case dt_geo_point:
				plug->data = src_plug->data;
				break;
			default:
				break;
		}
	}
}

void cls_plug_flow_geo_point(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_geo_point);
}

void __cls_plug_flow_geo_edge(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
}

void cls_plug_flow_geo_edge(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_geo);
}


// TRIGGER

void __cls_plug_flow_trigger(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
	t_brick *brick=plug->brick;

	if(src_plug)
	{
		t_data_type src_type=src_plug->data_type;

		int data;

		switch(src_type)
		{
			case(dt_int):

				data=drf_int(src_plug->data);
				if(data == 1)
					brick->action(brick);
				break;
				
			default:
				plug_warning(plug,src_plug);
				break;
		}
	}
	}
}

void cls_plug_flow_trigger(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_trigger);
}


// GENERIC

void cls_plug_flow_generic(t_plug *plug)
{
	t_plug *plug_in=plug->src;

	if(plug_in->state.is_connected)
	{
		t_plug *src_plug=plug_get_src(plug);
		t_data_type src_type=src_plug->data_type;

		switch(src_type)
		{
			default:
				plug_warning(plug,src_plug);
				break;
		}

	}

}

// CAMERA

void __cls_plug_flow_camera(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
		t_data_type src_type=src_plug->data_type;

		t_camera *camera;

		switch(src_type)
		{
			case(dt_camera):
				camera = src_plug->data;
				plug->data = camera;

				break;
			default:
				plug_warning(plug,src_plug);
				break;
		}
	}
}

void cls_plug_flow_camera(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_camera);
}

// FLOAT

void __cls_plug_flow_float(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
		t_data_type src_type=src_plug->data_type;

		float *data=plug->data;

		float increment=plug->brick->var.increment;

		if(data)
		{
			switch(src_type)
			{
				case dt_int:

					*data=(float)drf_int(src_plug->data);
					break;

				case dt_float:

					*data=drf_float(src_plug->data);
					break;
				default:
					plug_warning(plug,src_plug);
					break;
			}

			if(increment) *data=*data * increment;  
		}
	}

}

void cls_plug_flow_float(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_float);
}

// INT

void __cls_plug_flow_int(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
		t_data_type src_type=src_plug->data_type;

		int *data=plug->data;

		float increment=plug->brick->var.increment;

		switch(src_type)
		{
			case dt_int:

				*data=drf_int(src_plug->data);
				break;

			case dt_uint:

				*data=(int)drf_uint(src_plug->data);
				break;

			case dt_float:

				*data=(int)drf_float(src_plug->data);
				break;
			case dt_lst:
				break;

			default:
				plug_warning(plug,src_plug);
				break;
		}

		if(increment) *data=*data * increment;  
	}
}


void cls_plug_flow_int(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_int);
}

// UNSIGNED INT

void __cls_plug_flow_uint(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
		t_data_type src_type=src_plug->data_type;

		unsigned int *data=plug->data;

		float increment=plug->brick->var.increment;

		switch(src_type)
		{
			case dt_int:
				*data=(unsigned int) drf_int(src_plug->data);
				break;

			case dt_uint:
				*data= drf_uint(src_plug->data);
				break;

			case dt_float:

				*data=(unsigned int) drf_float(src_plug->data);
				break;
			case dt_lst:
				break;

			default:
				plug_warning(plug,src_plug);
				break;
		}

		if(increment) *data=*data * increment;  
	}
}


void cls_plug_flow_uint(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_int);
}

// :LST

void __cls_plug_flow_lst(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
		t_data_type src_type=src_plug->data_type;

		t_brick *src_brick=src_plug->brick;

		switch(src_type)
		{
			// + LST
			case dt_lst:
				//copy pointer
				plug->data=src_plug->data;
				break;

			// +INT (REWIND)
			case dt_int:

				if(is(src_brick->id.name,"rewind"))
				{
					int *state = src_plug->data;

					if(*state == 1)
					{
						t_lst *lst=(t_lst *)plug->data;
						lst->current=lst->first;
					}
				}

				break;
				
			default:
				plug_warning(plug,src_plug);
				break;
		}
	}
}

void cls_plug_flow_lst(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_lst);
}

// :VLST

void __cls_plug_flow_vlst(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
		t_data_type src_type=src_plug->data_type;
		t_vlst *vlst = plug->data;
		t_lst *lst = NULL;
		t_data_type lst_type = dt_null;

		switch(src_type)
		{
			// + LST
			case dt_vlst:
				//copy pointer
				plug->data=src_plug->data;
				break;
			case dt_geo:
				//geo_vlst_set(src_plug->data, plug->data);
				break;
			case dt_lst:
				lst = src_plug->data;
				lst_type = lst->type;
				switch(lst_type)
				{
					case(dt_geo_point):
						geo_vlst_points_set(lst, vlst);
						break;
					case(dt_geo_edge):
						geo_vlst_edges_set(lst, vlst);
						break;
					default:
						break;
				}
				break;
				
				
			default:
				plug_warning(plug,src_plug);
				break;
		}
	}
}

void cls_plug_flow_vlst(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_vlst);
}

// VERTEX

void __cls_plug_flow_vertex(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
		t_data_type src_type=src_plug->data_type;

		t_mesh *mesh;

		switch(src_type)
		{
			// + VLST
			case dt_vlst:

				mesh=plug->data;

				mesh->vertex=src_plug->data;
				mesh->state.need_update=1;

				break;
				
			default:
				plug_warning(plug,src_plug);
				break;
		}
	}
}

void cls_plug_flow_vertex(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_vertex);
}

// :MESH

void __cls_plug_flow_mesh(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
		t_mesh *mesh = plug->data;
		t_vlst *vlst = NULL;

		t_data_type src_type=src_plug->data_type;

		switch(src_type)
		{
			case dt_mesh:
				plug->data = src_plug->data;
				break;

			case dt_vlst:
				vlst = src_plug->data;
				if(vlst->data)
				{
					switch(vlst->type_target)
					{
						case(dt_vertex):
							mesh->vertex = vlst;
							mesh->var.tot_vertex = vlst->count;
							break;
						case(dt_edge):
							mesh->edges = vlst;
							break;
						default:
							break;
					}
				}
				break;
			default:
				plug_warning(plug,src_plug);
				break;
		}
	}
}

void cls_plug_flow_mesh(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_mesh);
}

// OBJECT

void __cls_plug_flow_object(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
		t_data_type src_type=src_plug->data_type;

		// node (pointer)
		t_node *node=plug->data;
		// object
		t_object *object=node->data;

		t_mesh *mesh;

		t_camera *camera_src;
		t_camera *camera_dst;

		switch(src_type)
		{
			// + MESH
			case dt_mesh:
				object = plug->data;

				if(object)
				{
					if(is(object->type,"mesh"))
					{
						mesh=src_plug->data;
						object->mesh=mesh;
					}
				}

				break;

			// + CAMERA
			case dt_camera:

				if(is(object->type,"camera"))
				{
					if(src_plug->state.open_out)
					{
						camera_src=src_plug->data;
						camera_dst=object->data;
						camera_copy(camera_dst,camera_src);
					}
				}

				break;

			case dt_lst:
				break;
				
			default:
				plug_warning(plug,src_plug);
				break;
		}
	}
}

void cls_plug_flow_object(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_object);
}

// POINTER

void __cls_plug_flow_pointer(t_plug_mode mode,t_plug *plug,t_plug *src_plug)
{
	if(src_plug)
	{
		t_data_type src_type=src_plug->data_type;

		t_object *object;

		switch(src_type)
		{
			// + MESH
			case dt_mesh:

				object=plug->data;
				object->mesh=src_plug->data;

				break;
				
			default:
				plug_warning(plug,src_plug);
				break;
		}
	}
}

void cls_plug_flow_pointer(t_plug *plug)
{
	_cls_flow_(plug,__cls_plug_flow_pointer);
}

// CLS

// FLOAT

t_plug_class plug_float ={
	.cls_type="plug",
	.type=dt_float,
	.make=cls_plug_make_float,
	.flow=cls_plug_flow_float,
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// INT

t_plug_class plug_int ={
	.cls_type="plug",
	.type=dt_int,
	.make=cls_plug_make_int,
	.flow=cls_plug_flow_int,
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// UINT

t_plug_class plug_uint ={
	.cls_type="plug",
	.type=dt_uint,
	.make=cls_plug_make_uint,
	.flow=cls_plug_flow_uint,
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// STRING

t_plug_class plug_string ={
	.cls_type="plug",
	.type=dt_string,
	.make=cls_plug_make_string,
	.flow=cls_plug_flow_generic, //XXX
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// POINTER

t_plug_class plug_pointer ={
	.cls_type="plug",
	.type=dt_pointer,
	.make=cls_plug_make_string,
	.flow=cls_plug_flow_pointer, //XXX
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// MESH

t_plug_class plug_mesh ={
	.cls_type="plug",
	.type=dt_mesh,
	.make=cls_plug_make_mesh,
	.flow=cls_plug_flow_mesh, 
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// VERTEX

t_plug_class plug_vertex ={
	.cls_type="plug",
	.type=dt_vertex,
	.make=cls_plug_make_vertex,
	.flow=cls_plug_flow_vertex, 
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// FACE

t_plug_class plug_face ={
	.cls_type="plug",
	.type=dt_face,
	.make=cls_plug_make_face,
	.flow=cls_plug_flow_generic, //XXX
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// VLST

t_plug_class plug_vlst ={
	.cls_type="plug",
	.type=dt_vlst,
	.make=cls_plug_make_vlst,
	.flow=cls_plug_flow_vlst, 
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// LST

t_plug_class plug_lst ={
	.cls_type="plug",
	.type=dt_lst,
	.make=cls_plug_make_lst,
	.flow=cls_plug_flow_lst, 
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// CAMERA

t_plug_class plug_camera ={
	.cls_type="plug",
	.type=dt_camera,
	.make=cls_plug_make_camera,
	.flow=cls_plug_flow_camera, 
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// CHAR

t_plug_class plug_char ={
	.cls_type="plug",
	.type=dt_char,
	.make=cls_plug_make_char,
	.flow=cls_plug_flow_generic, //XXX
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// OBJECT

t_plug_class plug_object ={
	.cls_type="plug",
	.type=dt_object,
	.make=cls_plug_make_object,
	.flow=cls_plug_flow_object, 
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// SELECTOR

t_plug_class plug_selector ={
	.cls_type="plug",
	.type=dt_selector,
	.make=cls_plug_make_selector,
	.flow=cls_plug_flow_object, 
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// TRIGGER

t_plug_class plug_trigger ={
	.cls_type="plug",
	.type=dt_trigger,
	.make=cls_plug_make_trigger,
	.flow=cls_plug_flow_trigger, 
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// OPERATOR

t_plug_class plug_operator ={
	.cls_type="plug",
	.type=dt_operator,
	.make=cls_plug_make_operator,
	.flow=cls_plug_flow_operator, 
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

// VECTOR

t_plug_class plug_vector ={
	.cls_type="vector",
	.type=dt_vector,
	.make=cls_plug_make_vector,
	.flow=cls_plug_flow_vector,  
	.connect = cls_plug_connect_vector,
	.disconnect = cls_plug_disconnect_vector,
};

// GEOMETRY

t_plug_class plug_geo ={
	.cls_type="geo",
	.type=dt_geo,
	.make=cls_plug_make_geo,
	.flow=cls_plug_flow_geo,  
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

t_plug_class plug_geo_point ={
	.cls_type="geo_point",
	.type=dt_geo_point,
	.make=cls_plug_make_geo_point,
	.flow=cls_plug_flow_geo_point,  
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

t_plug_class plug_geo_edge ={
	.cls_type="geo_edge",
	.type=dt_geo_edge,
	.make=cls_plug_make_geo_edge,
	.flow=cls_plug_flow_geo_edge,  
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};

t_plug_class plug_geo_array ={
	.cls_type="geo_array",
	.type=dt_geo_array,
	.make=cls_plug_make_geo_array,
	.flow=cls_plug_flow_geo_array,  
	.connect = cls_plug_connect_general,
	.disconnect = cls_plug_disconnect_general,
};


void cls_plug_make_float(t_plug *plug)
{
	plug->cls=&plug_float;
}

void cls_plug_make_int(t_plug *plug)
{
	plug->cls=&plug_int;
}

void cls_plug_make_uint(t_plug *plug)
{
	plug->cls=&plug_uint;
}

void cls_plug_make_string(t_plug *plug)
{
	plug->cls=&plug_int;
}

void cls_plug_make_pointer(t_plug *plug)
{
	plug->cls=&plug_pointer;
}


void cls_plug_make_mesh(t_plug *plug)
{
	plug->cls=&plug_mesh;
}

void cls_plug_make_vertex(t_plug *plug)
{
	plug->cls=&plug_vertex;
}

void cls_plug_make_face(t_plug *plug)
{
	plug->cls=&plug_face;
}

void cls_plug_make_vlst(t_plug *plug)
{
	plug->cls=&plug_vlst;
}

void cls_plug_make_lst(t_plug *plug)
{
	plug->cls=&plug_lst;
}

void cls_plug_make_camera(t_plug *plug)
{
	plug->cls=&plug_camera;
}

void cls_plug_make_char(t_plug *plug)
{
	plug->cls=&plug_char;
}

void cls_plug_make_object(t_plug *plug)
{
	plug->cls=&plug_object;
}

void cls_plug_make_selector(t_plug *plug)
{
	plug->cls=&plug_selector;
}

void cls_plug_make_trigger(t_plug *plug)
{
	plug->cls=&plug_trigger;
}

void cls_plug_make_operator(t_plug *plug)
{
	plug->cls=&plug_operator;
}

void cls_plug_make_vector(t_plug *plug)
{
	plug->cls=&plug_vector;
}

void cls_plug_make_geo(t_plug *plug)
{
	plug->cls=&plug_geo;
}

void cls_plug_make_geo_point(t_plug *plug)
{
	plug->cls=&plug_geo_point;
}

void cls_plug_make_geo_edge(t_plug *plug)
{
	plug->cls=&plug_geo_edge;
}

void cls_plug_make_geo_array(t_plug *plug)
{
	plug->cls=&plug_geo_array;
}

t_plug_class *plugs[] = {
	&plug_float,
	&plug_int,
	&plug_uint,
	&plug_char,
	&plug_string,
	&plug_mesh,
	&plug_vertex,
	&plug_face,
	&plug_vlst,
	&plug_lst,
	&plug_camera,
	&plug_object,
	&plug_selector,
	&plug_trigger,
	&plug_operator,
	&plug_vector,
	&plug_geo,
	&plug_geo_point,
	&plug_geo_edge,
	&plug_geo_array,
	};


// cls

void plug_cls_init(t_plug *plug)
{
	int i;
	int n = sizeof(plugs)/sizeof(plugs[0]);
	int found=0;

	for(i=0;i<n;i++)
	{
		if(plug->data_type==plugs[i]->type)
		{
			plugs[i]->make(plug);
			found=1;
			break;
		}
	}

	if(!found)
	{
		char *type=data_name_get(plug->data_type);
		printf("[ERROR:plug_cls_init] Unknown plug type [%s]\n",type);
	}
}

