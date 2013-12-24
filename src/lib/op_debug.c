/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "node.h"
#include "scene.h"

#include "opengl.h"
#include "op.h"
#include "app.h"
#include "process.h"
#include "engine.h"
#include "util.h"
#include "clock.h"
#include "list.h"

#include "txt.h"
#include "ui.h"
#include "event.h"
#include "plug.h"

#define DEBUG_LINE_HEIGHT 20
#define DEBUG_OFFSET 600
#define DEBUG_SCALE 1 


float get_framerate(t_context *C)
{
	gettimeofday(&C->app->clock->frame_now,NULL);
	double delta = 
		(C->app->clock->frame_now.tv_sec - C->app->clock->frame_before.tv_sec)
		+
		(C->app->clock->frame_now.tv_usec - C->app->clock->frame_before.tv_usec)*0.000001f;

	float one = 1.0f;
	float res;
	float div = (float)delta;
	res = (float)(one/div); //[WARNING] 0 division

	C->app->clock->frame_before = C->app->clock->frame_now;

	return res;
}

void op_debug_process(void)
{
	t_context *C = ctx_get();
	t_app *app = C->app;
	t_process *process = C->engine->processes->first->data;
	t_clock *clock = process->clock;

	float width = (float)C->app->window->width;

	int l=30;
	float pos[2]={300,200};
	char txt[l];
	float s=DEBUG_SCALE;
	float offset=DEBUG_OFFSET;
	float vert=DEBUG_LINE_HEIGHT;
	float color[]={0,0,0,0};

	int is_limit;
		
	if(process->limit)
		is_limit=1;
	else
		is_limit=0;

	//float scale=4;
	float txt_scale=1;
	
	glPushMatrix();
	glLoadIdentity();
		glTranslatef(width-offset,200,0);
		glScalef(s,s,s);

		glTranslatef(0,-vert,0);
		snprintf(txt,l,"msec %u",C->app->clock->msec);
		txt_draw_direct_2d(txt,pos,color,txt_scale);
		glTranslatef(0,-vert,0);

		glTranslatef(0,-vert,0);
		snprintf(txt,l,"%d.%d",C->app->clock->min,C->app->clock->sec);
		txt_draw_direct_2d(txt,pos,color,txt_scale);
		glTranslatef(0,-vert,0);


		// process
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"process");
		txt_draw_direct_2d(txt,pos,color,txt_scale);
		glTranslatef(0,-vert,0);

		// limit on/off
		glTranslatef(0,-vert,0);
		if(is_limit)
			snprintf(txt,l,"limit is on");
		else
			snprintf(txt,l,"limit is off");
		txt_draw_direct_2d(txt,pos,color,txt_scale);

		// limit
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"limit %f",clock->limit);
		txt_draw_direct_2d(txt,pos,color,txt_scale);

		// freq
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"freq %f",clock->freq);
		txt_draw_direct_2d(txt,pos,color,txt_scale);

		// delta
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"delta %f",clock->delta);
		txt_draw_direct_2d(txt,pos,color,txt_scale);

		// cycle
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"cycle %d",process->cycle);
		txt_draw_direct_2d(txt,pos,color,txt_scale);


		// app
		glTranslatef(0,-vert,0);
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"app");
		txt_draw_direct_2d(txt,pos,color,txt_scale);
		glTranslatef(0,-vert,0);

		// limit on/off
		glTranslatef(0,-vert,0);
		if(C->app->loop)
			snprintf(txt,l,"loop is on");
		else
			snprintf(txt,l,"loop is off");
		txt_draw_direct_2d(txt,pos,color,txt_scale);

		// limit
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"limit %f",app->clock->limit);
		txt_draw_direct_2d(txt,pos,color,txt_scale);

		// freq
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"freq is 1");
		txt_draw_direct_2d(txt,pos,color,txt_scale);

		// delta
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"delta %f",app->clock->delta);
		txt_draw_direct_2d(txt,pos,color,txt_scale);

		// frame
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"frame %d",C->app->frame);
		txt_draw_direct_2d(txt,pos,color,txt_scale);

		// frame rate
		glTranslatef(0,-vert,0);
		snprintf(txt,l,"framerate %-.2f",get_framerate(C));
		txt_draw_direct_2d(txt,pos,color,txt_scale);


	glPopMatrix();
}

void op_debug_all(t_context *C)
{
	// OFF
	if(C->event->debug_all)
	{
		C->event->debug_all = 0;
		C->ui->show_term = 0;
		C->ui->show_rhizome_bounding_box = 0;
		C->ui->show_rhizome_order = 0;
		C->scene->debug_all = 0;
		PLUG_DEBUG = 0;
	}
	// ON
	else
	{
		C->event->debug_all = 1;
		C->ui->show_term = 1;
		C->ui->show_rhizome_bounding_box = 1;
		C->ui->show_rhizome_order = 1;
		C->scene->debug_all = 1;
		PLUG_DEBUG = 1;
	}
}

	




