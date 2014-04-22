/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "ctx.h"
#include "node.h"
#include "scene.h"
#include "util.h"
#include "op.h"
#include "texture.h"
#include "image.h"
#include "memory.h"

/***		TEXTURE		***/

// CLONE 

void gl_show_type( int type)
{
	switch( type)
	{
		case GL_UNSIGNED_BYTE: 	printf( "Type:UNSIGNED_BYTE\n");	break;
		case GL_INT: 		printf( "Type:INT\n");			break;
		case GL_FLOAT: 		printf( "Type:FLOAT\n");		break;
		default:		printf( "Type:Unknown\n");		break;
	}
}

void gl_show_format( int format)
{
	switch( format)
	{
		case GL_RGB:	 	printf( "Format:RGB\n");		break;
		case GL_RGBA: 		printf( "Format:RGBA\n");		break;
		default:		printf( "Format:Unknown\n");		break;
	}
}

void texture_show( t_texture *texture)
{
	printf("Texture %s\n", texture->id.name);
	printf("id:%d\n", texture->id_gl);
	printf("width:%d height:%d\n", texture->width, texture->height);
	gl_show_format( texture->format);
	gl_show_type( texture->type);
	printf("internal_format: %d\n", texture->internal_format);

}

void texture_image_load( t_texture *texture, t_image *image)
{
	texture->width = image->width;
	texture->height = image->height;
	texture->format = image->format;
	texture->internal_format = image->bpp;
	texture->type = image->type;


	glGenTextures( 1, &texture->id_gl);

	glEnable( GL_TEXTURE_2D);

	glBindTexture( GL_TEXTURE_2D, texture->id_gl); 
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D (	GL_TEXTURE_2D,				// GLenum target		GL_TEXTURE_2D, GL_TEXTURE_CUBE, ...
			0,					// GLint level			Mimap, if only one resolution, level should be 0
			texture->internal_format,		// GLint internal_format	Witch components are selected (RGBA, LUMINANCE, ...) [1-4] or GL_ALPHA,...
			texture->width, 			// Glsizei width		width of the texture
			texture->height,			// GLsizei height		height of the texture
		      	0,					// GLint border			border width, 0 if no border
			texture->format,			// GLenum format		GL_RGB, ...
			texture->type,				// GLenum type			GL_UNSIGNED_BYTE, ...
			image->data);				// const GLvoid *textels	The data

	glDisable( GL_TEXTURE_2D);
}

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
	if(texture->texels) mem_free( texture->texels, texture->size);
}

void _texture_free(t_texture *texture)
{
	if(texture->texels) mem_free( texture->texels, texture->size);
	mem_free( texture, sizeof( t_texture));
}

// REBIND

void texture_rebind(t_scene *scene, void *ptr)
{
}

void texture_image_bind(t_texture *texture,t_image *image)
{
	texture->width=image->width;
	texture->height=image->height;

	texture->texels=image->data;
	texture->internal_format=image->bpp;
	texture->format=image->format;
	texture->size = image->size;
}


// NEW

t_texture *texture_new(const char *name)
{
	t_texture *texture = (t_texture *)mem_malloc(sizeof(t_texture));

	id_init(&texture->id, name);

	texture->size = 0;
	texture->width = 0;
	texture->height = 0;
	texture->texels = NULL;
	texture->internal_format = 0;
	texture->type = 0;
	texture->format = 0;
	texture->id_gl = 0;

	return texture;
}
