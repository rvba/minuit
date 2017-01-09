#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "base.h"

struct Scene;

typedef struct Array
{
	t_id id;

	t_data_type type;
	int elem_count;
	int elem_size;
	int width;
	int height;
	void *data;

}t_array;


void array_fill( t_array *array, void *value);
t_array *array_copy_bare( t_array *array_src);
void array_paste_array( t_array *array_dst, t_array *array_src, int px, int py, int sx, int sy, int ex, int ey);

void array_bzero( t_array *array);

int array_get_region_weight( t_array *array, int sx, int sy, int ex, int ey);
void array_get_region( t_array *array, int sx, int sy, int ex, int ey, void *_container);

void array_outline_get( t_array *array, int x, int y, void *( * outline)[8]);
int array_elem_count( t_array *array);
void *array_elem_get_by_indice( t_array *array, int i);
void *array_elem_get_near( t_array *array, int x, int y, int dir);
void *array_elem_loop( t_array *array, int i);
void *array_elem_get( t_array *array, int x, int y);
void array_size( t_array *array, int x, int y);



void array_rebind( struct Scene *scene, void *ptr);
void *array_get_ref( struct Node *node, const char *ref);
void array_init( struct Node *node);
void *array_new( const char *name);



#endif
