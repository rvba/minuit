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
#include "app.h"
#include "event.h"
#include "screen.h"
#include "memory.h"
#include "camera.h"
#include "ui.h"
#include "file.h"
#include "list.h"
#include "vlst.h"
#include "block.h"
#include "brick.h"
#include "viewport.h"
#include "sketch.h"
#include "mesh.h"
#include "term.h"
#include "object.h"
#include "util.h"
#include "mode.h"
#include "dict.h"
#include "draw.h"
#include "geometry.h"
#include "vector.h"

/*	*************************************************************	
 *		:SHOW
 *	*************************************************************
 */

float _WHITE_[3] = {1,1,1};

void vector_build(float *r,float *p, float *v)
{
	float vec[3];
	vsub(vec,v,p);
	vadd(r,p,vec);
}

void geo_point_show(t_geo_point *point)
{
	printf("[SHOW POINT]\n");
	printf("indice:%d\n",point->indice);
	printf("pos:");
	vprint3f(point->pos,'\n');
}

void geo_edge_show(t_geo_edge *edge)
{
	printf("[SHOW EDGE]\n");
	if(edge->a) printf("%d", edge->a->indice);
	else  printf("nil");
	printf(":");
	if(edge->a) printf("%d", edge->b->indice);
	else  printf("nil");
	printf("\n");
}

void geo_array_show( t_geo_array *array)
{
	printf( "[SHOW ARRAY]\n");
	printf("type:%s\n", data_name_get(array->type_element));
	printf("countt:%d\n", array->count);
	printf("lst count:%d\n", array->elements->count);
	printf("[ELEMENT]\n");
	if( array->type_element == dt_geo_point) geo_point_show( array->element);
	else if( array->type_element == dt_geo_edge) geo_edge_show( array->element);

	printf("[ELEMENTS]\n");
	t_link *l;
	if( array->type_element == dt_geo_point)
	{
		for(l=array->elements->first;l;l=l->next)
		{
			t_geo_point *point = l->data;
			geo_point_show( point);
		}
	}
	else if( array->type_element == dt_geo_edge)
	{
		for(l=array->elements->first;l;l=l->next)
		{
			t_geo_edge *edge = l->data;
			if( edge->a) geo_point_show( edge->a);
			if( edge->b) geo_point_show( edge->b);

			geo_edge_show( edge);
		}
	}
}

void geo_show(t_geo *geo)
{
	printf("[SHOW GEO]\n");
	t_link *l;
	t_geo_point *p;
	t_geo_edge *e;

	printf("[SHOW GEO POINTS]\n");
	if(geo->points->count > 0)
	{
		for(l=geo->points->first;l;l=l->next)
		{
			p=l->data;
			geo_point_show(p);
		}
	}
	else
	{
		printf("no points\n");
	}

	printf("[SHOW GEO EDGES]\n");

	if(geo->edges->count > 0)
	{
		for(l=geo->edges->first;l;l=l->next)
		{
			e=l->data;
			geo_edge_show(e);
		}
	}
	else
	{
		printf("no points\n");
	}
}


/*	*************************************************************	
 *		:NEW
 *	*************************************************************
 */


// NEW GEOMETRY 

t_geo *geo_new( const char *name)
{
	t_geo *geo = mem_malloc(sizeof(t_geo));
	id_init( &geo->id, name);

	geo->points = NULL;
	geo->edges = NULL;
	geo->faces = NULL;

	geo->selected = 0;

	return geo;
}

// NEW POINT 

t_geo_point *geo_point_new( const char *name)
{
	t_geo_point *point = mem_malloc(sizeof(t_geo_point));
	id_init( &point->id, name);

	vset3f(point->pos,0,0,0);
	point->indice = 0;

	return point;
}

// NEW EDGE 

t_geo_edge *geo_edge_new( const char *name)
{
	t_geo_edge *edge = mem_malloc(sizeof(t_geo_edge));
	id_init( &edge->id, name);

	edge->a = NULL;
	edge->b = NULL;

	return edge;
}

// NEW FACE 

