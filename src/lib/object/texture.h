/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __TEXTURE_H
#define __TEXTURE_H

#include "base.h"

#include <GL/gl.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Scene;
struct Image;

typedef struct Texture
{
	t_id id;

	int idcol[3];
	size_t size;

	GLuint width;
	GLuint height;

	GLenum format;
	GLenum type;
	GLint internal_format;
	GLuint id_gl;
	
	GLubyte *texels;

}t_texture;


void 		texture_update( t_texture *texture, struct Image *image);
void 		texture_show( t_texture *texture);
void 		texture_image_load( struct Texture *texture, struct Image *image);
t_texture *	texture_clone(t_texture *texture);
void	 	texture_rebind(struct Scene *scene, void *ptr);
void *		texture_new(const char *name);
void 		_texture_free(t_texture *texture);
void 		texture_free(t_texture *texture);
void 		texture_image_bind(t_texture *texture,struct Image *image);

#ifdef __cplusplus
}
#endif

#endif
