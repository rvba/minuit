/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include "image.h"
#include "node.h"
#include "memory.h"
#include "data.h"
#include "vlst.h"
#include "file.h"
#include "scene.h"
#include "file.h"
#include "ctx.h"


GLenum image_gl_color_type( t_image *image)
{
	switch( image->color_type)
	{
		case IMG_BITMAP:
			return GL_ALPHA;
			break;
		case IMG_GRAYSCALE:
			if( image->alpha) return GL_LUMINANCE_ALPHA;
			else return GL_LUMINANCE; 
			break;
		case IMG_RGB:
			if( image->alpha) return GL_RGBA;
			else return GL_RGB;
			break;
		case IMG_COLOR:
			printf("[IMAGE] Color type not set\n");
			return GL_FALSE;
			break;
	}
	return GL_FALSE;
}

GLenum image_gl_data_type( t_image *image)
{
	switch( image->data_type)
	{
		case IMG_FLOAT: return GL_FLOAT; break;
		case IMG_BYTE: return GL_UNSIGNED_BYTE; break;
		case IMG_DATA:
			printf("[IMAGE] Data type not set\n");
			return GL_FALSE;
			break;
	}
	return GL_FALSE;
}

t_image_orientation image_get_orientation( t_image *image)
{
	if( image->width == image->height) return IMG_SQUARE;
	else if( image->width > image->height) return IMG_LANSCAPE;
	else return IMG_PORTRAIT;
}

const char *image_get_color_type( t_image *image)
{
	switch( image->color_type)
	{
		case IMG_RGB: return "RGB"; break;
		case IMG_GRAYSCALE: return "Grayscale"; break;
		case IMG_BITMAP: return "Bitmap"; break;
		case IMG_COLOR: return "Not Set"; break;
	}

	return NULL;
}

void image_get_filename( const char *path, char *name)
{
	t_file *file = file_new( path);
	file_init( file);
	s_cp( name, file->file_name, _NAME_);
	file_free( file);
}

void image_show( t_image *image)
{
	printf("Image\n");
	id_show( &image->id);
	printf("width: %d\n", image->width);
	printf("height: %d\n", image->height);
	printf("color_type: %s\n", image_get_color_type( image));
	printf("alpha: %d\n", image->alpha);
	printf("bpp: %d\n", image->bpp);
}

void image_rebind( struct Scene *scene, void *ptr)
{
	t_image *image = ( t_image *) ptr;
	rebind( scene, "image", "vlst", (void **) &image->vlst);
}

void image_data_convert( t_image *image, t_data_type type)
{
	t_vlst *vlst = image->vlst;
	if(vlst)
	{
		if( type == dt_uchar && vlst->type == dt_float)
		{
			vlst_map( vlst, dt_uchar, 0, 255);
		}
		else printf("[Image] data_convert Not implemented for %s\n", data_name_get( type));
	}
}

t_image *image_copy( t_image *image_src)
{
	t_image *image = image_new( image_src->id.name);

	image->width = image_src->width;
	image->height = image_src->height;
	image->bpp = image_src->bpp;
	image->size = image_src->size;
	image->vlst = vlst_copy( image_src->vlst);
	image->color_type = image_src->color_type;
	image->data_type = image_src->data_type;
	image->file_type = image_src->file_type;

	return image;
}

t_image *image_open( const char *path)
{
	t_image *image = NULL;
	t_file *file = file_new( path);
	file_init( file);

	if( iseq( file->ext, "jpg"))
	{
		#ifdef HAVE_JPG
	     	image = img_read_jpg( path);
		#else
		printf("[WARNING] image_open: JPG not enabled\n");
		#endif
	}
	else if( iseq( file->ext, "png"))
	{
		#ifdef HAVE_PNG
	       	image = img_read_png( path);
		#else
		printf("[WARNING] image_open: PNG not enabled\n");
		#endif
	}
	else printf("[WARNING] image_open, unknown image type:%s\n", file->ext);

	file_free( file);

	return image;
}

void image_free( t_image *image)
{
	mem_free( image, sizeof( t_image));
}

void image_delete( t_image *image)
{
	if( image->vlst) vlst_delete( image->vlst);
	image_free( image);
}

void *image_new( const char *name)
{
	t_image *image = ( t_image *) mem_malloc( sizeof( t_image));

	id_init( &image->id, name);

	image->width=0;
	image->height=0;
	image->bpp=0;
	image->vlst=NULL;
	image->alpha = 0;
	image->size = 0;
	image->color_type = IMG_COLOR;
	image->data_type = IMG_DATA;
	image->file_type = IMG_FILE;

	return image;
}
