/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "base.h"
#include "ctx.h"
#include "op.h"
#include "action.h"
#include "app.h"
#include "event.h"
#include "viewport.h"
#include "camera.h"
#include "dict.h"
#include "data.h"
#include "list.h"
#include "vlst.h"
#include "ui.h"
#include "block.h"
#include "brick.h"
#include "vector.h"
#include "clock.h"
#include "set.h"
#include "rhizome.h"
#include "geometry.h"

// >

void *op_superior(t_brick *brick)
{
	t_block *block=brick->block;

	t_brick *brick_val_1 = block_brick_get(block,"a");
	t_brick *brick_val_2 = block_brick_get(block,"b");
	t_brick *brick_result = brick;

	t_plug *plug_val_1 = &brick_val_1->plug_intern;
	t_plug *plug_val_2 = &brick_val_2->plug_intern;
	t_plug *plug_result = &brick_result->plug_intern;

	float *f1;
	float *f2;

	int *i1;
	int *i2;

	int *result = plug_result->data;

	t_data_type type_1 = plug_val_1->data_type;
	t_data_type type_2 = plug_val_2->data_type;

	t_data_type type_common  = dt_null; 

	if(type_1 == type_2) type_common = type_1;

	if(type_common != dt_null)
	{
		if(type_common == dt_float)
		{
			f1 = plug_val_1->data;
			f2 = plug_val_2->data;

			if(*f1 > *f2) *result = 1;
			else *result = 0;
		}
		else
		{
			i1 = plug_val_1->data;
			i2 = plug_val_2->data;

			if(*i1 > *i2) *result = 1;
			else *result = 0;
		}
	}
	else
	{
		*result = 0;
	}

	return NULL;
}

// <

void *op_inferior(t_brick *brick)
{
	t_block *block=brick->block;

	t_brick *brick_val_1 = block_brick_get(block,"a");
	t_brick *brick_val_2 = block_brick_get(block,"b");
	t_brick *brick_result = brick;

	t_plug *plug_val_1 = &brick_val_1->plug_intern;
	t_plug *plug_val_2 = &brick_val_2->plug_intern;
	t_plug *plug_result = &brick_result->plug_intern;

	float *f1;
	float *f2;

	int *i1;
	int *i2;

	int *result = plug_result->data;

	t_data_type type_1 = plug_val_1->data_type;
	t_data_type type_2 = plug_val_2->data_type;

	t_data_type type_common  = dt_null; 

	if(type_1 == type_2) type_common = type_1;

	if(type_common != dt_null)
	{
		if(type_common == dt_float)
		{
			f1 = plug_val_1->data;
			f2 = plug_val_2->data;

			if(*f1 < *f2) *result = 1;
			else *result = 0;
		}
		else
		{
			i1 = plug_val_1->data;
			i2 = plug_val_2->data;

			if(*i1 < *i2) *result = 1;
			else *result = 0;
		}
	}
	else
	{
		*result = 0;
	}

	return NULL;
}

// EQUAL

void *op_equal(t_brick *brick)
{
	t_block *block=brick->block;

	t_brick *brick_val_1 = block_brick_get(block,"a");
	t_brick *brick_val_2 = block_brick_get(block,"b");
	t_brick *brick_result = brick;

	t_plug *plug_val_1 = &brick_val_1->plug_intern;
	t_plug *plug_val_2 = &brick_val_2->plug_intern;
	t_plug *plug_result = &brick_result->plug_intern;

	float *f1;
	float *f2;

	int *i1;
	int *i2;

	int *result = plug_result->data;

	t_data_type type_1 = plug_val_1->data_type;
	t_data_type type_2 = plug_val_2->data_type;

	t_data_type type_common  = dt_null; 

	if(type_1 == type_2) type_common = type_1;

	if(type_common != dt_null)
	{
		if(type_common == dt_float)
		{
			f1 = plug_val_1->data;
			f2 = plug_val_2->data;

			if(*f1 == *f2) *result = 1;
			else *result = 0;
		}
		else
		{
			i1 = plug_val_1->data;
			i2 = plug_val_2->data;

			if(*i1 == *i2) *result = 1;
			else *result = 0;
		}
	}
	else
	{
		*result = 0;
	}

	return NULL;
}

