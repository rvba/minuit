/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

// NEW

t_viewport *viewport_new(const char *name)
{
	t_viewport *viewport = (t_viewport *) malloc(sizeof(t_viewport));

	viewport->id=0;
	viewport->id_chunk=0;
	set_name(viewport->name,name);
	viewport->users=0;

	viewport->width = 0;
	viewport->height = 0;
	viewport->camera = NULL;

	return viewport;
}
