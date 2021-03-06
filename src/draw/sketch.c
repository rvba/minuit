/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */


#include "opengl.h"
#include "ctx.h"
#include "node.h"
#include "scene.h"

#include "base.h"
#include "op.h"
#include "event.h"
#include "sketch.h"
#include "ui.h"

#include "camera.h"
#include "memory.h"

#include "dict.h"
#include "vec.h"

// LOCAL
t_skt *SKT=NULL;

t_skt *skt_get(void)
{
	return SKT;
}

void skt_set( t_skt *skt)
{
	// free ...
	SKT = skt;
}

void skt_cp( t_skt *dst, t_skt *src)
{
	dst->line_width = src->line_width;
	dst->point_width = src->point_width;
	dst->point_size = src->point_size;
}

void skt_load( t_context *C)
{
	t_node *node = scene_get_node_by_type_name( C->scene, dt_dict, "dict_skt");
	t_dict *dict = node->data;
	t_symbol *symbol = dict_pop( dict, "symbol_skt");
	t_skt *skt = (t_skt *) symbol->data;

	skt_cp( SKT, skt);

	SKT->line_width = skt->line_width;
}

void skt_save( t_context *C)
{
	t_node *node = scene_node_exists( C->scene, dt_dict, "dict_skt");
	if( node)
	{
		t_dict *dict = ( t_dict *) node->data;
		t_symbol *symbol = dict_pop( dict, "symbol_skt");
		t_skt *skt = ( t_skt *) symbol->data;

		skt_cp( skt, SKT);
	}
	else
	{
		scene_store(C->scene,1);
		t_dict *dict = dict_make("dict_skt");
		dict_symbol_add(dict,"symbol_skt",dt_pointer, SKT);
		scene_add_data_var( C->scene, "int", "var_skt", sizeof( t_skt), SKT);
		scene_store(C->scene,0);
	}
}

int skt_get_line_scale( void)
{
	if(SKT->use_line_global_width) return SKT->line_width;
	else return 1;
}

void skt_color(float r,float g,float b,float a)
{
	glColor4f(r,g,b,a);
}

void skt_tint(const char tint[])
{
	if     (iseq(tint,"white")) skt_color(1,1,1,0);
	else if(iseq(tint,"black")) skt_color(0,0,0,0);
	else if(iseq(tint,"red")) skt_color(1,0,0,1);
}

// LINE

void skt_line( float *v1, float *v2, int width, float *color)
{
	float i = SKT->intensity;
	int scale = skt_get_line_scale();

	glColor3f(color[0]*i,color[1]*i,color[2]*i);
	glLineWidth(width * scale);

	if( SKT->line_smooth) glEnable(GL_LINE_SMOOTH);

	glBegin(GL_LINES);
		glVertex3f(v1[0],v1[1],v1[2]);
		glVertex3f(v2[0],v2[1],v2[2]);
	glEnd();

	if( SKT->line_smooth) glDisable(GL_LINE_SMOOTH);
}

void skt_closedline(float *points,int tot,float *color,int width)
{
	float i = SKT->intensity;
	glColor3f(color[0]*i,color[1]*i,color[2]*i);
	glLineWidth( skt_get_line_scale() * width);

	int p=0;
	glBegin(GL_LINE_LOOP); 

	while(tot)
	{
		glVertex3f(points[p],points[p+1],points[p+2]);
		p+=3;
		tot--;
	}
	glEnd();
}

void skt_closedline_filled(float *points,int tot,float *color,int width)
{
	glColor3f(color[0],color[1],color[2]);
	glLineWidth(width);

	int p=0;
	glBegin(GL_POLYGON); 

	while(tot)
	{
		glVertex3f(points[p],points[p+1],points[p+2]);
		p+=3;
		tot--;
	}
	glEnd();
}

void skt_line_quad(float *a,float *b,float *c,float *d,int line_width,float *color)
{
	glColor3f(color[0],color[1],color[2]);
	glLineWidth(line_width);

	glBegin(GL_LINE_LOOP);
		glVertex3f(a[0],a[1],a[2]);
		glVertex3f(b[0],b[1],b[2]);
		glVertex3f(c[0],c[1],c[2]);
		glVertex3f(d[0],d[1],d[2]);
	glEnd();
}

void _skt_rectangle(float *point, float *v1, float *v2, float *color, int width)
{
	float points[4*3];

	float *a = points;
	float *b = points + 3;
	float *c = points + 6;
	float *d = points + 9;

	vcp(a,point);
	vadd(b,a,v1);
	vadd(c,b,v2);
	vadd(d,a,v2);

	skt_closedline(points,4,color,width);
}

void skt_circle(float *pos)
{
	float resolution = (float) SKT->point_resolution;
	float radius = .2;

	int i;
	float division = 2 * PI / resolution;
	float angle = division;

	glPushMatrix();
		glTranslatef(pos[0],pos[1],pos[2]);

		glColor3f(1,1,1);

		glBegin(GL_TRIANGLE_FAN);

		glVertex3f(radius,0,0);

		for(i=0;i<resolution;i++)
		{
			glVertex3f(cos(angle)*radius,sin(angle)*radius,0);
			angle += division;
		}

		glEnd();
	glPopMatrix();
}

