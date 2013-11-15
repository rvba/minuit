/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "common.h"
#include "util.h"


/*
void calc_normals(float *v1,float *v2,float *v3,float *result)
{

	double v1x,v1y,v1z,v2x,v2y,v2z;
	double nx,ny,nz;
	double vLen;
	float x,y,z;

	// Calculate vectors
	v1x = v1[0] - v2[0];
	v1y = v1[1] - v2[1];
	v1z = v1[2] - v2[2];

	v2x = v2[0] - v3[0];
	v2y = v2[1] - v3[1];
	v2z = v2[2] - v3[2];

	// Get cross product of vectors
	nx = (v1y * v2z) - (v1z * v2y);
	ny = (v1z * v2x) - (v1x * v2z);
	nz = (v1x * v2y) - (v1y * v2x);

	// Normalize final vector
	vLen = sqrt( (nx * nx) + (ny * ny) + (nz * nz) );

	x = (float)(nx / vLen);
	y = (float)(ny / vLen);
	z = (float)(nz / vLen);

	result[0] = x;
	result[1] = y;
	result[2] = z;
}
*/

inline float deg_to_rad(int deg)
{
	return ((PI * deg) / 180.0f);
}

void vprint3i(int *v,char end)
{
	printf("[v (%d)(%d)(%d)]%c",v[0],v[1],v[2],end);
}

void vprint3f(float *v,char end)
{
	printf("[v (%f)(%f)(%f)]%c",v[0],v[1],v[2],end);
}

float *vnew(float x,float y,float z)
{
	float *v = (float *)malloc(sizeof(float)*3);
	v[0]=x;
	v[1]=y;
	v[2]=z;

	return v;
}

inline void vprint(float v[3])
{
	printf("v:%f %f %f\n",v[0],v[1],v[2]);
}

inline void vcp(float t[3],float s[3])
{
	t[0]=s[0];
	t[1]=s[1];
	t[2]=s[2];
}

inline void vcp3f(float t[3],float s[3])
{
	t[0]=s[0];
	t[1]=s[1];
	t[2]=s[2];
}

inline void vcpi(int t[3],int s[3])
{
	t[0]=s[0];
	t[1]=s[1];
	t[2]=s[2];
}

inline void vcp3i(int t[3],int s[3])
{
	t[0]=s[0];
	t[1]=s[1];
	t[2]=s[2];
}

inline void vcp4i(int t[4],int s[4])
{
	t[0]=s[0];
	t[1]=s[1];
	t[2]=s[2];
	t[3]=s[3];
}

inline void vset(float v[3],float a,float b,float c)
{
	v[0]=a;
	v[1]=b;
	v[2]=c;
}

inline void vset4f(float *v,float a,float b,float c,float d)
{
	v[0]=a;
	v[1]=b;
	v[2]=c;
	v[3]=d;
}

inline void vset3f(float *v,float a,float b,float c)
{
	v[0]=a;
	v[1]=b;
	v[2]=c;
}

inline void vset4i(int *v,int a,int b,int c,int d)
{
	v[0]=a;
	v[1]=b;
	v[2]=c;
	v[3]=d;
}
/*
inline void vset_i(int v[3],int a,int b,int c)
{
	v[0]=a;
	v[1]=b;
	v[2]=c;
}
*/

inline void vseti(int v[3],int a,int b,int c)
{
	v[0]=a;
	v[1]=b;
	v[2]=c;
}

inline void vset3i(int v[3],int a,int b,int c)
{
	v[0]=a;
	v[1]=b;
	v[2]=c;
}

inline void vadd(float r[3],float a[3],float b[3])
{
	r[0]=a[0]+b[0];
	r[1]=a[1]+b[1];
	r[2]=a[2]+b[2];
}

inline void vsub(float r[3],float a[3],float b[3])
{
	r[0]=a[0]-b[0];
	r[1]=a[1]-b[1];
	r[2]=a[2]-b[2];
}

inline void vmul(float v[3],float i)
{
	v[0]*=i;
	v[1]*=i;
	v[2]*=i;
}

inline void vcross(float r[3],float a[3],float b[3])
{
	r[0] = (a[1] * b[2]) - (a[2] * b[1]);
	r[1] = (a[2] * b[0]) - (a[0] * b[2]);
	r[2] = (a[0] * b[1]) - (a[1] * b[0]);
} 

