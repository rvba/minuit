/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

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

t_node *op_texture_jpg_add(t_file *file)
{
	t_context *C = ctx_get();

	t_node *node=scene_add(C->scene,nt_texture,file->name);
	t_texture *texture=node->data;

	//t_image *image = img_read_jpg(file->path);
	t_image *image = img_read_jpg(file->location);

	if(image)
	{
		texture_image_bind(texture,image);
	}

	op_texture_store(texture);

	return node;
}

t_node *op_texture_png_add(t_file *file)
{
	t_context *C = ctx_get();

	t_node *node=scene_add(C->scene,nt_texture,file->name);
	t_texture *texture=node->data;

	//t_image *image = img_read_png(file->path);
	t_image *image = img_read_png(file->location);

	if(image)
	{
		texture_image_bind(texture,image);
	}

	op_texture_store(texture);

	return node;
}

t_node *op_texture_add(const char *path)
{
	t_context *C = ctx_get();
	t_node *node_file = scene_add(C->scene,nt_file,path);
	t_file *file=node_file->data;

	char *ext=file->ext;

	if(*ext)
	{
		if(is(ext,"jpg"))
		{
			op_texture_jpg_add(file);
		}
		else if(is(ext,"png"))
		{
			op_texture_png_add(file);
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




