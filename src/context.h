/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __CONTEXT_H
#define __CONTEXT_H

#define ZOOM 1
#define PAN_X 0
#define PAN_Y 0

#define WITH_HIGHLIGHT 0
#define GLFONT 1
#define FREE_SCENE 0
#define DEBUG_PROCESS 0
#define DEBUG_CONSOLE 1
#define DEBUG_TERMINAL 0
#define DEBUG_LOOP 0

#define CTX_SHOW_TERM 1
#define CTX_FIXED_BUTTON 0
#define CTX_DEBUG_MOUSE 0
#define CTX_DEBUG_SELECT 0
#define EVENT_WITH_POLYGON_OFFSET 0

#define SKETCH_WITH_SCALE 1

typedef struct Context
{
	struct App *app;
	struct Mode *mode;
	struct Engine *engine;
	struct MNserver *server;
	struct Scene *scene;
	struct Event *event;
	struct Ui *ui;
	struct Draw *draw;
	struct Sketch *skt;
	struct Term *term;
	struct Lst *terms;

}t_context;

t_context *ctx_get(void);
t_context *ctx_init(int argc,char **argv);


#endif
