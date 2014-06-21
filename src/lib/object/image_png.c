/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "image.h"
#include "util.h"
#include "memory.h"
#include "vlst.h"

#include <png.h>

int img_save_png(int alpha,int width,int height,GLubyte *bitmap,const char name[])
{
	FILE *fp = fopen(name, "wb");
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	int x, y;
	png_uint_32 bytes_per_row;
	png_byte **row_pointers = NULL;

	int bytes_per_pixel;
	int COLOR_TYPE;

	if(alpha)
	{
		COLOR_TYPE = PNG_COLOR_TYPE_RGBA;
		bytes_per_pixel=4;
	}
	else
	{
		COLOR_TYPE = PNG_COLOR_TYPE_RGB;
		bytes_per_pixel=3;
	}

	if (fp == NULL) return -1;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (png_ptr == NULL)
	{
		fclose(fp);
		return -1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_write_struct(&png_ptr, NULL);
		fclose(fp);
		return -1;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return -1;
	}

	png_set_IHDR(png_ptr,
		 info_ptr,
		 width,
		 height,
		 8,
		 COLOR_TYPE,
		 PNG_INTERLACE_NONE,
		 PNG_COMPRESSION_TYPE_DEFAULT,
		 PNG_FILTER_TYPE_DEFAULT);

	bytes_per_row = width * bytes_per_pixel;
	row_pointers = png_malloc(png_ptr, height * sizeof(png_byte *));

	unsigned int v;
	for (y = height-1 ; y >= 0 ; --y)
	{
		unsigned char *row = png_malloc(png_ptr, sizeof(unsigned char) * bytes_per_row);
		row_pointers[y] = (png_byte *)row;
		for (x = 0; x < width; ++x)
		{
			if(alpha)
			{
			*row++=*bitmap++;
			*row++=*bitmap++;
			*row++=*bitmap++;
			*row++=*bitmap++;
			}
			else
			{
			*row++=*bitmap++;
			*row++=*bitmap++;
			*row++=*bitmap++;
			v=*bitmap++;
			v++;
			}
		}
	}

	png_init_io(png_ptr, fp);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	for (y = 0; y < height; y++)
	{
		png_free(png_ptr, row_pointers[y]);
	}

	png_free(png_ptr, row_pointers);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(fp);
	return 0;
}

t_image *img_read_png(char *filename)
{
	FILE *file ;
	png_byte magic[8];

	file = fopen(filename,"rb");
	if (!file)
	{
		printf("couldn't read %s\n",filename); return NULL;
	}

	size_t r = fread(magic,1,sizeof(magic),file);
	if(r != sizeof(magic)) printf("read error\n");

	if (!png_check_sig(magic,sizeof(magic)))
	{
		printf("no magic\n"); return NULL;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);

	if (!png_ptr)
	{
		fclose(file);
		return NULL;
	}

	png_infop info_ptr  = png_create_info_struct(png_ptr);

	if(!info_ptr)
	{
		fclose(file);
		png_destroy_read_struct(&png_ptr,NULL,NULL);
		return NULL;
	}

	png_init_io(png_ptr,file);
	png_set_sig_bytes(png_ptr,sizeof(magic));
	png_read_info(png_ptr,info_ptr);

	int bit_depth,color_type;

	bit_depth = png_get_bit_depth(png_ptr,info_ptr);
	color_type = png_get_color_type(png_ptr,info_ptr);

	if (color_type == PNG_COLOR_TYPE_PALETTE) 
		png_set_palette_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) 
		png_set_gray_1_2_4_to_8 (png_ptr);
	if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha (png_ptr);
	if (bit_depth == 16)
		png_set_strip_16 (png_ptr);
	else if (bit_depth < 8)
		png_set_packing (png_ptr);

	png_read_update_info(png_ptr,info_ptr);

	png_uint_32 w, h;

	png_get_IHDR (png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);


	t_image *image = image_new(filename);

	image->width=w;
	image->height=h;

	switch (color_type)
	{

		case PNG_COLOR_TYPE_GRAY:
			image->format=GL_LUMINANCE;
			image->bpp = 1;
			break;

		case PNG_COLOR_TYPE_GRAY_ALPHA:
			image->format=GL_LUMINANCE_ALPHA;
			image->bpp = 2;
			break;

		case PNG_COLOR_TYPE_RGB:
			image->format=GL_RGB;
			image->bpp = 3;
			break;

		case PNG_COLOR_TYPE_RGB_ALPHA:
			image->format=GL_RGBA;

			image->bpp = 4;
			break;

		default:
			break;
	}

	image->vlst = vlst_make( "vlst", dt_uchar, image->width * image->height, image->bpp, NULL);

	png_bytep *row_pointers;

	row_pointers = (png_bytep *)mem_malloc (sizeof (png_bytep) * image->height);

	int i;
	for (i = 0; i < image->height; ++i)
	{
		row_pointers[i] = (png_bytep)(image->vlst->data +
		((image->height - (i + 1)) * image->width * image->bpp));
	}

	png_read_image (png_ptr, row_pointers);

	free (row_pointers);

	png_read_end (png_ptr, NULL);
	png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

	fclose (file);

	return image;
}
