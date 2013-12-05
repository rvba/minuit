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

#include "common.h"

struct Dict;

typedef struct Action t_action;

struct Action
{
	t_id id;

	void (* act)(struct Dict *args);
	struct Dict *args;
};


// ACTION

void action_free(t_action *action);
t_action *action_new(const char *name);
void exe_add_action(t_action *action);


#endif
