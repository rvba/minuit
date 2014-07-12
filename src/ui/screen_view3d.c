/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "screen.h"
#include "ctx.h"
#include "ui.h"
#include "event.h"
#include "draw.h"
#include "sketch.h"

void screen_view3d(t_screen *screen)
{
	t_context *C = ctx_get();
	screen_draw_controls( screen);
	screen_switch( screen);

	C->event->ui.use_point_global_width = 1;
	skt_update( C);
	t_skt *skt = skt_get();
	skt->use_line_global_width = 1;

	if( C->draw->with_scene) draw_scene(C->draw,C->scene);
}
