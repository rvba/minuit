/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "scene.h"
#include "node.h"
#include "screen.h"
#include "context.h"
#include "event.h"
#include "ui.h"

void ctx_ui_browser( t_context *C)
{
	ctx_ui_log( "ui_browser");
	t_node *node = scene_node_get( C->scene, "block", "block_browser");
	if( node)
	{
		screen_switch_by_name( "screen_browser");
		ctx_ui_selection_set( C, node);
		ctx_show_browser = 1;
		UI_EVENT = UI_EVENT_NULL;
		UI_SWAP( C, state_ui_block_trigger); 
	}
}
