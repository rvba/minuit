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
#include "op.h"
#include "ctx.h"
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
#include "vlst.h"
#include "camera.h"
#include "rhizome.h"
#include "event.h"
#include "clock.h"
#include "image.h"
#include "texture.h"
#include "object.h"

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
	glWindowPos2i( x, y);

	if( image->vlst)
	{
		glDrawPixels( 
			image->width,		// GLsizei
			image->height,		// GLsizei
			image_gl_color_type( image),		// GLenum GL_RGB, GL_ALPHA, ...
			image_gl_data_type(image),// GL_UNSIGNED_BYTE, ...
			image->vlst->data);		// GLvoid *
	}
}

void ui_texture_draw( t_context *C, t_texture *texture, int px, int py, int sx, int sy)
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef( px, py, 0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, (GLuint) texture->id_gl);

		glBegin(GL_QUADS);

		glColor4f(1,1,1,1);

		glTexCoord2f(0, 0);
		glVertex2f(0, 0);

		glTexCoord2f( 1, 0);
		glVertex2f( texture->width * sx, 0);

		glTexCoord2f( 1, 1);
		glVertex2f( texture->width * sx , texture->height * sy);

		glTexCoord2f(0, 1);
		glVertex2f( 0, texture->height * sy);

		glEnd();

		glDisable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, 0);

	glPopMatrix();
}

// FREEZE ICON

void ui_draw_icon_freeze( void)
{
	t_context *C = ctx_get();

	if(!C->ui->update_links)
	{
		float *color = C->ui->front_color;
		int width = 1;

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


// TERM

void ui_draw_term(void)
{
	t_context *C=ctx_get();
	glPushMatrix();
	glTranslatef( 50, C->app->window->height - 100, 0);

	if(C->ui->show_term)
	{
		C->event->ui.use_scale = 0;

		t_link *l;
		t_term *t;
		for(l = C->terms->first; l; l = l->next)
		{
			t = ( t_term *) l->data;
			if( t->visible)
			{

				if( t->type == TERM_SIMPLE && C->draw->mode==mode_draw)
				{
					t->draw(t);
				}
				else
				{
					t->draw(t);
				}
				glTranslatef(t->width + 30, 0, 0);
			}
		}

		
		float h = -20;
		glTranslatef( 0, h, 0);
		t_node *node_screen = scene_node_get( C->scene, dt_block, "menu_screen");
		t_block *block = node_screen->data;
		block->cls->draw( block);

		C->event->ui.use_scale = 1;
	}
	glPopMatrix();
}


// DRAW SCREENS

void ui_draw_screens(t_context *C)
{
	t_lst *lst = scene_lst_get( C->scene, dt_screen);
	t_link *l = lst->first;
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

	C->event->ui.use_point_global_width = 1;
	C->event->ui.use_line_global_width = 1;
	C->event->ui.use_point_global_width = 1;
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
