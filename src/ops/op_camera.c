/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
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

void op_camera_switch_2d(t_context *C, t_camera *camera)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	t_app *app=C->app;

	int width = app->window->width;
	int height = app->window->height;

	double left = camera->left;
	double right = camera->right;	
	double bottom = camera->bottom;
	double top = camera->top;

	glOrtho(
		0 + left,
		width + right,
		0 + bottom,
		height + top,
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

void op_camera_restrict( t_context *C, t_camera *camera)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	int x = C->app->mouse->x;
	int y = C->app->mouse->y;
	gluPickMatrix((double)x,(double)y,1.0f,1.0f,viewport);
}

void op_camera_projection_perspective( t_context *C, t_camera *camera)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(C->draw->with_restrict_matrix && (C->draw->mode == mode_selection))
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT,viewport);
		int x = C->app->mouse->x;
		int y = C->app->mouse->y;
		gluPickMatrix((double)x,(double)y,1.0f,1.0f,viewport);
	}
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

void op_camera_projection_ortho( t_context *C, t_camera *camera)
{
	double left = camera->left;
	double right = camera->right;	
	double bottom = camera->bottom;
	double top = camera->top;

	float z=camera->ortho_zoom;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(C->draw->with_restrict_matrix && (C->draw->mode == mode_selection))
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

void op_camera_viewport( t_context *C, t_camera *camera, t_viewport *viewport)
{
	t_app *app=C->app;
	int w,h;
	if( viewport->fullscreen)
	{
		w = app->window->viewport_width;
		h = app->window->viewport_height;

	}
	else
	{
		w = viewport->width;
		h = viewport->height;
	}

	op_camera_frustum_init(camera, w, h);

	int x = viewport->x;
	int y = viewport->y;

	glViewport(x, y, w, h);
}

void _op_camera_update(t_context *C, t_camera *camera, t_viewport *viewport)
{
	// Viewport
	op_camera_viewport( C, camera, viewport);

	// Projection
	if (camera->type == camera_ortho)  op_camera_projection_ortho( C, camera); 
	else if (camera->type == camera_frustum) op_camera_projection_perspective( C, camera);
}

void op_camera_update(t_context *C, t_camera *camera)
{
	t_app *app=C->app;

	glViewport(0,0,app->window->viewport_width,app->window->viewport_height);

	// ORTHO
	if (camera->type == camera_ortho)
	{
		double left = camera->left;
		double right = camera->right;	
		double bottom = camera->bottom;
		double top = camera->top;

		float z=camera->ortho_zoom;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(C->draw->with_restrict_matrix && (C->draw->mode == mode_selection))
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

	// FRUSTUM
	else if (camera->type == camera_frustum)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(C->draw->with_restrict_matrix && (C->draw->mode == mode_selection))
		{
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT,viewport);
			int x = C->app->mouse->x;
			int y = C->app->mouse->y;
			gluPickMatrix((double)x,(double)y,1.0f,1.0f,viewport);
		}
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
}

void op_camera_frustum_init(t_camera *camera, int w, int h)
{
//	t_app *app = app_get();
	double r=3.14159265/180;
	double fovy = CAM_FOVY;
	double near = CAM_NEAR;
//	int w=app->window->width;
//	int h=app->window->height;
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
	camera->near = CAM_NEAR;
	camera->far = CAM_FAR;
	camera->aspect =(double)((double)app->window->width/(double)app->window->height);
	camera->fovy = 60;
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
	camera->ortho_zoom = camera->ortho_zoom + (i * OP_CAM_ORTHO_ZOOM_FAC * speed *camera->ortho_zoom * .001);

	camera->is_moving=1;
	op_camera_update(C, camera);
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

