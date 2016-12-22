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

int cpt_map_fi( float x, float ostart, float oend, int tstart, int tend);
float calc_normalize(int n,int range);
float *calc_circle(float center[3],double radius,int divisions);
float *calc_arc(float center[3],double radius,int divisions, int start, int end);
void calc_normals(float *v1,float *v2,float *v3,float *result);

#ifdef __cplusplus
}
#endif

#endif