t_geo_face *geo_face_new( const char *name)
{
	t_geo_face *face = mem_malloc(sizeof(t_geo_face));
	id_init( &face->id, name);

	face->a = NULL;
	face->b = NULL;
	face->c = NULL;
	face->d = NULL;

	face->ab = NULL;
	face->bc = NULL;
	face->cd = NULL;
	face->da = NULL;

	return face;
}

// NEW ARRAY

t_geo_array *geo_array_new(const char *name)
{
	t_geo_array *array = mem_malloc(sizeof(t_geo_array));
	id_init( &array->id, name);

	array->count = 0;
	array->element = NULL;
	array->type_element = dt_null;
	array->elements = NULL;
	array->vector = NULL;

	return array;
}


/*	*************************************************************	
 *		:FREE
 *	*************************************************************
 */


void geo_lst_delete( t_lst *lst);

void geo_point_free( t_geo_point *point)
{
	mem_free( point, sizeof( t_geo_point));
}

void geo_point_delete( t_geo_point *point)
{
	t_scene *sc = ctx_scene_get();
	if( sc->debug_all) printf("geo_point_delete _%s\n", point->id.name);
	if( point->id.store) scene_struct_delete( sc, point);
	else geo_point_free( point);
}

void geo_edge_free( t_geo_edge *edge) 
{
	mem_free( edge, sizeof( t_geo_edge));
}

void geo_edge_delete( t_geo_edge *edge)
{
	t_scene *sc = ctx_scene_get();
	if( sc->debug_all) printf("geo_edge_delete _%s\n", edge->id.name);
	if( edge->id.store) scene_struct_delete( sc, edge);
	else geo_edge_free( edge);
}

void geo_face_free( t_geo_face *face)
{
	mem_free( face, sizeof( t_geo_face));
}

void geo_face_delete( t_geo_face *face)
{
	t_scene *sc = ctx_scene_get();
	if( face->id.store) scene_struct_delete( sc, face);
	else geo_face_free( face);
}

void geo_array_free( t_geo_array *array)
{
	if( array->elements) geo_lst_delete( array->elements);
	if( array->vector) vector_delete( array->vector);
}

void geo_array_delete( t_geo_array *array)
{
	t_scene *sc = ctx_scene_get();
	if( sc->debug_all) printf("geo_array_delete %s\n", array->id.name);
	if( array->id.store) scene_struct_delete( sc, array);
	else geo_array_free( array);
}

void geo_lst_delete( t_lst *lst)
{
	t_link *l;
	t_data_type type = lst->type;
	t_geo_point *point;
	t_geo_edge *edge;

	for(l=lst->first;l;l=l->next)
	{
		if( type == dt_geo_point)
		{
			point = l->data;
			geo_point_delete( point);
		}
		else if( type == dt_geo_edge)
		{
			edge = l->data;
			geo_edge_delete( edge);
		}
		else printf("remove .... ?\n");
	}

	list_delete( lst);
}

void geo_free( t_geo *geo)
{
	if(geo->points) geo_lst_delete( geo->points);
	if(geo->edges) geo_lst_delete( geo->edges);
	if(geo->faces) geo_lst_delete( geo->faces);
}

void geo_delete( t_geo *geo)
{
	t_scene *sc = ctx_scene_get();
	if( geo->id.store) scene_struct_delete( sc, geo);
	else geo_free( geo);
}


/*	*************************************************************	
 *		:MAKE
 *	*************************************************************
 */


t_geo *geo_make( t_context *C, const char *name)
{
	t_geo *geo;
	if( C->scene->store)
	{
		t_node *node_geo = scene_add( C->scene, dt_geo, name);
		geo = node_geo->data;
	}
	else
	{
		geo = geo_new( name);
	}

	geo->points = list_make( dt_geo_point, "points");
	geo->edges = list_make( dt_geo_edge, "edges");

	if(C->ui->add_bricks)
	{
		add_geometry(C,"data",geo);
	}

	return geo;
}

