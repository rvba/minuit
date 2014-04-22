/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __VIEWPORT_H
#define __VIEWPORT_H


#include "node.h"
#include "scene.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int opt_viewport_show_controls;

struct Brick;

typedef struct Viewport t_viewport;

struct Viewport
{
	t_id id;

	int width;
	int height;

	int x;
	int y;

	int show_controls;
	int show_outline;
	int fullscreen;
	int use_fullscreen;
	int use_ui;

	struct Camera *camera;
	void (* draw)(struct Viewport *viewport);
	struct Block *controls;
};

void viewport_register( struct Context *C);

void *op_viewport_set_dimensions( struct Brick *brick);

void 		viewport_switch_3d( t_viewport *viewport);
void 		viewport_switch_2d( t_viewport *viewport);
void 		viewport_add_controls( t_viewport *viewport);
void 		viewport_draw_controls( t_viewport *viewport);
void 		viewport_draw_outline( t_viewport *viewport);
void *		viewport_get_ref(t_viewport *viewport, const char *ref);
void 		viewport_draw_scene(t_viewport *viewport);
void 		viewport_draw(t_viewport *viewport);
void		viewport_rebind(t_scene *scene, void *ptr);
t_node *	viewport_add(const char *name);
t_viewport *	viewport_clone(t_viewport *viewport);
void 		_viewport_free(t_viewport *viewport);
void 		viewport_free(t_viewport *viewport);
t_viewport *	viewport_new(const char *name);
t_node *	viewport_make(const char *name);

#ifdef __cplusplus
}
#endif

#endif

