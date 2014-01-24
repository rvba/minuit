/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __CPT_H
#define __CPT_H

#ifdef __cplusplus
extern "C" {
#endif

float calc_normalize(int n,int range);
float *calc_circle(float center[3],double radius,int divisions);
float *calc_arc(float center[3],double radius,int divisions, int start, int end);

#ifdef __cplusplus
}
#endif

#endif

