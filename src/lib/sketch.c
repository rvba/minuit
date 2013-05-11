/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

// LOCAL
t_skt *SKT=NULL;

t_skt *skt_get(void)
{
	return SKT;
}

void skt_color(float r,float g,float b,float a)
{
	glColor4f(r,g,b,a);
}

void skt_tint(const char tint[])
{
	if     (is(tint,"white")) skt_color(1,1,1,0);
	else if(is(tint,"black")) skt_color(0,0,0,0);
	else if(is(tint,"red")) skt_color(1,0,0,1);
}

// LINE

void skt_line(float *v1,float *v2,int width,float *color)
{
	float i = SKT->intensity;
	glColor3f(color[0]*i,color[1]*i,color[2]*i);
	glLineWidth(width);

	glBegin(GL_LINES);
		glVertex3f(v1[0],v1[1],v1[2]);
		glVertex3f(v2[0],v2[1],v2[2]);
	glEnd();
}

void skt_closedline(float *points,int tot,float *color,int width)
{
	float i = SKT->intensity;
	glColor3f(color[0]*i,color[1]*i,color[2]*i);
	glLineWidth(width);

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

void skt_point(float *pos,int width,float *color) 
{
	t_context *C = ctx_get();

	glEnable(GL_POINT_SMOOTH);

	float scale;
	if(C->event->ui.use_point_global_width)
		scale = SKT->scale * SKT->point_size;
	else
		scale = 1;

	glPointSize(width * scale);
	glColor3f(color[0],color[1],color[2]);

	glBegin(GL_POINTS);
		glVertex3f(pos[0],pos[1],pos[2]);
	glEnd();
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

	float *vw = vnew(w,0,0);
	float *vh = vnew(0,h,0);

	vcp(a,p);
	vadd(b,a,vw);
	vadd(c,b,vh);
	vadd(d,a,vh);

	skt_line_quad(a,b,c,d,line_width,color);
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

	float *vw = vnew(w,0,0);
	float *vh = vnew(0,h,0);

	vcp(a,p);
	vadd(b,a,vw);
	vadd(c,b,vh);
	vadd(d,a,vh);

	free(vw);
	free(vh);

	glColor3f(color[0],color[1],color[2]);

	glBegin(GL_POLYGON);
		glVertex3f(a[0],a[1],a[2]);
		glVertex3f(b[0],b[1],b[2]);
		glVertex3f(c[0],c[1],c[2]);
		glVertex3f(d[0],d[1],d[2]);
	glEnd();
}
	

t_skt *skt_new(void)
{
	t_skt *skt = (t_skt *)malloc(sizeof(t_skt));

	SKT=skt;

	skt->point_size=SKT_POINT_SIZE;
	skt->line_width=SKT_LINE_WIDTH_FRONT;
	skt->with_scale=SKT_WITH_SCALE;
	skt->scale=SKT_SCALE;
	//skt->intensity=SKT_INTENSITY;
	skt->intensity=1.0;
	vset(skt->color,SKT_COLOR_FRONT,SKT_COLOR_FRONT,SKT_COLOR_FRONT);

	return skt;
}
