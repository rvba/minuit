/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "common.h"

#ifndef __PTR_H
#define __PTR_H

inline int switch_int(int i);

inline void set_float(void *_ptr,float i);
inline void set_int(void *_ptr,int i);
inline void set_uint(void *_ptr,int i);

inline void flow_int_int(void *_dst, void *_src);
inline void flow_int_float(void *_dst, void *_src);
inline void flow_float_float(void *_dst, void *_src);
inline void flow_float_int(void *_dst, void *_src);

inline void negate_int(void *_dst);
inline void negate_float(void *_dst);

inline void abs_int(void *_dst);
inline void abs_float(void *_dst);

inline void srf_float(void *ptr, void *data, int indice);
inline void srf_uint(void *ptr, void *data, int indice);
inline void srf_int(void *ptr, void *data, int indice);

inline void cprf_int(void *ptr, void *data, int indice);
inline void cprf_float(void *ptr, void *data, int indice);

inline void *grf_float(void *ptr, int indice);
inline void *grf_int(void *ptr, int indice);
inline void *grf_uint(void *ptr, int indice);

inline int drf_int(void *ptr);
inline int drf_uint(void *ptr);
inline float drf_float(void *ptr);
inline char drf_char(void *ptr);
inline char *drf_string(void *ptr);



#endif
