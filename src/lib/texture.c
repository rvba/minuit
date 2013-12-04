/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "node.h"
#include "scene.h"

#include "util.h"
#include "op.h"
#include "texture.h"
#include "image.h"
#include "memory.h"

/***		TEXTURE		***/

// CLONE 

t_texture *texture_clone(t_texture *texture)
{
	if(texture)
	{
		t_texture *clone = texture_new(texture->id.name);

		vcp3i(clone->idcol,texture->idcol);
		clone->width = texture->width;
		clone->height = texture->height;
		clone->format = texture->format;
		clone->internal_format = texture->internal_format;
		clone->id_gl = texture->id_gl;
		clone->texels = NULL;//XXX

		return clone;
	}
	else
	{
		return NULL;
	}
} 

// FREE

void texture_free(t_texture *texture)
{
	if(texture->texels) free(texture->texels);
}

void _texture_free(t_texture *texture)
{
	if(texture->texels) free(texture->texels);
	free(texture);
}

// REBIND

t_texture *texture_rebind(t_scene *scene, void *ptr)
{
	return ptr;
}

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
	t_texture *texture = (t_texture *)mem_malloc(sizeof(t_texture));

	id_init(&texture->id, name);

	texture->width=0;
	texture->height=0;
	texture->texels=NULL;
	texture->internal_format=0;
	texture->format=0;
	texture->id_gl=0;

	return texture;
}
