/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __DRAW_H
#define __DRAW_H

#include "opengl.h"

#define SEL_LINE_WIDTH 200

#define DRAW_SELECTION_PASS 1
#define DRAW_MODE 1

#define DRAW_COLOR color_black
#define DRAW_COLOR_BACKGROUND_RED 255
#define DRAW_COLOR_BACKGROUND_GREEN 255
#define DRAW_COLOR_BACKGROUND_BLUE 255
#define DRAW_COLOR_BACKGROUND_ALPHA 0

#define DRAW_SHOW_AXIS_WORLD 0
#define DRAW_WITH_SCALE 0
#define DRAW_SCALE 1
#define DRAW_WITH_CLEAR 1
#define DRAW_WITH_SELECTION 1
#define DRAW_WITH_POINT 0
#define DRAW_WITH_EDGE 0
#define DRAW_WITH_EDGE_COLOR 0
#define DRAW_WITH_POINT_ID 0;
#define DRAW_WITH_FACE 1;
#define DRAW_WITH_FACE_OUTLINE 0;
#define DRAW_WITH_DEPTH 1;
#define DRAW_WITH_POLYGON_OFFSET 1;
#define DRAW_WITH_NORMAL 1
#define DRAW_WITH_TEXTURE 1
#define DRAW_WITH_HIGHLIGHT 1
#define DRAW_WITH_LIGHT 1
#define DRAW_WITH_ALPHA 1
#define DRAW_WITH_MATERIAL 1
#define DRAW_WITH_DRAW 1
#define DRAW_WITH_UI 1
#define DRAW_WITH_BLEND 1

struct Context;
struct Scene;
struct Mesh;

typedef enum Draw_Mode
{
	mode_selection,
	mode_draw

}t_draw_mode;

typedef enum Color_Mode 
{
	color_white,
	color_black

}t_color_mode;

typedef struct Draw 
{
	int background_color[4];
	float front_color[4];
	float back_color[4];

	t_draw_mode mode;
	t_color_mode color;

	int mode_direct;

	int with_selection_pass; //with_color
	int with_draw_pass;
	int with_ui;

	int with_clear;
	int with_normal;
	int with_texture;
	int with_material;
	int with_point;
	int with_point_id;
	int with_face;
	int with_edge;
	int with_edge_color;
	int with_face_outline;
	int with_blend;
	int with_restrict_matrix;
	int with_shaders;
	int with_scene;

	int with_highlight;
	int with_light;
	int with_depth;
	int with_polygon_offset;
	int with_alpha;
	int with_grid;
	int divx;
	int divy;
	int usex;
	int usey;

	int show_axis_world;
	int draw_lights;

	int edit_mode;

}t_draw;

void draw_render_start(struct Context *C);
void draw_render_stop(struct Context *C);
void draw_clear(float r,float g,float b,float a);
t_draw *draw_new(void);
void draw_init(t_draw *draw);
void draw_scene(t_draw *draw, struct Scene *scene);
void draw_switch_axis_world(t_draw *draw);
void draw_mesh_direct(t_draw *draw,struct Scene *scene,struct Mesh *mesh);
void draw_mesh(t_draw *draw, struct Scene *scene, struct Mesh *mesh);
void type_font_3d(char* string,float pos[3]);

void draw_switch_scene( t_draw *draw);

#endif
