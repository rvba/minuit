/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __SCREEN_H_
#define __SCREEN_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Context;
struct Scene;
struct Block;
struct MINscreen;
struct Camera;

typedef struct MINscreen
{
	t_id id;

	int idcol[3];

	int is_active;
	int is_visible;
	int always_active;
	int always_visible;
	int width;
	int height;
	int zoom;
	int pan_x;
	int pan_y;

	struct Lst *blocks;
	struct Lst *viewports;

	void (* draw) (struct MINscreen *screen);
	void (* keymap) (int key);
	void *data;

}t_screen;

// SCREEN BROWSER.C

t_screen *	screen_browser_make( struct Context *C);

// SCREEN_INIT.C

t_screen *	screen_new_generic( struct Context *C, char *name);
t_screen *	screen_main_make( struct Context *C);
t_screen *	screen_sets_make( struct Context *C);
t_screen * 	screen_bricks_make( struct Context *C);
t_screen * 	screen_intro_make( struct Context *C);

void screen_sets(t_screen *screen);

// SCREEN.C

void 		screen_always(t_screen *screen);
void 		screen_on(t_screen *screen);
void 		screen_off(t_screen *screen);
void 		screen_block_add(t_screen *screen, struct Block *block);
void 		screen_switch_by_name( const char *name);
void 		screen_switch_to_main( void);


t_screen *	screen_clone(t_screen *screen);
t_screen *	screen_rebind(struct Scene *scene, void *ptr);
void 		_screen_free(t_screen *screen);
void 		screen_free(t_screen *screen);
t_screen *	screen_new(const char *name);
void 		screen_init( struct Context *C);

#ifdef __cplusplus
}
#endif

#endif
