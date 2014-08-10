/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "base.h"
#include "draw.h"
#include "sketch.h"
#include "scene.h"
#include "object.h"
#include "ctx.h"
#include "event.h"
#include "sketch.h"
#include "ui.h"
#include "node.h"

t_draw *DRAW;

void draw_switch_scene( t_draw *draw)
{
	if( draw->with_scene) draw->with_scene = 0;
	else draw->with_scene = 1;
}

void draw_switch_axis_world(t_draw *draw)
{
	if (draw->show_axis_world) draw->show_axis_world=0;
	else draw->show_axis_world=1;
}

void draw_clear(float r,float g,float b,float a)
{
	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0,0,0,0);
	glClear(GL_DEPTH_BUFFER_BIT);
	
}

void draw_lights(t_draw *draw, t_scene *scene)
{
	t_node *node;
	t_link *link;

	t_lst *lst = scene_lst_get( scene, dt_light);
	if( lst)
	{
		for(link=lst->first;link;link=link->next)
		{
			node = ( t_node *) link->data;
			draw_light(draw,node);
		}
	}
}

void draw_objects(t_draw *draw, t_scene *scene)
{
	t_link *link;
	t_node *node;

	t_lst *lst = scene_lst_get( scene, dt_object);
	link=lst->first;

	for(link=lst->first;link;link=link->next)
	{
		node = ( t_node *) link->data;
		t_object *object = ( t_object *) node->data;

		if(!object)
		{
			printf("[ERROR draw_objects] No data\n");
			break;
		}
		else
		{
			object->cls->draw(object);
		}
	}
}

void draw_axis_world(t_draw *draw)
{
	if (draw->show_axis_world) 
	{
		float v0[] = {0,0,0};
		float vx[] = {1,0,0};
		float vy[] = {0,1,0};
		float vz[] = {0,0,1};
		float red[]={1,0,0};
		float green[]={0,1,0};
		float blue[]={0,0,1};

		glDisable(GL_DEPTH_TEST);

		skt_vector(v0,vx,1,red);
		skt_vector(v0,vy,1,green);
		skt_vector(v0,vz,1,blue);

		glEnable(GL_DEPTH_TEST);
	}
}

void draw_scene(t_draw *draw, t_scene *scene)
{
	// Edit Mode
	if(scene->edit_mode) draw->edit_mode = 1;
	else draw->edit_mode = 0;

	// Objects
	draw_lights(draw,scene);
	draw_objects(draw,scene);
	draw_axis_world(draw);
}

void draw_init(t_draw *draw)
{
	float c[4];
	bzero(c,4);
	cconv(c,draw->background_color);

	// clear
	if(draw->with_clear)
	{
		draw_clear(c[0],c[1],c[2],0);
	}

	// set depth
	if(draw->with_depth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	
	// polygon offset
	if(draw->with_polygon_offset)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0,1.0);
	}
	else
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	if(draw->with_blend)
	{
		glEnable(GL_BLEND);
    		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	t_context *C = ctx_get();
	C->event->ui.use_line_global_width = 1;
	skt_update( C);

}

t_draw *draw_new(void)
{
	t_draw *draw = (t_draw *)mem_malloc(sizeof(t_draw));

	DRAW=draw;
	
	draw->background_color[0]=DRAW_COLOR_BACKGROUND_RED;
	draw->background_color[1]=DRAW_COLOR_BACKGROUND_GREEN;
	draw->background_color[2]=DRAW_COLOR_BACKGROUND_BLUE;
	draw->background_color[3]=DRAW_COLOR_BACKGROUND_ALPHA;

	vset4f(draw->front_color,0,0,0,0);
	vset4f(draw->back_color,1,1,1,0);

	draw->mode=mode_draw;
	draw->color=DRAW_COLOR;

	draw->with_selection_pass=DRAW_SELECTION_PASS; 
	draw->with_draw_pass=DRAW_WITH_DRAW;

	draw->with_clear=DRAW_WITH_CLEAR;
	draw->with_ui=DRAW_WITH_UI;

	draw->with_restrict_matrix=0;

	draw->mode_direct=1;

	draw->show_axis_world=DRAW_SHOW_AXIS_WORLD;
	draw->with_normal=DRAW_WITH_NORMAL;
	draw->with_texture=DRAW_WITH_TEXTURE;
	draw->with_material=DRAW_WITH_MATERIAL;
	draw->with_point=DRAW_WITH_POINT;
	draw->with_point_id=DRAW_WITH_POINT_ID;
	draw->with_face=DRAW_WITH_FACE;
	draw->with_face_outline=DRAW_WITH_FACE_OUTLINE;
	draw->with_edge = DRAW_WITH_EDGE;
	draw->with_edge_color = DRAW_WITH_EDGE_COLOR;
	draw->with_highlight=DRAW_WITH_HIGHLIGHT;
	draw->with_light=DRAW_WITH_LIGHT;
	draw->with_depth=DRAW_WITH_DEPTH;
	draw->with_polygon_offset=DRAW_WITH_POLYGON_OFFSET;
	draw->with_alpha=DRAW_WITH_ALPHA;
	draw->with_blend=DRAW_WITH_BLEND;
	draw->with_scene = 1;
	draw->with_grid=0;

	draw->with_shaders = 1;

	draw->draw_lights = 1;
	draw->edit_mode = 0;

	return draw;
}



