/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __RND_H
#define __RND_H

#ifdef __cplusplus
extern "C" {
#endif

float rndr(void);
float rndf(void);
void rnd_init(void);
int rnd(int n);
int rnd_range(int start,int end);

#ifdef __cplusplus
}
#endif

#endif
