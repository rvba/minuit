/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __VOLUME_H
#define __VOLUME_H

int mesh_cube_lines[24];
int mesh_cube_tot_lines;
float cube_verts[8*3];
int cube_quads[6*4];
float plane_verts[4*3];
int plane_quads[4];
float vertices[72]; 
unsigned int faces[24];
float quad_vert[12]; 
int uvlst[48];
unsigned quad_face[4];
float plane_v[12];
unsigned int plane_f[4];
int plane_uv[8];


#endif
