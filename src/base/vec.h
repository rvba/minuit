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

#ifdef __cplusplus
extern "C" {
#endif

inline float deg_to_rad(int deg);
void calc_normals(float *v1,float *v2,float *v3,float *result);
inline void vprint3i(int *v,char end);
inline void vprint3f(float *v,char end);
inline float *vnew(float x,float y,float z);
inline void vprint(float v[3]);
inline void vcp(float t[3],float s[3]);
inline void vcp3f(float t[3],float s[3]);
inline void vcpi(int t[3],int s[3]);
inline void vset3i(int v[3],int a,int b,int c);
inline void vcp3i(int t[3],int s[3]);
inline void vcp4i(int t[4],int s[4]);
inline void vset4i(int *v,int a,int b,int c,int d);
inline void vset3f(float *v,float a,float b,float c);
inline void vset4f(float *v,float a,float b,float c,float d);
inline void vseti(int v[3],int a,int b,int c);
inline void vset(float v[3],float a,float b,float c);
inline void vsub(float r[3],float a[3],float b[3]);
inline void vadd(float r[3],float a[3],float b[3]);
inline void vmul(float v[3],float i);
inline void vcross(float r[3],float a[3],float b[3]);
inline float vdot(float a[3],float b[3]);
inline float vmag(float v[3]);
inline void vnormalize(float v[3]);
inline void cnorm(float c[3],float o[3]);
inline void cconv(float c[3],int o[3]);
inline void vplus4i(int *v,int i);
inline void vplus4f(float *v,float i);
inline float vlength3f(float v0[3], float v1[3]);
inline float vnorm3f(float v[3]);
void vrot2d3f(float *v, int angle);
void vcenter3f( float *r, float *a, float *b);
void vinverse3f( float *v);

#ifdef __cplusplus
}
#endif

#endif
