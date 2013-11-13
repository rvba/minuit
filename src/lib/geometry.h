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

}t_geo_point;

typedef struct Geo_Edge
{
	struct Geo_Point *a;
	struct Geo_Point *b;
	struct Lst *faces;
	int indice;

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

t_geo *geo_make(const char *name);
t_geo *geo_new(const char *name);
void geo_free(t_geo *geo);


#endif
