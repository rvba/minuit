/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "app.h"
#include "util.h"

Display *display;
XVisualInfo *vi;
Colormap cmap;
XSetWindowAttributes swa;
Window win;
GLXContext cx;
XEvent event;

static int attributeListSgl[] = { GLX_RGBA, GLX_RED_SIZE, 1, //get the deepest buffer with 1 red bit
				 GLX_GREEN_SIZE, 1,
				 GLX_BLUE_SIZE, 1,
				 None };
/*
static int attributeListDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER, //In case single buffering is not supported
				 GLX_RED_SIZE, 1,
				 GLX_GREEN_SIZE, 1,
				 GLX_BLUE_SIZE, 1,
				 None };
*/

void glx_main_loop(void)
{
	while(1)
	{
		XNextEvent(display, &event);
		switch (event.type)
		{

			unsigned int key=0;

			case KeyPress:
				key = event.xkey.keycode;
				printf("key %d\n",key);
				if (XLookupKeysym(&event.xkey, 0) == XK_Escape)
				{
				}
				if(key==38)
				{
					XCloseDisplay(display);
					exit(0);
				}
				break;
			case ButtonPress:
				printf("pos %d %d\n",event.xbutton.x,event.xbutton.y);
				break;
		}
	}
}

long event_mask = ButtonPressMask | ExposureMask | KeyPressMask;

void glx_win(t_app *app)
{
	// get a connection 
	display = XOpenDisplay(0);

	// get an appropriate visual /
	vi = glXChooseVisual(display, DefaultScreen(display), attributeListSgl);

	// create a GLX context 
	cx = glXCreateContext(display, vi, 0, GL_TRUE);

	// create a color map 
	cmap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);

	// create a window  
	swa.colormap = cmap;
	swa.border_pixel = 0;
	swa.event_mask = StructureNotifyMask;
	win = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, 800, 600, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);
	XMapWindow(display,win);

	//XIfEvent(display, &event, WaitForNotify, (char*)win);
	XSelectInput(display,win,event_mask);

	// connect the context to the window 
	glXMakeCurrent(display, win, cx);

	// clear the buffer  
	glClearColor(1,1,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

