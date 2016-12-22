/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __OPTION_H
#define __OPTION_H

#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Dict;

typedef struct Action t_action;

struct Action
{
	t_id id;

	int done;
	struct Brick *brick;
	void (* act)(struct Action *action);
	struct Dict *args;
};


// ACTION

void action_free(t_action *action);
void *action_new(const char *name);
void exe_add_action(t_action *action);

#ifdef __cplusplus
}
#endif


#endif
