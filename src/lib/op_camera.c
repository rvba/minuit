/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

#define OP_CAM_SPEED 50
#define OP_CAM_ORTHO_ZOOM_FAC 10

/** record mode : clone camera and link it to last one **/

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

void op_camera_switch_2d(t_context *C, t_camera *camera, int z,int p)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	t_app *app=C->app;

	int width = app->window->width;
	int height = app->window->height;

	double left = camera->ui_left;
	double right = camera->ui_right;	
	double bottom = camera->ui_bottom;
	double top = camera->ui_top;

	float pan_x=-C->ui->pan_x*p;
	float pan_y=-C->ui->pan_y*p;
	float zoom;

	if(z)
		zoom=C->ui->zoom;
	else
		zoom=1;

	glOrtho(
		0+left+pan_x,
		(width * zoom)+right+pan_x,
		0+bottom+pan_y,
		(height * zoom)+top+pan_y,
		-1,
		1
		);

	glMatrixMode(GL_MODELVIEW);
}

void op_camera_switch_3d(t_context *C, t_camera *camera)
{
	op_camera_update(C, camera);
	op_3d_orientation();
}

void op_camera_update(t_context *C, t_camera *camera)
{
	t_app *app=C->app;

	glViewport(0,0,app->window->viewport_width,app->window->viewport_height);

	// ORTHO
	if (camera->type == camera_ortho)
	{
		double left = camera->o_left;
		double right = camera->o_right;	
		double bottom = camera->o_bottom;
		double top = camera->o_top;

		float z=camera->ortho_zoom;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(camera->restrict_matrix)
		{
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT,viewport);
			int x = C->app->mouse->x;
			int y = C->app->mouse->y;
			gluPickMatrix((double)x,(double)y,1.0f,1.0f,viewport);
		}
		glOrtho(left*z,right*z,bottom*z,top*z,camera->ortho_near,camera->ortho_far);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(camera->angle,
			camera->ortho_rotation[0],
			camera->ortho_rotation[1],
			camera->ortho_rotation[2]
			);

		glTranslatef(
				camera->ortho_location[0],
				camera->ortho_location[1],
				camera->ortho_location[2]
				);
	}

	// FRUSTUM
	else if (camera->type == camera_frustum)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(camera->restrict_matrix)
		{
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT,viewport);
			int x = C->app->mouse->x;
			int y = C->app->mouse->y;
			gluPickMatrix((double)x,(double)y,1.0f,1.0f,viewport);
		}
		glFrustum(
			camera->f_left,
			camera->f_right,
			camera->f_bottom,
			camera->f_top,
			camera->f_near,
			camera->f_far);

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
}

void op_camera_frustum_init(t_camera *camera)
{
	t_app *app = app_get();
	double r=3.14159265/180;
	double fovy = CAM_FOVY;
	double near = CAM_NEAR;
	//double far = CAM_FAR;
	int w=app->window->width;
	int h=app->window->height;
	double aspect = (double)((double)w/(double)h);
	double tangent = tan(fovy/2 * r);
	double height = near*tangent;
	double width = height*aspect;

	camera->f_fovy=fovy;
	camera->f_aspect=aspect;
	camera->f_left = -width;
	camera->f_right = width;
	camera->f_bottom = -height;
	camera->f_top = height;
	camera->f_near = CAM_NEAR;
	camera->f_far = CAM_FAR;

	camera->o_left = -width;
	camera->o_right = width;
	camera->o_bottom = -height;
	camera->o_top = height;
	camera->ortho_near=CAM_ORTHO_NEAR;
	camera->ortho_far=CAM_ORTHO_FAR;

	camera->ui_left = -width;
	camera->ui_right = width;
	camera->ui_bottom = -height;
	camera->ui_top = height;
}

void op_camera_reset(t_context *C, t_camera *camera)
{
	t_app *app=C->app;

	vset(camera->cross,0,0,0);
	vset(camera->pos,0,0,0);
	vset(camera->eye,3,3,3);
	vset(camera->target,0,0,0);
	vset(camera->up,0,1,0);
	camera->ortho_view = 0;
	camera->ortho_zoom = CAM_ORTHO_ZOOM;
	camera->f_near = CAM_NEAR;
	camera->f_far = CAM_FAR;
	camera->f_aspect =(double)((double)app->window->width/(double)app->window->height);
	camera->f_fovy = 60;
	camera->zenith = 0;
	camera->angle = 90;
	vset(camera->ortho_location,0,0,0);
	vset(camera->ortho_rotation,1,0,0);
	camera->ortho_near=CAM_ORTHO_NEAR;
	camera->ortho_far=CAM_ORTHO_FAR;

	op_camera_update(C, camera);
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
	t_app *app=C->app;

	float speed = -camera->speed / OP_CAM_SPEED;
	float delta = 0.0001;
	float left[3];
	float front[3];
	float vec[3];
	
	vcp(left,camera->cross);
	vcp(front,camera->eye);
	vmul(left,(app->mouse->delta_x*delta*speed));
	front[1]=0;
	vmul(front,(app->mouse->delta_y*(delta)*speed));
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
	camera->ortho_zoom = camera->ortho_zoom + (i * OP_CAM_ORTHO_ZOOM_FAC * speed);
	camera->new_ortho_zoom+=.005*i;

	camera->is_moving=1;
	op_camera_update(C, camera);
}

void op_camera_set_ortho_pan(t_context *C, t_camera *camera)
{
	t_app *app=C->app;

	float speed = camera->speed;
	float d = .001;
	float dx = app->mouse->dx * d *speed * app->mouse->sign_x;
	float dy = app->mouse->dy * d * speed * app->mouse->sign_y;
	float v[3];
	vset(v,dx,dy,0);
	vadd(camera->ortho_location,camera->ortho_location,v);
}

