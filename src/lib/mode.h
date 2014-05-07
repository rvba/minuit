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

struct Context;
struct Lst;

typedef struct Module
{
	t_id id;

	void *data;
	void (* update)( struct Module *module);
}t_module;

typedef struct Mode
{
	t_id id;

	struct Lst *modules;
	void (* init)( struct Mode *mode);
	void (* update)( struct Mode *mode);
}t_mode;


void module_rebind( struct Scene *scene, void *ptr);
void *module_get_ref( t_module *module, const char *ref);
void module_init( t_module *module);
t_module *		mode_module_get( t_mode *mode, const char *name);
t_module *		mode_module_add( t_mode *mode, const char *name, void *data);
t_module *		ctx_module_get(struct Context *C, char *name);
void 			ctx_module_add( struct Context *C, char *name, void *data);
void 			mode_init( t_mode *mode);
void *_module_new( const char *name);
void * 			module_new( const char *name, void *data);
t_mode *		mode_new( const char *name);

#endif
