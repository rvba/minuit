/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "node.h"
#include "opengl.h"
#include "util.h"
#include "image.h"
#include "memory.h"

t_image  *img_read_jpg(char* name)
{
	unsigned char a,r,g,b;
	int width, height;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	FILE * infile;        
	JSAMPARRAY pJpegBuffer;       
	int row_stride;       
	if ((infile = fopen(name, "rb")) == NULL) 
	{
		printf("can't open [%s]\n", name);
		return 0;
	}
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);

	(void) jpeg_read_header(&cinfo, TRUE);
	(void) jpeg_start_decompress(&cinfo);

	width = cinfo.output_width;
	height = cinfo.output_height;

	unsigned char *ptr = (unsigned char*)mem_malloc(sizeof(unsigned char)*width*height*4); 

	ptr +=(width*height*4); 

	if (!ptr)
	{
		printf("memory error\n");
		return 0;
	}
	row_stride = width * cinfo.output_components ;
	pJpegBuffer = (*cinfo.mem->alloc_sarray)
	((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	while (cinfo.output_scanline < cinfo.output_height)
	{
		(void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
		int x;
		for (x=width;x>0;x--)
		{
			a = 0; // no alpha 
			r = pJpegBuffer[0][cinfo.output_components*x-2];

			if (cinfo.output_components>2)
			{
				g = pJpegBuffer[0][cinfo.output_components*x-1];
				b = pJpegBuffer[0][cinfo.output_components*x];
			}
			else
			{
				g = r;
				b = r;
			}

			*(ptr--) = r; 
			*(ptr--) = g; 
			*(ptr--) = b; 
			*(ptr--) = a; 
		}
	}
	fclose(infile);
	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	//t_texture *texture = (t_texture*)mem_malloc(sizeof(t_texture));
	t_image *image = image_new(name);

	/*
	texture->height=height;
	texture->width=width;
	texture->internal_format=4;
	texture->texels=(GLubyte *)ptr;
	texture->format=GL_RGBA;
	*/
	
	image->width=width;
	image->height=height;
	image->bpp=4;
	image->data=ptr;

	printf("image is done %s\n", image->id.name);

	//return texture;
	return image;
}

void img_save_video(int width, int height, const char *name,unsigned char *image)
{
	//GLubyte *image = (GLubyte *)mem_malloc(width*height*sizeof(GLubyte)*3);
	/*
	unsigned char *image = (unsigned char *)mem_malloc(width*height*sizeof(unsigned char)*3);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
	*/

	int quality = 100;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
 
 	FILE * outfile;		
	JSAMPROW row_pointer[1];
	int row_stride;		

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(name, "wb")) != NULL) 
	{
		jpeg_stdio_dest(&cinfo, outfile);
		
		cinfo.image_width = width; 
		cinfo.image_height = height;
		cinfo.input_components = 3;	
		cinfo.in_color_space = JCS_RGB;
	 
		jpeg_set_defaults(&cinfo);
		jpeg_set_quality(&cinfo, quality, TRUE);
		jpeg_start_compress(&cinfo, TRUE);

		row_stride = width * 3;	

		while (cinfo.next_scanline < cinfo.image_height) 
		{
			row_pointer[0] = & image[(cinfo.image_height-1-cinfo.next_scanline) * row_stride];
			(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}

		jpeg_finish_compress(&cinfo);
		fclose(outfile);

		jpeg_destroy_compress(&cinfo);
	//	free(image);

	}
}

void img_save_jpg(int width, int height, const char *name)
{
//	GLubyte *image = (GLubyte *)mem_malloc(width*height*sizeof(GLubyte)*3);
	size_t malloc_size = width * height * sizeof( unsigned char) * 3;
	unsigned char *image = (unsigned char *)mem_malloc(malloc_size);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

	int quality = 100;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
 
 	FILE * outfile;		
	JSAMPROW row_pointer[1];
	int row_stride;		

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if ((int)strlen(name) == 0) name="./data/screenshot.jpg";
 
	if ((outfile = fopen(name, "wb")) != NULL) 
	{
		jpeg_stdio_dest(&cinfo, outfile);
		
		cinfo.image_width = width; 
		cinfo.image_height = height;
		cinfo.input_components = 3;	
		cinfo.in_color_space = JCS_RGB;
	 
		jpeg_set_defaults(&cinfo);
		jpeg_set_quality(&cinfo, quality, TRUE);
		jpeg_start_compress(&cinfo, TRUE);

		row_stride = width * 3;	

		while (cinfo.next_scanline < cinfo.image_height) 
		{
			row_pointer[0] = & image[(cinfo.image_height-1-cinfo.next_scanline) * row_stride];
			(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}

		jpeg_finish_compress(&cinfo);
		fclose(outfile);

		jpeg_destroy_compress(&cinfo);
		mem_free( image, malloc_size);

	}
	else
	{
		printf("can't save jpg image\n");
	}
}

void img_save_jpg_highres(int width, int height, const char name[],GLubyte *image)
{
	int quality = 100;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
 
 	FILE * outfile;		
	JSAMPROW row_pointer[1];
	int row_stride;		

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if ( (int)strlen(name) == 0) name="./data/screenshot_hd.jpg";
	printf("path %s\n",name);
 
	if ((outfile = fopen(name, "wb")) != NULL) 
	{
  	
		jpeg_stdio_dest(&cinfo, outfile);
		
		cinfo.image_width = width; 
		cinfo.image_height = height;
		cinfo.input_components = 3;	
		cinfo.in_color_space = JCS_RGB;
	 
		jpeg_set_defaults(&cinfo);
		jpeg_set_quality(&cinfo, quality, TRUE);
		jpeg_start_compress(&cinfo, TRUE);

		row_stride = width * 3;	

		while (cinfo.next_scanline < cinfo.image_height) 
		{
			row_pointer[0] = & image[(cinfo.image_height-1-cinfo.next_scanline) * row_stride];
			(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}

		jpeg_finish_compress(&cinfo);
		fclose(outfile);

		jpeg_destroy_compress(&cinfo);
		/* **MEM** */
		free(image);
	}
	else
	{
		printf("can't save image\n");
	}
}
