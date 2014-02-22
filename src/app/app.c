/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#define APP_DEBUG 0
#define APP_LOOP 0
#define APP_BUFFER 1
#define APP_PLAY 1
#define APP_WITH_GLUT 1
#define APP_VIDEO_LIMIT 20

#include "opengl.h"
#include "util.h"
#include "app.h"
#include "list.h"
#include "clock.h"
#include "file.h"
#include "memory.h"

// used for GL calls without args
t_app *APP;

t_app *app_get(void)
{
	return APP;
}

void app_clock(t_app *app)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned int delta = (unsigned int)((tv.tv_sec*1000 + tv.tv_usec/1000) - app->clock->start_time); 

	app->clock->msec=delta;

	double seconds = (double)(delta/1000);
	double intpart;

	modf(seconds,&intpart);

	int sec = intpart - (app->clock->min*60);
	
	if(sec>59)
	{
		app->clock->min++;
		app->clock->sec=1;
	}
	else
	{
		app->clock->sec=sec;
	}
}

void app_redisplay(t_app *app)
{
	if(app->off_screen)
	{
		APP->main_func();
	}
	else
	{
		glutPostRedisplay();
	}
}

void app_sleep(t_app *app)
{
	gettimeofday(&app->clock->now,NULL);

	// compute delta time from last call
	app->clock->delta = 
		(app->clock->now.tv_sec - app->clock->before.tv_sec)
		+
		(app->clock->now.tv_usec - app->clock->before.tv_usec)*0.000001f;

	// if delta is less than limit
	if (app->clock->delta < app->clock->limit)
	{
		//1 hz
		if(app->clock->limit - app->clock->delta < 1.0f)
		{
			// sleep
			usleep((int)((app->clock->limit - app->clock->delta)*1000000.0f));
		}
	}
	else
	{
		// record time now
		app->clock->before=app->clock->now;
		app_redisplay(app);
	}
}

void app_gl_idle(void)
{
	t_app *app = APP;

	// reset mouse
	app->mouse->dx=0;
	app->mouse->dy=0;

	// loop
	if(app->loop) glutPostRedisplay();
	// or sleep
	else app_sleep(app);
}

void app_default_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void app_gl_display(void)
{
	APP->main_func();
}

void app_swap(t_app *app)
{
	// double buffer
	if (app->buffer==1)
	{
		glDrawBuffer(GL_BACK);
		glutSwapBuffers();
	}
	// single buffer
	else
	{
		glDrawBuffer(GL_FRONT);
		glFlush();
	}
}

void app_launch(t_app *app)
{
	if(app->off_screen)
	{
		//app_off_screen(app);
		if(app->client)
		{
			 tcp_client();
		}
		else if(app->slave)
		{
			 slave();
		}
		else if(app->osc_server)
		{
			osc_server( app->osc_port);
		}
		else if(app->osc_client)
		{
			osc_client( app->osc_port);
		}
	}
	else if(app->with_glut==0)
	{
		//glx_main_loop();	
	}
	else
	{
		glutMainLoop();
	}
}
void app_scan_port( t_app *app, const char *data)
{
	app->osc_port = atoi( data);
	//printf("OSC port: %d\n", app->osc_port);
}

void app_args_scan(t_app *app)
{
	int i;
	int scan_port = 0;

	for(i=0;i<app->argc;i++)
	{
		if( scan_port)
		{
			app_scan_port( app, app->argv[i]);
			scan_port = 0;
		}

		if(is(app->argv[i],"off"))
		{
			app->off_screen=1;
		}
		
		if(is(app->argv[i],"x"))
		{
			app->with_glut=0;
		}

		if(is(app->argv[i],"client"))
		{
			app->off_screen=1;
			app->client=1;
		}

		if(is(app->argv[i],"slave"))
		{
			app->off_screen=1;
			app->slave=1;
		}

		if(is(app->argv[i],"load"))
		{
			char *path = app->argv[2];
			printf("load %s\n",path);
			app->load_file = 1;
			app->file_path=s_allocate(path);
		}

		if(is(app->argv[i],"osc_server"))
		{
			app->off_screen = 1;
			app->osc_server = 1;
		}

		if(is(app->argv[i],"osc_client"))
		{
			app->off_screen = 1;
			app->osc_client = 1;
		}

		if(is(app->argv[i],"-port"))
		{
			scan_port = 1;
		}
	}
}

