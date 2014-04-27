/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "ctx.h"
#include "op.h"
#include "app.h"
#include "event.h"
#include "draw.h"
#include "ui.h"
#include "viewport.h"
#include "list.h"
#include "object.h"
#include "mesh.h"

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
	C->draw->with_edge=C->event->with_edge;
	C->draw->with_edge_color=C->event->with_edge_color;
	C->draw->with_point_id=C->event->with_point_id;
	C->draw->with_face=C->event->with_face;
	C->draw->with_face_outline=C->event->with_face_outline;
}

void ctx_get_selection(t_context *C)
{
	// PIXEL
	unsigned char pixel[]={0,0,0};

	// VIEWPORT
	GLint viewport[]={0,0,0,0};
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

	// Draw Viewports
	t_lst *lst = scene_lst_get( scene, dt_viewport);
	for(link = lst->first; link; link = link->next)
	{
		node = ( t_node *) link->data;
		viewport = ( t_viewport *) node->data;
		if(viewport->draw) viewport->draw(viewport);
	}
}

void ctx_render_scene_selection_pass(t_context *C)
{
	ctx_render_set_selection_pass(C);
	draw_init(C->draw);
}

void ctx_render_scene_full_pass(t_context *C)
{
	ctx_render_set_full_pass(C);
	draw_init(C->draw);
}

void ctx_render_scene_clear_pass(t_context *C)
{
	ctx_render_set_full_pass(C);
	draw_init(C->draw);
}

void ctx_render_init( t_context *C)
{
	t_link *l;
	t_object *object;
	t_node *node;
	t_mesh *mesh;
	t_lst *lst = scene_lst_get( C->scene, dt_object);
	if( lst)
	{
	for( l = lst->first; l; l=l->next)
	{
		node = l->data;
		object = node->data;
		mesh = object->mesh;
		if( mesh)
		{
			if(!mesh->state.is_buffer_built)  mesh_init_buffers(mesh,buffer_direct); 
		}
	}
	}
}

void ctx_render(t_context *C)
{
	// Init Buffers
	ctx_render_init( C);

	// Check Not Off Screen
	if(!C->app->off_screen)
	{
		// Selection Pass
		if(C->draw->with_selection_pass)
		{
			ctx_render_scene_selection_pass(C);

			ui_draw();

			// Get Color
			ctx_get_selection(C);
		}

		// Render Pass
		if(C->draw->with_draw_pass)
		{
			if( C->draw->with_scene) ctx_render_scene_full_pass(C);
			else ctx_render_scene_clear_pass( C);

			ui_draw();
		}

		// Swap Buffers
		app_swap(C->app);

		// Video Record
		if(C->event->video_record) ctx_render_video(C);
	}
}
