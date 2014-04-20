/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "ctx.h"
#include "scene.h"
#include "app.h"
#include "draw.h"
#include "camera.h"
#include "viewport.h"
#include "util.h"
#include "ui.h"

#define OP_CAM_SPEED 50
#define OP_CAM_ORTHO_ZOOM_FAC 100
#define OP_CAM_ORTHO_PAN_FAC 30 

void op_3d_orientation(void)
{
	glRotatef(-90,1,0,0);
}

void op_camera_view_top(t_camera *camera)
{
	camera->ortho_view = camera_top;
	op_camera_switch_view(camera);
}

void op_camera_view_axo(t_camera *camera)
{
	camera->ortho_view = camera_axo;
	op_camera_switch_view(camera);
}

void op_camera_view_front(t_camera *camera)
{
	camera->ortho_view = camera_front;
	op_camera_switch_view(camera);
}

void op_camera_view_left(t_camera *camera)
{
	camera->ortho_view = camera_left;
	op_camera_switch_view(camera);
}

void op_camera_view_right(t_camera *camera)
{
	camera->ortho_view = camera_right;
	op_camera_switch_view(camera);
}

void op_camera_ortho_rotate(t_camera *camera,int x,int y,int z)
{
	camera->angle=camera->angle-1;
}

void op_camera_switch_view(t_camera *camera)
{
	switch (camera->ortho_view)
	{
		case camera_front:
			vset(camera->ortho_rotation,1,0,0);
			camera->angle = 0;
			break;
		case camera_top:
			vset(camera->ortho_rotation,1,0,0);
			camera->angle = 90;
			break;
		case camera_right:
			vset(camera->ortho_rotation,1,0,0);
			camera->angle = 90;
			break;
		case camera_left:
			vset(camera->ortho_rotation,0,1,0);
			camera->angle = 90;
			break;
		case camera_axo:
			vset(camera->ortho_rotation,1,1,1);
			camera->angle = 90;
			break;
	}
}

void op_camera_switch_type(t_camera *camera)
{
	if (camera->type == camera_ortho) camera->type = camera_frustum;
	else camera->type = camera_ortho;
}

void op_camera_change_speed(t_camera *camera)
{
	t_context *C=ctx_get();
	if (C->app->keyboard->ctrl) camera->speed = 0.05; 
	else if( C->app->keyboard->shift) camera->speed = 10;
	else  camera->speed = 1.0;
}

void op_camera_translate_key(t_camera *camera,float x,float y)
{
	float speed = camera->speed;
	float delta = 0.001;
	float cross[3];
	float eye[3];
	float vec[3];
	
	vcp(cross,camera->cross);
	vcp(eye,camera->eye);
	vmul(cross,(x*delta*speed));
	eye[1]=0;
	vmul(eye,(y*(-delta)*speed));
	vadd(vec,cross,eye);
	vadd(camera->pos,camera->pos,vec);

	camera->is_moving=1;
}

void op_camera_restrict( t_context *C, t_camera *camera)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	int x = C->app->mouse->x;
	int y = C->app->mouse->y;
	gluPickMatrix((double)x,(double)y,1.0f,1.0f,viewport);
}

void op_camera_frustum_init(t_camera *camera, int w, int h)
{
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

void op_camera_reset_pos(t_camera *camera)
{
	vset(camera->pos,0,0,0);
}

void op_camera_rotate(t_context *C, t_camera *camera, float dx,float dy)
{
	float speed = camera->speed;

	float x = camera->eye[0];
	float y = camera->eye[1];
	float z = camera->eye[2];

	// XZ
	float alpha = -0.03 * dx * speed;

	float xn = ( x * cos(alpha) ) - ( z * sin(alpha) ); 
	float zn = ( z * cos(alpha) ) + ( x * sin(alpha) );

	vset(camera->eye,xn,y,zn);

	float ox[3];
	float oy[3];

	vset(ox,camera->eye[0],0,camera->eye[2]);
	vset(oy,0,camera->eye[1],0);
	vcross(camera->cross,ox,oy);

	float beta = 0.5 * dy * speed; 

	camera->zenith +=  beta;

	camera->is_moving=1;
}

void op_camera_translate(t_context *C, t_camera *camera)
{
	float speed = -camera->speed / OP_CAM_SPEED;
	float delta = .5;
	float left[3];
	float front[3];
	float vec[3];

	vcp(left,camera->cross);
	vcp(front,camera->eye);
	vmul( left, ( C->ui->mouse_dx * delta *speed));
	front[1]=0;
	vmul(front,(C->ui->mouse_dy * delta * speed));
	vadd(vec,left,front);
	vadd(camera->pos,camera->pos,vec);

	camera->is_moving=1;
}

void op_camera_zoom(t_context *C, t_camera *camera, int dir)
{
	vmul(camera->eye, 1 + (30 * .005 * camera->speed * dir));
	camera->is_moving=1;
}

void op_camera_set_ortho_zoom(t_context *C, t_camera *camera, int i)
{
	float speed = camera->speed;
	camera->ortho_zoom += ( i * 200 * speed) ;
	camera->is_moving=1;
}

void op_camera_set_ortho_pan(t_context *C, t_camera *camera)
{
	//float speed = camera->speed;
	float d = .00001;
	float factor = camera->ortho_zoom *.1;

	float dx = (float) (C->ui->mouse_dx);
	float dy = (float) ( C->ui->mouse_dy);

	dx *= d * factor;
	dy *= d * factor;

	float v[3];

	vset(v,dx,dy,0);
	vadd(camera->ortho_location,camera->ortho_location,v);
}

