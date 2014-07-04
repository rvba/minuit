/* Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "ctx.h"
#include "scene.h"
#include "app.h"
#include "mode.h"
#include "event.h"
#include "sketch.h"
#include "term.h"
#include "draw.h"
#include "ui.h"
#include "memory.h"
#include "log.h"
#include "screen.h"
#include "list.h"
#include "brick.h"
#include "util.h"

t_context *CONTEXT;

void ctx_handler(void);

t_context *ctx_get(void)
{
	return CONTEXT;
}

t_scene *ctx_scene_get(void)
{
	return CONTEXT->scene;
}

t_context *ctx_new(int argc,char **argv)
{
	t_context *C = (t_context *) mem_malloc(sizeof(t_context));

	C->app = app_new(argc,argv);
	C->event = main_event_new();
	C->scene = scene_new(); 
	C->engine = engine_new( "engine");
	C->draw = draw_new();
	C->skt = skt_new();
	C->ui = ui_new();
	C->terms = lst_new( "terms");
	C->mode = mode_new( "mode");
	C->server = server_new( "server");

	return C;
}

void ctx_init_file( t_context *C)
{
	mode_module_add( C->mode, "save", mod_save);
	mode_module_add( C->mode, "load", mod_load);
}

t_context *ctx_init(int argc,char **argv)
{
	t_context *C = ctx_new(argc,argv);

	CONTEXT=C;

	scene_init( C->scene);
	app_init( C->app, "minuit");
	log_init();
	term_init();
	mem_init();
	op_init( C); 
	ui_init();
	mode_init( C->mode);
	ctx_thread_init( C);
	exe_init();

	// Set GL Callback
	C->app->main_func = ctx_handler;

	ctx_init_file( C);


	return C;
}

void ctx_load_file(t_context *C)
{
	if(C->app->load_file)
	{
		C->app->load_file = 0;
		C->app->loaded_file = 1;
		add_mn();
	}
}

void ctx_app(t_context *C)
{
	ctx_load_file(C);
	app_clock(C->app);
	C->app->frame++;
	if(C->app->timer_count) C->app->timer += C->app->timer_add;
}

void ctx_mode(t_context *C)
{
	C->mode->update(C->mode);
}

void ctx_engine( t_context *C)
{
	engine_cleanup( C->engine);
}

void ctx_clean( t_context *C)
{
	scene_cleanup(C->scene);
}

void ctx_update(t_context *C)
{
	ctx_app(C);
	ctx_keyboard(C);	
	ctx_mouse( C);

	ctx_event( C);
	ctx_ui( C);

	ctx_render( C);

	ctx_mode( C);
	ctx_compute( C);
	ctx_engine( C);
	ctx_clean( C);
}

void ctx_handler(void)
{
	if(CONTEXT->app->quit) op_post_quit(NULL); 
	else  ctx_update( CONTEXT); 
}



