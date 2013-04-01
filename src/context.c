/* Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

t_context *CONTEXT;

void ctx_handler(void);

t_context *ctx_get(void)
{
	return CONTEXT;
}

t_context *ctx_new(int argc,char **argv)
{
	t_context *C = (t_context *)malloc(sizeof(t_context));

	C->app=app_init(argc,argv); 
	C->event=event_new();
	C->scene=scene_init(); 
	C->engine=engine_new("engine");
	C->draw=draw_new();
	C->skt=skt_new();
	C->ui=ui_new();
	C->term=term_new();

	C->camera=NULL;

	C->mode=mode_new();
	C->server=server_new();

	return C;
}

t_context *ctx_init(int argc,char **argv)
{
	// new context
	t_context *C = ctx_new(argc,argv);

	// store localy
	CONTEXT=C;

	// set fullscreen
	//if(SET_FULLSCREEN && !C->app->off_screen) app_screen_fullscreen(C->app);
	if(SET_FULLSCREEN && !C->app->off_screen) app_screen_set_fullscreen(C->app,1);

	// init log
	log_init();

	// init mem
	mem_init();

	// init ui
	ui_init();

	// init term,mode
	C->term->init(C->term);
	C->mode->init(C->mode);

	// init multi-threading
	// add process to scene
	ctx_thread_init(C);

	// set gl callbacks
	app_set_main_func(C->app,ctx_handler);
	app_set_dsp_func(C->app,ctx_render_scene);

	// add global
	C->scene->store=1;

	t_node *node_global=scene_add(C->scene,nt_list,"global");
	t_lst *global=node_global->data;
	C->scene->global=global;

	C->scene->store=0;

	// add camera
	t_node *node_camera=op_add_camera_main(); //! store=0
	t_object *ob_camera=node_camera->data;
	t_camera *cam=ob_camera->data;
	C->camera=cam;

	// build menus, register datas
	op_init(C); 

	// add screens to sceen
	screen_main_make();
	screen_browser_make();
	screen_desk_make();

	// add texture
	op_texture_add("./data/image.png");

	// exe init
	exe_init();

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
	// save image
	screen_record();
	// frame++
	C->app->frame++;
	// set timer
	if(C->app->timer_count) C->app->timer++;
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

	// app special keys
	t_app *app = C->app;

	if(!(app->mouse->button_left == button_pressed))
	{
		app->keyboard->ctrl=0;
		app->keyboard->shift=0;
	}
}

void ctx_update(t_context *C)
{
	ctx_app(C);
	ctx_camera(C);
	ctx_keyboard(C);	
	ctx_ui(C);
	ctx_scene(C); // selection status for: meshes,lights
	ctx_mode(C);
}

/** the main callback C->app->main_func */

void ctx_handler(void)
{
	t_context *C = CONTEXT;

	// draw 
	ctx_render(C);
	// update
	ctx_update(C);
	//reset
	ctx_reset(C);
}



