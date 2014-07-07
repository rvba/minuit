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

typedef enum Image_File
{
	IMG_FILE,
	IMG_PPM,
	IMG_JPG,
	IMG_PNG

}t_image_file;

typedef enum Image_Data
{
	IMG_DATA,
	IMG_FLOAT,
	IMG_BYTE

}t_image_data;

typedef enum Image_Color
{
	IMG_COLOR,
	IMG_BITMAP,
	IMG_GRAYSCALE,
	IMG_RGB

}t_image_color;

typedef enum Image_Orientation
{
	IMG_ORIENTATION,
	IMG_PORTRAIT,
	IMG_LANSCAPE,
	IMG_SQUARE

}t_image_orientation;

typedef struct Image
{
	t_id id;

	int width;
	int height;
	int bpp; 		//byte per pixel
	int alpha;

	t_image_color color_type;
	t_image_data data_type;
	t_image_file file_type;

	size_t size;
	struct VLst *vlst;

}t_image;

GLenum 		image_gl_color_type( t_image *image);
GLenum 		image_gl_data_type( t_image *image);

void image_get_filename( const char *path, char *name);

t_image_orientation image_get_orientation( t_image *image);
void 		image_show( t_image *image);
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
t_image *	img_read_png( const char *filename);

int 		img_save_png( t_image *image);
int 		img_save_png_hd(int alpha,int width,int height, const char name[],unsigned char *bitmap);
void 		img_save_video(int width, int height, const char *name,unsigned char *image);

#ifdef __cplusplus
}
#endif

#endif
