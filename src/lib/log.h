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

char *LOG;

#ifdef WITH_DEBUG
#define log(msg) __log__ msg 
#else
#define log(msg) (void)0
#endif

void __log__(int status,char *fmt, ...);

int debug_lst;


#endif
