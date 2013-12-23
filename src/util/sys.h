/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <sys/stat.h>
#include <sys/resource.h>
#include <unistd.h>

size_t get_memory_usage(void);
void sys_get_cd(const char *cmd);
int sys_get_hostname(char* data);
void sys_git_get(void);


#endif
