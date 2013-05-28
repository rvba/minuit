/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __MODE_H_
#define __MODE_H_

#include "common.h"

typedef struct Mode t_mode;
typedef struct Module t_module;

struct Context;
struct Lst;

struct Module
{
	char name[10];
	void *data;
	void (* update)(t_module *module);
};

struct Mode
{
	struct Lst *modules;
	void (* init)(t_mode *mode);
	void (* update)(t_mode *mode);
};

t_module *mode_module_get(t_mode *mode,char *name);
t_module *mode_module_add(t_mode *mode,char *name,void *data);

t_module *ctx_module_get(struct Context *C,char *name);
void ctx_module_add(struct Context *C,char *name,void *data);
t_mode *mode_new(void);
void mode_init(t_mode *mode);

#endif
