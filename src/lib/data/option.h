/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __OPTION_H
#define __OPTION_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


struct Scene;

typedef struct Option
{
	t_id id;
	short users;
	char name[_NAME_];

	// app

	int window_width_def;
	int window_height_def;

	int window_width;
	int window_height;

	float timer_add_low;

	// skecth

	float skt_color[3];
	float skt_point_color[3];
	int skt_line_width;
	int skt_point_width;
	float skt_point_size;
	int skt_with_scale;
	float skt_scale;
	int skt_point_smooth;

	// draw

	int draw_background_color[4];
	float draw_front_color[4];
	int draw_with_ui;
	int draw_with_clear;
	int draw_with_normal;
	int draw_with_texture;
	int draw_with_material;
	int draw_with_point;
	int draw_with_point_id;
	int draw_with_face;
	int draw_with_highlight;
	int draw_with_light;
	int draw_with_depth;
	int draw_with_polygon_offset;
	int draw_with_alpha;
	int draw_with_grid;
	int draw_divx;
	int draw_divy;
	int draw_show_axis_world;
	int draw_lights;

	int fullscreen;
	int color;

}t_option;

// OPTION

void option_free(t_option *option);
void * option_new(const char *name);
t_option *option_rebind(struct Scene *scene, void *ptr);

// OPTION

void option_free(t_option *option);
t_option *option_rebind(struct Scene *scene, void *ptr);

#ifdef __cplusplus
}
#endif

#endif
