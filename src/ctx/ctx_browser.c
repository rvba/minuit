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
#include "ctx.h"
#include "event.h"
#include "ui.h"

int BROWSER_EXIT = 0;

void browser_exit( t_context *C)
{
	ctx_ui_log( "browser_exit");
	BROWSER_EXIT = 0;
	screen_switch_by_name( "screen_main");
	ctx_show_browser = 0;
	UI_EVENT = UI_EVENT_NULL;
	UI_STATE = state_ui_default;
	UI_SWAP( C, state_ui_default); 
}

void state_browser( t_context *C, t_event *e)
{
	ctx_ui_log( "state_browser");

	if( BROWSER_EXIT)
	{
		browser_exit( C);
	}
	else
	{
		t_node *node = scene_node_get( C->scene, "block", "block_browser");

		switch( e->type)
		{
			case ESCKEY:

				browser_exit( C);
				break;
			default:

				ctx_ui_selection_set( C, node);
				UI_SWAP( C, state_ui_block_trigger); 

				break;
		}
	}
}

void ctx_ui_browser( t_context *C)
{
	ctx_ui_log( "ui_browser");
	t_node *node = scene_node_get( C->scene, "block", "block_browser");
	if( node)
	{
		screen_switch_by_name( "screen_browser");
		ctx_ui_selection_set( C, node);
		ctx_show_browser = 1;
		UI_STATE = state_browser;
		UI_SWAP( C, state_ui_block_trigger); 
	}
}
