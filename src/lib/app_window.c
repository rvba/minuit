/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "app.h"

// SCREEN -- glViewport

void app_gl_reshape(int width,int height)
{
	t_app *app = app_get();

	app->window->width=width;
	app->window->height=height;
	app->window->viewport_width=width;
	app->window->viewport_height=height;

	if(!app->window->fullscreen)
	{
		app->window->width_def=width;
		app->window->height_def=height;
	}

	app->window->change=1;

	//set viewport as window size
	glViewport(0,0,width,height);
}

// FULLSCREEN -- glViewport

void app_screen_set_fullscreen(t_app *app,int state)
{
	if(app->with_glut)
	{
		app->window->fullscreen = state;

		if (app->window->fullscreen == 1)
		{
			app->window->width = glutGet(GLUT_SCREEN_WIDTH);
			app->window->height = glutGet(GLUT_SCREEN_HEIGHT);
			app->window->change=1;
			glutFullScreen();
			glViewport(0,0,app->window->width,app->window->height);
		}
		else if (app->window->fullscreen == 0)
		{
			app->window->width=app->window->width_def; 
			app->window->height=app->window->height_def;
			app->window->change=1;
			glutReshapeWindow(app->window->width,app->window->height);
			glViewport(0,0,app->window->width,app->window->height);
		}
	}
}

void app_screen_switch_fullscreen(t_app *app)
{
	if(app->window->fullscreen) app_screen_set_fullscreen(app,0);
	else app_screen_set_fullscreen(app,1);
}

t_window *window_new(int with_glut)
{
	t_window *window = (t_window  *)malloc(sizeof(t_window));

	int width;
	int height;

	if(with_glut)
	{
		width=glutGet(GLUT_SCREEN_WIDTH)/2;
		height=glutGet(GLUT_SCREEN_HEIGHT)/2;
	}
	else
	{
		width=100;
		height=100;
	}

	window->fullscreen=0;
	window->width = width;
	window->height = height;
	window->width_def = width;
	window->height_def = height;
	window->viewport_width=width;
	window->viewport_height=height;
	window->change=0;

	return window;
}