// MODULO

void *op_mod(t_brick *brick)
{
	t_block *block=brick->block;

	t_brick *brick_val_1 = block_brick_get(block,"a");
	t_brick *brick_val_2 = block_brick_get(block,"b");
	t_brick *brick_result = brick;

	t_plug *plug_val_1 = &brick_val_1->plug_intern;
	t_plug *plug_val_2 = &brick_val_2->plug_intern;
	t_plug *plug_result = &brick_result->plug_intern;

	float *f1;
	float *f2;

	int *i1;
	int *i2;

	int *result = plug_result->data;

	t_data_type type_1 = plug_val_1->data_type;
	t_data_type type_2 = plug_val_2->data_type;

	t_data_type type_common  = dt_null; 

	if(type_1 == type_2) type_common = type_1;

	if(type_common != dt_null)
	{
		if(type_common == dt_float)
		{
			f1 = plug_val_1->data;
			f2 = plug_val_2->data;

			if(*f1 == *f2) *result = 1;
			else *result = 0;
		}
		else
		{
			i1 = plug_val_1->data;
			i2 = plug_val_2->data;

			if(*i2) *result = *i1 % *i2;
		}
	}
	else
	{
		*result = 0;
	}

	return NULL;
}

// ++

void *op_plusplus(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;
	t_plug *plug_out=&brick->plug_out;

	int *intern = plug_intern->data;
	int *data;

	if(plug_out->state.is_connected && (*intern == 1))
	{
		t_plug *plug_dst = plug_get_dst(plug_intern);

		switch(plug_dst->data_type)
		{
			case(dt_int):
				data = plug_dst->data;
				(*data)++;

				break;
			default:
				break;
		}
	}

	return NULL;
}

// PIPE

void *op_pipe(t_brick *brick)
{
	t_block *block = brick->block;
	t_plug *plug_intern = &brick->plug_intern;
	int tot_bricks = block->tot_bricks;

	// flow
	plug_intern->cls->flow(plug_intern);

	t_brick *brick_state = block_brick_get(block,"state");
	t_plug *plug_state = &brick_state->plug_intern;
	int *state = plug_state->data;

	// update bricks
	if(tot_bricks>0)
	{
		t_link *l;
		t_brick *b;

		for(l = block->bricks->first;l;l = l->next)
		{
			b = l->data;
			if(!is(b->id.name,"clone"))
			{
				t_plug *plug_clone = &b->plug_intern;
				t_plug *plug_in_clone = &b->plug_in;
				t_plug *plug_out_clone = &b->plug_out;

				if(plug_in_clone->state.is_connected)
				{
					t_plug *plug_src = plug_get_src(plug_clone);

					// match current target type
					if(plug_clone->data_type != plug_src->data_type)
					{
						t_brick *brick_clone = plug_clone->brick;
						brick_type_change(brick_clone,plug_src);
					}
				}

				plug_out_clone->state.open_out = *state;

			}
		}
	}

	return NULL;
}

// CLONE

void *op_clone(t_brick *brick)
{
	t_block *block = brick->block;
	int tot_bricks = block->tot_bricks;

	// Get Brick
	t_brick *brick_brick = block_brick_get(block,"brick");
	t_plug *plug_brick = &brick_brick->plug_intern;
	t_plug *plug_out_brick = &brick_brick->plug_out;
	t_plug *plug_in_brick = &brick_brick->plug_in;

	// Update Clones
	if(tot_bricks>0)
	{
		t_link *l;
		t_brick *b;

		for(l = block->bricks->first;l;l = l->next)
		{
			b = l->data;
			if(!is(b->id.name,"brick") && !is(b->id.name,"clone"))
			{
				t_plug *plug_clone = &b->plug_intern;
				t_plug *plug_in_clone = &b->plug_in;

				// Change Type 
				if(plug_brick->data_type != plug_clone->data_type)
				{
					brick_type_change(b,plug_brick);
				}

				// Connect Clone
				plug_in_clone->src=plug_out_brick;

				if(plug_out_brick->dst || plug_in_brick->dst)
				plug_in_clone->state.is_connected = 1;
				else
				plug_in_clone->state.is_connected = 0;

				// Flow
				plug_clone->cls->flow(plug_clone);

				// Disconnect Clone
				plug_in_clone->src=NULL;
				plug_in_clone->state.can_connect = 0;
				plug_in_clone->state.is_connected = 0;
			}
		}
	}

	return NULL;
}

