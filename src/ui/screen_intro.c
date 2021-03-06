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
#include "app.h"
#include "screen.h"
#include "camera.h"
#include "ui.h"
#include "list.h"
#include "txt.h"
#include "block.h"
#include "sketch.h"
#include "event.h"
#include "viewport.h"
#include "memory.h"
#include "opengl.h"

t_txt *txt_intro;
t_txt *txt_version;

int frame_offset=0;
float *stars=NULL;
float *stars_color=NULL;
int star_init=0;
int star_count=10000;
int *stars_chance=NULL;
float *stars_velocity=NULL;
int *stars_size = NULL;
float intro_intensity=1;

	int dist = 500;
	int color_factor = 100;

int get_sign(void)
{
	int i = rnd_range(0,1);
	if(i>=1)
		return -1;
	else
		return 1;
}

int get_chance(void)
{
	int tot = 500;
	int range = 400;
	int i = rnd_range(0,tot);
	if(i>=range)
		return 1;
	else
		return 0;
}

void star_intensity(int i)
{
	int sign = get_sign();
	float *c = stars_color + (i*3);
	c[0]+=sign*.1;
	c[1]+=sign*.1;
	c[2]+=sign*.1;
}


void star_mvt(int i)
{
	t_context *C = ctx_get();
	int DIST = 200;
	if(stars_chance[i] && i > 1)
	{
		float *s = stars + (i*3);
		float velocity = stars_velocity[i];

		if((s[0] > DIST)&&(s[1] > DIST)&&(s[2]>DIST))
		{
			if(i > DIST)
			{
				float origin[]={0,0,0};
				float vv[3];
				float rot[3]={cos(C->app->frame),sin(C->app->frame),0};

				vsub(vv,s,origin);
				vadd(vv,vv,rot);
				vmul(rot,velocity*20);
				vadd(s,s,rot);
			}
			else
			{

			float *s_next = s -1; 

			float v[3];

			int pos = C->app->mouse->x;
			float acc = 1;

			if(i > pos)
				acc = 4;
			if(i == pos)
				acc = 50;

			vsub(v,s_next,s);
			vmul(v,stars_velocity[i]*.001*acc);
			vadd(s,s,v);
			}
		}
	}
}

void screen_intro_init()
{
	int i;
	t_context *C=ctx_get();
	if(!star_init)
	{
		star_init = 1;
		stars = ( float *) mem_malloc(sizeof(float)* star_count * 3);
		stars_color = ( float *) mem_malloc(sizeof(float)* star_count * 3);
		stars_chance = ( int *) mem_malloc(sizeof(int) * star_count);
		stars_velocity = ( float *) mem_malloc(sizeof(float) * star_count);
		stars_size = ( int *) mem_malloc(sizeof(int) * star_count);
		for(i=0;i<star_count;i++)
		{
			stars[(i*3)+0] = rnd_range(0,dist)*get_sign();
			stars[(i*3)+1] = rnd_range(0,dist)*get_sign();
			stars[(i*3)+2] = rnd_range(0,dist)*get_sign();

			float c = (float)rnd_range(50,color_factor)/color_factor;
			stars_color[(i*3)+0] = c;
			stars_color[(i*3)+1] = c;
			stars_color[(i*3)+2] = c;

			if(get_chance()) stars_chance[i] = 1;
			else stars_chance[i] = 0;

			stars_velocity[i] = (float)rnd_range(0,1000)/1000;
			int size = rnd_range(0,100);
			if(size > 90) stars_size[i] = rnd_range(2,3);
			else stars_size[i] = 1;
		}

		txt_intro=txt_new(C->app->app_name);
		txt_intro->use_bitmap_font=0;

		txt_version=txt_new(C->app->version);
		txt_version->use_bitmap_font=0;
	}


}

void screen_intro_draw( t_context *C)
{
	if(C->ui->show_intro || C->ui->show_intro_always)
	{
		float p[3]={(C->app->window->width)/4,(C->app->window->height)/2,0};
		int time_limit = 25;
		float scale= 5;
		int i;

		glPushMatrix();
		glLoadIdentity();

			glTranslatef(p[0],p[1],p[2]);
			glScalef(scale,scale,scale);

			int frame = C->app->frame;
			float rot = (float)frame*.05*intro_intensity;

			glRotatef(rot,0,0,1);
			glRotatef(rot,0,1,0);
			glRotatef(rot,1,0,0);

			if(C->app->frame > time_limit)
			{
				float *s=stars;
				float *col=stars_color;
				float col_var[3];
				float iii = (float) C->app->frame / 100;
				int size;
				for(i=0;i<star_count;i++)
				{
					if(iii > 1) iii= 1;
					vcp(col_var,col);
					vmul(col_var,iii*intro_intensity);

					star_intensity(i);
					C->event->ui.use_point_global_width = 0;
					size = stars_size[i];
					skt_point(s,size,col_var);
					C->event->ui.use_point_global_width = 1;
					star_mvt(i);
					s+=3;
					col+=3;
				}

			}

		glPopMatrix();
	}
}

void screen_intro(t_screen *screen)
{
	t_context *C=ctx_get();

	screen_intro_init();
	screen_switch_3d( screen);
	screen_intro_draw( C);

}