t_geo_point *geo_point_make( t_context *C, const char *name)
{
	t_geo_point *geo_point;
	if( C->scene->store)
	{
		t_node *node = scene_add( C->scene, dt_geo_point, name);
		geo_point = node->data;
	}
	else
	{
		geo_point = geo_point_new( name);
	}

	if(C->ui->add_bricks)
	{
		add_geo_point(C,"point",geo_point);
	}

	return geo_point;
}

t_geo_edge *geo_edge_make( t_context *C, const char *name)
{
	t_geo_edge *geo_edge;
	if( C->scene->store)
	{
		t_node *node_geo_edge = scene_add(C->scene,dt_geo_edge,name);
		geo_edge = node_geo_edge->data;
	}
	else
	{
		geo_edge = geo_edge_new( name);
	}

	if(C->ui->add_bricks)
	{
		add_geo_edge(C,"point",geo_edge);
	}

	return geo_edge;
}

t_geo_array *geo_array_make( t_context *C, const char *name)
{
	t_node *node_geo_array = scene_add(C->scene,dt_geo_array,name);
	t_geo_array *geo_array = node_geo_array->data;

	geo_array->elements = list_make( dt_null, "elements");

	if(C->ui->add_bricks)
	{
		add_geo_array(C, name, geo_array);
	}

	return geo_array;
}

/*	*************************************************************	
 *		:TRANSFORM
 *	*************************************************************
 */

void geo_point_pos_set( t_geo_point *point, float *p)
{
	vcp(point->pos, p);
}

void geo_point_transform( t_geo_point *point, float *v)
{
	vadd( point->pos, point->pos, v);
}

void geo_edge_transform( t_geo_edge *edge, float *v)
{
	geo_point_pos_set( edge->b, v);
	vinverse3f( v);
	geo_point_pos_set( edge->a, v);
}

void geo_transform( t_geo *geo, float *v)
{
	t_link *l;
	t_geo_point *point;
	for(l=geo->points->first;l;l=l->next)
	{
		point = l->data;
		geo_point_transform( point, v);
	}
}

/*	*************************************************************	
 *		:DUPLICATE
 *	*************************************************************
 */

t_geo_point *geo_point_duplicate( t_geo_point *point, float *v)
{
	t_context *C = ctx_get();
	C->ui->add_bricks = 0;
	t_geo_point *point_new = geo_point_make( C, "point");
	C->ui->add_bricks = 1;

	// Transform
	geo_point_transform( point, v);

	return point_new;
}

t_geo_edge *geo_edge_duplicate( t_geo_edge *edge, float *point)
{
	float nul[] = {0,0,0};
	t_context *C = ctx_get();
	C->ui->add_bricks = 0;
	t_geo_edge *edge_new = geo_edge_make( C, "edge");
	edge_new->a = geo_point_duplicate(edge->a, nul);
	edge_new->b = geo_point_duplicate(edge->b, nul);
	C->ui->add_bricks = 1;

	geo_edge_transform( edge_new, point);

	return edge_new;
}

t_geo *geo_duplicate( t_geo *geo, float *point)
{
	t_context *C = ctx_get();
	C->ui->add_bricks = 0;
	t_geo *geo_new = geo_make( C, "geo");

	t_link *l;
	t_geo_edge *edge;
	t_geo_edge *edge_new;
	t_geo_point *a;
	t_geo_point *b;

	float nul[] = {0,0,0};

	for(l=geo->edges->first;l;l=l->next)
	{
		edge = l->data;
		edge_new = geo_edge_duplicate( edge, nul);
		list_add( geo_new->edges, edge_new);
		a = edge_new->a;
		b = edge_new->b;
		list_add( geo_new->points, a);
		list_add( geo_new->points, b);

	}
	C->ui->add_bricks = 1;

	// Transform
	geo_transform( geo_new, point);

	return geo_new;
}

/*	*************************************************************	
 *		:REBIND
 *	*************************************************************
 */

t_geo *geo_rebind( t_scene *sc, void *data)
{
	t_geo *geo = (t_geo *) data;

	rebind( sc, "geo", "points", (void **) &geo->points);
	rebind( sc, "geo", "edges", (void **) &geo->edges);
	rebind( sc, "geo", "faces", (void **) &geo->faces);
	
	return geo;
}