void *op_case( t_brick *brick)
{
	t_block *block = brick->block;
	t_brick *brick_case = block_brick_get( block, "case");
	t_brick *brick_result = block_brick_get( block, "result");
	int c = drf_int( brick_case->plug_intern.data);
	if( c > 0)
	{
		c = c  - 1;
		t_brick *b = ( t_brick *) lst_get_by_range( block->bricks, c);
		if( b)
		{
			float val = drf_float( b->plug_intern.data);
			set_float( brick_result->plug_intern.data, val);
		}
	}

	return NULL;
}

// SIN

void *op_sin(t_brick *brick)
{
	t_plug *plug_intern= &brick->plug_intern;
	float *data = brick->plug_intern.data;

	// flow
	plug_intern->cls->flow(plug_intern);

	// compute
	t_plug *plug_in=&brick->plug_in;
	if(plug_in->state.is_connected) *data=sin(*data);

	return NULL;
}

// COS

void *op_cos(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;
	float *data = brick->plug_intern.data;

	// flow
	plug_intern->cls->flow(plug_intern);

	// compute
	t_plug *plug_in=&brick->plug_in;
	if(plug_in->state.is_connected) *data=cos(*data);

	return NULL;
}

// WINDOW

void *op_window(t_brick *brick)
{
	t_app *app = app_get();
	app->window->viewport_width=app->window->width;
	app->window->viewport_height=app->window->height;
	glutReshapeWindow(app->window->width,app->window->height);
	return NULL;
}

// CAMERA ROTATE XY

void *op_camera_rotate_xy(t_brick *brick)
{
	t_context *C=ctx_get();

	float *data=brick->plug_intern.data;

	// Get default Viewport
	t_node *node_viewport = scene_node_get( C->scene, dt_viewport, "viewport");
	t_viewport *viewport;
	t_camera *camera;

	if(node_viewport)
	{
		viewport = node_viewport->data;
		camera = viewport->camera;
		op_camera_rotate(C,camera,*data,0);	
	}

	return NULL;
}

// CAMERA ROTATE Z

void *op_camera_rotate_z(t_brick *brick)
{
	t_context *C=ctx_get();

	// Get default Viewport
	t_node *node_viewport = scene_node_get( C->scene, dt_viewport, "viewport");
	t_viewport *viewport;
	t_camera *camera;

	if(node_viewport)
	{
		viewport = node_viewport->data;
		camera = viewport->camera;

		op_slider(brick);
		float *data=brick->plug_intern.data;
		op_camera_rotate(C,camera,0,*data);	
	}

	return NULL;
}

// DELETE

void *op_delete(t_brick *brick)
{
	t_context *C = ctx_get();
	if(C->scene->selected)
	{
		t_node *node=C->scene->selected;

		node->cls->del(node);

		C->scene->selected=NULL;
	}

	return NULL;
}

// MENU

void *op_menu(t_brick *brick)
{
	return NULL;
}

// VOID

void *op_void_exe(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;
	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}

// POINTER

void *op_pointer(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}

// MATHS

