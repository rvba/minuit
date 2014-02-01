/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __GEOMETRY_EXTRUDE_H
#define __GEOMETRY_EXTRUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#define ST_DB_POINT_ADD 0
#define ST_DB_POINT_EXTRUDE 0

struct Context;
struct Geometry;
struct Lst;
struct Block;
struct Vector;
struct VLst;

// GEOMETRY

typedef struct Geo_Point
{
	t_id id;

	float pos[3];
	float color[3];
	int indice;
	int is_virtual;

}t_geo_point;

typedef struct Geo_Edge
{
	t_id id;
	struct Geo_Point *a;
	struct Geo_Point *b;
	int indice;
	int is_virtual;

}t_geo_edge;

typedef struct Geo_Face
{
	t_id id;
	struct Geo_Face *child;

	struct Geo_Point *a;
	struct Geo_Point *b;
	struct Geo_Point *c;
	struct Geo_Point *d;

	struct Geo_Edge *ab;
	struct Geo_Edge *bc;
	struct Geo_Edge *cd;
	struct Geo_Edge *da;

	int is_virtual;

}t_geo_face;

typedef struct Geo_Array
{
	t_id id;
	t_data_type type_element;
	int count;
	void *element;
	struct Lst *elements;
	struct Vector *vector;

	int is_virtual;

	struct Block *block;

}t_geo_array;

typedef struct Geometry
{
	t_id id;

	struct Lst *points;
	struct Lst *edges;
	struct Lst *faces;

	int use_final_pos;
	int selected;
	int is_virtual;

	struct Block *block;

}t_geo;

void geo_point_vector_update(t_geo_point *point, struct Vector *vector);

void geo_point_show(t_geo_point *point);

// GEO


t_geo *geo_rebind( t_scene *sc, void *data);
void geo_show(t_geo *geo);
t_geo *geo_make( struct Context *C, const char *name);
t_geo *geo_new(const char *name);
void geo_free(t_geo *geo);

// POINT

t_geo_point *geo_point_rebind( t_scene *sc, void *data);
void geo_point_free(t_geo_point *point);
t_geo_point *geo_point_make( struct Context *C, const char *name);
t_geo_point *geo_point_new(const char *name);

// EDGE

t_geo_edge *geo_edge_rebind( t_scene *sc, void *data);
void geo_edge_free(t_geo_edge *edge) ;
t_geo_edge *geo_edge_make( struct Context *C, const char *name);
t_geo_edge *geo_edge_new(const char *name);

// ARRAY

void *geo_array_get_ref(t_geo_array *array, const char *ref);
t_geo_array *geo_array_rebind( t_scene *sc, void *data);
void geo_array_build( t_geo_array *array);
void geo_array_free(t_geo_array *array);
t_geo_array *geo_array_make( struct Context *C, const char *name);
t_geo_array *geo_array_new(const char *name);

void geo_array_get_edges( t_geo_array *array, t_lst *lst);
void geo_array_get_points( t_geo_array *array, t_lst *lst);


void geo_data_set(t_geo *geo, t_data_type type, struct Lst *points);
void geo_vlst_set(t_geo *geo, t_data_type type,  struct VLst *vlst);
void geo_vlst_points_set( struct Brick *brick, struct Lst *lst, struct VLst *vlst);
void geo_vlst_edges_set( struct Brick *brick, struct Lst *lst, struct VLst *vlst);

void geo_reset(t_geo *geo);
void geo_array_init( t_geo_array *array);
void geo_init( t_geo *geo);

#ifdef __cplusplus
}
#endif

#endif
