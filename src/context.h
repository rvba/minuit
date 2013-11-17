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

void ctx_test(void);
t_context *ctx_get(void);
struct Scene *ctx_scene_get(void);
t_context *ctx_init(int argc,char **argv);


#endif
