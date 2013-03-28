/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

/***		TEXTURE		***/

// FREE

void texture_free(t_texture *texture)
{
	if(texture->name) free(texture->name);
	if(texture->texels) free(texture->texels);
}

// REBIND

void texture_image_bind(t_texture *texture,t_image *image)
{
	texture->width=image->width;
	texture->height=image->height;

	texture->texels=image->data;
	texture->internal_format=image->bpp;
	texture->format=image->format;
}

// NEW

t_texture *texture_new(const char *name)
{
	t_texture *texture = (t_texture *)malloc(sizeof(t_texture));

	texture->id=0;
	texture->id_chunk=0;

	set_name(texture->name,name);
	texture->users=0;

	texture->width=0;
	texture->height=0;
	texture->texels=NULL;
	texture->internal_format=0;
	texture->format=0;
	texture->id_gl=0;

	return texture;
}
