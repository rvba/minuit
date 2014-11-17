#include "scene.h"
#include "node.h"
#include "base.h"
#include "array.h"
#include "ctx.h"
#include "vlst.h"

void *array_new( const char *name)
{
	t_array *array = ( t_array *) mem_malloc( sizeof( t_array));
	id_init( &array->id, name);
	array->elem_size = 0;
	array->elem_count = 0;
	array->data = NULL;
	array->width = 0;
	array->height = 0;
	array->type = 0;
	return array;
}

void array_rebind( struct Scene *scene, void *ptr)
{
	t_array *array = ( t_array *) ptr;
	rebind( scene, "array", "data", (void **) &array->data);
}

void array_init( struct Node *node)
{
}

void *array_get_ref( struct Node *node, const char *ref)
{
	return NULL;
}

void array_show( t_array *array)
{
	printf("ARRAY:%s\n", array->id.name);
	printf("width:%d\n", array->width);
	printf("height:%d\n", array->height);
}

// Free and Allocate New Memory
void *array_malloc( t_array *array, int x, int y)
{
	t_scene *scene = scene_get();

	int size = array->elem_size * x * y;

	// Free Old Data
	if( array->data)
	{
		// get data,var
		t_node *node_data = scene_get_data( scene, array->data);
		t_node *node_var = scene_get_var( scene, array->data);

		// remove data,var
		scene_node_free( scene, node_data);
		scene_node_free( scene, node_var);
	}

	// Malloc New Data
	void *data = mem_malloc( size);
	bzero( data, size);

	// Store It
	scene_store(scene,1);
	scene_add_data_var( scene, "array_data", "a_data", size, data);
	scene_store(scene,0);

	return data;
}

// Incresase Array Space
void array_alloc( t_array *array, int x, int y)
{
	// Free and Malloc new space
	void *data = array_malloc( array, x, y);

	// Copy Old Data (if exists)
	if( array->data)
	{
		if( data)
		{
			int i;
			unsigned char *old_data = ( unsigned char *) array->data;
			unsigned char *new_data = ( unsigned char *) data;

			int old_size;
			int new_size;
			int copy_size;
			int max_lines;

			int elem_size = array->elem_size;

			old_size = array->width * elem_size;
			new_size = x * elem_size;

			if( y <= array->height) max_lines = y;
			else max_lines = array->height;

			if( x < array->width) copy_size = new_size;
			else copy_size = old_size;

			for( i = 0; i < max_lines; i++)
			{
				memcpy( new_data, old_data, copy_size); 

				old_data += old_size;
				new_data += new_size;
			}

			mem_free( array->data, array->elem_size * array->width * array->height);
		}
	}

	array->data = data;
	array->width = x;
	array->height = y;
}

void array_size( t_array *array, int x, int y)
{
	if( array->elem_size == 0)
	{
		printf("[ARRAY] elem_size is null\n");
		return;
	}
	
	if( array->width != x || array->height != y)
	{
		array_alloc( array, x, y);
		array->elem_count = x * y;
	}
}

inline void *array_pointer_get( void *_ptr, int size, int width, int x, int y)
{
	unsigned char *ptr = ( unsigned char *) _ptr;
	return ( ptr + ( width * size * y) + ( size * x));
}

inline void *array_elem_get( t_array *array, int x, int y)
{
	unsigned char *ptr = ( unsigned char *) array->data;
	return ( ptr + (( array->width * array->elem_size * y) + ( array->elem_size * x)));
}

inline void *array_elem_get_by_indice( t_array *array, int i)
{
	unsigned char *ptr = ( unsigned char *) array->data;
	return ( ptr +  ( array->elem_size * i));
}

int array_elem_count( t_array *array)
{
	return( array->width * array->height);
}

void *array_elem_loop( t_array *array, int i)
{
	unsigned char *data = ( unsigned char *) array->data;
	return data + ( i * array->elem_size);
}