t_geo_point *geo_point_rebind( t_scene *sc, void *data)
{
	t_geo_point *point = (t_geo_point *) data;
	return point;
}

t_geo_edge *geo_edge_rebind( t_scene *sc, void *data)
{
	t_geo_edge *edge = (t_geo_edge *) data;

	rebind( sc, "geo_edge", "a", (void **) &edge->a);
	rebind( sc, "geo_edge", "b", (void **) &edge->b);
	return edge;
}

t_geo_array *geo_array_rebind( t_scene *sc, void *data)
{
	t_geo_array *array = (t_geo_array *) data;

	rebind( sc, "geo_array", "element", (void **) &array->element);
	rebind( sc, "geo_array", "elements", (void **) &array->elements);
	rebind( sc, "geo_array", "vector", (void **) &array->vector);
	return array;
}

void *geo_array_get_ref(t_geo_array *array, const char *ref)
{
	void *p;

	if(is(ref,"count"))  			p=&array->count; 
	else
	{
		printf("[ERROR geo_array_get_ref] Unknown ref [%s] \n",ref);
		return NULL;
	}

	return p;
}


/*	*************************************************************	
 *		:RESET
 *	*************************************************************
 */

void geo_point_reset( t_geo_point *point)
{
	geo_point_delete( point);
}

void geo_edge_reset( t_geo_edge *edge)
{
	geo_edge_delete( edge);
}

void geo_reset_elements( t_context *C, t_data_type type, t_lst *lst)
{
	t_link *l;
	for(l=lst->first;l;l=l->next)
	{
		if( type == dt_geo_point)
		{
			geo_point_reset( ( t_geo_point *) l->data) ;
		}
		else if( type == dt_geo_edge)
		{
			geo_edge_reset( ( t_geo_edge *) l->data) ;
		}
	}
}

void geo_reset(t_geo *geo)
{
	t_context *C = ctx_get();
	if(geo->points)
	{
		geo_reset_elements( C, dt_geo_point, geo->points);
		lst_cleanup( geo->points);
	}

	if(geo->edges)
	{
		geo_reset_elements( C, dt_geo_edge, geo->edges);
		lst_cleanup( geo->edges);
	}
}

void geo_lst_reset( t_data_type type, void *data)
{
	switch( type)
	{
		case dt_geo_point: geo_point_reset( ( t_geo_point *) data); break;
		case dt_geo_edge: geo_edge_reset( ( t_geo_edge *) data); break;
		default: break;
	}
}

void geo_array_reset( t_geo_array *array)
{
	if( array->elements) 
	{
		t_link *l;
		t_lst *lst = array->elements;
		for(l=lst->first;l;l=l->next)
		{
			geo_lst_reset( array->type_element, l->data);
		}

		lst_cleanup( array->elements);
	}
}

/*	*************************************************************	
 *		:UTIL
 *	*************************************************************
 */


void geo_point_vector_update(t_geo_point *point, t_vector *vector)
{
	t_vlst *vlst = vector->vector;
	float *points = vlst->data;
	vcp(point->pos,points);
}

int geo_point_equal(t_geo_point *a, t_geo_point *b)
{
	if(a->id.id == b->id.id) return 1;
	else return 0;
}

int geo_edge_equal(t_geo_edge *edge, int a, int b)
{
	int _a = edge->a->indice;
	int _b = edge->b->indice;

	if(
		((_a == a) && (_b == b))
		||
		((_a == b) && (_b == a))
		)
		return 1;
	else
		return 0;
}

int geo_elem_exists(t_geo *geo, t_data_type type, void *data)
{
	t_link *l;
	t_lst *lst=NULL;
	t_id *_id = (t_id *) data;
	int id = _id->id;
	void *d;
	if( type == dt_geo_point) lst = geo->points;
	else if( type == dt_geo_edge ) lst = geo->edges;

	for(l=lst->first;l;l=l->next)
	{
		d = l->data;
		t_id *__id = (t_id *) d;
		int ___id = __id->id;
		if(___id == id)
		{
			return 1;
		}
	}

	return 0;
}

