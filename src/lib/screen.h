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

struct Scene;
struct Block;
struct MINscreen;

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
	void (* keymap) (unsigned char key);

}t_screen;

// SCREEN BROWSER.C

void screen_browser_make(void);

// SCREEN_INIT.C

struct Node *screen_new_generic(char *name);
void screen_main_make(void);
void screen_sets_make(void);
void screen_bricks_make(void);
void screen_intro_make(void);

void screen_sets(t_screen *screen);

// SCREEN.C

void screen_on(t_screen *screen);
void screen_off(t_screen *screen);

void screen_block_add(t_screen *screen, struct Block *block);

void screen_switch_by_name(char *name);
void screen_switch_to_main(void);


void screen_intro(t_screen *screen);
void screen_bricks(t_screen *screen);
void screen_generic(t_screen *screen);
void screen_main(t_screen *screen);


t_screen *	screen_clone(t_screen *screen);
t_screen *	screen_rebind(struct Scene *scene, void *ptr);
void 		_screen_free(t_screen *screen);
void 		screen_free(t_screen *screen);
t_screen *	screen_new(const char *name);
void 		screen_init(void);

#endif
