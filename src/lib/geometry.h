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

#define ST_DB_POINT_ADD 0
#define ST_DB_POINT_EXTRUDE 0

struct Geometry;
struct Lst;
struct Block;
struct Vector;
struct VLst;

// GEOMETRY

typedef struct Geo_Point
{
	t_id id;
	struct Geo_Point *parent;
	struct Geo_Point *child;

	struct Lst *edges;

	float pos[3];
	float vector[3];
	float color[3];
	int indice;
	int is_moving;
	float velocity;

	struct Block *ref;

}t_geo_point;

typedef struct Geo_Edge
{
	t_id id;
	struct Geo_Point *a;
	struct Geo_Point *b;
	struct Lst *faces;
	int indice;
	struct Block *ref;

}t_geo_edge;

typedef struct Geo_Face
{
	struct Geo_Face *child;

	struct Geo_Point *a;
	struct Geo_Point *b;
	struct Geo_Point *c;
	struct Geo_Point *d;

	struct Geo_Edge *ab;
	struct Geo_Edge *bc;
	struct Geo_Edge *cd;
	struct Geo_Edge *da;

}t_geo_face;

typedef struct Geometry
{
	t_id id;

	struct Lst *points;
	struct Lst *edges;
	struct Lst *faces;

	int use_final_pos;
	int selected;

	struct Block *ref;


}t_geo;

void geo_point_vector_update(t_geo_point *point, struct Vector *vector);

void geo_point_show(t_geo_point *point);

t_geo *geo_make(const char *name);
t_geo *geo_new(const char *name);
void geo_free(t_geo *geo);


void geo_point_free(t_geo_point *point);
t_geo_point *geo_point_make(const char *name);
t_geo_point *geo_point_new(const char *name);

void geo_edge_free(t_geo_edge *edge) ;
t_geo_edge *geo_edge_make(const char *name);
t_geo_edge *geo_edge_new(const char *name);

void geo_data_set(t_geo *geo, t_data_type type, struct Lst *points);
void geo_vlst_set(t_geo *geo, struct VLst *vlst);

#endif
