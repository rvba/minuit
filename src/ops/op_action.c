/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "app.h"
#include "ctx.h"
#include "node.h"
#include "scene.h"
#include "event.h"
#include "draw.h"
#include "ui.h"
#include "file.h"
#include "base.h"

static void (* QUIT_OP)(void) = NULL;

void op_set_color(t_context *C,int color)
{
	if(C->event->color_transition)
	{
		if(!C->event->color_transition_set)
		{
			C->event->color_transition_set = 1;

			if(C->draw->color==color_black)
			{
				C->draw->color = color_white;
				C->event->color_transition_count = 0;
			}
			else
			{
				C->draw->color = color_black;
				C->event->color_transition_count = 0;
			}
		}

		if(C->draw->color==color_black)
		{
			if(C->ui->background_color[0] >= 0)
			{
				float i = (float)((float)C->event->color_transition_count/255.0f);
				vplus4i(C->ui->background_color,-C->event->color_transition_count);
				vplus4f(C->ui->front_color,i);
				vplus4f(C->draw->front_color,i);
				C->event->color_transition_count++;
			}
			else
			{
				vset4i(C->draw->background_color,0,0,0,0);
				vset4i(C->ui->background_color,0,0,0,0);
				vset4f(C->ui->front_color,1,1,1,0);
				vset4f(C->draw->front_color,1,1,1,0);
				vset4f(C->draw->back_color,0,0,0,0);
				vset4f(C->ui->back_color,0,0,0,0);

				C->event->color_transition = 0;
				C->event->color_transition_set = 0;
			}

		}
		else
		{
			if(C->ui->background_color[0] < 256)
			{
				float i = (float)-((float)C->event->color_transition_count/255.0f);
				vplus4i(C->ui->background_color,C->event->color_transition_count);
				vplus4f(C->ui->front_color,i);
				vplus4f(C->draw->front_color,i);
				C->event->color_transition_count++;
			}
			else
			{
				vset4i(C->ui->background_color,255,255,255,0);
				vset4f(C->ui->front_color,0,0,0,0);

				/*
				vset4f(C->draw->front_color,0,0,0,0);
				vset4f(C->draw->back_color,1,1,1,0);
				*/

				/* remain black */
				vset4f(C->draw->front_color,1,1,1,0);
				vset4f(C->draw->back_color,0,0,0,0);

				vset4f(C->ui->back_color,1,1,1,0);

				C->event->color_transition = 0;
				C->event->color_transition_set = 0;
			}
		}
	}
	else
	{
		//C->draw->color = color;

		if(C->draw->color==color_black)
		{
			vset4i(C->draw->background_color,0,0,0,0);
			vset4i(C->ui->background_color,0,0,0,0);
			vset4f(C->ui->front_color,1,1,1,0);
			vset4f(C->draw->front_color,1,1,1,0);
			vset4f(C->draw->back_color,0,0,0,0);
			vset4f(C->ui->back_color,0,0,0,0);
		}
		else
		{
			vset4i(C->draw->background_color,255,255,255,0);
			vset4i(C->ui->background_color,255,255,255,0);
			vset4f(C->ui->front_color,0,0,0,0);
			vset4f(C->draw->front_color,0,0,0,0);
			vset4f(C->draw->back_color,1,1,1,0);
			vset4f(C->ui->back_color,1,1,1,0);
		}
	}
}

void op_switch_color(t_context *C)
{
	if(C->event->color_transition_use)
	{
		C->event->color_transition = 1;
	}
	else
	{
		if(C->draw->color==color_white)
		{
			op_set_color(C,color_black);
		}
		else
		{
			op_set_color(C,color_white);
		}
	}
}

int op_exit(void)
{
	//log_close();

	printf("...\n");

	exit(1);

	return 0;
}

int op_quit(t_node *node)
{
	t_context *C = ctx_get();
	C->app->quit = 1;

	return 1;
}

int op_post_quit(t_node *node)
{
	t_context *C = ctx_get();

	//glutLeaveMainLoop();

	engine_quit(C->engine);

	if(C->event->free_scene)
	{
	}

	if( strlen( C->app->filename))
	{
		char *path = app_get_file_path( C->app, APP_FILENAME_SAVE);
		t_file *file = NULL;

		if( !sys_file_exists( path))
		{
			if( file_create( path)) file = file_access( path);
		}
		else
		{
			file = file_access( path);
		}

		if( file)
		{
			if( strlen( C->app->path_file))
			{
				char filename[_PATH_];
				bzero( filename, _PATH_);
				s_convert_endline_newline( filename, C->app->path_file);
				file_write( file, filename, strlen( filename));
			}
			else
			{
				printf("[OP] Quit error, invalid file path\n");
			}
		}
	}

	/* Quit Operator */
	if( QUIT_OP) QUIT_OP();

	// exit badly
	op_exit();

	return 1;
}

void op_quit_operator_set( void (* ptr)( void))
{
	QUIT_OP = *ptr;
}