void op_maths_plug(t_operation operation,t_plug *dst,t_plug *src)
{
	t_context *C=ctx_get();

	t_data_type type_dst=dst->data_type;
	t_data_type type_src=src->data_type;

	int *dst_int;
	float *dst_float;
	t_vlst *dst_vlst;

	void *data_src=src->data;

	int count;
	int length; 
	int i,j;

	float *data_dst;
	float *data = NULL;
	t_data_type type=dt_null;

	t_vector *vector_src;
	t_vector *vector_dst;

	t_plug *plug_src;

	switch(type_dst)
	{
		case(dt_int):

			dst_int=dst->data;

			switch(type_src)
			{
				case(dt_int):

					switch(operation)
					{
						case (t_op_add): 	*dst_int += drf_int(data_src); break;
						case (t_op_mult): 	*dst_int *= drf_int(data_src); break;
					}

					break;

				case(dt_float):
					
					switch(operation)
					{
						case(t_op_add): 	*dst_int += (int)drf_float(data_src); break;
						case(t_op_mult): 	*dst_int *= (int)drf_float(data_src); break;
					}

					break;

				default:
					break;
			}

			break;

		case(dt_float):

			dst_float=dst->data;

			switch(type_src)
			{

				case(dt_int):

					switch(operation)
					{
						case(t_op_add):		*dst_float += (float)drf_int(data_src); break;
						case(t_op_mult):	*dst_float *= (float)drf_int(data_src); break;
					}

					break;

				case(dt_float):
				
					switch(operation)
					{
						case(t_op_add):		*dst_float += drf_float(data_src); break;
						case(t_op_mult):	*dst_float *= drf_float(data_src); break;
					}
					
					break;

				default:
					break;
			}

			break;

		case(dt_vlst):

			dst_vlst=dst->data;

			count=dst_vlst->count;
			length=dst_vlst->length; 

			data_dst=dst_vlst->data;

			plug_src=src->src;

			for(i=0;i<count;i++)
			{
				for(j=0;j<length;j++)
				{
					if(plug_src->state.is_connected)
					{

						C->event->update_connections=1;
						t_plug *plug_source=plug_get_src(src);

						//XXX!!!
						//t_brick *brick_source=plug_source->brick;
						//brick_source->cls->trigger(brick_source);

						data=plug_source->data;
						type=plug_source->data_type;

						C->event->update_connections=0;
					}

					switch(type)
					{
						case(dt_int):

							switch(operation)
							{
								case(t_op_add):	 	*data_dst += (float)drf_int(data); break;
								case(t_op_mult):	*data_dst *= (float)drf_int(data); break;
							}

							break;

						case(dt_float):

							switch(operation)
							{
								case(t_op_add):	 	*data_dst += drf_float(data); break;
								case(t_op_mult):	*data_dst *= drf_float(data); break;
							}

							break;
							
						default:
							break;
					}

					data_dst++;
				}
			}


			break;

		case(dt_vector):

			switch(type_src)
			{
				case(dt_vector):

					switch(operation)
					{
						case(t_op_add): 

							vector_src = src->data;
							vector_dst = dst->data;

							vector_op_add(vector_dst,vector_src);

							break;
						default:
							break;
					}

					break;

				default:
					break;

			}

			break;


		default:printf("[?]\n");break;

	}
}

t_vlst *get_first_vlst(t_block *block)
{
	t_link *l;
	t_brick *b;
	t_plug *p;

	// offset first two bricks

	l=block->bricks->first;
	l=l->next;
	l=l->next;

	for(;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		switch(p->data_type)
		{
			case(dt_vlst):return p->data ;break;
			default:break;
		}
	}

	return NULL;
}

int get_first_vlst_pos(t_block *block)
{
	t_link *l;
	t_brick *b;
	t_plug *p;

	// offset first two bricks

	l=block->bricks->first;
	l=l->next;
	l=l->next;

	int pos=2;

	for(;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		switch(p->data_type)
		{
			case(dt_vlst):return pos;break;
			default:break;
		}
		pos++;
	}

	return pos;
}

// OP MATHS COMPUTE

void op_maths_exe(t_operation operation,t_brick *brick)
{
	t_link *l;
	t_brick *b;
	t_plug *p;
	t_block *block = brick->block;

	t_brick *brick_result=block_brick_get(block,"result");
	t_plug *plug_result=&brick_result->plug_intern;

	int pos=0;
	int count = block->bricks->count;
	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		// don't evaluate result plug
		if(pos < count -2 ) 
			// compute
			op_maths_plug(operation,plug_result,p);
		pos++;
	}
}

