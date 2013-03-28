/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

int VIEWPORT_WIDTH;
int VIEWPORT_HEIGHT;

void ctx_camera_set(t_context *C);
/** set drawing options for the selection pass*/
void ctx_render_set_selection_pass(t_context *C)
{
	t_camera *camera=C->camera;
	if(C->draw->with_restrict_matrix)
		camera->restrict_matrix=1;
	else
		camera->restrict_matrix=0;

	C->draw->mode=mode_selection;

	C->draw->with_light=0;
	C->draw->with_material=0;
	C->draw->with_normal=0;
	C->draw->with_texture=0;
	C->draw->with_depth=1;
	C->draw->with_blend=0;

	//set white background
	if(C->draw->with_clear)
	{
		C->draw->background_color[0]=255;
		C->draw->background_color[1]=255;
		C->draw->background_color[2]=255;
		C->draw->background_color[3]=0;
	}
}

/** set drawing options for the main pass*/
void ctx_render_set_full_pass(t_context *C)
{
	t_camera *camera=C->camera;
	camera->restrict_matrix=0;

	C->draw->mode=mode_draw;

	C->draw->with_light=1;
	C->draw->with_material=1;
	C->draw->with_normal=C->event->with_normal;
	C->draw->with_light=C->event->with_light;
	C->draw->with_texture=C->event->with_texture;
	C->draw->with_polygon_offset=C->event->with_polygon_offset;

	C->draw->with_depth=C->event->with_depth;
	C->draw->with_highlight=C->event->with_highlight;
	C->draw->with_blend=C->event->with_blend;

	C->draw->background_color[0]=C->ui->background_color[0];
	C->draw->background_color[1]=C->ui->background_color[1];
	C->draw->background_color[2]=C->ui->background_color[2];
	C->draw->background_color[3]=C->ui->background_color[3];

	C->draw->with_point=C->event->with_point;
	C->draw->with_point_id=C->event->with_point_id;
	C->draw->with_face=C->event->with_face;
}

/** the drawing callback */
void ctx_render_scene(void)
{
	t_context *C = ctx_get();
	if(C->draw->with_draw_pass && C->scene->is_ready)
	{
		// set draw mode
		ctx_render_set_full_pass(C);
		// draw the ui and the scene
		ctx_camera_set(C);
		draw_scene(C->draw,C->scene);
	}
}

/** grab the color under the mouse*/
void ctx_render_selection_pass(t_context *C)
{
	// PIXEL
	unsigned char pixel[3];
	memset(pixel,3,0);
	// VIEWPORT
	GLint viewport[4];
	memset(viewport,4,0);
	glGetIntegerv(GL_VIEWPORT,viewport);

	int x = C->app->mouse->x;
	int y = C->app->mouse->y;

	// set selection pass
	ctx_render_set_selection_pass(C);

	/*
	if (C->draw->with_restrict_matrix)
	{
		C->app->window->viewport_width=VIEWPORT_WIDTH/10;
		C->app->window->viewport_height=VIEWPORT_HEIGHT/10;
	}
	*/


	ctx_camera_set(C);

	// DRAW SCENE
	if(C->scene->is_ready) draw_scene(C->draw,C->scene);

	/*
	if ( C->draw->with_restrict_matrix)
	{
		C->app->window->viewport_width=VIEWPORT_WIDTH;
		C->app->window->viewport_height=VIEWPORT_HEIGHT;
	}
	*/

	// read pixel under mouse

	glPixelStorei(GL_PACK_ALIGNMENT,1); 
	glReadPixels(x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel); 

	int r = (int)pixel[0];
	int g = (int)pixel[1];
	int b = (int)pixel[2];
	int a = (int)pixel[3];

	// store pixel color
	C->event->color[0]=r;
	C->event->color[1]=g;
	C->event->color[2]=b;
	C->event->color[3]=a;

	// debug
	if (C->event->debug_select)
		 printf("pixel color : %d %d %d %d\n",C->event->color[0],C->event->color[1],C->event->color[2],a);
}

/** update camera */
void ctx_camera_set(t_context *C)
{
	// set frustum
	op_camera_update(C);
	// flip global orientation (model)
	op_3d_orientation(); 
}

void ctx_render_selection(t_context *C)
{
	if(C->draw->with_selection_pass)
	{
		ctx_render_selection_pass(C); 
	}
}

int video_init=0;
unsigned char *buffer=NULL;

int frame_offset=10;
int last_frame;

void ctx_render_video(t_context *C)
{
	/*
	int width = C->app->window->width;
	int height = C->app->window->height;

	if(!video_init)
	{
		video_init = 1;
		buffer = (unsigned char *)malloc(sizeof(unsigned char *)*width*height*4);
		last_frame=0;
	}

	int f = C->app->frame - last_frame ;

	if(f>frame_offset) 
	{
		last_frame = C->app->frame;

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	char name[30];
	sprintf(name,"./video/%d.jpg",C->app->frame);
	img_save_video(width,height,name,buffer);

	}
	*/

}

void ctx_render(t_context *C)
{
	if(!C->app->off_screen)
	{
		VIEWPORT_WIDTH=C->app->window->viewport_width;
		VIEWPORT_HEIGHT=C->app->window->viewport_height;

		ctx_render_selection(C);
		ctx_render_scene();

		app_swap(C->app);

		if(C->event->video) ctx_render_video(C);
	}
}
