/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __LOG_H
#define __LOG_H

#include <stdarg.h>

char *LOG;

#ifdef WITH_DEBUG
#define ulog(msg) __log__ msg 
#else
#define ulog(msg) (void)0
#endif

#define LOG_ALWAYS 1
#define LOG_MEMORY 0
#define LOG_SAVE 0
#define LOG_READ 0
#define LOG_SCENE 0
#define LOG_SCENE_NODES 0
#define LOG_CHECK 0
#define LOG_REBIND 0

void __log__(int status,char *fmt, ...);
void log_init(void);

int debug_lst;


#endif
