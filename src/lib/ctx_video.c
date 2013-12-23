/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "context.h"
#include "ctx.h"
#include "app.h"
#include "process.h"
#include "engine.h"
#include "event.h"
#include "term.h"
#include "ui.h"
#include "image.h"
#include "list.h"
#include "memory.h"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

/* **MEM** */

void ctx_switch_record_video(t_context *C)
{
	if(C->event->video_record)
	{
		C->ui->show_mouse = 0;
		C->event->video_stop_call = 1;
		term_log("record stop");
		
	}
	else
	{
		if(C->ui->visualize_mouse)
		{
			C->ui->show_mouse = 1;
		}

		C->event->video_record = 1;
		term_log("record start");
	}
}

void ctx_check_dir(t_context *C)
{
	DIR* dir = opendir("video");
	if (dir)
	{
 		closedir(dir);
	}
	else
	{
		mkdir("video",S_IRWXU);
	}
}

void ctx_render_build_frames(t_context *C, t_lst *lst)
{
	t_link *link;
	unsigned char *frame;

	int width = C->app->window->width;
	int height = C->app->window->height;

	ctx_check_dir(C);

	for(link = lst->first; link; link = link->next)
	{
		frame = link->data;
		char filename[200];
		sprintf(filename,"video/f%04d.jpg",C->event->video_frame_number);
		img_save_video(width,height,filename,frame);
		C->event->video_frame_number++;
	}
}

void ctx_render_free_frames(t_context *C, t_lst *lst)
{
	t_link *link;
	unsigned char *frame;

	for(link = lst->first; link; link = link->next)
	{
		frame = link->data;
		free(frame);
	}

	lst_free(lst);
}

void *ctx_record_process(void *ptr)
{
	t_context *C = ctx_get();
	t_process *process = (t_process *) ptr;


	if(C->app->video_frames_swap)
	{
		process->busy = 1;
		ctx_render_build_frames(C,C->app->video_frames_swap);
		ctx_render_free_frames(C,C->app->video_frames_swap);
		C->app->video_frames_swap = NULL;
		process->busy = 0;
	}

	return NULL;
}

void ctx_render_video(t_context *C)
{
	int width = C->app->window->width;
	int height = C->app->window->height;

	if(!C->event->video_init)
	{
		C->event->video_init = 1;
		C->event->video_store = 1;
		C->event->video_frame = C->app->frame;
		C->event->video_frame_number = 1;

		C->event->video_frames_in_memory = 0;

		t_process *process=process_add(C,"record",ctx_record_process);
		process_launch(process);
	}

	if(C->event->video_stop)
	{
		C->event->video_store = 0;
		t_process *process = engine_process_get(C->engine, "record");

		if(!process->busy)
		{
			engine_process_remove(C->engine,process);

			char command[1024];
			sprintf(command,"ffmpeg -f image2 -i video/f%%04d.jpg -r 25 -b %dk video/video.avi &",C->ui->bitrate);

			if(C->app->video_build)
			{
				//system("ffmpeg -f image2 -i video/f%04d.jpg -r 25 -b 5000k video/video.avi &");
				int r = system(command);
				(void) r; // unused value:)
			}

			C->event->video_stop = 0;
			C->event->video_stop_call = 0;
			C->event->video_record = 0;
			C->event->video_init = 0;
		}
	}
	else if(C->event->video_record)
	{
		int f = C->app->frame - C->event->video_frame;

		if(f >= C->app->video_offset) 
		{
			if(C->event->video_store)
			{
				if(C->event->video_stop_call)
				{
					t_process *process = engine_process_get(C->engine, "record");

					if(!process->busy)
					{
						t_lst *lst = lst_copy(C->app->video_frames);
						lst_cleanup(C->app->video_frames);
						C->app->video_frames_swap = lst;
						C->event->video_frames_in_memory = 0;

						C->event->video_stop = 1;
					}
				}
				else
				{
					C->event->video_frame = C->app->frame;

					unsigned char *buffer = (unsigned char *)mem_malloc(sizeof(unsigned char *)*width*height*4);

					glPixelStorei(GL_PACK_ALIGNMENT, 1);
					glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

					lst_add(C->app->video_frames,buffer,"frame");

					C->event->video_frames_in_memory++;

					if(C->event->video_frames_in_memory > C->app->video_limit)
					{
						t_process *process = engine_process_get(C->engine, "record");

						if(!process->busy)
						{
							t_lst *lst = lst_copy(C->app->video_frames);
							lst_cleanup(C->app->video_frames);
							C->app->video_frames_swap = lst;
							C->event->video_frames_in_memory = 0;
						}
					}
				}
			}
		}
	}
}