// OP MATHS SET TYPE 

void *op_maths(t_operation operation,t_brick *brick)
{
	// get result type

	int is_int=0;
	int is_float=0;
	int is_vlst=0;
	int is_vector=0;

	t_link *l;
	t_brick *b;
	t_plug *p;

	t_block *block=brick->block;
	t_brick *brick_result=block_brick_get(block,"result");
	t_plug *plug_result=&brick_result->plug_intern;

	// get type
	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		switch(p->data_type)
		{
			case(dt_int):is_int=1;break;
			case(dt_float):is_float=1;break;
			case(dt_vlst):is_vlst=1;break;
			case(dt_vector):is_vector=1;break;
			default:break;
		}
	}

	// Change Type && Init Result

	if(is_vlst)
	{
		brick_change_type_by_name(brick_result,dt_vlst);
		t_vlst *v=get_first_vlst(block);
		if(v) brick_result->plug_intern.data=v;
		else printf("[ERROR] Can't get vlst\n");
	}
	else if(is_float)
	{
		brick_change_type_by_name(brick_result,dt_float);
		float *r=plug_result->data;
		switch(operation)
		{
			case(t_op_add): *r=0; break;
			case(t_op_mult): *r=1; break;
		}
	}
	else if(is_int)
	{
		brick_change_type_by_name(brick_result,dt_int);
		int *r=plug_result->data;

		switch(operation)
		{
			case(t_op_add): *r=0; break;
			case(t_op_mult): *r=1; break;
		}
	}
	else if(is_vector)
	{
		brick_change_type_by_name(brick_result,dt_vector);

		t_plug *plug_vector = &brick_result->plug_intern;
		t_vector *vector = plug_vector->data;
		t_vlst *vlst = vector->vector;
		float *v = vlst->data;
		
		vset3f(v,0,0,0);
	}

	return NULL;
}

// :GEOMETRY


void *op_geo_array( t_brick *brick)
{
	t_plug *plug_self = &brick->plug_intern;
	t_geo_array *array = plug_self->data;
	t_block *block = brick->block;
	t_brick *brick_target = block_brick_get( block, "target");
	t_brick *brick_vector = block_brick_get( block, "vector");
	t_plug *plug_target = &brick_target->plug_intern;
	t_plug *plug_vector = &brick_vector->plug_intern;
	void *target = plug_target->data;
	t_vector *vector = plug_vector->data;
	t_data_type type = plug_target->cls->type;

	array->type_element = type;
	array->element = target;
	array->vector = vector;

	geo_array_build( array); 

	return NULL;
}

void op_geo_exe(t_brick *brick)
{
	//t_context *C = ctx_get();
	t_link *l;
	t_brick *b;
	t_plug *p;
	t_geo_point *point;
	t_geo_edge *edge;
	t_geo_array *array;

	t_block *block=brick->block;
	t_brick *brick_geometry = block_brick_get(block,"data");
	t_geo *geo = brick_geometry->plug_intern.data;

	t_lst *points = lst_new("lst");
	t_lst *edges = lst_new("lst");


	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		if(p->cls->type == dt_geo_point)
		{
			point = p->data;
			lst_add( points, point, "point");
		}
		else if(p->cls->type == dt_geo_edge)
		{
			edge = p->data;
			if(edge->a && edge->b)
			{
				lst_add(edges, edge, "edge");
			}
		}
		else if( p->cls->type == dt_geo_array)
		{
			array = p->data;

			if( array->elements->count > 0)
			{
				geo_array_get_points( array, points);
				geo_array_get_edges( array, edges);
			}
		}
	}

	// Reset

	int count_points = points->count;
	int count_edges = edges->count;

	//scene_store( C->scene, 1);

	if( count_points || count_edges)
	{
		geo_reset( geo);

		if(points->count > 0) geo_data_set(geo, dt_geo_point, points);
		if(edges->count > 0) geo_data_set(geo, dt_geo_edge, edges);
	}

	//scene_store( C->scene, 0);

	// Free
	lst_free(points);
	lst_free(edges);
}

