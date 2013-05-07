/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef _CAMERA_H
#define _CAMERA_H

#include "minuit.h"

#define CAM_SPEED 50
#define CAM_ORTHO_NEAR -1
#define CAM_ORTHO_FAR 1
#define CAM_EYE 10
#define CAM_FAR 8000
#define CAM_NEAR .1
#define CAM_FOVY 60 //60

#define CAM_ORTHO_ZOOM 1000
#define CAM_ORTHO_ZOOM_FAC 10
#define CAM_IS_MOVING 0
#define CAM_REC_MODE 1 //0 all 1 only when moving

typedef struct Camera t_camera;
typedef struct Camera_Class t_camera_cls;

enum VIEW
{
	camera_top,
	camera_front,
	camera_left,
	camera_right,
	camera_axo
};

enum CAMERA_TYPE
{
	camera_ortho,
	camera_frustum
};

struct Camera_Class
{
	char cls_type[_NAME_];
	void (* init)(t_camera *camera);
};

struct Camera
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];
	t_camera_cls *cls;

	int type; 
	int frame;
	int restrict_matrix;

	// Perspective
	float eye[3];
	float target[3];
	float up[3];
	float pos[3];
	float cross[3];
	float zenith;

	// Ortho
	int ortho_view;
	float ortho_zoom;
	float new_ortho_zoom;
	float angle;
	int ortho_near;
	int ortho_far;
	float ortho_location[3];
	float ortho_rotation[3];

	double o_left;
	double o_right;
	double o_bottom;
	double o_top;

	double ui_left;
	double ui_right;
	double ui_bottom;
	double ui_top;
	float ui_zoom;

	float speed;

	//glFrustum
	double f_left;
	double f_right;
	double f_bottom;
	double f_top;

	double f_aspect;
	double f_fovy;
	double f_near;
	double f_far;

	//sate
	int is_moving;
	int rec_mode;
};

void *camera_get_ref(t_camera *camera, const char *ref);

void camera_show(t_camera *camera);
t_camera *	camera_clone(t_camera *source);
void 		camera_copy(t_camera *target,t_camera *source);
void 		camera_free(t_camera *camera);
t_node *camera_add(const char *name);
t_camera *	camera_new(const char *name);
t_node *	camera_make(const char *name);

#endif
