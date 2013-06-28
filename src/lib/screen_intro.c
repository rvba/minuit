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
#include "ctx.h"
#include "viewport.h"

t_txt *txt_intro;
t_txt *txt_version;

int frame_offset=0;
float *stars=NULL;
float *stars_color=NULL;
int star_init=0;
int star_count=10000;
int *stars_chance=NULL;
float *stars_velocity=NULL;
float intro_intensity=1;

int get_sign(void)
{
	int i = u_randrange(0,2);
	if(i>=1)
		return -1;
	else
		return 1;
}

int get_chance(void)
{
	int tot = 500;
	int range = 400;
	int i = u_randrange(0,tot);
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

void screen_intro(t_screen *screen)
{
	t_context *C=ctx_get();

	int time_limit = 25;

	float scale= 5;
	float scv = .3;
	float p[3]={(C->app->window->width)/4,(C->app->window->height)/2,0};
	float offset = 10;
	int dist = 500;
	int color_factor = 100;

	int i;
	if(!star_init)
	{
		star_init = 1;
		stars = malloc(sizeof(float)* star_count * 3);
		stars_color = malloc(sizeof(float)* star_count * 3);
		stars_chance = malloc(sizeof(int) * star_count);
		stars_velocity = malloc(sizeof(float) * star_count);
		for(i=0;i<star_count;i++)
		{
			stars[(i*3)+0] = u_randrange(0,dist)*get_sign();
			stars[(i*3)+1] = u_randrange(0,dist)*get_sign();
			stars[(i*3)+2] = u_randrange(0,dist)*get_sign();

			float c = (float)u_randrange(0,color_factor)/color_factor;
			stars_color[(i*3)+0] = c;
			stars_color[(i*3)+1] = c;
			stars_color[(i*3)+2] = c;

			if(get_chance()) stars_chance[i] = 1;
			else stars_chance[i] = 0;

			stars_velocity[i] = (float)u_randrange(0,1000)/1000;
		}

		//txt_intro=txt_new("minuit");
		txt_intro=txt_new(C->app->app_name);
		txt_intro->use_bitmap_font=0;
		//txt_intro->letter_width=30;

		txt_version=txt_new("0.4");
		txt_version->use_bitmap_font=0;
	}

	// Lst
	t_lst *lst = screen->viewports;

	// Link
	t_link *link = lst->first;

	// Viewport
	t_viewport *viewport = link->data;

	// Camera
	t_camera *camera = viewport->camera;

	// 2d
	op_camera_switch_2d(C,camera);

	if(C->ui->show_intro || C->ui->always_show_intro)
	{
		glPushMatrix();
		glTranslatef(p[0]-offset,p[1]-offset,p[2]);

		glPopMatrix();

		glPushMatrix();
		glLoadIdentity();
		glTranslatef(p[0],p[1],p[2]);

		glScalef(scale,scale,scale);
		float z = C->ui->zoom;
		C->ui->zoom = 5;

		glPushMatrix();

		if(C->ui->show_intro)
		{
			// Minuit
			txt_intro->draw(txt_intro);

			glTranslatef(txt_get_width(txt_intro)+1,0,0);
			// Version
			C->ui->zoom = z;
			//glTranslatef(35,0,0);
			//glTranslatef(txt_get_width(txt_intro),0,0);
			//glTranslatef(35,0,0);
			glScalef(scv,scv,scv);
			txt_version->draw(txt_version);
		}

		C->ui->zoom = z;

		// 3D
		op_camera_frustum_init(camera);
		camera->type = camera_frustum;
		camera->target[0]=500;
		camera->target[1]=500;
		camera->target[2]=500;

		camera->eye[0]=0;
		camera->eye[1]=0;
		camera->eye[2]=0;


		op_camera_switch_3d(C,camera);
		glPopMatrix();

		//glLoadIdentity();

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
			float iii = (float) C->app->frame / 500;
			for(i=0;i<star_count;i++)
			{
				if(iii > .9) iii= .9;
				vcp(col_var,col);
				vmul(col_var,iii*intro_intensity);

				star_intensity(i);
				skt_point(s,3,col_var);
				star_mvt(i);
				s+=3;
				col+=3;
			}

		}


		camera->type = camera_ortho;

		glPopMatrix();

	}
	else
	{
		/*
		if(stars) free(stars);
		if(stars_color) free(stars_color);
		if(stars_velocity) free(stars_velocity);

		stars=NULL;
		stars_color=NULL;
		stars_velocity=NULL;

		star_init = 0;
		*/
	}
}