void *op_geometry(t_brick *brick)
{
	op_geo_exe(brick);
	return NULL;
}

void *op_geo(t_brick *brick)
{
	return NULL;
}

void *op_geo_point(t_brick *brick)
{
	t_plug *plug = &brick->plug_intern;
	// flow
	plug->cls->flow(plug);

	t_geo_point *point = brick->plug_intern.data;

	t_block *block = brick->block;
	t_brick *brick_vector = block_brick_get(block,"vector");
	t_vector *vector = brick_vector->plug_intern.data;

	geo_point_vector_update(point,vector);

	return NULL;
}

void *op_geo_edge(t_brick *brick)
{
	t_geo_edge *edge = brick->plug_intern.data;
	t_block *block = brick->block;
	t_brick *brick_point_1 = block_brick_get_by_order(block,1);
	t_brick *brick_point_2 = block_brick_get_by_order(block,2);
	t_geo_point *point_1 = brick_point_1->plug_intern.data;
	t_geo_point *point_2 = brick_point_2->plug_intern.data;

	edge->a = point_1;
	edge->b = point_2;

	return NULL;
}

void *op_add(t_brick *brick)
{
	op_maths(t_op_add,brick);
	op_maths_exe(t_op_add,brick);
	return NULL;
}

void *op_mult(t_brick *brick)
{
	op_maths(t_op_mult,brick);
	op_maths_exe(t_op_mult,brick);
	return NULL;
}

void *op_and(t_brick *brick)
{
	t_link *l;
	t_brick *b;
	t_plug *p;

	t_block *block=brick->block;
	t_brick *brick_result=block_brick_get(block,"result");
	t_plug *plug_result=&brick_result->plug_intern;
	int *result = plug_result->data;

	int pos = 0;
	int count = block->bricks->count;
	int r = 1; // true
	for(l=block->bricks->first;l;l=l->next)
	{
		b=l->data;
		p=&b->plug_intern;
		// don't evaluate result plug
		if(pos < count - 2)
		{
			int *_r = p->data;
			if(*_r == 0) r=0; // false

		}
		pos++;
	}

	*result = r;

	return NULL;
}


// REWIND

void *op_rewind(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}

// NOT

void *op_not(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;
	t_plug *plug_in=&brick->plug_in;

	// flow
	plug_intern->cls->flow(plug_intern);

	int *data=plug_intern->data;

	switch(plug_intern->data_type)
	{
		case(dt_int):

			if(plug_in->state.is_connected)
			{
				if(*data == 0) *data = 1;
				else *data =0;
			}

			break;
		default:printf("op_not need an int\n");break;
	}

	return NULL;
}

// SET VLST

void *op_set_vlst(t_brick *brick)
{
	t_block *block=brick->block;
	t_brick *brick_vlst = NULL;

	if(is(block->id.name,"vertex"))
		brick_vlst=block_brick_get(block,"vertex");
	else if(is(block->id.name,"color"))
		brick_vlst=block_brick_get(block,"colors");
	else if(is(block->id.name,"faces"))
		brick_vlst=block_brick_get(block,"quads");
	else if(is(block->id.name,"vlst"))
		brick_vlst=block_brick_get(block,"vlst");
	else 
		printf("err!\n");

	if(brick_vlst)
	{

		t_vlst *vlst=brick_vlst->plug_intern.data;

		if(vlst)
		{
			if(vlst->count != vlst->count_new) 
			{
				vlst_update_data(brick, vlst, NULL);
			}
		}
	}

	return NULL;
}

// RND

void *op_rnd(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;
	int *data=plug_intern->data;
	*data=rnd_range(0,100);

	return NULL;
}

// NEG

