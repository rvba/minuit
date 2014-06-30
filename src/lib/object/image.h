/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __IMAGE_H
#define __IMAGE_H

#include <GL/gl.h>
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Scene;
struct VLst;

typedef enum Image_Data
{
	IMG_FLOAT,
	IMG_BYTE

}t_image_data;

typedef enum Image_Color
{
	IMG_BITMAP,
	IMG_GRAYSCALE,
	IMG_RGB

}t_image_color;

typedef struct Image
{
	t_id id;

	int width;
	int height;
	int bpp; 		//byte per pixel
	//int format;		// GL_RGB, GL_RGBA, GL_LULMINACE
	t_image_color color_type;
	//int type;
	t_image_data data_type;
	int alpha;

	size_t size;
	struct VLst *vlst;

}t_image;

GLenum 		image_gl_color_type( t_image *image);
GLenum 		image_gl_data_type( t_image *image);

void 		image_data_convert( t_image *image, t_data_type type);
void 		image_delete( t_image *image);
t_image *	image_copy( t_image *image_src);
void		image_rebind( struct Scene *sc, void *prt);
t_image *	image_open( const char *path);
void 		image_free(t_image *image);
void * 		image_new(const char *name);
t_image *	img_read_jpg(const char *path);
void 		img_save_jpg(int width,int height,const char name[]);
void 		img_save_jpg_highres(int width,int height,const char name[],unsigned char *image);
t_image *	img_read_png(char *filename);
int 		img_save_png(int alpha,int width,int height,unsigned char *bitmap, const char name[]);
int 		img_save_png_hd(int alpha,int width,int height, const char name[],unsigned char *bitmap);
void 		img_save_video(int width, int height, const char *name,unsigned char *image);

#ifdef __cplusplus
}
#endif

#endif
