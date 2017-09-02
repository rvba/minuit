/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include "ctx.h"
#include "scene.h"
#include "node.h"
#include "texture.h"
#include "file.h"
#include "image.h"

int op_texture_store(t_texture *texture)
{
	glGenTextures(1,&texture->id_gl);
	glBindTexture(GL_TEXTURE_2D,texture->id_gl); 
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D (GL_TEXTURE_2D, 0, texture->internal_format, texture->width, texture->height,
		      0, texture->format, GL_UNSIGNED_BYTE, texture->texels);

	return 1;
}

#ifdef HAVE_JPG

t_node *op_texture_jpg_add(t_file *file)
{
	t_context *C = ctx_get();

	t_node *node = scene_add(C->scene, dt_texture, file->id.name);
	t_texture *texture = ( t_texture *) node->data;

	t_image *image = img_read_jpg( file->path);

	if(image)
	{
		texture_image_bind(texture,image);
	}

	op_texture_store(texture);

	return node;
}

#endif

#ifdef HAVE_PNG
t_node *op_texture_png_add(t_file *file)
{
	t_context *C = ctx_get();

	t_node *node = scene_add( C->scene, dt_texture, file->id.name);
	t_texture *texture = ( t_texture *) node->data;

	t_image *image = img_read_png( file->path);

	if(image)
	{
		texture_image_bind( texture, image);
	}

	op_texture_store(texture);

	return node;
}

#endif

t_node *op_texture_add(const char *path)
{
	t_context *C = ctx_get();
	t_node *node_file = scene_add(C->scene,dt_file,path);
	t_file *file = ( t_file *) node_file->data;

	char *ext=file->ext;

	if(*ext)
	{
		if(iseq(ext,"jpg"))
		{
			#ifdef HAVE_JPG
			op_texture_jpg_add(file);
			#endif
		}
		else if(iseq(ext,"png"))
		{
			#ifdef HAVE_PNG
			op_texture_png_add(file);
			#endif
		}
		else
		{
			printf("[ERROR:op_texture_add] Unknonw extension:%s\n",file->ext);
		}
	}
	else
	{
		printf("[ERROR:op_texture_add] No extension\n");
		return NULL;
	}

	return node_file;
}




