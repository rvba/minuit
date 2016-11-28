#include "base.h"
#include "opengl.h"
#include "mesh.h"
#include "txt.h"
#include "file.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define LIBERATION "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf" 

static int TTF_INIT = 0;
static int TTF_READY = 0;

FT_Library    TTF_library;
FT_Face       TTF_face;
FT_GlyphSlot  TTF_slot;

typedef struct MGlyph_Mesh
{
	GLenum type;
	double *points;
	unsigned int count;

}t_glyph_mesh;

typedef struct MGlyph
{
	GLenum error;
	t_glyph_mesh **meshes;
	unsigned int mesh_count;
	double **verts;
	unsigned int vert_count;
	unsigned int list;
	float width;
	float height;

}t_glyph;

/* GLYPHS */

t_glyph *GLYPH[512] = {NULL};


/* NEW */

t_glyph_mesh *glyph_mesh_new( GLenum type)
{
	t_glyph_mesh *mesh = (t_glyph_mesh *) malloc( sizeof( t_glyph_mesh));
	mesh->points = NULL;
	mesh->count = 0;
	mesh->type = type;
	return mesh;
}

t_glyph *glyph_new( void)
{
	t_glyph *glyph = malloc(sizeof(t_glyph));

	glyph->meshes = NULL;
	glyph->mesh_count = 0;
	glyph->verts = NULL;
	glyph->vert_count = 0;
	glyph->width = 0;
	glyph->height = 0;

	return glyph;
}

/* ADD */

void glyph_mesh_add( GLenum type, t_glyph *glyph)
{
	glyph->mesh_count++;
	t_glyph_mesh *mesh = glyph_mesh_new(type);
	if( glyph->meshes) glyph->meshes = realloc( glyph->meshes, glyph->mesh_count * sizeof( t_glyph_mesh *));
	else glyph->meshes = malloc( glyph->mesh_count * sizeof( t_glyph_mesh *));
	glyph->meshes[glyph->mesh_count - 1 ] = mesh;

}

void glyph_mesh_add_point( t_glyph_mesh *mesh, double x, double y, double z)
{
	mesh->count++;
	if(mesh->points != NULL)
	{
		mesh->points = realloc( mesh->points, sizeof(double) * mesh->count * 3);
	}
	else
	{
		mesh->points = malloc( sizeof(double) * mesh->count * 3);
	}
	
	mesh->points[(mesh->count -1)*3 + 0 ] = x;
	mesh->points[(mesh->count -1)*3 + 1 ] = y;
	mesh->points[(mesh->count -1)*3 + 2 ] = z;
}

void glyph_vert_add( t_glyph *glyph, double *v)
{
	glyph->vert_count++;
	glyph->verts = realloc( glyph->verts, sizeof(double *) * glyph->vert_count);
	glyph->verts[glyph->vert_count-1] = v;
}

void glyph_vert_clear( t_glyph *glyph)
{
	int i;
	for( i = 0; i < glyph->vert_count; i++)
	{
		free(glyph->verts[i]);
	}

	free(glyph->verts);

}

/* RENDER */

void txt_ttf_render_outline(const FT_GlyphSlot glyph_slot)
{
	unsigned int start=0;
	glLineWidth(1);
	int c;
	for(c=0; c<glyph_slot->outline.n_contours; c++)
	{
		glBegin(GL_LINE_LOOP);
		unsigned int end = glyph_slot->outline.contours[c]+1;
		unsigned int p;
		for(p = start; p<end; p++)
		{
			glVertex3f(glyph_slot->outline.points[p].x, glyph_slot->outline.points[p].y, 0);
		}
		glEnd();
		start=end;
	}
}

void txt_ttf_render_geometry(t_glyph *glyph)
{
	int i,j;
	for ( i = 0; i < glyph->mesh_count ;i++)
	{
		t_glyph_mesh *mesh = glyph->meshes[i];
		glBegin(mesh->type);
		for ( j = 0; j < mesh->count * 3; j+=3)
		{
			glVertex3f(mesh->points[j+0],mesh->points[j+1], mesh->points[j+2]);
		}
		glEnd();
	}
}

/* CALLBACKS */

void cb_error( GLenum errCode, t_glyph* glyph)
{
    glyph->error = errCode;
}

void cb_vertex( void* data, t_glyph* glyph)
{
	double *ptr = (double*)data;
	t_glyph_mesh *mesh = glyph->meshes[glyph->mesh_count-1];
	glyph_mesh_add_point( mesh, ptr[0], ptr[1], ptr[2]);
}

void cb_combine( double coords[3], void* vertex_data[4], float weight[4], void** outData, t_glyph* glyph)
{
	double *out = mem_malloc(sizeof(double) * 3);
	out[0]=coords[0];
	out[1]=coords[1];
	out[2]=coords[2];
	glyph_vert_add( glyph, out);
	*outData=out;
}

void cb_begin( GLenum type, t_glyph* glyph)
{
	glyph_mesh_add( type, glyph);
}

void cb_end(t_glyph* glyph)
{
}

/* BUILD */