void skt_point_polygon( float *pos, int width, float *color)
{
	int resolution = SKT->point_resolution;
	double radius = (double) width / 2;

	float *points = calc_circle( pos, radius, resolution);

	int i;
	int u = 0;
	float *p = points;

	glColor3f( color[0], color[1], color[2]);
	glBegin(GL_TRIANGLE_FAN);
	for( i = 0; i < resolution; i++)
	{
		if( i == (resolution - 1))
		{
			glVertex3f( pos[0], pos[1], pos[2]);
			glVertex3f( p[u], p[u+1], p[u+2]);
			glVertex3f( p[0], p[1], p[2]);

		}
		else
		{
			glVertex3f( pos[0], pos[1], pos[2]);
			glVertex3f( p[u], p[u+1], p[u+2]);
			glVertex3f( p[u+3], p[u+4], p[u+5]);
		}
		u+=3;
	}
	glEnd();

	free( p);
}

void skt_point(float *pos,int width,float *color) 
{
	if( SKT->point_smooth)
	{
	if(SKT->point_smooth) glEnable(GL_POINT_SMOOTH);
	else glDisable(GL_POINT_SMOOTH);

	float scale;
	if(SKT->use_point_global_width)
		scale = SKT->scale * SKT->point_size;
	else
		scale = 1;

	glPointSize(width * scale);
	glColor3f(color[0],color[1],color[2]);

	glBegin(GL_POINTS);
		glVertex3f(pos[0],pos[1],pos[2]);
	glEnd();
	}
	else
	{
		skt_point_polygon( pos, width, color);
	}
}

// VECTOR

void skt_vector(float *a,float *b,int width,float *color)
{
	skt_point(b,width,color);
	skt_line(a,b,width,color);
}

// SHAPES

void skt_line_rectangle(float *p,float w,float h,int line_width,float *color)
{
	float a[3];
	float b[3];
	float c[3];
	float d[3];

	float vw[] = {w,0,0};
	float vh[] = {0,h,0};

	vcp(a,p);
	vadd(b,a,vw);
	vadd(c,b,vh);
	vadd(d,a,vh);

	skt_line_quad(a,b,c,d,line_width,color);
}


void skt_rectangle(float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float p4_x, float p4_y)
{
	//t_context *C =ctx_get();
	//float *color = C->ui->front_color;

	float color[3]={1,0,0};
	glColor3f(color[0],color[1],color[2]);

	float z=0;

	//glBegin(GL_QUADS);
	glBegin(GL_POLYGON);
		glVertex3f(p1_x,p1_y,z);
		glVertex3f(p2_x,p2_y,z);
		glVertex3f(p3_x,p3_y,z);
		glVertex3f(p4_x,p4_y,z);
	glEnd();
}

void skt_msh_square(float *pos,float width,float *color)
{
	glColor3f(color[0],color[1],color[2]);

	glBegin(GL_POLYGON);
		glVertex3f(pos[0],pos[1],pos[2]);
		glVertex3f(pos[0] + width,pos[1],pos[2]);
		glVertex3f(pos[0]+width,pos[1]+width,pos[2]);
		glVertex3f(pos[0],pos[1]+width,pos[2]);
	glEnd();
}

void skt_msh_rectangle(float *p,float w,float h,float *color)
{
	float a[3];
	float b[3];
	float c[3];
	float d[3];

	float vw[] = {w,0,0};
	float vh[] = {0,h,0};

	vcp(a,p);
	vadd(b,a,vw);
	vadd(c,b,vh);
	vadd(d,a,vh);

	glColor3f(color[0],color[1],color[2]);

	glBegin(GL_POLYGON);
		glVertex3f(a[0],a[1],a[2]);
		glVertex3f(b[0],b[1],b[2]);
		glVertex3f(c[0],c[1],c[2]);
		glVertex3f(d[0],d[1],d[2]);
	glEnd();
}

void skt_msh_rectangle_uv(float *p,float w,float h)
{
	float a[3];
	float b[3];
	float c[3];
	float d[3];

	float vw[] = {w,0,0};
	float vh[] = {0,h,0};

	vcp(a,p);
	vadd(b,a,vw);
	vadd(c,b,vh);
	vadd(d,a,vh);

	glBegin(GL_POLYGON);
		glTexCoord2d( 0, 0);
		glVertex3f(a[0],a[1],a[2]);
		glTexCoord2d( 1, 0);
		glVertex3f(b[0],b[1],b[2]);
		glTexCoord2d( 1, 1);
		glVertex3f(c[0],c[1],c[2]);
		glTexCoord2d( 0, 1);
		glVertex3f(d[0],d[1],d[2]);
	glEnd();
}

void skt_update( t_context *C)
{
	if( C->event->ui.use_point_global_width) SKT->use_point_global_width = 1;
	else SKT->use_point_global_width = 0;

	if( C->event->ui.use_line_global_width) SKT->use_line_global_width = 1;
	else SKT->use_line_global_width = 0;
}
	

t_skt *skt_new(void)
{
	t_skt *skt = (t_skt *)mem_malloc(sizeof(t_skt));

	SKT=skt;

	skt->point_size=SKT_POINT_SIZE;
	skt->line_width=SKT_LINE_WIDTH_FRONT;
	skt->line_smooth = 0;
	skt->with_scale=SKT_WITH_SCALE;
	skt->scale=SKT_SCALE;
	//skt->intensity=SKT_INTENSITY;
	skt->intensity=1.0;
	skt->point_resolution = SKT_POINT_RESOLUTION;
	vset(skt->color,SKT_COLOR_FRONT,SKT_COLOR_FRONT,SKT_COLOR_FRONT);
	skt->point_smooth = SKT_POINT_SMOOTH;
	skt->use_point_global_width = 1;
	skt->use_line_global_width = 1;

	return skt;
}
