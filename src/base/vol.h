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

#ifdef __cplusplus
extern "C" {
#endif

extern int mesh_cube_lines[24];
extern int mesh_cube_tot_lines;
extern float cube_verts[8*3];
extern int cube_quads[6*4];
extern unsigned int cube_edges[2*12];
extern float plane_verts[4*3];
extern int plane_quads[4];
extern float vertices[72]; 
extern unsigned int faces[24];
extern float quad_vert[12]; 
extern int uvlst[48];
extern unsigned quad_face[4];
extern float plane_v[12];
extern unsigned int plane_f[4];
extern int plane_uv[8];

#ifdef __cplusplus
}
#endif

#endif
