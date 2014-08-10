/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "base.h"
#include "app.h"
#include "clock.h"
#include "memory.h"

void app_gl_mouse( int button, int state, int x, int y)
{
	t_app *app = app_get();

	app_gl_keyboard_special(app); 

	app->mouse->is_moving=0;
	app->mouse->event = 1;
	app->mouse->button = 1;

	if (button == GLUT_LEFT_BUTTON )
	{
		if (state == GLUT_DOWN) app->mouse->button_left = button_pressed;
		else app->mouse->button_left = button_released;
	}

	if (button == GLUT_MIDDLE_BUTTON)
	{
		if (state == GLUT_DOWN) app->mouse->button_middle = button_pressed;
		else app->mouse->button_middle=button_released;
	}

	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN) app->mouse->button_right = button_pressed;
		else app->mouse->button_right=button_released;
	}

	if (button == 3) // WHEEL UP
	{
		if (state == 0)
		{
			app->mouse->wheel=wheel_up;
		}
	}

	if (button == 4) // WHELL DOWN
	{
		if (state == 0)
		{
			app->mouse->wheel=wheel_down;
		}
	}

	app->mouse->x = x;
	app->mouse->y = app->window->height - y;

	glutPostRedisplay();
}

void app_gl_passive_motion(int x,int y)
{
	t_app *app = app_get();
	
	app->mouse->event = 1;
	app->mouse->motion_passive = 1;

	app->mouse->x=x;
	app->mouse->y=app->window->height-y;

	app_gl_keyboard_special(app);

	glutPostRedisplay();
}

void app_gl_motion(int x,int y)
{
	t_app *app = app_get();

	app->mouse->event = 1;
	app->mouse->motion = 1;
	app->mouse->is_moving=1;

	app->mouse->x=x;
	app->mouse->y=app->window->height-y;

	if(app->mouse->button_left==button_clic)
	{
		app->mouse->button_left=button_pressed;
	}

	if(app->mouse->button_middle==button_clic)
		app->mouse->button_middle=button_pressed;

	if(app->mouse->button_left==button_pressed)
	{
		gettimeofday(&app->mouse->clic_clock->now,NULL);
	}

	app_gl_keyboard_special(app);

	glutPostRedisplay();
}

t_mouse *mouse_new(void)
{
	t_mouse *mouse  = (t_mouse *)mem_malloc(sizeof(t_mouse));

	mouse->button_left=button_idle;
	mouse->button_right=button_idle;
	mouse->button_middle=button_idle;
	mouse->wheel=wheel_idle;

	mouse->x=0;
	mouse->y=0;
	mouse->delta_x=0;
	mouse->delta_y=0;
	mouse->last_x=0;
	mouse->last_y=0;
	mouse->dx=0;
	mouse->dy=0;
	mouse->is_moving=0;
	mouse->event = 0;
	mouse->motion = 0;
	mouse->motion_passive = 0;
	mouse->button = 0;
	mouse->button_left_is_ready=1;
	mouse->sign_x=0;
	mouse->sign_y=0;
	mouse->clic_clock = clock_new( "clock");
	mouse->release_clock = clock_new( "clock");
	mouse->dbclic=0;
	mouse->lgclic=0;

	return mouse;
}
