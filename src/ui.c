/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "app.h"
#include "event.h"

t_txt *txt_intro;

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

void ui_draw_intro(void)
{
	t_context *C=ctx_get();

	if(C->ui->show_intro)
	{
		float scale=4;
		float p[3]={(C->app->window->width)/4,(C->app->window->height)/2,0};
		glPushMatrix();
		glTranslatef(p[0],p[1],p[2]);
		glScalef(scale,scale,scale);
		txt_intro->draw(txt_intro);
		glPopMatrix();
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
}

// INIT

void ui_init(void)
{
	txt_alphabet_make();
	txt_layout_init();
	txt_intro=txt_new("minuit");
	txt_intro->use_bitmap_font=0;

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
