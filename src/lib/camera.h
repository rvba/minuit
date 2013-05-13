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

	// Sate

	int is_moving;
	int type; 
	int frame;
	int restrict_matrix;
	float speed;

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
	float angle;
	int ortho_near;
	int ortho_far;
	float ortho_location[3];
	float ortho_rotation[3];

	// Frustum

	double left;
	double right;
	double bottom;
	double top;

	double aspect;
	double fovy;
	double near;
	double far;

};

void *		camera_get_ref(t_camera *camera, const char *ref);
void 		camera_show(t_camera *camera);
t_camera *	camera_clone(t_camera *source);
void 		camera_copy(t_camera *target,t_camera *source);
void 		camera_free(t_camera *camera);
t_node *	camera_add(const char *name);
t_camera *	camera_rebind(t_scene *scene, void *ptr);
t_camera *	camera_new(const char *name);
t_node *	camera_make(const char *name);

#endif
