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

#include "minuit.h"

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
	t_camera *camera;
};


t_viewport *viewport_new(const char *name);


#endif

