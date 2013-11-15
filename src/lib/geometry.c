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
#include "system.h"
#include "list.h"
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
	printf("vector:");
	vprint3f(point->vector,'\n');
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

t_geo *geo_new(const char *name)
{
	t_geo *geo = malloc(sizeof(t_geo));

	geo->points = NULL;
	geo->edges = NULL;
	geo->faces = NULL;

	geo->selected = 0;

	id_init(&geo->id, name);

	return geo;
}

// NEW POINT 

t_geo_point *geo_point_new(const char *name)
{
	t_geo_point *point = malloc(sizeof(t_geo_point));

	set_name(point->id.name, name);

	point->parent = NULL;
	point->child = NULL;
	point->edges = lst_new("edges");
	vset3f(point->pos,0,0,0);
	vset3f(point->vector,0,0,0);
	point->is_moving = 0;
	point->velocity = 0;
	point->indice = 0;

	return point;
}

// NEW EDGE 

t_geo_edge *geo_edge_new(const char *name)
{
	t_geo_edge *edge = malloc(sizeof(t_geo_edge));

	set_name(edge->id.name, name);

	edge->a = NULL;
	edge->b = NULL;
	edge->faces = lst_new("faces");

	return edge;
}

// NEW FACE 

t_geo_face *geo_face_new(void)
{
	t_geo_face *face = malloc(sizeof(t_geo_face));
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
	t_geo_array *array = malloc(sizeof(t_geo_array));

	set_name( array->id.name, name);

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

void geo_point_free(t_geo_point *point)
{
	if(point->edges)
	{
		lst_free(point->edges);
	}

	free(point);
}

void geo_edge_free(t_geo_edge *edge) 
{
	if(edge->faces)
	{
		lst_free(edge->faces);
	}

	free(edge);
}

void geo_face_free(t_geo_face *face)
{
	free(face);
}

void geo_array_free(t_geo_array *array)
{
	if(array->elements) lst_free(array->elements);
}

void geo_free(t_geo *geo)
{
	if(geo->points) lst_free(geo->points);
	if(geo->edges) lst_free(geo->edges);
	if(geo->faces) lst_free(geo->faces);
}

/*	*************************************************************	
 *		:RESET
 *	*************************************************************
 */

void geo_reset(t_geo *geo)
{
	if(geo->points)
	{
		lst_cleanup( geo->points);
	}

	if(geo->edges)
	{
		lst_cleanup( geo->edges);
	}
}


/*	*************************************************************	
 *		:MESH
 *	*************************************************************
 */


// UPDATE MESH POINT

void geo_mesh_point_update(t_geo *geo, t_mesh *mesh, t_geo_point *point)
{
	t_vlst *vlst = mesh->vertex;
	float *vertex = vlst->data;
	float *pos = point->pos;
	int indice = point->indice*3;

	srf_float(vertex,pos+0,indice+0);
	srf_float(vertex,pos+1,indice+1);
	srf_float(vertex,pos+2,indice+2);
}

/*	*************************************************************	
 *		:UDPATE
 *	*************************************************************
 */

void geo_point_vector_update(t_geo_point *point, t_vector *vector)
{
	t_vlst *vlst = vector->vector;
	float *points = vlst->data;
	vcp(point->pos,points);
}

/*	*************************************************************	
 *		:ADD
 *	*************************************************************
 */


// ADD GEOMETRY

t_geo *geo_add(const char *name)
{
	// New Geo
	t_geo *geo = geo_new(name);

	// Lsts
	geo->points = lst_new("points");
	geo->edges = lst_new("edges");
	geo->faces = lst_new("faces");

	return geo;
}

void geo_point_pos_set( t_geo_point *point, float *p)
{
	vcp(point->pos, p);
}

t_geo_point *geo_add_point(t_geo *geo, float *p, float *v, int extrude, float *color)
{
	t_geo_point *point = geo_point_new("point");

	vcp(point->vector,v);
	vcp(point->color, color);
	lst_add(geo->points,point,"point");

	if(extrude)
	{

		point->is_moving = 1;
		vcp(point->pos,p);
	}
	else
	{

		vcp(point->pos,v);
	}

	if(ST_DB_POINT_ADD)
	{
		printf("point add\n");
		geo_point_show(point);
	}

	return point;
}

int geo_point_equal(t_geo_point *a, t_geo_point *b)
{
	if(a->id.id == b->id.id) return 1;
	else return 0;
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

void geo_fill_point(t_geo *geo, t_geo_point *point)
{
	if(!geo_elem_exists(geo, dt_geo_point, point))
	{
		point->indice = geo->points->count;
		lst_add(geo->points,point,"point");
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

				lst_add(geo->edges,edge,"edge");
			}
		}
	}
}

