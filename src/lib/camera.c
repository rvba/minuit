/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "ctx.h"
#include "scene.h"
#include "node.h"
#include "op.h"
#include "camera.h"
#include "memory.h"
#include "opengl.h"

void cls_camera_init(t_camera *camera)
{
}

t_camera_cls cls_camera=
{
	.cls_type="camera",
	.init=cls_camera_init,
};

void camera_set_frustum( t_camera *camera, int w, int h)
{
	//printf("set frustum %d %d\n", w, h);
	double r=3.14159265/180;
	double fovy = CAM_FOVY;
	double near = CAM_NEAR;
	double aspect = (double)((double)w/(double)h);
	double tangent = tan(fovy/2 * r);
	double height = near*tangent;
	double width = height*aspect;

	camera->fovy=fovy;
	camera->aspect=aspect;
	camera->left = -width;
	camera->right = width;
	camera->bottom = -height;
	camera->top = height;
	camera->near = CAM_NEAR;
	camera->far = CAM_FAR;

	camera->ortho_near=CAM_ORTHO_NEAR;
	camera->ortho_far=CAM_ORTHO_FAR;
}

void camera_set_restrict_matrix( t_camera *camera, int x, int y)
{
	if( camera->restrict_matrix)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT,viewport);
		gluPickMatrix((double)x,(double)y,1.0f,1.0f,viewport);
	}
}

void camera_projection_perspective( t_camera *camera)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(
		camera->left,
		camera->right,
		camera->bottom,
		camera->top,
		camera->near,
		camera->far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		camera->eye[0],
		camera->eye[1],
		camera->eye[2],
		camera->target[0],
		camera->target[1],
		camera->target[2],
		camera->up[0],
		camera->up[1],
		camera->up[2]
		);

	float zenith = camera->zenith;
	glTranslatef(camera->pos[0],camera->pos[1],camera->pos[2]);

	// Camera Up rotation
	glRotatef(zenith,camera->cross[0],camera->cross[1],camera->cross[2]);
}

