/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "screen.h"
#include "camera.h"
#include "ctx.h"
#include "opengl.h"
#include "ui.h"
#include "event.h"
#include "util.h"
#include "block.h"
#include "app.h"
#include "term.h"
#include "draw.h"
#include "node.h"
#include "scene.h"
#include "brick.h"
#include "plug.h"
#include "block.h"
#include "object.h"
#include "vlst.h"


void brick_info( t_term *term, t_brick *brick)
{
	t_plug *plug = &brick->plug_intern;
	t_data_type type = plug->data_type;
	term_l( term, "%s", data_name_get( type));
	switch(type)
	{

		case( dt_vlst):
			vlst_info( term, ( t_vlst *) plug->data);

			break;
		default:
			break;
	}
}

void screen_info( t_screen *screen)
{
	t_context *C=ctx_get();
	screen_switch_2d( screen);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef( 50, C->app->window->height - 100, 0);

		C->event->ui.use_scale = 0;

		t_term *t = term_get("term_info");
		term_reset( t);
		t_node *node = C->scene->hover;
		t_block *block;
		t_brick *brick;
		t_object *object;
		if( node)
		{
			term_l( t, "%d", node->id);
			t_data_type type = node->type;
			switch( type)
			{
				case( dt_block):
					block = node->data;
					term_l( t, "%s", block->id.name);
					break;
				case( dt_brick):
					brick = node->data;
					term_l( t, "%s", brick->id.name);
					brick_info( t, brick);
					break;
				case( dt_object):
					object = node->data;
					term_l( t, "%s", object->id.name);
					break;
				default:
					break;
			}

		}


		if( t->type == TERM_SIMPLE && C->draw->mode==mode_draw)
		{
			t->draw(t);
		}
		else
		{
			t->draw(t);
		}
		glTranslatef(t->width + 30, 0, 0);


		C->event->ui.use_scale = 1;

	glPopMatrix();
}
