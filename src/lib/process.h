/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef _PROCESS_H
#define _PROCESS_H

#include <pthread.h>

struct Context;
struct Clock;

typedef struct Process
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

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


void *		ctx_compute_rhizome(void *data);
void *		ctx_compute_objects(void *data);
void *		ctx_thread_main(void *data);
void 		ctx_thread_init(struct Context *C);

void 		process_remove(t_process *process);
t_process *	process_new(const char *name,void*(* func)(void *data));
t_process *	process_add(struct Context *C, const char *name, void *(* f)(void *d));
void 		process_launch(t_process *process);
void 		process_free(t_process *process);

#endif