void camera_projection_ortho( t_camera *camera)
{
	double left = camera->left;
	double right = camera->right;	
	double bottom = camera->bottom;
	double top = camera->top;
	float z=camera->ortho_zoom;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(left*z,right*z,bottom*z,top*z,camera->ortho_near,camera->ortho_far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(
			camera->ortho_location[0],
			camera->ortho_location[1],
			camera->ortho_location[2]
			);

	glRotatef(camera->angle,
		camera->ortho_rotation[0],
		camera->ortho_rotation[1],
		camera->ortho_rotation[2]
		);
}


void camera_set_viewport( t_camera *camera, int px, int py, int width, int height)
{
	//printf("set viewport %d %d %d %d\n", px, py, width, height);
	glViewport( px, py, width, height);
}

void camera_set_projection(t_camera *camera, int type)
{
	camera->type = type;
	if (camera->type == camera_ortho)  camera_projection_ortho( camera); 
	else if (camera->type == camera_frustum) camera_projection_perspective( camera);
}

void camera_show(t_camera *camera)
{
	printf("Camera: %s\n",camera->id.name);
	printf("frustum\n");
	printf("left %f\n",camera->left);
	printf("right %f\n",camera->right);
	printf("bottom %f\n",camera->bottom);
	printf("top %f\n",camera->top);
	printf("aspect %f\n",camera->aspect);
	printf("fovy %f\n",camera->fovy);
	printf("near %f\n",camera->near);
	printf("far %f\n",camera->far);
}

void camera_copy(t_camera *target,t_camera *source)
{
	target->type = source->type;

	vcp(target->cross,source->cross);
	vcp(target->pos,source->pos);
	vcp(target->eye,source->eye);
	vcp(target->target,source->target);
	vcp(target->up,source->up);

	target->ortho_view = source->ortho_view;
	target->ortho_zoom = source->ortho_zoom;

	vcp(target->ortho_location,source->ortho_location);
	vcp(target->ortho_rotation,source->ortho_rotation);

	target->ortho_near=source->ortho_near;
	target->ortho_far=source->ortho_far;

	target->aspect=source->aspect;
	target->fovy=source->fovy;
	target->left=source->left;
	target->right=source->right;
	target->bottom=source->bottom;
	target->top=source->top;
	target->near=source->near;
	target->far=source->far;

	target->zenith=source->zenith;

	target->frame=source->frame;
}

void _camera_free(t_camera *camera)
{
	mem_free( camera, sizeof( t_camera));
}

void camera_free(t_camera *camera)
{
	mem_free( camera, sizeof( t_camera));
}

t_node *camera_add(const char *name)
{
	t_node *node = camera_make(name); //XXX
	return node;
}

t_node *camera_make(const char *name)
{
	t_context *C=ctx_get();
	t_node *node_camera=scene_add(C->scene,dt_camera,name);
	return node_camera;
}

void camera_init(t_camera *camera)
{
	camera->cls=&cls_camera;
}

void *camera_get_ref(t_camera *camera, const char *ref)
{
	void *p;

	if(is(ref,"pos_x"))  			p=&camera->pos[0]; 
	else if(is(ref,"pos_y"))  			p=&camera->pos[1]; 
	else if(is(ref,"pos_z"))  			p=&camera->pos[2]; 
	else if(is(ref,"eye_x"))  			p=&camera->eye[0]; 
	else if(is(ref,"eye_y"))  			p=&camera->eye[1]; 
	else if(is(ref,"eye_z"))  			p=&camera->eye[2]; 
	else
	{
		printf("[ERROR camera_get_ref] Unknown ref [%s] \n",ref);
		return NULL;
	}

	return p;
}

t_camera *camera_clone(t_camera *camera)
{
	t_camera *clone = camera_new(camera->id.name);

	clone->cls = camera->cls;
	clone->is_moving = camera->is_moving;
	clone->type = camera->type;
	clone->frame = camera->frame;
	clone->restrict_matrix = camera->restrict_matrix;
	clone->speed = camera->speed;

	vcp3f(clone->eye,camera->eye);
	vcp3f(clone->target,camera->target);
	vcp3f(clone->up,camera->up);
	vcp3f(clone->pos,camera->pos);
	vcp3f(clone->cross,camera->cross);

	clone->zenith = camera->zenith;

	clone->ortho_view = camera->ortho_view;
	clone->ortho_zoom = camera->ortho_zoom;
	clone->angle = camera->angle;
	clone->ortho_near = camera->ortho_near;
	clone->ortho_far = camera->ortho_far;

	vcp3f(clone->ortho_location,camera->ortho_location);
	vcp3f(clone->ortho_rotation,camera->ortho_rotation);

	clone->left = camera->left;
	clone->right = camera->right;
	clone->bottom = camera->bottom;
	clone->top = camera->top;

	clone->aspect = camera->aspect;
	clone->fovy = camera->fovy;
	clone->near = camera->near;
	clone->far = camera->far;

	return clone;

};

t_camera *camera_rebind(t_scene *scene, void *ptr)
{
	return ptr;
}


t_camera *camera_new(const char *name)
{
	t_camera *camera  = (t_camera *)mem_malloc(sizeof(t_camera));

	id_init(&camera->id, name);

	camera->restrict_matrix=0;
	camera->type = camera_frustum;

	vset(camera->cross,0,0,0);
	vset(camera->pos,0,0,0);
	vset(camera->eye,CAM_EYE,CAM_EYE,CAM_EYE);
	vset(camera->target,0,0,0);
	vset(camera->up,0,1,0);

	vset(camera->ortho_rotation,1,0,0);
	vset(camera->ortho_location,0,0,0);
	camera->ortho_view = camera_top;
	camera->angle = 90;
	//camera->ortho_zoom = CAM_ORTHO_ZOOM;
	camera->ortho_zoom = 1;

	camera->ortho_near=CAM_ORTHO_NEAR;
	camera->ortho_far=CAM_ORTHO_FAR;
	camera->near=CAM_NEAR;
	camera->far=CAM_FAR;
	camera->left=0;
	camera->right=0;
	camera->bottom=0;
	camera->top=0;
	camera->zenith=0;
	camera->frame=0;
	camera->is_moving=CAM_IS_MOVING;
	camera->update_frustum = 1;


	return camera;
}
