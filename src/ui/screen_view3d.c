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
#include "context.h"
#include "opengl.h"
#include "ui.h"
#include "event.h"
#include "util.h"
#include "block.h"

#include "app.h"
#include "txt.h"
#include "viewport.h"

#include "draw.h"

void screen_view3d(t_screen *screen)
{
	t_context *C = ctx_get();
	screen_draw_controls( screen);
	screen_switch( screen);

	if( C->draw->with_scene) draw_scene(C->draw,C->scene);
}
