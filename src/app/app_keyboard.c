/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "app.h"
#include "base.h"
#include "memory.h"

// KEYBOARD

void app_gl_special(int key,int x,int y)
{
	t_app *app = app_get();

	app->keyboard->special=key;

	if(key)
	{
		/*
		// transalte
		if(app->keyboard->shift)
		{
			if(key==102) // right
				app_gl_keyboard('_',x,y);
			else if(key==100) //left
				app_gl_keyboard('-',x,y);
			else if(key==101) //up
				app_gl_keyboard('&',x,y);
			else if(key==103) //down
				app_gl_keyboard(']',x,y);
		}
		// rotate
		else
		{
			if(key==102) // right
				app_gl_keyboard('@',x,y);
			else if(key==100) //left
				app_gl_keyboard('^',x,y);
			else if(key==101) //up
				app_gl_keyboard('(',x,y);
			else if(key==103) //down
				app_gl_keyboard(')',x,y);
		}
		*/
	}

	if(app->debug_keyboard)
		printf("special key:(%d)\n",key);
}

void app_gl_keyboard_special(t_app *app)
{
	#ifdef HAVE_GLUT
	int modifier = glutGetModifiers();

	app->keyboard->shift = 0;
	app->keyboard->alt = 0;
	app->keyboard->ctrl = 0;
	app->keyboard->modifier = 0;

	if (modifier == GLUT_ACTIVE_SHIFT)
	{
		app->keyboard->shift=1;
		app->keyboard->modifier = 1;
	}
	if (modifier == GLUT_ACTIVE_CTRL)
	{
		app->keyboard->ctrl=1;
		app->keyboard->modifier = 1;
	}
	if (modifier == GLUT_ACTIVE_ALT)
	{
		app->keyboard->alt=1;
		app->keyboard->modifier = 1;
	}
	#endif
}

void app_gl_keyboard(unsigned char key,int x,int y)
{
	t_app *app = app_get();

	app_gl_keyboard_special(app);

	x=y; // stop complaining

	if(key) app->keyboard->key_pressed = key;
	else app->keyboard->key_pressed = 0;

	if(app->debug_keyboard) printf("key:%c(%d)\n",key,(int)key);

	app_gl_keyboard_special(app);
}

t_keyboard *keyboard_new(void)
{
	t_keyboard *keyboard = (t_keyboard *)mem_malloc(sizeof(t_keyboard));

	keyboard->key_pressed = 0;
	keyboard->shift = 0;
	keyboard->ctrl = 0;
	keyboard->alt = 0;
	keyboard->special = 0;
	keyboard->modifier = 0;

	return keyboard;
}
