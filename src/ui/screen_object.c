/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "screen.h"
#include "context.h"
#include "util.h"
#include "object.h"
#include "scene.h"
#include "node.h"
#include "opengl.h"

void screen_object( t_screen *screen)
{
	t_context *C = ctx_get();
	t_node *node = C->scene->selected;
	if( node)
	{
		if( node->type == dt_object)
		{
			t_object *object = node->data;

				screen_switch_2d( screen);
				glPushMatrix();
				glLoadIdentity();

				object_draw_blocks( C, object);

				glPopMatrix();
		}
	}
}
