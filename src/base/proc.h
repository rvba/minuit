/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __PROC_H_
#define __PROC_H_

#include "common.h"
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Clock;
struct Engine;

typedef struct Process
{
	t_id id;

	pthread_t thread;
	struct Clock *clock;

	void *(* func)(void *ptr);
	void *(* loop)(void *ptr);
	void *(* exec)(void *ptr);

	void *data;

	int exit;
	int limit;
	int play;
	int cycle;
	int done;
	int busy;

	int engine_id;

}t_process;

void *		process_loop(void *data);
void 		process_remove( t_process *process, struct Engine *engine);
t_process *	process_new(const char *name,void*(* func)(void *data));
t_process *	process_add( struct Engine *engine, const char *name, void *(* f)(void *d));
t_process *	process_start( t_engine *engine, const char *name, void *(* f)(void *d), void *data, float clock);
void 		process_launch(t_process *process);
void 		process_free(t_process *process);

#ifdef __cplusplus
}
#endif


#endif
