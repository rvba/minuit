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
#include "rhizome.h"
#include "event.h"
#include "clock.h"
#include "image.h"
#include "texture.h"

t_lst *sets = NULL;

int ui_get_window_width( t_context *C)
{
	return C->app->window->width;
}

int ui_get_window_height( t_context *C)
{
	return C->app->window->height;
}

void ui_image_draw( t_context *C, t_image *image, int x, int y)
{
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1);
	glRasterPos2i( x, y);

	if( image->data)
	{
		glDrawPixels( 
			image->width,		// GLsizei
			image->height,		// GLsizei
			image->format,		// GLenum GL_RGB, GL_ALPHA, ...
			image->type,		// GL_UNSIGNED_BYTE, ...
			image->data);		// GLvoid *
	}
}

void ui_texture_draw( t_context *C, t_texture *texture, int px, int py, int sx, int sy)
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef( px, py, 0);

		glEnable(GL_TEXTURE_2D);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture( GL_TEXTURE_2D, (GLuint) texture->id_gl);

		glBegin(GL_QUADS);

		glColor4f(1,1,1,1);

		glTexCoord2f(0, 0);
		glVertex2f(0, 0);

		glTexCoord2f( 1, 0);
		glVertex2f( texture->width, 0);

		glTexCoord2f( 1, 1);
		glVertex2f( texture->width, texture->height);

		glTexCoord2f(0, 1);
		glVertex2f( 0, texture->height);

		glEnd();

		glBindTexture( GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// FREEZE ICON

void ui_draw_icon_freeze(t_context *C)
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

// MOUSE

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

// LINES

void ui_draw_lines( t_context *C)
{
	C->event->ui.use_line_global_width = 0;

	op_camera_switch_2d(C,C->ui->camera);

	glPushMatrix();
	glLoadIdentity();

	float start[3] = { (float)C->event->start_x, (float)C->event->start_y, 0 };
	float end[3] = { (float)C->event->end_x , (float)C->event->end_y , 0 };

	float *color=C->ui->front_color;
	skt_line( start, end, 1, color);
	C->event->ui.use_line_global_width = 1;

	glPopMatrix();
}

// GRID

void ui_draw_grid(void)
{
	t_context *C=ctx_get();
	if(C->ui->show_grid && (C->draw->mode==mode_draw))
	{
		glPushMatrix();

		float *color = C->draw->front_color;
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

		int usex = C->draw->usex;
		int usey = C->draw->usey;

		float ax = usex * dx;
		float ay = usey * dy ;
		float origin[3] = {ax,ay,0};
		float v1[3] = {dx,0,0};
		float v2[3] = {0,dy,0};
		int _width = 5;

		_skt_rectangle(origin,v1,v2,color,_width);

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

// TERM

void ui_draw_term(void)
{
	t_context *C=ctx_get();
	glPushMatrix();
	glTranslatef( 50, C->app->window->height - 100, 0);

	if(C->ui->show_term)
	{
		C->event->ui.use_scale = 0;

		if(C->draw->mode==mode_draw)
		{
			t_link *l;
			t_term *t;
			for(l = C->terms->first; l; l = l->next)
			{
				t = ( t_term *) l->data;
				t->draw(t);
				glTranslatef(t->width + 30, 0, 0);
			}
		}

		C->event->ui.use_scale = 1;
	}
	glPopMatrix();
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
		menu = ( t_block *) node->data;
		menu->cls->draw(menu);
		C->event->ui.use_scale = 1;
	}
}

// DRAW BAR 

void ui_draw_bar(void)
{
	t_context *C=ctx_get();
	t_node *node=C->scene->selected;
	t_block *menu=NULL;

	glPushMatrix();
	if(C->ui->show_intro)
	{
		glTranslatef(0,2,0);
		C->event->ui.use_scale = 0;
		node=scene_node_get(C->scene,"block","bar");
		menu = ( t_block *) node->data;
		menu->cls->draw(menu);
		C->event->ui.use_scale = 1;
	}
	glPopMatrix();
}

// DRAW SETS

void ui_draw_sets(void)
{
	t_context *C=ctx_get();

	t_link *link;

	for(link = C->scene->sets->first; link; link = link->next)
	{
		t_node *node = ( t_node *) link->data;
		t_set *set = ( t_set *) node->data;
		set_draw(set);
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
			n = ( t_node *) l->data;
			s = ( t_screen *) n->data;
			if(s->is_visible)
			{
				s->draw(s);
			}
		}
	}
}

// DRAW

void ui_draw(void)
{
	t_context *C = ctx_get();

	C->event->ui.use_point_global_width = 0;
	C->event->ui.use_line_global_width = 0;
	C->event->ui.use_point_global_width = 0;

	// Skt
	skt_update( C);

	// Draw Screens
	if(C->ui->draw) ui_draw_screens(C);

	// Freeze Icon
	if(!C->ui->update_links)
	{
		ui_draw_icon_freeze(C);
	}

	// Link
	if( C->ui->draw_link) ui_draw_lines( C); 

	C->event->ui.use_point_global_width = 1;
	C->event->ui.use_line_global_width = 1;
	C->event->ui.use_point_global_width = 1;
}

// INIT

void screen_init( t_context *C)
{
	t_screen *screen_main = screen_main_make( C);
	screen_browser_make( C);
	screen_sets_make( C);
	screen_bricks_make( C);
	screen_intro_make( C);
	C->ui->screen_active = screen_main;

}

// INIT

void ui_init(void)
{
	txt_alphabet_make();
	txt_layout_init();

	t_context *C=ctx_get();
	op_set_color(C,C->draw->color);

	C->ui->camera = camera_new("camera_ui");
	ctx_ui_init( C);

	C->ui->clock = clock_new( "clock");
	clock_init( C->ui->clock);
	screen_init( C);
}

// NEW

t_ui *ui_new(void)
{
	t_ui *ui = (t_ui *)mem_malloc(sizeof(t_ui));

	ui->show_plug_state = 1;

	ui->draw_link = 0;

	ui->show_intro=UI_SHOW_INTRO;
	ui->show_intro_always=0;
	ui->show_bricks = 0;
	ui->show_menu = 0;
	ui->show_nodes = 0;
	ui->show_meshes = 0;
	ui->show_objects = 0;
	ui->show_mouse = UI_SHOW_MOUSE;
	ui->show_mouse_rec = 1;
	ui->show_term = UI_SHOW_TERM;
	ui->show_grid = 0;
	ui->show_states = 0;
	ui->show_step = 0;
	ui->show_brick_step = 0;
	ui->show_sets = 0;
	ui->show_rhizome_bounding_box = UI_SHOW_RHIZOME_BOUNDING_BOX;
	ui->show_rhizome_order = UI_SHOW_RHIZOME_ORDER;
	ui->show_status = UI_SHOW_STATUS;
	ui->show_bar = UI_SHOW_BAR;

	ui->mouse_state = UI_MOUSE_IDLE;
	ui->mouse_motion = UI_MOUSE_STATIC;
	ui->mouse_x = 0;
	ui->mouse_y = 0;
	ui->mouse_last_x = 0;
	ui->mouse_last_y = 0;
	ui->mouse_last_x_pressed = 0;
	ui->mouse_last_y_pressed = 0;
	ui->mouse_dx = 0;
	ui->mouse_dy = 0;
	ui->mouse_delta_x = 0;
	ui->mouse_delta_y = 0;
	ui->mouse_delta_x_last = 0;
	ui->mouse_delta_y_last = 0;
	ui->mouse_drag = 0;
	ui->mouse_right_pressed = 0;
	ui->mouse_left_pressed = 0;

	ui->key_shift = 0;
	ui->key_alt = 0;
	ui->key_ctrl = 0;

	ui->step = 0;
	ui->step_reset = 0;
	ui->add_bricks = UI_ADD_BRICKS;

	ui->update_links = 1;

	ui->draw=UI_DRAW;
	ui->font_width = 1;
	ui->use_bitmap_font = 1;
	ui->fixed_menu = UI_FIXED_MENU;
	ui->zoom = UI_ZOOM;
	ui->pan_x = UI_PAN_X;
	ui->pan_y = UI_PAN_Y;
	ui->object_selection = 1;

	ui->brick_selected = NULL;
	ui->link = NULL;
	ui->link_node = NULL;

	ui->brick_in = NULL;
	ui->brick_out = NULL;

	vset4i(ui->background_color,255,255,255,255);
	vset4f(ui->front_color,0,0,0,0);
	vset4f(ui->back_color,1,1,1,0);

	ui->screens = lst_new("lst");
	ui->screen_active = NULL;
	ui->screen_link_active = NULL;
	ui->screen_direction = 1;
	ui->mouse_size = 9;
	ui->camera = NULL;

	ui->sets = NULL;

	ui->connect_brick_in = NULL;
	ui->connect_brick_out = NULL;
	ui->do_connect = 0;
	ui->do_disconnect = 0;

	ui->bitrate = 15000;
	ui->state = NULL;
	ui->clock = NULL;

	vset3f( ui->last_pan, 0, 0, 0);

	return ui;
}
