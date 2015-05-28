#include "opengl.h"
#include "base.h"
#include "vlst.h"
#include "draw.h"
#include "sketch.h"
#include "mesh.h"

void draw_mesh_edge( t_draw *draw, struct Mesh *mesh)
{
	if( draw->with_edge)
	{
		glVertexPointer(3,GL_FLOAT,0,(float *)mesh->vertex->data);
		glDrawElements( GL_LINES, mesh->edges->count * 2, GL_UNSIGNED_INT, mesh->edges->data); 
	}
}
