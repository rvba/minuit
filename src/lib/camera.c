/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "camera.h"

void cls_camera_init(t_camera *camera)
{
}

t_camera_cls cls_camera=
{
	.cls_type="camera",
	.init=cls_camera_init,
};

t_camera *camera_clone(t_camera *source)
{
	t_node *node_target = camera_make("clone");
	t_camera *target = node_target->data;
	camera_copy(target,source);

	return target;
}

void camera_show(t_camera *camera)
{
	printf("Camera: %s\n",camera->name);
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

void camera_free(t_camera *camera)
{
	free(camera);
}

t_node *camera_add(const char *name)
{
	t_node *node = camera_make(name); //XXX
	return node;
}

t_node *camera_make(const char *name)
{
	t_context *C=ctx_get();
	t_node *node_camera=scene_add(C->scene,nt_camera,name);
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

t_camera *camera_rebind(t_scene *scene, void *ptr)
{
	return ptr;
}

t_camera *camera_new(const char *name)
{
	t_camera *camera  = (t_camera *)malloc(sizeof(t_camera));

	camera->id=0;
	camera->id_chunk=0;
	set_name(camera->name,name);
	camera->users=0;

	camera->restrict_matrix=0;
	camera->type = camera_frustum;

	vset(camera->cross,0,0,0);
	vset(camera->pos,0,0,0);
	vset(camera->eye,CAM_EYE,CAM_EYE,CAM_EYE);
	vset(camera->target,0,0,0);
	vset(camera->up,0,1,0);

	camera->ortho_view = 0;
	camera->ortho_zoom = CAM_ORTHO_ZOOM;

	vset(camera->ortho_location,0,0,0);
	vset(camera->ortho_rotation,1,0,0);
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

	return camera;
}
