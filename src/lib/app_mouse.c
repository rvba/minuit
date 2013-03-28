/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "app.h"

void set_mouse_delta(t_app *app,int x,int y)
{
	t_mouse *mouse = app->mouse;
	if(mouse->button_left==button_pressed || mouse->button_right==button_pressed || mouse->button_middle==button_pressed)
	{
		int _x = app->mouse->x;
		int _y = app->mouse->y;

		int _last_x = app->mouse->last_x;
		int _last_y = app->mouse->last_y;
	
		int _dx;
		int _dy;

		int _delta_x;
		int _delta_y;

		int _sign_x=0;
		int _sign_y=0; 

		_delta_x = _x - _last_x;
		_delta_y = _y - _last_y;

		if(x > _x) {_dx = x - _x; _sign_x=1;}
		else if(x < _x) {_dx = _x - x;_sign_x=-1;}
		else _dx =0;

		if(y > _y){_dy = y - _y;_sign_y=1;}
		else if(y < _y){_dy = _y - y;_sign_y=-1;}
		else _dy =0;

		app->mouse->delta_x = _delta_x;
		app->mouse->delta_y = _delta_y;

		app->mouse->dx = _dx;
		app->mouse->dy = _dy;

		app->mouse->sign_x = _sign_x;
		app->mouse->sign_y = _sign_y;
	}
}

void app_mouse_store(t_app *app,int x,int y)
{
	app->mouse->last_x=x;
	app->mouse->last_y=app->window->height-y;
}

void app_mouse_reset(t_app *app)
{
	app->mouse->last_x=0;
	app->mouse->last_y=0;
	app->mouse->delta_x=0;
	app->mouse->delta_y=0;
}


void app_gl_mouse(int brick,int state,int x,int y)
{
	t_app *app = app_get();

	t_mouse *mouse = app->mouse;
	app_gl_keyboard_special(app); 

	app->mouse->is_moving=0;

	if (brick == GLUT_LEFT_BUTTON )
	{
		if (state == GLUT_DOWN)
		{
			// db clic
			gettimeofday(&mouse->clic_clock->now,NULL); 
			
			mouse->clic_clock->delta = 
				(mouse->clic_clock->now.tv_sec - mouse->clic_clock->before.tv_sec)
				+
				(mouse->clic_clock->now.tv_usec - mouse->clic_clock->before.tv_usec)*0.000001f;
			
			if(mouse->clic_clock->delta < .2)		
			{
				mouse->dbclic=1;
			}
			else mouse->dbclic=0;

			mouse->clic_clock->before = mouse->clic_clock->now;

			// clic
			if(app->mouse->button_left==button_released || app->mouse->button_left==button_idle)
			{
				app->mouse->button_left = button_clic;
				app_mouse_store(app,x,y);
				//##
				app->mouse->button_left=button_pressed;
			}

			
		}
		if (state == GLUT_UP)
		{
			gettimeofday(&mouse->release_clock->now,NULL);
			
			mouse->release_clock->delta = 
				(mouse->release_clock->now.tv_sec - mouse->clic_clock->now.tv_sec)
				+
				(mouse->release_clock->now.tv_usec - mouse->clic_clock->now.tv_usec)*0.000001f;

			if(mouse->release_clock->delta > .6 )		
			{
				mouse->lgclic=1;
			}
			else mouse->lgclic=0;

			mouse->release_clock->before = mouse->release_clock->now;

			app_mouse_reset(app);
			app->mouse->button_left = button_released; //

			// global blocking
			app->mouse->button_left_is_ready=1;
		}
	}

	if (brick == GLUT_MIDDLE_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			app->mouse->button_middle=button_pressed;
		}
		if (state == GLUT_UP)
		{
			app_mouse_reset(app);
			app->mouse->button_middle=button_released;
		}
	}

	if (brick == GLUT_RIGHT_BUTTON)
	{
		if (state == 0) app->mouse->button_right=button_pressed;
		else app->mouse->button_right=button_released;
	}

	if (brick == 3) // WHEEL UP
	{
		if (state == 0)
		{
			app->mouse->wheel=wheel_up;
		}
	}

	if (brick == 4) // WHELL DOWN
	{
		if (state == 0)
		{
			app->mouse->wheel=wheel_down;
		}
	}

	app->mouse->x = x;
	app->mouse->y = app->window->height - y;

	app_mouse_store(app,x,y);

	glutPostRedisplay();
}

void app_gl_passive_motion(int x,int y)
{
	t_app *app = app_get();
	
	app->mouse->x=x;
	app->mouse->y=app->window->height-y;

	glutPostRedisplay();
}

void app_gl_motion(int x,int y)
{
	t_app *app = app_get();

	app->mouse->is_moving=1;

	set_mouse_delta(app,x,app->window->height-y);

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

	glutPostRedisplay();
}

t_mouse *mouse_new(void)
{
	t_mouse *mouse  = (t_mouse *)malloc(sizeof(t_mouse));

	mouse->button_left=button_released;
	mouse->button_right=button_released;
	mouse->button_middle=button_released;
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
	mouse->button_left_is_ready=1;
	mouse->sign_x=0;
	mouse->sign_y=0;
	mouse->clic_clock=clock_new();
	mouse->release_clock=clock_new();
	mouse->dbclic=0;
	mouse->lgclic=0;

	return mouse;
}