// BUILD CENTER

float geo_build_area( t_geo *geo)
{
	t_link *l;
	t_link *next;
	t_geo_point *point;
	t_geo_point *point_next;

	float sum = 0;
	for(l=geo->points->first;l;l=l->next)
	{
		next = l->next;
		if(next)
		{
			point = l->data;
			point_next = next->data;
			sum += ( point->pos[0] * point_next->pos[1]) - ( point_next->pos[0] * point->pos[1]);
		}
	}

	return( sum / 2);
}

float geo_build_center_x( t_geo *geo, float area)
{
	t_link *l;
	t_link *next;
	t_geo_point *point;
	t_geo_point *point_next;

	float sum = 0;
	float x,y,nx,ny;
	for(l=geo->points->first;l;l=l->next)
	{
		next = l->next;
		if(next)
		{
			point = l->data;
			point_next = next->data;
			x = point->pos[0];
			y = point->pos[1];
			nx = point_next->pos[0];
			ny = point_next->pos[1];
			sum += ( ( x + nx) * ( ( x * ny) - ( nx * y)));
		}
	}

	return( sum / ( area *6));
}

float geo_build_center_y( t_geo *geo, float area)
{
	t_link *l;
	t_link *next;
	t_geo_point *point;
	t_geo_point *point_next;

	float sum = 0;
	float x,y,nx,ny;
	for(l=geo->points->first;l;l=l->next)
	{
		next = l->next;
		if(next)
		{
			point = l->data;
			point_next = next->data;
			x = point->pos[0];
			y = point->pos[1];
			nx = point_next->pos[0];
			ny = point_next->pos[1];
			sum += ( ( y + ny) * ( ( x * ny) - ( nx * y)));
		}
	}

	return( sum / ( area *6));
}

void geo_build_center( t_geo *geo, float *c)
{
	float area = geo_build_area( geo);
	float x = geo_build_center_x( geo, area);
	float y = geo_build_center_y( geo, area);

	c[0] = x;
	c[1] = y;
	c[2] = 0;
}

/*	*************************************************************	
 *		:FILL
 *	*************************************************************
 */

void geo_fill_point(t_geo *geo, t_geo_point *point)
{
	if(!geo_elem_exists(geo, dt_geo_point, point))
	{
		point->indice = geo->points->count;
		list_add( geo->points, point);
	}
}

void geo_fill_points(t_geo *geo, t_lst *lst)
{
	t_link *l;
	t_geo_point *point;

	if(geo->points)
	{
		for(l=lst->first;l;l=l->next)
		{
			point = l->data;
			geo_fill_point( geo, point);
		}
	}
}

void geo_fill_edges(t_geo *geo, t_lst *lst)
{
	t_link *l;
	t_geo_edge *edge;

	if(geo->edges)
	{
		for(l=lst->first;l;l=l->next)
		{
			edge = l->data;

			if(!geo_elem_exists(geo, dt_geo_edge, edge))
			{
				if( edge->a) geo_fill_point( geo, edge->a);
				if( edge->b) geo_fill_point( geo, edge->b);

				list_add( geo->edges, edge);
			}
		}
	}
}


/*	*************************************************************	
 *		:GET
 *	*************************************************************
 */

void geo_array_get_points( t_geo_array *array, t_lst *lst)
{
	t_data_type type = array->type_element;
	t_link *l;
	t_geo_edge *edge;
	t_geo *geo;
	if( type == dt_geo_point)
	{
		lst_add_lst( lst, array->elements);
	}
	else if( type  == dt_geo_edge)
	{
		for(l=array->elements->first;l;l=l->next)
		{
			edge = l->data;
			if( edge->a) lst_add( lst, edge->a, "point");
			if( edge->b) lst_add( lst, edge->b, "point");
		}
	}
	else if( type == dt_geo)
	{
		for(l=array->elements->first;l;l=l->next)
		{
			geo = l->data;
			lst_add_lst( lst, geo->points);
		}
	}
}

