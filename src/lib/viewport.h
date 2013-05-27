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


typedef struct Viewport t_viewport;

struct Viewport
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	t_generic_cls *cls;

	int width;
	int height;

	int x;
	int y;

	struct Camera *camera;
};

void viewport_draw(t_viewport *viewport);
t_viewport *viewport_rebind(t_scene *scene, void **ptr);
t_node *viewport_add(const char *name);
void viewport_free(t_viewport *viewport);
t_viewport *viewport_new(const char *name);
t_node *viewport_make(const char *name);


#endif