int array_check_inside( t_array *array, int x, int y)
{
	if( x >= 0 && y >=0 && x <= array->width && y <= array->height)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void *array_get_point( t_array *array, int x, int y)
{
	if( array_check_inside( array, x, y))
	{
		unsigned char *ptr = ( unsigned char *) array->data;
		return ( ptr + (( array->width * array->elem_size * y) + ( array->elem_size * x)));
	}
	else
	{
		return NULL;
	}
}

int array_get_line_length( t_array *array, int sx, int sy, int ex, int ey)
{
	// Vertical
	if( sx == ex)
	{
		return (ey - sy);
	}
	// Horizontal
	else if( sy == ey)
	{
		return (ex - sx);
	}
	else
	// Diagonal
	{
		printf("[ARRAY] Warning: Diagonal not implemented\n");
		return 0;
	}
}

int array_get_region_weight( t_array *array, int sx, int sy, int ex, int ey)
{
	int height = ey - sy;
	int width = ex - sx;
	int area = width * height;
	return area * array->elem_size;
}

int array_get_weight( t_array *array)
{
	return array->width * array->height * array->elem_size;
}

void array_bzero( t_array *array)
{
	if( array->data)
	{
		bzero( array->data, array_get_weight( array));
	}
	else
	{
		printf("[ARRAY] bzero: No Data\n");
	}

}

void array_fill( t_array *array, void *value)
{
	int i;
	for( i = 0; i < array->elem_count; i++)
	{
		void *ptr = array_elem_get_by_indice( array, i);
		memcpy( ptr, value, array->elem_size);
	}
}

void array_copy_values( t_array *array_dst, t_array *array_src)
{
	array_dst->elem_size = array_src->elem_size;
	array_dst->elem_count = array_src->elem_count;
	array_dst->data = NULL;
	array_dst->width = array_src->width;
	array_dst->height = array_src->height;
	array_dst->type = array_src->type;
}

t_array *array_copy_bare( t_array *array_src)
{
	t_array *array_dst = ( t_array *) array_new( array_src->id.name);
	array_copy_values( array_dst, array_src);
	return array_dst;
}

inline void array_copy_row( t_array *array, int sx, int sy, int length, void *container)
{
	void *ptr = array_get_point( array, sx, sy);
	memcpy( container, ptr, length * array->elem_size);
}

void array_get_row( t_array *array, int sx, int sy, int ex, int ey, void *_container)
{
	// Check Start < End
	if( ( sx >= ex ))
	{
		printf("[ARRAY] get_row Error: Start point is less than end point: sx:%d ex:%d\n", sx, ex);
		return;
	}

	// Check points are in bound
	if( array_check_inside( array, sx, sy) && array_check_inside( array, ex, ey))
	{
		void *container = _container;
		int length = array_get_line_length( array, sx, sy, ex, ey);

		// Check Length
		if( length > 0)
		{
			// Check for container
			if( !container)
			{
				container = mem_malloc( length * array->elem_size);
			}

			// Copy Data
			array_copy_row( array, sx, sy, length, container); 
		}
		else
		{
			printf("[ARRAY] Error: Length is zero\n");
		}
	}
	else
	{
		printf("[ARRAY] get_row Error: Points are out of bound\n");
	}
}

void array_get_region( t_array *array, int sx, int sy, int ex, int ey, void *_container)
{
	// Check Inside
	if( !array_check_inside( array, sx, sy) && !array_check_inside( array, ex, ey))
	{
		printf("[ARRAY] Error: Points are out of bound\n");
		return;
	}

	// Check E > S
	if( ( sx >= ex ))
	{
		printf("[ARRAY] get_row Error: Start point is less than end point: sx:%d ex:%d\n", sx, ex);
		return;
	}

	if( ( sy >= ey ))
	{
		printf("[ARRAY] get_row Error: Start point is less than end point: sx:%d ex:%d\n", sx, ex);
		return;
	}

	// Check Area
	int height = ey - sy;
	int width = ex - sx;
	int area = width * height;
	if( area > 0)
	{
		// Check container
		void *container = _container;
		int area = width * height;
		if( !container)
		{
			container = mem_malloc( area * array->elem_size);
		}

		int i;
		int x = sx;
		int y = sy;
		int y_point = 0;
		for( i = 0; i < height; i++)
		{
			// Get Next Line Start Point
			void *pointer = array_pointer_get( container, array->elem_size, width, 0, y_point);

			// Get Row
			array_get_row( array, x, y, x + width, y, pointer); 
			y_point++;
			y++;
		}
	}
	else
	{
		printf("[ARRAY] Error: Area is zero\n");
		return;
	}
}

void array_paste_row( t_array *array_dst, t_array *array_src, int px, int py, int sx, int sy, int ex, int ey)
{
	// Check Bounds
	int length = ex - sx;
	int end_line = px + length;
	if( !array_check_inside( array_dst, end_line, 0))
	{
		printf("[ARRAY] Paste Row: Line overflow\n");
		return;
	}

	void *ptr_dst = array_get_point( array_dst, px, py);
	void *ptr_src = array_get_point( array_src, sx, sy);
	int mem_length = length * array_src->elem_size;

	memcpy( ptr_dst, ptr_src, mem_length);
}

void array_paste_array( t_array *array_dst, t_array *array_src, int px, int py, int sx, int sy, int ex, int ey)
{
	int width = ex - sx;
	int height = ey - sy;
	int end_x = px + width;
	int end_y = py + height;

	// Check Source bounds
	if( !array_check_inside( array_src, sx, sy) || !array_check_inside( array_src, ex, ey))
	{
		printf("[ARRAY] Paste: Region out of SRC bounds\n");
		return;
	}
	// Check Dest Bounds
	if( !array_check_inside( array_dst, end_x, end_y))
	{
		printf("[ARRAY] Paste: Region out of DST bounds\n");
		return;
	}

	int i;
	int src_x = sx;
	int src_y = sy;
	int dst_x = px;
	int dst_y = py;
	for( i = 0; i < height; i++)
	{
		array_paste_row( array_dst, array_src, dst_x, dst_y, src_x, src_y, src_x + width, src_y); 
		dst_y++;
		src_y++;
	}
}

void *array_elem_get_near( t_array *array, int x, int y, int dir)
{
	unsigned char *near = NULL;
	unsigned char *ptr = ( unsigned char *) array_elem_get( array, x, y);
	int width = array->width -1 ;
	int height = array->height - 1;
	int size = array->elem_size;
	int width_size = (width + 1) * size;

	switch( dir)
	{
		case( NORTH): if( y < height) near = ptr + width_size; break;
		case( NORTHEAST): if( y < height && x < width) near = ptr + width_size + size; break; 
		case( EAST): if( x < width) near = ptr + size; break;
		case( SOUTHEAST): if( x < width && y > 0) near = ptr - width_size + size; break;
		case( SOUTH): if( y > 0) near = ptr - width_size; break;
		case( SOUTHWEST): if( y > 0 && x > 0) near = ptr - width_size - size; break;
		case( WEST): if( x > 0) near = ptr - size; break;
		case( NORTHWEST): if( x > 0 && y < height) near = ptr + width_size - size; break;
	}

	return near;
}

void array_outline_get( t_array *array, int x, int y, void *( * outline)[8])
{
	*outline[0] = array_elem_get_near( array, x, y, NORTH);
	*outline[1] = array_elem_get_near( array, x, y, NORTHEAST);
	*outline[2] = array_elem_get_near( array, x, y, EAST);
	*outline[3] = array_elem_get_near( array, x, y, SOUTHEAST);
	*outline[4] = array_elem_get_near( array, x, y, SOUTH);
	*outline[5] = array_elem_get_near( array, x, y, SOUTHWEST);
	*outline[6] = array_elem_get_near( array, x, y, WEST);
	*outline[7] = array_elem_get_near( array, x, y, NORTHWEST);
}



