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

#ifdef WITH_DEBUG
#define ulog(msg) __log__ msg 
#else
#define ulog(msg) (void)0
#endif

#define LOG_DO_ALWAYS 1
#define LOG_DO_MEMORY 0
#define LOG_DO_SAVE 0
#define LOG_DO_READ 0
#define LOG_DO_SCENE 0
#define LOG_DO_SCENE_NODES 0
#define LOG_DO_CHECK 0
#define LOG_DO_REBIND 0

#define LOG_ALWAYS 1
#define LOG_MEMORY 2
#define LOG_SAVE 3
#define LOG_READ 4
#define LOG_SCENE 5
#define LOG_SCENE_NODES 6
#define LOG_CHECK 7
#define LOG_REBIND 8

void __log__( int status, int type, const char *fmt, ...);


#endif
