/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "context.h"
#include "op.h"
#include "ctx.h"
#include "app.h"
#include "event.h"
#include "draw.h"
#include "ui.h"
#include "viewport.h"
#include "list.h"
#include "object.h"
#include "engine.h"
#include "process.h"


// RENDER PASS

void ctx_render_set_selection_pass(t_context *C)
{
	// Mode
	C->draw->mode=mode_selection;

	// Mate Rendering
	C->draw->with_light=0;
	C->draw->with_material=0;
	C->draw->with_normal=0;
	C->draw->with_texture=0;
	C->draw->with_depth=1;
	C->draw->with_blend=0;

	// Set White background
	if(C->draw->with_clear)
	{
		C->draw->background_color[0]=255;
		C->draw->background_color[1]=255;
		C->draw->background_color[2]=255;
		C->draw->background_color[3]=0;
	}
}

// SELECTION PASS

void ctx_render_set_full_pass(t_context *C)
{
	// Mode
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
	C->draw->with_face_outline=C->event->with_face_outline;
}

void ctx_get_selection(t_context *C)
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

void ctx_render_scene(t_context *C)
{
	t_scene *scene = C->scene;
	t_link *link;
	t_node *node;
	t_viewport *viewport;

	//ctx_render_set_full_pass(C);
	//draw_init(C->draw);

	// Draw Screens
	for(link = scene->viewports->first; link; link = link->next)
	{
		node = link->data;
		viewport = node->data;
		//viewport_draw(viewport);
		if(viewport->draw) viewport->draw(viewport);
	}
}

void ctx_render_scene_selection_pass(t_context *C)
{
	ctx_render_set_selection_pass(C);
	draw_init(C->draw);
	ctx_render_scene(C);
}

void ctx_render_scene_full_pass(t_context *C)
{
	ctx_render_set_full_pass(C);
	draw_init(C->draw);
	ctx_render_scene(C);
}

void ctx_render_start(t_context *C)
{
	if(!C->event->graph_computing)
	{
		if(C->event->graph_updated)
		{
			draw_render_start(C);
			ui_draw_start(C);
		}
	}
}

void ctx_render_stop(t_context *C)
{
	if(!C->event->graph_computing)
	{
		if(C->event->graph_updated)
		{
			draw_render_stop(C);
			ui_draw_stop(C);
		}
	}
}

void ctx_render_check_thread(t_context *C)
{
	// Get Process
	t_process *graph_process = engine_process_get(C->engine,"graph");

	// If Thread Being Computed
	if(C->event->graph_computing)
	{
		// If Is Done
		if(!graph_process->busy)
		{
			// Set Updated
			C->event->graph_updated = 1;
			C->event->graph_computing = 0;

			ctx_render_stop(C);
			ctx_render_start(C);
		}
	}
	// Else Start Computing
	else
	{
		if(!C->event->graph_init)
		{
			C->event->graph_init = 1;
			ctx_render_start(C);
		}

		C->event->graph_computing = 1;
		C->event->graph_updated = 0;
	}
}

void ctx_render(t_context *C)
{
	// Check Not Off Screen
	if(!C->app->off_screen)
	{
		int t = C->event->use_threading;

		C->event->use_threading = 0;


		// Selection Pass
		if(C->draw->with_selection_pass)
		{

			ctx_render_scene_selection_pass(C);

			ui_draw();

			// Get Color
			ctx_get_selection(C);
		}

		C->event->use_threading = t;

		// Check Thread
		ctx_render_check_thread(C);

		// Start
		//if(C->event->use_threading) ctx_render_start(C);

		// Render Pass
		if(C->draw->with_draw_pass)
		{
			ctx_render_scene_full_pass(C);

			ui_draw();
		}

		// Swap Buffers
		app_swap(C->app);

		// Video Record
		if(C->event->video_record) ctx_render_video(C);

		// Stop
		//if(C->event->use_threading) ctx_render_stop(C);
	}
}