inline float vdot(float a[3],float b[3])
{
	return (float)(a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

inline float vmag(float v[3])
{
	return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

inline void vnormalize(float v[3])
{
	vmul(v,1/vmag(v));
}

inline float vlength3f(float v0[3], float v1[3])
{
	float v[3];
	vsub(v,v1,v0);
	return sqrt(SQ(v[0])+SQ(v[1])+SQ(v[2]));
}

inline float vnorm3f(float v[3])
{
	return sqrt(SQ(v[0])+SQ(v[1])+SQ(v[2]));
}

inline void cnorm(float c[3],float o[3])
{
	c[0]=o[0]/255;
	c[1]=o[1]/255;
	c[2]=o[2]/255;
}

inline void cconv(float c[3],int o[3])
{
	c[0]=((float)o[0])/255.0f;
	c[1]=((float)o[1])/255.0f;
	c[2]=((float)o[2])/255.0f;
}

inline void vplus4i(int *v,int i)
{
	v[0] += i;
	v[1] += i;
	v[2] += i;
	v[3] += i;
}

inline void vplus4f(float *v,float i)
{
	v[0] += i;
	v[1] += i;
	v[2] += i;
	v[3] += i;
}

void vrot2d3f(float *v, int angle)
{
	float x0 = v[0];
	float y0 = v[1];

	float r = deg_to_rad(angle);

	float c = cos(r);
	float s = sin(r);

	float x1 = (x0 * c) - (y0 * s);
	float y1 = (x0 * s) + (y0 * c);

	v[0] = x1;
	v[1] = y1;
}

void vcenter3f( float *r, float *a, float *b)
{
	float h[3];
	vsub( r, b, a);
	vcp( h, r);
	vmul( h, .5);
	vadd( r, a, h);
}

void vinverse3f( float *v)
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}


/*
void m3_print(float m[3][3])
{
	printf("print m3\n");
	printf("[%f %f %f]\n",m[0][0],m[0][1],m[0][2]);
	printf("[%f %f %f]\n",m[1][0],m[1][1],m[1][2]);
	printf("[%f %f %f]\n",m[2][0],m[2][1],m[2][2]);

}
void  mmul_m3_scalar(float m[3][3], float num)
{
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			m[i][j] *= num;
		}
	}
}
void mmul_m3_m3m3(float m1[3][3], float m2[3][3], float m3[3][3])
{

	m1[0][0] = m2[0][0]*m3[0][0] + m2[0][1]*m3[1][0] + m2[0][2]*m3[2][0]; 
	m1[0][1] = m2[0][0]*m3[0][1] + m2[0][1]*m3[1][1] + m2[0][2]*m3[2][1];
	m1[0][2] = m2[0][0]*m3[0][2] + m2[0][1]*m3[1][2] + m2[0][2]*m3[2][2];

	m1[1][0] = m2[1][0]*m3[0][0] + m2[1][1]*m3[1][0] + m2[1][2]*m3[2][0];
	m1[1][1] = m2[1][0]*m3[0][1] + m2[1][1]*m3[1][1] + m2[1][2]*m3[2][1];
	m1[1][2] = m2[1][0]*m3[0][2] + m2[1][1]*m3[1][2] + m2[1][2]*m3[2][2];

	m1[2][0] = m2[2][0]*m3[0][0] + m2[2][1]*m3[1][0] + m2[2][2]*m3[2][0];
	m1[2][1] = m2[2][0]*m3[0][1] + m2[2][1]*m3[1][1] + m2[2][2]*m3[2][1];
	m1[2][2] = m2[2][0]*m3[0][2] + m2[2][1]*m3[1][2] + m2[2][2]*m3[2][2];

}

void madd_m3_m3(float m1[3][3], float m2[3][3])
{

	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			m1[i][j] += m2[i][j] ;
		}
	}

}

void mmul_m3_v3(Vec *vec,float m[3][3])
{
	float x = vec->x;
	float y = vec->y;
	float z = vec->z;

	vec->x = m[0][0] * x + m[0][1] * y + m[0][2] * z;
	vec->y = m[1][0] * x + m[1][1] * y + m[1][2] * z;
	vec->z = m[2][0] * x + m[2][1] * y + m[2][2] * z;
}  

void mrot_m3_v3(Vec *vec,Vec axis,float alpha)
{

	float x = vec->x;
	float y = vec->y;
	float z = vec->z;

	float ax = axis.x;
	float ay = axis.y;
	float az = axis.z;

	float c = cos(alpha);
	float s = sin(alpha);

	vec->x = 
		( c +( (1 - c) * ( ax * ax) ) )    * x +
		( (1 - c) * (ax * ay) - (s * ay) ) * y +
		( (1 - c) * (ax * az) + (s * ay) ) * z;
	vec->y =
		( (1 - c) * (ax * ay) + (s * az) ) * x +
		( c + ((1 -c) * (ay * ay)) )       * y +
		( ((1 - c)*(ay * az)) - ( s * az) )* z;

	vec->z =
		( ((1 - c) * (ax * az)) - (s * az)) * x +
		( ((1 - c) * (ay * az)) + (s * ax)) * y +
		( c + ( (1 -c) * (az * az)) ) * z;
}


*/

