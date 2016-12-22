/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __VEC_H
#define __VEC_H

#include <math.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PI
#define PI 3.14159655f
#endif

#define SQ(x) ((x) *(x))

extern inline float deg_to_rad(int deg);
extern void calc_normals(float *v1,float *v2,float *v3,float *result);
extern inline void vprint3i(int *v,char end);
extern inline void vprint3f(float *v,char end);
extern inline float *vnew(float x,float y,float z);
extern inline void vprint(float v[3]);
extern inline void vcp(float t[3],float s[3]);
extern inline void vcp3f(float t[3],float s[3]);
extern inline void vcpi(int t[3],int s[3]);
extern inline void vset3i(int v[3],int a,int b,int c);
extern inline void vcp3i(int t[3],int s[3]);
extern inline void vcp4i(int t[4],int s[4]);
extern inline void vset4i(int *v,int a,int b,int c,int d);
extern inline void vset3f(float *v,float a,float b,float c);
extern inline void vset4f(float *v,float a,float b,float c,float d);
extern inline void vseti(int v[3],int a,int b,int c);
extern inline void vset(float v[3],float a,float b,float c);
extern inline void vsub(float r[3],float a[3],float b[3]);
extern inline void vadd(float r[3],float a[3],float b[3]);
extern inline void vmul(float v[3],float i);
extern inline void vcross(float r[3],float a[3],float b[3]);
extern inline float vdot(float a[3],float b[3]);
extern inline float vmag(float v[3]);
extern inline void vnormalize(float v[3]);
extern inline void cnorm(float c[3],float o[3]);
extern inline void cconv(float c[3],int o[3]);
extern inline void vplus4i(int *v,int i);
extern inline void vplus4f(float *v,float i);
extern inline float vlength3f(float v0[3], float v1[3]);
extern inline float vnorm3f(float v[3]);
extern void vrot2d3f(float *v, int angle);
extern void vcenter3f( float *r, float *a, float *b);
extern void vinverse3f( float *v);


#ifdef __cplusplus
}
#endif

#endif
