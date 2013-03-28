/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "minuit.h"

#ifndef __TEXTURE_H
#define __TEXTURE_H

typedef struct Texture t_texture;

struct Texture
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	int idcol[3];

	GLuint width;
	GLuint height;

	GLenum format;
	GLint internal_format;
	GLuint id_gl;
	
	GLubyte *texels;
};

t_texture*	texture_new(const char *name);
void 		texture_free(t_texture *texture);
void 		texture_image_bind(t_texture *texture,t_image *image);


#endif
