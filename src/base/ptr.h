/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#ifndef __PTR_H
#define __PTR_H


#ifdef __cplusplus
extern "C" {
#endif

extern inline int switch_int(int i);

extern inline void set_float(void *_ptr,float i);
extern inline void set_int(void *_ptr,int i);
extern inline void set_uint(void *_ptr,int i);

extern inline void flow_int_int(void *_dst, void *_src);
extern inline void flow_int_float(void *_dst, void *_src);
extern inline void flow_float_float(void *_dst, void *_src);
extern inline void flow_float_int(void *_dst, void *_src);

extern inline void negate_int(void *_dst);
extern inline void negate_float(void *_dst);

extern inline void abs_int(void *_dst);
extern inline void abs_float(void *_dst);

extern inline void srf_float(void *ptr, void *data, int indice);
extern inline void srf_uint(void *ptr, void *data, int indice);
extern inline void srf_int(void *ptr, void *data, int indice);

extern inline void cprf_int(void *ptr, void *data, int indice);
extern inline void cprf_float(void *ptr, void *data, int indice);

extern inline void *grf_float(void *ptr, int indice);
extern inline void *grf_int(void *ptr, int indice);
extern inline void *grf_uint(void *ptr, int indice);

extern inline int drf_int(void *ptr);
extern inline int drf_uint(void *ptr);
extern inline float drf_float(void *ptr);
extern inline char drf_char(void *ptr);
extern inline char *drf_string(void *ptr);


#ifdef __cplusplus
}
#endif


#endif
