/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "app.h"
#include "SDL.h"

int quit = 0;

SDL_Window *window = NULL;                    // Declare a pointer

int sdl_init( int argc, char **argv)
{

	SDL_Init(SDL_INIT_VIDEO);         
	window = SDL_CreateWindow(
		"MINUIT",                  
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,                   
		480,                  
		SDL_WINDOW_OPENGL   
	);

	if (window == NULL)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}

void sdl_quit( void)
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void sdl_swap( t_app *app)
{
}

void sdl_mainloop( t_app *app)
{
	SDL_Event e;
	while( !quit)
	{
		while( SDL_PollEvent( &e))
		{
			if( e.type == SDL_KEYDOWN)
			{
				quit = 1;
			}

			//app->main_func();
		}
	}

	sdl_quit();
}
