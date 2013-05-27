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
#include "process.h"
#include "engine.h"

void op_set_color(t_context *C,int color)
{
	C->draw->color = color;

	if(C->draw->color==color_black)
	{
		//C->draw->color=color_black;
		vset4i(C->draw->background_color,0,0,0,0);
		vset4i(C->ui->background_color,0,0,0,0);
		vset4f(C->ui->front_color,1,1,1,0);
		vset4f(C->draw->front_color,1,1,1,0);
		vset4f(C->draw->back_color,0,0,0,0);
		vset4f(C->ui->back_color,0,0,0,0);
	}
	else
	{
		//C->draw->color=color_white;
		vset4i(C->draw->background_color,255,255,255,0);
		vset4i(C->ui->background_color,255,255,255,0);
		vset4f(C->ui->front_color,0,0,0,0);
		vset4f(C->draw->front_color,0,0,0,0);
		vset4f(C->draw->back_color,1,1,1,0);
		vset4f(C->ui->back_color,1,1,1,0);
	}

}

void op_switch_color(t_context *C)
{
	if(C->draw->color==color_white)
	{
		op_set_color(C,color_black);
		/*
		C->draw->color=color_black;
		vset4i(C->draw->background_color,0,0,0,0);
		vset4i(C->ui->background_color,0,0,0,0);
		vset4f(C->ui->front_color,1,1,1,0);
		vset4f(C->draw->front_color,1,1,1,0);
		vset4f(C->ui->back_color,0,0,0,0);
		*/
	}
	else
	{
		op_set_color(C,color_white);
		/*
		C->draw->color=color_white;
		vset4i(C->draw->background_color,255,255,255,0);
		vset4i(C->ui->background_color,255,255,255,0);
		vset4f(C->ui->front_color,0,0,0,0);
		vset4f(C->draw->front_color,0,0,0,0);
		vset4f(C->ui->back_color,1,1,1,0);
		*/
	}
}


void op_scene_loop_over(t_lst *list,FILE *file)
{
	/*
	t_node *node;
	t_link *link;
	for(link=list->first;link;link=link->next)
	{	
		node=link->data;
		node->cls->write(node,file);
	}
	*/
}

void op_save_add_header(char *section)
{
	FILE *file = fopen("./save","a+");
	fprintf(file,"%s\n",section);
}

void op_save(void)
{
	t_context *C = ctx_get();
	FILE *file = fopen("./save","w+");
	fclose(file);

	file = fopen("./save","a+");

	op_scene_loop_over(C->scene->bricks,file);
	op_scene_loop_over(C->scene->meshes,file);
	op_scene_loop_over(C->scene->blocks,file);
	op_scene_loop_over(C->scene->files,file);
	op_scene_loop_over(C->scene->objects,file);

	fclose(file);
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

	engine_quit(C->engine);

	if(C->event->free_scene)
	{
	}

	// minuit.save

	t_file *file = file_new("minuit.save");
	file_init(file);
	file_open(file);
	fprintf(file->file,"%s\n",C->app->file->location);
	file_close(file);

	// exit badly
	op_exit();

	return 1;
}

// SCREEN

/*
void screen_start_record(void)
{ 
	t_context *C = ctx_get();
	C->event->rec = 1;
}

void screen_stop_record(void)
{ 
	t_context *C = ctx_get();
	C->event->rec = 0;
	C->event->recording = 0;
	if(C->event->rec_save)
		system("ffmpeg -f image2 -i ./data/movie/file-%04d.jpg -r 25 -b 5000k ./data/movie/out.avi &");
}
*/

void screen_capture(const char name[])
{
	t_context *C = ctx_get();

	img_save_jpg(C->app->window->width,C->app->window->height,name);
}

/*
void screen_record(void)
{
	t_context *C = ctx_get();
	if (C->event->rec == 1)
	{
		if (C->event->recording == 0)
		{
			printf("init\n");
			C->event->recording = 1;
			C->event->frame_rec = 0;
		}
		char output[200] = "./data/movie/";
		char name[] = "file-";
		char date[30];
		memset(date,30,0);
		sprintf(date,"%04d",C->event->frame_rec);
		printf("name %s\n",date);
		char ext[] = ".jpg";

		strcat(output,name);
		strcat(output,date);
		strcat(output,ext);

		screen_capture(output);

		C->event->frame_rec++;
	}
}
*/