void geo_point_edge_add(t_geo_point *point, t_geo_edge *edge)
{
	lst_add(point->edges, edge, "edge");
}

t_geo_edge *geo_edge_add(t_geo *geo, t_geo_point *a, t_geo_point *b, float *color)
{
	// New Edge
	t_geo_edge *edge = geo_edge_new("edge");
	edge->a = a;
	edge->b = b;

	// Link Edges to Points
	geo_point_edge_add(a,edge);
	geo_point_edge_add(b,edge);

	// Set Indice
	edge->indice = geo->edges->count;

	// Add to Edges
	lst_add(geo->edges,edge,"edge");

	return edge;
}

t_geo_point *geo_point_duplicate( t_geo_point *point)
{
	t_context *C = ctx_get();
	C->ui->add_bricks = 0;
	t_geo_point *point_new = geo_point_make( "point");
	vcp(point_new->pos, point->pos);
	C->ui->add_bricks = 1;
	return point_new;
}

t_geo_edge *geo_edge_duplicate( t_geo_edge *edge)
{
	t_context *C = ctx_get();
	C->ui->add_bricks = 0;
	t_geo_edge *edge_new = geo_edge_make("edge");
	edge_new->a = geo_point_duplicate(edge->a);
	edge_new->b = geo_point_duplicate(edge->b);
	C->ui->add_bricks = 1;
	return edge_new;
}

