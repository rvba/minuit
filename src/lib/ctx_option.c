/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "option.h"
#include "app.h"
#include "event.h"
#include "sketch.h"
#include "draw.h"

#define PRINT_FILE 0
#define PRINT_CHUNK 0
#define PRINT_DATA 0
#define PRINT_CHECK 0
#define PRINT_CHECK_OK 0
#define PRINT_DEBUG 1
#define PRINT_DEBUG_OK 0
#define PRINT_POINTERS 0
#define OUTPUT 1
#define LOG_OUT 1

t_node *ctx_node_find_option(t_scene *sc)
{
	t_link *l;
	t_node *n;

	for(l=sc->tmp_node->first;l;l=l->next)
	{
		n=l->data;

		//if(is(n->cls->type,"option"))
		if(n->type==nt_option)
		{
			return n;
		}
	}
	 
	return NULL;
}

void option_load(t_context *C,t_node *node)
{
	t_option *option=node->data;

	// app

	C->app->window->width_def=option->window_width_def;
	C->app->window->height_def=option->window_height_def;
	C->app->window->width=option->window_width;
	C->app->window->height=option->window_height;
	C->app->timer_add_low = option->timer_add_low;

	// sketch

	vcp3f(C->skt->color,option->skt_color);
	vcp3f(C->skt->point_color,option->skt_point_color);
	C->skt->line_width=option->skt_line_width;
	C->skt->point_width=option->skt_point_width;
	C->skt->point_size=option->skt_point_size;
	C->skt->with_scale=option->skt_with_scale;
	C->skt->scale=option->skt_scale;

	C->event->sketch_with_scale=option->skt_with_scale;

	// draw

	vcp3i(C->draw->background_color,option->draw_background_color);
	vcp3f(C->draw->front_color,option->draw_front_color);

	C->draw->with_grid=option->draw_with_grid;
	C->draw->img_with_alpha=option->draw_img_with_alpha;
	C->draw->divx=option->draw_divx;
	C->draw->divy=option->draw_divy;
	C->draw->show_axis_world=option->draw_show_axis_world;

	// event

	C->event->with_clear=option->draw_with_clear;
	C->event->with_normal=option->draw_with_normal;
	C->event->with_texture=option->draw_with_texture;
	C->event->with_point=option->draw_with_point;
	C->event->with_point_id=option->draw_with_point_id;
	C->event->with_face=option->draw_with_face;
	C->event->with_highlight=option->draw_with_highlight;
	C->event->with_light=option->draw_with_light;
	C->event->with_depth=option->draw_with_depth;
	C->event->with_polygon_offset=option->draw_with_polygon_offset;

	C->app->window->fullscreen = option->fullscreen;
	app_screen_set_fullscreen(C->app,C->app->window->fullscreen);

	C->draw->color = option->color;
	op_set_color(C,C->draw->color);
	C->draw->draw_lights = option->draw_lights;

}

void option_save(t_context *C)
{
	C->scene->store=1;

	//t_node *node=scene_add(C->scene,"option","option");
	t_node *node=scene_add(C->scene,nt_option,"option");
	C->scene->store=0;

	t_option *option=node->data;

	// app

	option->window_width_def=C->app->window->width_def;
	option->window_height_def=C->app->window->height_def;
	option->window_width=C->app->window->width;
	option->window_height=C->app->window->height;
	option->timer_add_low = C->app->timer_add_low;

	// sketch

	vcp3f(option->skt_color,C->skt->color);
	vcp3f(option->skt_point_color,C->skt->point_color);

	option->skt_line_width=C->skt->line_width;
	option->skt_point_width=C->skt->point_width;
	option->skt_point_size=C->skt->point_size;
	option->skt_with_scale=C->event->sketch_with_scale;
	option->skt_scale=C->skt->scale;

	// draw

	vcp3i(option->draw_background_color,C->draw->background_color);
	vcp3f(option->draw_front_color,C->draw->front_color);

	option->draw_with_grid=C->draw->with_grid;
	option->draw_img_with_alpha=C->draw->img_with_alpha;
	option->draw_divx=C->draw->divx;
	option->draw_divy=C->draw->divy;
	option->draw_show_axis_world=C->draw->show_axis_world;

	// event

	option->draw_with_clear=C->event->with_clear;
	option->draw_with_normal=C->event->with_normal;
	option->draw_with_texture=C->event->with_texture;
	option->draw_with_point=C->event->with_point;
	option->draw_with_point_id=C->event->with_point_id;
	option->draw_with_face=C->event->with_face;
	option->draw_with_highlight=C->event->with_highlight;
	option->draw_with_light=C->event->with_light;
	option->draw_with_depth=C->event->with_depth;
	option->draw_with_polygon_offset=C->event->with_polygon_offset;

	option->fullscreen = C->app->window->fullscreen;
	option->color = C->draw->color;
	option->draw_lights = C->draw->draw_lights;

}

void load_option(t_context *C,t_scene *sc)
{
	t_node *node = ctx_node_find_option(sc);
	if(node)
	{
		option_load(C,node);
	}
	else
	{
		printf("[ERROR load_option] Can't find option\n");
	}
}

t_option *option_rebind(t_scene *scene, void *ptr)
{
	return ptr;
}
void option_free(t_option *option)
{
}

t_option *option_new(const char *name)
{
	t_option *option = (t_option *)malloc(sizeof(t_option));
	set_name(option->name,name);
	return option;
}
