/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __VLST_H
#define __VLST_H

#ifdef __cplusplus
extern "C" {
#endif

struct Brick;

typedef struct VLst
{
	t_id id;

	// generic

	int count;	// number of block
	int length;	// number of element in block 
	int size;	// size of element (int,float,...)

	int count_new;
	int need_update;
	int is_linked;

	struct VLst *link;

	t_data_type type;
	t_data_type type_target;

	void *data;
	struct Block *ref;
}t_vlst;


void 		vlst_update_data( struct Brick *brick, t_vlst *vlst,t_vlst *caller);
void		vlst_free(t_vlst *vlst);
void 		vlst_delete( t_vlst *vlst);
void 		vlst_init(t_vlst *vlst);
t_vlst *	vlst_new(const char *name);
t_vlst *	vlst_make(const char *name,t_data_type type,int length, int count);

void *		vlst_get_ref(t_vlst *vlst, const char *ref);
void *		vlst_get_pointer(t_vlst *vlst, int indice);
void 		vlst_set_data(t_vlst *vlst, void *data, int indice);

t_vlst *	vlst_rebind(struct Scene *sc,void *ptr);
void 		vlst_normal_3f(t_vlst *dst,t_vlst *vertex,t_vlst *face);
void 		vlst_normal_4f(t_vlst *dst,t_vlst *vertex,t_vlst *face);
void		vlst_normal_4f_direct(t_vlst *dst,t_vlst *vertex,t_vlst *face);
void 		vlst_fill_3f(t_vlst *vlst,float x,float y,float z);
void 		vlst_fill_4f(t_vlst *vlst,float x,float y,float z,float w);
void 		vlst_color_3f(t_vlst *vlst,int *color);
void 		vlst_color_4f(t_vlst *vlst,int *color);
void 		vlst_show(t_vlst *vlst);
void 		vlst_vertex(const char *type,t_vlst *dst,t_vlst *vertex,t_vlst *face);
void 		_do_face(void *src,void *dst,int count,int size);
void 		vlst_build(t_vlst *vlst,void *ptr);
void 		vlst_add_data(t_vlst *vlst,void *ptr);
void 		vlst_copy(t_vlst *dst,t_vlst *src);
void 		vlst_normal_4f(t_vlst *dst,t_vlst *vertex,t_vlst *face);
void 		vlst_add_vlst(t_vlst *vlst_dst,t_vlst *vlst_src);

void 		vlst_add_number(t_vlst *vlst,t_data_type type,void *data);

t_vlst *	vlst_duplicate(t_vlst *vlst);
void 		vlst_set(t_vlst *vlst,int i);
t_vlst *	vlst_clone(t_vlst *vlst);

void __vlst_update_data( struct Brick *brick, t_vlst *vlst,t_vlst *caller);

#ifdef __cplusplus
}
#endif


#endif
