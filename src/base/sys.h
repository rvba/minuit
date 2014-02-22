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

#ifdef __cplusplus
extern "C" {
#endif

//size_t 	sys_mem_use( void);
int 	sys_file_exists( const char *path);
void 	sys_get_cd( const char *cmd);
int 	sys_get_hostname( char* name, int size);
void 	sys_git_get( void);
int sys_get( const char *cmd, char *data, int size);

#ifdef __cplusplus
}
#endif

#endif
