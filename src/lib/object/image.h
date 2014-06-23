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

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Scene;
struct VLst;

typedef struct Image
{
	t_id id;

	int width;
	int height;
	int bpp; 		//byte per pixel
	int format;
	int type;

	size_t size;
	struct VLst *vlst;

}t_image;

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
