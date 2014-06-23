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
#include "vlst.h"
#include "file.h"
#include "scene.h"

void image_rebind( struct Scene *sc, void *ptr)
{
}

t_image *image_copy( t_image *image_src)
{
	t_image *image = image_new( image_src->id.name);

	image->width = image_src->width;
	image->height = image_src->height;
	image->bpp = image_src->bpp;
	image->size = image_src->size;
	image->vlst = vlst_copy( image_src->vlst);

	return image;
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
	mem_free( image, sizeof( t_image));
}

void image_delete( t_image *image)
{
	if( image->vlst) vlst_delete( image->vlst);
	image_free( image);
}

void *image_new(const char *name)
{
	t_image *image = (t_image *)mem_malloc(sizeof(t_image));

	id_init(&image->id, name);

	image->width=0;
	image->height=0;
	image->bpp=0;
	image->vlst=NULL;

	image->size = 0;

	return image;
}