t_glyph *txt_ttf_build_geometry( const FT_GlyphSlot glyph_slot)
{
	t_glyph* glyph = glyph_new();

	double *points = (double *) malloc(sizeof(double) * 3 * glyph_slot->outline.n_points);

	GLUtesselator* t = gluNewTess();

	gluTessCallback(t, GLU_TESS_BEGIN_DATA, (GLvoid (*)()) cb_begin);
	gluTessCallback(t, GLU_TESS_VERTEX_DATA, (GLvoid (*)()) cb_vertex);
	gluTessCallback(t, GLU_TESS_COMBINE_DATA, (GLvoid (*)()) cb_combine);
	gluTessCallback(t, GLU_TESS_END_DATA, (GLvoid (*)()) cb_end);
	gluTessCallback(t, GLU_TESS_ERROR_DATA, (GLvoid (*)()) cb_error);

	gluTessProperty(t, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);

	gluTessProperty(t, GLU_TESS_TOLERANCE, 0);
	gluTessNormal(t, 0.0f, 0.0f, 1.0f);
	gluTessBeginPolygon(t, glyph);

	unsigned int start=0;
	int i = 0;
	int c;
	for(c=0; c<glyph_slot->outline.n_contours; c++)
	{
		unsigned int end = glyph_slot->outline.contours[c]+1;
		unsigned int p;
		for(p = start; p<end; p++)
		{
			points[i+0] = glyph_slot->outline.points[p].x;
			points[i+1] = glyph_slot->outline.points[p].y;
			points[i+2] = 0;
			i += 3;
		}
		start=end;
	}

	start=0;
	for(c=0; c<glyph_slot->outline.n_contours; c++)
	{
		unsigned int end = glyph_slot->outline.contours[c]+1;
		gluTessBeginContour(t);
		unsigned int p;
		for(p = start; p<end; p++)
		{
			gluTessVertex(t, points, points);
			points+=3;
		}
		start=end;
		gluTessEndContour(t);
	}

	gluTessEndPolygon(t);
	gluDeleteTess(t);

	glyph_vert_clear( glyph);
	
	return glyph;
}

/* INIT */

t_glyph *txt_ttf_build_glyph( const char l)
{
	FT_Error error;

	/* Load char */
	error = FT_Load_Char(TTF_face, l, FT_LOAD_DEFAULT);
	if(error)
	{
		printf("[FreeType Error] Could not load char (error=%d)\n", error);
		return NULL;
	}

	/* Build geometry */
	t_glyph *glyph = txt_ttf_build_geometry(TTF_slot);

	/* width, height */
	glyph->width = TTF_slot->metrics.horiAdvance;
	glyph->height = TTF_slot->metrics.vertAdvance;

	/* Store it */
	GLYPH[(unsigned int) l] = glyph;

	/* Build lists */
	int glList = glGenLists(2);
	glyph->list = glList;

	glNewList(glList+1, GL_COMPILE);
	txt_ttf_render_outline(TTF_slot);
	glEndList();

	glNewList(glList, GL_COMPILE);
	txt_ttf_render_geometry(glyph);
	glTranslatef(TTF_slot->metrics.horiAdvance,0,0);
	glEndList();

	glColor3f(1,1,1);
	glCallList(glList+1);
	glColor3f(1,1,1);
	glCallList(glList);

	return glyph;
}

/* GET */

t_glyph *txt_ttf_glyph_get( char c)
{
	/* Find char*/
	t_glyph *g = GLYPH[(unsigned int) c];

	/* Build char */
	if(!g) g = txt_ttf_build_glyph( c);

	return g;
}

float txt_ttf_glyph_get_width( char letter)
{
	t_glyph *g = txt_ttf_glyph_get( letter);
	return g->width;
}

float txt_ttf_glyph_get_height( char letter)
{
	t_glyph *g = txt_ttf_glyph_get( letter);
	return g->height;
}

/* DRAW */
void txt_ttf_vertical_offset( float factor)
{
	glTranslatef(0, TTF_slot->metrics.vertAdvance * factor,0);
}

void txt_ttf_draw_char( char l)
{
	t_glyph *g = txt_ttf_glyph_get( l);

	if(g)
	{
		glCallList(g->list);
		glTranslatef(g->width,0,0);
	}	
}

void txt_ttf_load(char *ttffilename)
{
	FT_Error error;
	error = FT_Init_FreeType(&TTF_library);
	error = FT_New_Face(TTF_library, ttffilename, 0, &TTF_face);

	if (error)
	{
  	        printf("[FreeType] Could not load face\n");
		TTF_READY = 0;
		return;
	}

	error = FT_Set_Char_Size(TTF_face, 100 * 64, 0, 10, 0);
	TTF_slot = TTF_face->glyph;
}
	

int txt_ttf_init( void)
{
	if(!TTF_INIT)
	{
		TTF_INIT = 1;
		if(file_exists(file_new("font.ttf")))
		{
			txt_ttf_load( "font.ttf");
			TTF_READY = 1;
		}
		else if( file_exists( file_new( LIBERATION))) 
	       	{
			txt_ttf_load( LIBERATION);
			TTF_READY = 1;
		}
		else
		{
			printf("[FreeType] Couldn't find ttf file %s\n","font.ttf");

		}
	}

	return TTF_READY;
}

void txt_ttf_draw( char *str)
{
	/* Init */
	txt_ttf_init();

	if(TTF_READY)
	{
		glColor3f(1,1,1);
		char *l;
		for( l = str; *l; l++)
		{
			txt_ttf_draw_char(*l);
		}
	}
}