t_geo *geo_duplicate( t_geo *geo)
{
	t_context *C = ctx_get();
	C->ui->add_bricks = 0;
	t_geo *geo_new = geo_make("geo");

	t_link *l;
	t_geo_edge *edge;
	t_geo_edge *edge_new;
	t_geo_point *a;
	t_geo_point *b;
	for(l=geo->edges->first;l;l=l->next)
	{
		edge = l->data;
		edge_new = geo_edge_duplicate( edge);
		lst_add( geo_new->edges, edge_new, "edge");
		a = edge_new->a;
		b = edge_new->b;
		lst_add( geo_new->points, a, "point");
		lst_add( geo_new->points, b, "point");

	}
	C->ui->add_bricks = 1;
	return geo_new;
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

void geo_edge_rotate( t_geo_edge *edge, float *v)
{
	/*
	float center[3];
	float v[3];
	float *a = edge->a->pos;
	float *b = edge->b->pos;
	vsub(v,b,a);
	*/
	
}

void geo_face_add(t_geo *geo, t_geo_point *a, t_geo_point *b, t_geo_point *c, t_geo_point *d)
{
	// New Face
	t_geo_face *face = geo_face_new();
	face->a = a;
	face->b = b;
	face->c = c;
	face->d = d;

	// Add To Faces
	lst_add(geo->faces, face, "face");

}


/*	*************************************************************	
 *		:GET
 *	*************************************************************
 */

// EDGE EQUAL 

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

// FIND CHILDS

t_lst *geo_get_childs(t_geo *geo)
{
	t_lst *points = geo->points;
	t_link *l;
	t_geo_point *point;
	if(points)
	{
		t_lst *lst = lst_new("lst");
		for(l=points->first;l;l=l->next)
		{
			point = l->data;
			if(!point->child)
			{
				lst_add(lst,point,"child");
			}
		}

		return lst;
	}
	else
	{
		return NULL;
	}
}

// GET EDGES

t_geo_edge *geo_get_edge_from_points(t_geo_point *a, t_geo_point *b)
{
	t_link *l;
	t_geo_edge *edge = NULL;
	for(l=a->edges->first;l;l=l->next)
	{
		edge = l->data;
		if(geo_edge_equal(edge,a->indice,b->indice))
		{
			return edge;
		}
	}
	return NULL;
}



/*	*************************************************************	
 *		:EXTRUDE
 *	*************************************************************
 */

float *geo_get_point_vector(t_geo *geo, t_geo_point *point)
{
	return point->vector;
}


// EXTRUDE POINT

t_geo_point *geo_extrude_point(t_geo *geo, t_geo_point *a, float *v, float *color)
{
	float *pos = geo_get_point_vector(geo,a);

	float vector[3];

	vadd(vector,a->vector,v);

	t_geo_point *b = geo_add_point(geo,pos,vector,1,color);
	geo_edge_add(geo,a,b,color);
	a->child = b;

	if(ST_DB_POINT_EXTRUDE)
	{
		printf("[POINT EXTRUDE]\n");
		printf("SRC\n");
		geo_point_show(a);
		printf("DST\n");
		geo_point_show(b);
	}

	return b;
}

// EXTRUDE EDGE

t_geo_edge *geo_edge_extrude(t_geo *geo, t_geo_edge *edge, float *v, float *color)
{
	t_geo_point *a = edge->a;
	t_geo_point *b = edge->b;

	float va[3];
	float vb[3];

	float *pos_a = geo_get_point_vector(geo, a);
	float *pos_b = geo_get_point_vector(geo, b);

	vadd(va,pos_a,v);
	vadd(vb,pos_b,v);

	t_geo_point *c = geo_extrude_point(geo,b,vb,color);
	t_geo_point *d = geo_extrude_point(geo,a,va,color);

	t_geo_edge *new_edge = geo_edge_add(geo, c, d, color);
	(void)new_edge;
	geo_face_add(geo,a,b,c,d);

	return new_edge;
}

// EXTRUDE FACE
// TDO: Doubles edges !!

void geo_face_extrude(t_geo *geo, t_geo_face *face, float *v, float *color)
{
	t_geo_point *a = face->a;
	t_geo_point *b = face->b;
	t_geo_point *c = face->c;
	t_geo_point *d = face->d;

	t_geo_edge *ab = geo_get_edge_from_points(a,b);
	t_geo_edge *bc = geo_get_edge_from_points(b,c);
	t_geo_edge *cd = geo_get_edge_from_points(c,d);
	t_geo_edge *da = geo_get_edge_from_points(d,a);

	t_geo_edge *_ab;
	t_geo_edge *_bc;
	t_geo_edge *_cd;
	t_geo_edge *_da;

	t_geo_point *_a;
	t_geo_point *_b;
	t_geo_point *_c;
	t_geo_point *_d;

	float vv[3];

	if(ab && bc && cd && da)
	{
		vector_build(vv,a->pos,v);
		_ab = geo_edge_extrude(geo,ab,vv,color);

		vector_build(vv,b->pos,v);
		_bc = geo_edge_extrude(geo,bc,vv,color);

		vector_build(vv,c->pos,v);
		_cd = geo_edge_extrude(geo,cd,vv,color);

		vector_build(vv,d->pos,v);
		_da = geo_edge_extrude(geo,da,vv,color);

		_a = _ab->a;
		_b = _bc->a;
		_c = _cd->a;
		// inverted
		_d = _da->b;

		geo_face_add(geo,_a,_b,_c,_d);
	}
	else
	{
		printf("errrrr\n");
	}
}

/*	*************************************************************	
 *		:ANIMATE
 *	*************************************************************
 */


// MOVE

void geo_point_move(t_geo *geo, t_geo_point *point)
{
	float *src = point->pos;
	float *dst = point->vector; 
	float vec[3];
	float v[3];
	vsub(vec,dst,src);

	if(vmag(vec) < 0.0001f)
	{
		point->is_moving = 0;
	}
	else
	{
		vmul(vec,point->velocity);
		vadd(v,point->pos,vec);
		vcp(point->pos,v);
	}
}


// UPDATE DATA

void geo_update_data(t_geo *geo)
{
	t_link *l;
	t_geo_point *point;

	// MOVE POINTS
	if(geo->points)
	{
		//lst_show(geo->points);
		for(l=geo->points->first;l;l=l->next)
		{
			point = l->data;
			if(point->is_moving)
			{
				geo_point_move(geo,point);
			}
		}
	}
}

/*	*************************************************************	
 *		:OPERATORS
 *	*************************************************************
 */


void geo_extrude_circle(t_geo *geo, t_geo_point *point, double radius, int count)
{
	float center[] = {0,0,0};
	float *circle = calc_circle(center, radius, count);
	int i;

	float cc[3];
	for(i=0; i < count; i++)
	{
		vadd(cc,center,circle+(i*3));
		geo_extrude_point(geo,point,cc,_WHITE_);
	}

	free(circle);
}

void geo_extrude_last(t_geo *geo, float *vector, float *color)
{
	t_lst *lst = geo->points;

	t_link *l;
	t_geo_point *a = NULL;
	for(l=lst->first;l;l=l->next)
	{
		a = l->data;
	}

	geo_extrude_point(geo,a,vector,color);
}

void geo_extrude_simple_edge(t_geo *geo)
{
	if(geo->edges->count == 0)
	{
		float v[]={1,0,0};
		geo_extrude_last(geo,v,_WHITE_);
	}
	else if(geo->faces->count == 0)
	{
		t_geo_edge *edge = geo->edges->first->data;
		float v[3]={0,1,0};
		geo_edge_extrude(geo,edge,v,_WHITE_);
	}
	else
	{
		float v[]={0,0,1};
		t_geo_face *face = geo->faces->first->data;
		geo_face_extrude(geo,face,v,_WHITE_);
	}
}

void geo_extrude_spirale(t_geo *geo, t_geo_point *point, int n, float radius)
{
	int i;
	float v[3];
	float r=.5;
	t_geo_point *p = point;
	t_geo_point *last = point;
	for(i=0;i<n;i++)
	{
		v[0] = cos(i*r) * radius;
		v[1] = sin(i*r) * radius;
		v[2] = 0;

		radius += .1;

		p = geo_extrude_point(geo,last,v,_WHITE_);
		last = p;
	}
}

void geo_extrude_cone(t_geo *geo, t_geo_point *point, int step, float radius, int start, int end)
{
	float *center = point->vector;
	float *arc = calc_arc(center,radius,step,start,end);
	int i;
	float *v;

	// Add First Point
	t_geo_point *p = geo_add_point(geo,center,center,0,_WHITE_);

	for(i=0;i<step;i++)
	{
		v = arc + (i*3);
		geo_extrude_point(geo, p, v, _WHITE_);
	}
}

void geo_add_cone(t_geo *geo, float *origin, int step, float radius, int start, int end)
{
	t_geo_point *point = geo_add_point(geo,origin,origin,0,_WHITE_);
	geo_extrude_cone(geo, point, step, radius, start, end);
}

void geo_add_circle(t_geo *geo, float *center, int step, float radius)
{
	float *circle = calc_circle(center,radius,step);
	float *p;
	float *po;
	float v[3];
	int i;

	// Add First Point
	t_geo_point *point = geo_add_point(geo,circle,circle,0,_WHITE_);
	t_geo_point *new_point;

	for(i=0;i<step;i++)
	{
		if((i == step -1))
		{
			p = circle;
		}
		else
		{
			p = circle + ((i+1) * 3);
		}

		po = point->vector;

		vsub(v,p,po);

		new_point = geo_extrude_point(geo, point, v, _WHITE_);
		point = new_point;
	}
}

void geo_add_parallel(t_geo *geo, int count, float length, float offset, float *origin, float *dst)
{
	t_geo_point *point;
	float p[3];
	float perp[3];

	vcp(p,origin);

	vcp(perp,dst);
	vrot2d3f(perp,90);
	vnormalize(perp);
	vmul(perp,offset);
	vmul(dst,length);
	
	int i;
	for(i=0;i<count;i++)
	{
		point = geo_add_point(geo,p,p,0,_WHITE_);
		geo_extrude_point(geo, point, dst, _WHITE_);
		vadd(p,p,perp);
	}
}

void geo_add_rectangle(t_geo *geo, float *origin, float width, float height)
{
	float vwidth[] = { width, 0, 0};
	float _vwidth[] = {-width, 0, 0};
	float vheight[] = { 0, height, 0};
	float _vheight[] = { 0, -height, 0};

	t_geo_point *point;
	point = geo_add_point(geo, origin, origin, 0, _WHITE_);
	point = geo_extrude_point(geo, point, vwidth, _WHITE_);
	point = geo_extrude_point(geo, point, vheight, _WHITE_);
	point = geo_extrude_point(geo, point, _vwidth, _WHITE_);
	geo_extrude_point(geo, point, _vheight, _WHITE_);
}

void geo_add_star(t_geo *geo, float *origin, float radius, int count)
{
	t_geo_point *point = geo_add_point(geo, origin, origin, 0, _WHITE_);
	geo_extrude_circle(geo, point, radius, count);
}

t_geo_point *geo_get_last(t_geo *geo)
{
	t_link *l;
	t_geo_point *point = NULL;
	for(l=geo->points->first;l;l=l->next)
	{
		point = l->data;
	}

	return point;
}

t_geo *geo_make(const char *name)
{
	t_context *C = ctx_get();
	t_node *node_geo = scene_add(C->scene,nt_geo,name);
	t_geo *geo = node_geo->data;

	geo->points = lst_new("points");
	geo->edges = lst_new("edges");

	geo->points->type = dt_geo_point;
	geo->edges->type = dt_geo_edge;

	if(C->ui->add_bricks)
	{
		t_node *node = add_geometry(C,"data",geo);
		t_block *block = node->data;
		geo->ref = block;
	}

	return geo;
}

t_geo_point *geo_point_make(const char *name)
{
	t_context *C = ctx_get();
	t_node *node_geo_point = scene_add(C->scene,nt_geo_point,name);
	t_geo_point *geo_point = node_geo_point->data;

	if(C->ui->add_bricks)
	{
		t_node *node = add_geo_point(C,"point",geo_point);
		t_block *block = node->data;
		geo_point->ref = block;
	}

	return geo_point;
}

t_geo_edge *geo_edge_make(const char *name)
{
	t_context *C = ctx_get();
	t_node *node_geo_edge = scene_add(C->scene,nt_geo_edge,name);
	t_geo_edge *geo_edge = node_geo_edge->data;

	if(C->ui->add_bricks)
	{
		t_node *node = add_geo_edge(C,"point",geo_edge);
		t_block *block = node->data;
		geo_edge->ref = block;
	}

	return geo_edge;
}

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

void geo_array_reset( t_geo_array *array)
{
	if( array->type_element == dt_geo_point)
	{
		if( array->elements->count > 0)
		{
			t_link *l;
			t_geo_point *point;
			for(l=array->elements->first;l;l=l->next)
			{
				point = l->data;
				geo_point_free( point);
			}

			lst_cleanup( array->elements);
		}
	}

	else if( array->type_element == dt_geo_edge)
	{
		if( array->elements->count > 0)
		{
			t_link *l;
			t_geo_edge *edge;
			for(l=array->elements->first;l;l=l->next)
			{
				edge = l->data;
				if( edge->a) geo_point_free( edge->a);
				if( edge->b) geo_point_free( edge->b);
				geo_edge_free( edge);
			}

			lst_cleanup( array->elements);
		}
	}

	else if( array->type_element == dt_geo)
	{
		if( array->elements->count > 0)
		{
			t_link *l;
			t_geo *geo;
			for(l=array->elements->first;l;l=l->next)
			{
				geo = l->data;
				geo_free( geo);
			}

			lst_cleanup( array->elements);
		}
	}
}

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

void geo_array_build_element( t_geo_array *array, float *point)
{
	t_data_type type = array->type_element;

	if( type == dt_geo_point)
	{
		t_geo_point *geo_point = geo_point_make( "point");
		geo_point_pos_set( geo_point, point);
		lst_add( array->elements, geo_point, "point");
	}
	else if( type == dt_geo_edge)
	{
		t_geo_edge *edge = array->element;
		t_geo_edge *new_edge = geo_edge_duplicate( edge);
		geo_edge_transform( new_edge, point);
		lst_add( array->elements, new_edge, "edge");
	}
	else if( type == dt_geo)
	{
		t_geo *geo = array->element;
		t_geo *new_geo  = geo_duplicate( geo);
		geo_transform( new_geo, point);
		lst_add( array->elements, new_geo, "geo");
	}
}

void geo_array_build( t_geo_array *array)
{
	t_context *C = ctx_get();
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
					geo_array_build_element( array, p);
					p+=3;
				}

				// Free
				free(points);
			}
		}
	}
	C->ui->add_bricks = 1;
}

t_geo_array *geo_array_make(const char *name)
{
	t_context *C = ctx_get();
	t_node *node_geo_array = scene_add(C->scene,nt_geo_array,name);
	t_geo_array *geo_array = node_geo_array->data;

	geo_array->elements = lst_new("lst");

	if(C->ui->add_bricks)
	{
		add_geo_array(C, name, geo_array);
	}

	return geo_array;
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