void *op_neg(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;
	t_plug *plug_in=&brick->plug_in;

	// flow
	plug_intern->cls->flow(plug_intern);

	// negate
	if(plug_in->state.is_connected)
		plug_data_negate(plug_intern);

	return NULL;
}

// ABS

void *op_abs(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;
	t_plug *plug_in=&brick->plug_in;

	// flow
	plug_intern->cls->flow(plug_intern);

	// negate
	if(plug_in->state.is_connected)
		plug_data_abs(plug_intern);

	return NULL;
}

// IS_LAST

void *op_is_last(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;
	t_plug *plug_src=plug_get_src(plug_intern);

	int *data = plug_intern->data;

	if(plug_src)
	{
		if(plug_src->data_type==dt_lst)
		{
			t_lst *lst = plug_src->data;

			if(lst->current && lst->last)
			{
				if(lst->current == lst->last)
				{
					*data = 1;
				}
				else
				{
					*data = 0;
				}
			}
			else
			{
				*data = 0;
			}
		}
	}
	else
	{
		*data = 0;
	}

	return NULL;
}

// FOR

void *op_for(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}

// OPERATOR

void *op_operator(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}


// VECTOR

void *op_vector(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}

// BANG

void *op_bang(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	int *state = plug_intern->data;

	// [TRUE] : IN (true) + BANG (false) = BANG (true)
	// let true pass throw
	if(*state == 1 && !(plug_intern->state.bang))
	{
		plug_intern->state.bang = 1;
	}
	// [FALSE] IN (false) && BANG (true) = BANG (flase)
	else if(*state == 0 && plug_intern->state.bang)
	{
		plug_intern->state.bang=0;
	}
	// ELSE [FALSE]
	else
	{
		*state=0;
	}

	return NULL;
}

void *op_float(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}


void *op_int(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	return NULL;
}

void *op_do_quit(t_brick *brick)
{
	t_plug *plug_intern = &brick->plug_intern;

	// flow
	plug_intern->cls->flow(plug_intern);

	int i = drf_int(plug_intern->data);

	if(i) op_quit(NULL);

	return NULL;
}

void *op_const(t_brick *brick)
{
	t_block *block = brick->block;

	t_brick *brick_i = block_brick_get(block,"i");

	t_plug *plug_const = &brick->plug_intern;
	t_plug *plug_i = &brick_i->plug_intern;

	int *_const = plug_const->data;
	int *_i = plug_i->data;

	// flow
	plug_const->cls->flow(plug_const);

	t_context *C = ctx_get();

	if(brick->brick_state.frame_loop != C->app->frame)
	{
		brick->brick_state.frame_loop = C->app->frame;
		*_const = *_i;
	}

	return NULL;
}


void *op_stack(t_brick *brick)
{
	t_context *C = ctx_get();

	op_slider(brick);


	t_block *block = brick->block;

	t_brick *brick_i = block_brick_get(block,"i");
	t_brick *brick_counter = block_brick_get(block,"counter");
	t_brick *brick_limit = block_brick_get(block,"limit");

	int *i = brick_i->plug_intern.data;
	int *counter = brick_counter->plug_intern.data;
	int *limit = brick_limit->plug_intern.data;

	// reset by frame

	if(brick->brick_state.frame_loop != C->app->frame)
	{
		brick->brick_state.frame_loop = C->app->frame;
		*counter = 0;
		*i = 0;
	}
	else
	{
		(*counter)++;
	}

	// test

	if(*counter > *limit)
	{
		*counter = 0;
		(*i)++;
	}

	return NULL;
}

void *op_if(t_brick *brick)
{
	t_block *block = brick->block;
	t_brick *brick_result = block_brick_get(block,"result");
	t_brick *brick_true = block_brick_get(block,"true");
	t_brick *brick_false = block_brick_get(block,"false");

	// Flow
	t_plug *plug = &brick->plug_intern;
	plug->cls->flow(plug);

	if(drf_int(brick->plug_intern.data) == 1)
	{
		brick_copy_data(brick_result,brick_true);
	}
	else
	{
		brick_copy_data(brick_result,brick_false);
	}

	return NULL;
}