int app_free(t_app *app)
{
	mem_free( app->mouse, sizeof( t_mouse));
	mem_free( app->keyboard, sizeof( t_keyboard));
	mem_free(app->window, sizeof( t_window));
	mem_free(app->clock, sizeof( t_clock));

	return 1;
}

void app_init_home( t_app *app)
{
	int size = 16;
	char name[size];

	int size_home = _PATH_;
	char path_home[size_home]; 

	sys_get( "whoami", name, size);
	
	s_cp( path_home, "/home/", size_home);
	s_cat( path_home, name, size);
	s_cat( path_home, "/.minuit/", size_home);

	if( !sys_file_exists( path_home))
	{
		printf("Creating minuit home directory: %s\n", path_home);
		mkdir( path_home, 0777);
	}

	s_cp( app->app_home, path_home, _PATH_);
}

// INIT

void app_init(t_app *app, const char *name)
{
	// Store Localy
	APP = app;

	// Set Version
	set_name(app->version, APP_VERSION);

	// Home
	app_init_home( app);

	// GL
	if(app->off_screen)
	{
		//printf("(OFF)\n");
	}
	else
	{
		if(app->with_glut)
		{
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL | GLUT_ALPHA);
			glutInitWindowPosition(WIN_X,WIN_Y);
			glutInitWindowSize(app->window->width,app->window->height);
			glutCreateWindow(app->name);
			glutDisplayFunc(app_gl_display);
			glutMouseFunc(app_gl_mouse);
			glutKeyboardFunc(app_gl_keyboard);
			glutSpecialFunc(app_gl_special);
			glutReshapeFunc(app_gl_reshape);
			glutMotionFunc(app_gl_motion);
			glutPassiveMotionFunc(app_gl_passive_motion);
			glutIdleFunc(app_gl_idle);
		}
		//else glx_win(app);
	}

	// Set Fullscreen
	if(SET_FULLSCREEN && !app->off_screen) app_screen_set_fullscreen(app,1);

	set_name(app->app_name,name);
}

// NEW

t_app *app_new(int argc,char **argv)
{
	t_app *app = (t_app *)mem_malloc(sizeof(t_app));

	app->argc=argc;
	app->argv=argv;
	set_name(app->name,"minuit");
	app->main_func = app_default_func;
	app->off_screen=0;
	app->frame=0;
	app->timer=0;
	app->timer_add = 1;
	app->timer_add_low = .1;
	app->timer_count=0;
	app->loop=APP_LOOP;
	app->buffer=APP_BUFFER;
	app->with_glut=APP_WITH_GLUT;
	app->debug_keyboard=0;
	app->load_file = 0;
	app->loaded_file = 0;

	app->client = 0;
	app->slave = 0;
	app->osc_server = 0;
	app->osc_port = 0;

	// ARGS
	app_args_scan(app);

	// GLUT
	if(app->with_glut) glutInit(&app->argc, app->argv);

	app->mouse=mouse_new();
	app->window=window_new(app->with_glut);
	app->keyboard=keyboard_new();
	app->clock=clock_new( "clock");

	app->file = file_new("void");
	file_init(app->file);

	app->video_frames = lst_new("video");
	app->video_frames_swap = lst_new("video swap");
	app->video_offset = 0;
	app->video_build = 1;
	app->video_limit = APP_VIDEO_LIMIT;

	clock_init(app->clock);

	app->quit = 0;

	return app;
}
