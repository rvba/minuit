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

#ifdef __cplusplus
extern "C" {
#endif


#define CAM_SPEED 50
#define CAM_ORTHO_NEAR -1
#define CAM_ORTHO_FAR 1
#define CAM_EYE 10
#define CAM_FAR 80000
#define CAM_NEAR .001
#define CAM_FOVY 60 //60

#define CAM_ORTHO_ZOOM 1000
#define CAM_ORTHO_ZOOM_FAC 10
#define CAM_IS_MOVING 0

struct Context;
struct Node;
struct Scene;

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
	t_id id;
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
void 		_camera_free(t_camera *camera);
void 		camera_free(t_camera *camera);
struct Node *	camera_add(const char *name);

t_camera *	camera_clone(t_camera *camera);
t_camera *	camera_rebind(struct Scene *scene, void *ptr);
t_camera *	camera_new(const char *name);
struct Node *	camera_make(const char *name);



void 		op_3d_orientation(void);
void 		op_camera_view_top(t_camera *camera);
void 		op_camera_view_axo(t_camera *camera);
void 		op_camera_view_front(t_camera *camera);
void 		op_camera_view_left(t_camera *camera);
void 		op_camera_view_right(t_camera *camera);
void 		op_camera_ortho_rotate(t_camera *camera,int x,int y,int z);
void 		op_camera_switch_view(t_camera *camera);
void 		op_camera_switch_type(t_camera *camera);
void 		op_camera_change_speed(t_camera *camera);
void 		op_camera_translate_key(t_camera *camera,float x,float y);
void 		op_camera_switch_2d(struct Context *C, t_camera *camera);
void 		op_camera_switch_3d(struct Context *C, t_camera *camera);
void 		op_camera_update(struct Context *C, t_camera *camera);
void 		op_camera_frustum_init(t_camera *camera);
void 		op_camera_reset(struct Context *C, t_camera *camera);
void 		op_camera_reset_pos(t_camera *camera);
void 		op_camera_rotate(struct Context *C, t_camera *camera, float dx, float dy);
void 		op_camera_translate(struct Context *C, t_camera *camera);
void 		op_camera_zoom(struct Context *C, t_camera *camera, int dir);
void 		op_camera_set_ortho_zoom(struct Context *C, t_camera *camera, int i);
void 		op_camera_set_ortho_pan(struct Context *C, t_camera *camera);

char *		render_null;
char *		render_camera;
char *		render_fbo;
char *		render_tiled;

void 		op_3d_orientation(void);
void camera_free( t_camera *camera);




#ifdef __cplusplus
}
#endif



#endif
