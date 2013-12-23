/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __UTIL_H
#define __UTIL_H

#define SQ(x) ((x) *(x))

#include "mem.h"
#include "vec.h"
#include "rnd.h"
#include "vol.h"
#include "str.h"
#include "net.h"
#include "cpt.h"
#include "sys.h"
#include "lst.h"
#include "ptr.h"

extern int db;
void color_id(int *color);
void id_init(t_id *id, const char *name);

void set_name(char *dst,const char *src);
void set_name_long(char *dst,const char *src);
void set_path(char *dst,const char *src);
inline int is(const char a[],const char b[]);


#endif


