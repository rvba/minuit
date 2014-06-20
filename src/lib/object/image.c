/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "image.h"
#include "node.h"
#include "memory.h"
#include "file.h"

void		image_rebind( struct Scene *sc, void *ptr)
{
}


t_image *image_open( const char *path)
{
	t_image *image = NULL;
	t_file *file = file_new( path);
	file_init( file);

	if( is( file->ext, "jpg")) image = img_read_jpg( path);
	else printf("[WARNING] image_open, unknown image type:%s\n", file->ext);

	file_free( file);

	return image;
}

void image_free(t_image *image)
{
}

void *image_new(const char *name)
{
	t_image *image = (t_image *)mem_malloc(sizeof(t_image));

	id_init(&image->id, name);

	image->width=0;
	image->height=0;
	image->bpp=0;
	image->data=NULL;

	image->size = 0;

	return image;
}
