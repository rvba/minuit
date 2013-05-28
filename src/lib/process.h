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

	int exit;
	int limit;
	int play;
	int cycle;
	int done;
	int busy;

}t_process;

void *ctx_compute(void *data);
void *ctx_thread_main(void *data);
void ctx_thread_init(struct Context *C);
t_process *process_new(char *name,void*(* func)(void *data));
t_process *process_add(struct Context *C,char *name,void*(* func)(void *ptr));
void process_launch(t_process *process);

void process_free(t_process *process);

#endif
