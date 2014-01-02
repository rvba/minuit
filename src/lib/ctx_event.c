/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "node.h"
#include "app.h"
#include "event.h"
#include "camera.h"
#include "ui.h"
#include "list.h"
#include "util.h"
#include "screen.h"
#include "ctx.h"

void ctx_event_exec( t_context *C, t_event *event)
{
	if( EVENT_KEYBOARD( event->type))
	{
		ctx_ui_keyboard_set( C, event);
		t_screen *screen = event->screen;
		if( screen) screen->keymap( event->type);
	}

	if( EVENT_MOUSE( event->type))
	{
		ctx_ui_mouse_set( C, event);
	}
}

void ctx_event_cleanup( t_context *C)
{
	t_link *l;
	t_event *event;
	for(l=C->event->events->first;l;l=l->next)
	{
		event = l->data;
		event_free( event);
	}

	lst_cleanup( C->event->events);
}

void ctx_event_dispatch( t_context *C)
{
	t_link *l;
	t_main_event *main_event = C->event;
	t_lst *lst = main_event->events;
	t_event *event = NULL;

	for(l=lst->first;l;l=l->next)
	{
		event = l->data;
		ctx_event_exec( C, event);
	}

}

void ctx_event( t_context *C)
{
	ctx_event_dispatch( C);
}
