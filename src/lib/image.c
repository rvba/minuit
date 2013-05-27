/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "image.h"
#include "util.h"


void image_free(t_image *image)
{
}

t_image *image_new(const char *name)
{
	t_image *image = (t_image *)malloc(sizeof(t_image));

	set_name(image->name,name);
	image->id=0;
	image->id_chunk=0;

	image->width=0;
	image->height=0;
	image->bpp=0;
	image->data=NULL;

	return image;
}
