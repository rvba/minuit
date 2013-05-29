/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "op.h"
#include "context.h"
#include "scene.h"
#include "node.h"
#include "app.h"
#include "event.h"
#include "sketch.h"
#include "term.h"
#include "block.h"
#include "brick.h"
#include "ui.h"
#include "draw.h"
#include "set.h"
#include "screen.h"
#include "camera.h"
#include "list.h"
#include "data.h"
#include "camera.h"

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

void ui_draw_mouse(void)
{
	t_context *C = ctx_get();
	if(C->ui->show_mouse && (C->draw->mode != mode_selection))
	{
		float *color=C->ui->front_color;
		float pos[3];

		vset(pos,0,0,0);

		pos[0] = (float) C->app->mouse->x;
		pos[1] = (float) C->app->mouse->y;
		pos[2] = 0;

		glPushMatrix();
		glLoadIdentity();
		C->event->ui.use_point_global_width = 0;
		skt_point(pos,C->ui->mouse_size,color);
		C->event->ui.use_point_global_width = 1;
		glPopMatrix();
	}
}

void ui_draw_lines(void)
{
	t_context *C=ctx_get();
	if(C->event->is_drawing && C->draw->mode==mode_draw)
	{
		float start[3] = {
				(float)C->event->start_x,
				(float)C->event->start_y,
				0
				};

		float end[3] = {
				(float)C->event->end_x ,
				(float)C->event->end_y ,
				0
				};

		float *color=C->ui->front_color;
		skt_line(start,end,1,color);
	}
}


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

