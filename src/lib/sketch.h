/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __SKETCH_H
#define __SKETCH_H

#define SKT_WITH_SCALE 1  //see EVENT
#define SKT_SCALE 1
#define SKT_COLOR_FRONT 0
#define SKT_COLOR_BACK 1
#define SKT_COLOR_BUFFER 0
#define SKT_POINT_SIZE 2
#define SKT_LINE_WIDTH_FRONT 1
#define SKT_LINE_WIDTH_BACK 20
#define SKT_INTENSITY 1

#include "util.h"

typedef struct Sketch t_skt;
typedef enum t_skt_buffer skt_buffer;

struct Sketch
{
	float color[3];
	float point_color[3];

	// line
	int line_width;

	//point
	int point_width;
	float point_size;
	float intensity;

	//scale
	int with_scale;
	float scale;
};

// NEW

t_skt *skt_new(void);

// COLOR

void skt_col(void);
void skt_col_swap(void);
void skt_col_show(void);
void skt_col_set(float r, float g,float b);
void skt_tint_set(const char tint[]);
void skt_color(float r,float g,float b,float a);
void skt_tint(const char tint[]);

// LINE

void skt_line_width_set(int width);
void skt_line(float *v1,float *v2,int width,float *color);
void skt_polyline(float *points,int tot);
void skt_closedline(float *points,int tot,float *color,int width);
void skt_closedline_filled(float *points,int tot,float *color,int width);

// POINT

void skt_point_size_set(float size) ;
void skt_point(float *pos,int width,float *color);

// VECTOR

void skt_vector(float *a,float *b,int width,float *color);

// SHAPES

void skt_line_rectangle(float *p,float w,float h,int line_width,float *color);
void skt_quadangle(float *a,float *b,float *c,float *d);
void skt_square(float *pos,float width);

// MSH

void skt_msh_square(float *pos,float width,float *color);
void skt_msh_rectangle(float *p,float w,float h,float *color);

// SPHERE

void skt_sphere(float radius,float sx,float sy);

t_skt *skt_get(void);

#endif
