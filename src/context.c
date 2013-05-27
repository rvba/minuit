/* Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "ctx.h"
#include "app.h"
#include "mode.h"
#include "engine.h"
#include "server.h"
#include "event.h"
#include "sketch.h"

t_context *CONTEXT;

void ctx_handler(void);

t_context *ctx_get(void)
{
	return CONTEXT;
}

t_context *ctx_new(int argc,char **argv)
{
	t_context *C = (t_context *)malloc(sizeof(t_context));

	C->app = app_new(argc,argv);
	C->event=event_new();
	C->scene=scene_new(); 
	C->engine=engine_new("engine");
	C->draw=draw_new();
	C->skt=skt_new();
	C->ui=ui_new();
	C->term=term_new("main");
	C->terms = lst_new("terms");
	C->mode=mode_new();
	C->server=server_new();

	return C;
}

t_context *ctx_init(int argc,char **argv)
{
	// New Context
	t_context *C = ctx_new(argc,argv);

	// Store Local
	CONTEXT=C;

	// Init
	scene_init(C->scene);
	app_init(C->app);
	log_init();
	mem_init();
	ui_init();
	term_init();
	mode_init(C->mode);
	ctx_thread_init(C);
	op_init(C); 
	screen_init();
	exe_init();

	// Set GL Callback
	C->app->main_func = ctx_handler;

	return C;
}

void ctx_load_file(t_context *C)
{
	if(C->app->load_file)
	{
		C->app->load_file = 0;
		C->app->loaded_file = 1;
		C->event->standby_string = C->app->file_path;
		add_mn();
		C->event->standby_string = NULL;
	}
}

void ctx_app(t_context *C)
{
	ctx_load_file(C);
	// set time
	app_clock(C->app);
	// frame++
	C->app->frame++;
	// set timer
	if(C->app->timer_count) C->app->timer += C->app->timer_add;
	// engine
	engine_cleanup(C->engine);
}

void ctx_mode(t_context *C)
{
	C->mode->update(C->mode);
}

void ctx_reset(t_context *C)
{
	// event
	t_event *event = C->event;

	event->switch_plug_in_flow_in = 0;
	event->switch_plug_in_flow_out = 0;
	event->switch_plug_in_open_in = 0;
	event->switch_plug_in_open_out = 0;
	event->switch_plug_in_follow_in = 0;
	event->switch_plug_in_follow_out = 0;

	event->switch_plug_out_flow_in = 0;
	event->switch_plug_out_flow_out = 0;
	event->switch_plug_out_open_in = 0;
	event->switch_plug_out_open_out = 0;
	event->switch_plug_out_follow_in = 0;
	event->switch_plug_out_follow_out = 0;

	event->switch_brick_debug = 0;

	// app special keys
	t_app *app = C->app;

	if(
		   !(app->mouse->button_left == button_pressed) 
		&& !(app->mouse->button_right == button_pressed)
		)
	{
		app->keyboard->ctrl=0;
		app->keyboard->shift=0;
		app->keyboard->alt = 0;
	}
}

void ctx_update(t_context *C)
{
	ctx_app(C);
	ctx_camera(C);
	ctx_keyboard(C);	
	ctx_ui(C);
	ctx_scene(C); 
	ctx_mode(C);
}

/** the main callback C->app->main_func */

void ctx_handler(void)
{
	ctx_render(CONTEXT);
	ctx_update(CONTEXT);
	ctx_reset(CONTEXT);
}



