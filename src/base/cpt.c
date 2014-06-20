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
#include "memory.h"

#define _PI 3.14159655f


int cpt_map_fi( float x, float ostart, float oend, int tstart, int tend)
{
		return
		(
			(int)
			(
				(
					( x - ostart) / ( oend - ostart)
					*
					(float) ( tend - tstart)
				)
				+
				(float) tstart
			)
		);

}

float calc_normalize(int n,int range)
{
	return (float)((float)n/(float)range);

}

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

float *calc_circle(float center[3],double radius,int divisions)
{
	float *points = (float *)mem_malloc(sizeof(float)*divisions*3);
	int i;
	int j=0;

	double delta = (double)((double)(_PI * 2) / divisions);

	for (i=0;i<divisions;i++)
	{
		float r[3];
		float result[3];
		vset(r,cos(i*delta),sin(i*delta),0);
		vmul(r,(float)radius);
		vadd(result,center,r);
		points[j]=result[0];
		points[j+1]=result[1];
		points[j+2]=result[2];
		j+=3;
	}

	return points;
}

float *calc_arc(float center[3],double radius,int divisions, int start, int end)
{
	float *points = (float *)mem_malloc(sizeof(float)*(divisions*3));
	int i;
	int j=0;

	int angle = end - start;
	float phi = deg_to_rad(angle);

	double delta = (double)((double)(phi ) / (divisions -1 ));
	double a = 0;

	for (i=0;i<divisions;i++)
	{
		float r[3];
		float result[3];
		vset(r,cos(a),sin(a),0);
		a += delta;

		vmul(r,(float)radius);
		vadd(result,center,r);
		points[j]=result[0];
		points[j+1]=result[1];
		points[j+2]=result[2];
		j+=3;
	}

	return points;
}