void ui_draw_intro(void)
{
	t_context *C=ctx_get();

	if(C->ui->show_intro || C->ui->always_show_intro)
	{
		int time_limit = 25;

		float scale= 5;
		float scv = .3;
		float p[3]={(C->app->window->width)/4,(C->app->window->height)/2,0};
		float offset = 10;
		glPushMatrix();
		glTranslatef(p[0]-offset,p[1]-offset,p[2]);

		glPopMatrix();

		glPushMatrix();
		glTranslatef(p[0],p[1],p[2]);

		glScalef(scale,scale,scale);
		float z = C->ui->zoom;
		C->ui->zoom = 5;

		glPushMatrix();

		if(C->ui->show_intro)
		{
			// Minuit
			txt_intro->draw(txt_intro);

			// Version
			C->ui->zoom = z;
			glTranslatef(35,0,0);
			glScalef(scv,scv,scv);
			txt_version->draw(txt_version);
		}
		C->ui->zoom = z;

		// Camera
		t_camera *camera = C->ui->camera;
		op_camera_frustum_init(camera);
		camera->type = camera_frustum;
		camera->target[0]=500;
		camera->target[1]=500;
		camera->target[2]=500;

		camera->eye[0]=0;
		camera->eye[1]=0;
		camera->eye[2]=0;

		int dist = 500;
		int color_factor = 100;

		op_camera_switch_3d(C,C->ui->camera);
		glPopMatrix();

		//glLoadIdentity();

		int frame = C->app->frame;
		float rot = (float)frame*.05*intro_intensity;

		glRotatef(rot,0,0,1);
		glRotatef(rot,0,1,0);
		glRotatef(rot,1,0,0);

		if(C->app->frame > time_limit)
		{
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
			}

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
				skt_point(s,1,col_var);
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

void ui_draw_grid(void)
{
	t_context *C=ctx_get();
	if(C->ui->show_grid && (C->draw->mode==mode_draw))
	{
		glPushMatrix();

		float color[]={0,0,0};
		int w=1;

		int i;
		float x=0;
		float y=0;

		float width,height;

		int divx,divy;
		float ori_x=1;
		float ori_y=0;

		width = (float)(C->app->window->width);
		height = (float)(C->app->window->height-1);

		divx=C->draw->divx;
		divy=C->draw->divy;

		// main frame
		float a[3]={ori_x,ori_y,0};
		float b[3]={width,ori_x,0};
		float c[3]={width,height,0};
		float d[3]={ori_x,height,0};

		skt_line(a,b,w,color);
		skt_line(b,c,w,color);
		skt_line(c,d,w,color);
		skt_line(d,a,w,color);

		// divisions
		float xx = (float)divx;
		float yy = (float)divy;
		float dx = width/xx;
		float dy = height/yy;

		// verticals
		for(i=0;i<divx-1;i++)
		{
			x+=dx;

			float l0[3]={x,0,0};
			float l1[3]={x,height,0};
			float l2[3]={x+1,0,0};
			float l3[3]={x+1,height,0};

			skt_line(l0,l1,w,color);
			skt_line(l2,l3,w,color);
		}
			
		// horizontals
		for(i=0;i<divy-1;i++)
		{
			y+=dy;

			float l0[3]={0,y,0};
			float l1[3]={width,y,0};
			float l2[3]={0,y+1,0};
			float l3[3]={width,y+1,0};

			skt_line(l0,l1,w,color);
			skt_line(l2,l3,w,color);
		}

		glPopMatrix();
	}
}

void ui_draw_status_bar(void)
{
	t_context *C=ctx_get();
	float width = (float)(C->app->window->width); 
	float height = (float)(C->app->window->height-1);
	width++;

	float d=20;
	float x0=0;
	float y0=height-d;

	float color[]={0,0,0,0};

	float scale=1;
	float txt[3]={x0+20,y0+5,0};

	glPushMatrix();
	glTranslatef(txt[0],txt[1],txt[2]);
	txt_draw_direct_2d(C->ui->status_top,txt,color,scale);
	glPopMatrix();
}

void ui_draw_term(void)
{
	t_context *C=ctx_get();
	if(C->ui->show_term)
	{
		C->event->ui.use_scale = 0;

		if(C->draw->mode==mode_draw)
		{
			t_link *l;
			t_term *t;
			for(l = C->terms->first; l; l = l->next)
			{
				t = l->data;
				t->draw(t);
			}
		}

		C->event->ui.use_scale = 1;
	}
}

// DRAW MENU 

void ui_draw_menu(void)
{
	t_context *C=ctx_get();
	t_node *node=C->scene->selected;
	t_block *menu=NULL;

	if(C->ui->show_menu)
	{
		C->event->ui.use_scale = 0;
		node=scene_node_get(C->scene,"block","menu_mouse");
		menu=node->data;
		menu->cls->draw(menu);
		C->event->ui.use_scale = 1;
	}
}

// DRAW DESK

void ui_draw_sets(void)
{
	t_context *C=ctx_get();

	t_link *link;

	for(link = C->scene->sets->first; link; link = link->next)
	{
		t_node *node = link->data;
		t_set *set = node->data;
		t_lst *lst = set->lst;
		t_link *l = lst->first;

		if(l)
		{
			for(;l;l=l->next)
			{
				t_block *b;
				b=l->data;
				b->cls->draw(b);
			}
		}
	}
}

// DRAW SCREENS

void ui_draw_screens(t_context *C)
{
	t_link *l=C->ui->screens->first;
	t_node *n;
	t_screen *s;

	if(l)
	{
		for(;l;l=l->next)
		{
			n=l->data;
			s=n->data;
			if(s->is_visible)
			{
				s->draw(s);
			}
		}
	}
}

void ui_navigation(t_context *C)
{
	// PAN
	if(C->app->mouse->button_right == button_pressed && C->app->keyboard->ctrl)
	{

		C->ui->pan_x = C->event->ui.pan_x + (C->app->mouse->x - C->app->mouse->last_x);
		C->ui->pan_y = C->event->ui.pan_y + (C->app->mouse->y - C->app->mouse->last_y);

		C->event->ui.pan = 1;
	}

	if(
		(C->event->ui.pan || C->event->ui.zoom)
		&& C->app->mouse->button_right == button_released)
	{
		C->event->ui.pan = 0;
		C->event->ui.zoom = 0;

		C->event->ui.pan_x = C->ui->pan_x;
		C->event->ui.pan_y = C->ui->pan_y;
	}

	if(C->app->mouse->button_right == button_pressed && C->app->keyboard->alt)
	{
		C->event->ui.zoom = 1;
		float zoom = C->ui->zoom;
		zoom += C->app->mouse->sign_y * C->app->mouse->dy * 0.01;

		if(zoom  > 0.1)
			C->ui->zoom+=(C->app->mouse->sign_y * C->app->mouse->dy * 0.01);
	}
}

// DRAW

void ui_draw(void)
{
	t_context *C = ctx_get();

	ui_navigation(C);
	if(C->ui->draw) ui_draw_screens(C);
	if(!C->ui->update_links)
	{
		float *color = C->ui->front_color;
		int width = 1;

		op_camera_switch_2d(C,C->ui->camera);

		glPushMatrix();
		glLoadIdentity();

			float o[]={0,0,0};
			float w=200;
			float h=200;
			float size = .2;

			glTranslatef((float)C->app->window->width - 200,50,0);
			glRotatef(45,0,0,1);
			glScalef(size,size,size);

			skt_line_rectangle(o,w,h,width,color);

		glPopMatrix();
	}
}

// INIT

void ui_init(void)
{
	txt_alphabet_make();
	txt_layout_init();
	txt_intro=txt_new("minuit");
	txt_intro->use_bitmap_font=0;

	txt_version=txt_new("0.4");
	txt_version->use_bitmap_font=0;

	t_context *C=ctx_get();
	op_set_color(C,C->draw->color);

	C->ui->camera = camera_new("camera_ui");
}

// NEW

t_ui *ui_new(void)
{
	t_ui *ui = (t_ui *)malloc(sizeof(t_ui));

	ui->draw_plug_state = 1;

	ui->show_intro=UI_SHOW_INTRO;
	ui->always_show_intro=0;
	ui->show_bricks = 0;
	ui->show_menu = 0;
	ui->show_nodes = 0;
	ui->show_meshes = 0;
	ui->show_objects = 0;
	ui->show_mouse = UI_SHOW_MOUSE;
	ui->show_term = UI_SHOW_TERM;
	ui->show_grid = 0;
	ui->show_states = 0;
	ui->show_step = 0;
	ui->show_brick_step = 0;
	ui->show_sets = 0;
	ui->step = 0;
	ui->step_reset = 0;

	ui->update_links = 1;
	ui->use_threading = 0;
	ui->threading_on = 0;
	ui->graph_updated = 1;

	ui->draw=UI_DRAW;
	ui->font_width = 1;
	ui->use_bitmap_font = 1;
	ui->fixed_menu = UI_FIXED_MENU;
	ui->flow_brick = 0;
	ui->zoom = UI_ZOOM;
	ui->pan_x = UI_PAN_X;
	ui->pan_y = UI_PAN_Y;
	ui->object_selection = 1;

	memset(ui->status_top,0,100);

	ui->brick_selected = NULL;
	ui->link = NULL;
	ui->link_node = NULL;

	ui->brick_in = NULL;
	ui->brick_out = NULL;

	ui->mouse_mode = 1;

	ui->record_camera = 0;

	vset4i(ui->background_color,255,255,255,255);
	vset4f(ui->front_color,0,0,0,0);
	vset4f(ui->back_color,1,1,1,0);
	vset3f(ui->max,0,0,0);
	ui->is_max = 0;

	ui->screens = lst_new("lst");
	ui->screen_link_active = NULL;
	ui->screen_direction = 1;
	ui->mouse_size = 9;
	ui->camera = NULL;

	return ui;
}