void geo_array_get_edges( t_geo_array *array, t_lst *lst)
{
	t_data_type type = array->type_element;
	t_link *l;
	t_geo *geo;
	if( type  == dt_geo_edge)
	{
		for(l=array->elements->first;l;l=l->next)
		{
			lst_add( lst, l->data, "edge");
		}
	}
	else if( type == dt_geo)
	{
		for(l=array->elements->first;l;l=l->next)
		{
			geo = l->data;
			lst_add_lst( lst, geo->edges);
		}
	}
}

/*	*************************************************************	
 *		:ARRAY
 *	*************************************************************
 */



float *geo_array_build_circle( t_geo_array *array)
{
	t_data_type type = array->type_element;
	float c[3];
	t_vector *vector = array->vector;
	float *v = vector->vector->data;
	float l = vnorm3f( v);
	int count = array->count;

	if( type == dt_geo_point)
	{
		t_geo_point *point = array->element;
		vcp( c, point->pos);
	}
	else if( type == dt_geo_edge)
	{
		t_geo_edge *edge = (t_geo_edge *) array->element;
		vcenter3f( c, edge->a->pos, edge->b->pos);
	}
	else if( type == dt_geo)
	{
		t_geo *geo = (t_geo *) array->element;
		geo_build_center( geo, c);
	}

	return( calc_circle( c, l, count));
}



void geo_data_set(t_geo *geo, t_data_type type, t_lst *lst)
{
	if( type == dt_geo_point) 	geo_fill_points(geo, lst);
	else if( type == dt_geo_edge) 	geo_fill_edges(geo, lst);
}

void geo_vlst(t_vlst *vlst, t_lst *lst)
{
	if(lst->count != vlst->count)
	{
		if(lst->count > 0)
		{
			vlst->count_new = lst->count;
			 __vlst_update_data(vlst, NULL);
		}
	}
}

void geo_vlst_points_set(t_lst *lst, t_vlst *vlst)
{
	t_link *l;

	geo_vlst(vlst, lst);

	float *v = vlst->data;

	for(l=lst->first;l;l=l->next)
	{
		t_geo_point *point = l->data;
		float *p = point->pos;
		vcp(v,p);
		v+=3;
	}

	// Set Type
	vlst->type_target = dt_vertex;
}

void geo_vlst_edges_set(t_lst *lst, t_vlst *vlst)
{
	t_link *l;

	geo_vlst(vlst, lst);

	int *i = vlst->data;

	for(l=lst->first;l;l=l->next)
	{
		t_geo_edge *edge = l->data;

		t_geo_point *pa = edge->a;
		t_geo_point *pb = edge->b;
		int a = pa->indice;
		int b = pb->indice;
		*i = a;
		*(i+1) = b;
		i+=2;
	}

	vlst->type_target = dt_edge;
}

void geo_array_duplicate_elements( t_geo_array *array, float *point)
{
	switch( array->type_element)
	{
		case dt_geo_point: list_add( array->elements, geo_point_duplicate( ( t_geo_point *) array->element, point)); break;
		case dt_geo_edge: list_add( array->elements, geo_edge_duplicate( ( t_geo_edge *) array->element, point)); break;
		case dt_geo: list_add( array->elements, geo_duplicate( ( t_geo *) array->element, point)); break;
		default: printf("warning geo_array_build_element\n"); break;
	}
}

void geo_array_build( t_geo_array *array)
{
	t_context *C = ctx_get();
	if(C->scene->debug_all) printf("geo_array_build %s\n", array->id.name);
	C->ui->add_bricks = 0;

	if( array->element)
	{
		if( array->vector)
		{
			int i;
			int count = array->count;
			if( count)
			{
				// Reset
				geo_array_reset( array);

				// Build Points
				float *points = geo_array_build_circle( array);
				float *p = points;

				// Build Elements
				for(i=0;i<count;i++)
				{
					geo_array_duplicate_elements( array, p);
					p+=3;
				}

				// Free
				free(points);
			}
		}
	}

	C->ui->add_bricks = 1;
}
