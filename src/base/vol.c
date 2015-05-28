/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"

int mesh_cube_lines[24]={0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,1,5,2,6,3,7};
int mesh_cube_tot_lines = 24;

float plane_verts[4*3] = {
	0,0,0,
	1,0,0,
	1,1,0,
	0,1,0,
};

int plane_quads[4] = {
	0,3,2,1,
	};		


float cube_verts[8*3] = {
	0,0,0,
	1,0,0,
	1,1,0,
	0,1,0,
	0,0,1,
	1,0,1,
	1,1,1,
	0,1,1
};

unsigned int cube_edges [] = {

	0,1,
	1,2,
	2,3,
	3,0,
	0,4,
	1,5,
	2,6,
	3,7,
	4,5,
	5,6,
	6,7,
	7,4
};


int cube_quads[6*4] = {
	0,3,2,1,
	0,1,5,4,		
	1,2,6,5,
	2,3,7,6,
	3,0,4,7,
	4,5,6,7
	};		


float plane_v[12]={
		0,0,0,
		0,1,0,
		1,1,0,
		1,0,0,
};

unsigned int plane_f[4] = { 0,1,2,3 };
int plane_uv[8]={
	0,0,
	0,1,
	1,1,
	1,0
};

float vertices[72] = {
		0,0,0,
		0,1,0,
		1,1,0,
		1,0,0,
		
		0,0,0,
		1,0,0,
		1,0,1,
		0,0,1,

		1,0,0,
		1,1,0,
		1,1,1,
		1,0,1,

		1,1,0,
		0,1,0,
		0,1,1,
		1,1,1,

		0,1,0,
		0,0,0,
		0,0,1,
		0,1,1,

		0,0,1,
		1,0,1,
		1,1,1,
		0,1,1,

	};
		
unsigned int faces[24] = {
			0,1,2,3,
			4,5,6,7,
			8,9,10,11,
			12,13,14,15,
			16,17,18,19,
			20,21,22,23
		};


float quad_vert[12] = {
		0,0,0,
		0,1,0,
		1,1,0,
		1,0,0,

	};
		
unsigned int quad_face[4] = {
			0,1,2,3,
		};

int uvlst[48] = {
	0,0,
	1,0,
	1,1,
	0,1,

	0,0,
	1,0,
	1,1,
	0,1,

	0,0,
	1,0,
	1,1,
	0,1,

	0,0,
	1,0,
	1,1,
	0,1,

	0,0,
	1,0,
	1,1,
	0,1,

	0,0,
	1,0,
	1,1,
	0,1,
};

