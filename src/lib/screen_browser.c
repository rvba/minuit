/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "node.h"
#include "scene.h"

#include "op.h"
#include "ctx.h"
#include "app.h"
#include "event.h"
#include "screen.h"
#include "memory.h"
#include "camera.h"
#include "ui.h"
#include "file.h"

#include "list.h"

#include "block.h"
#include "brick.h"

t_node *BROWSER_BUTTONS;
t_file *path;

int is_init=0;
int is_built=0;

char cmd[1024];
char default_path[1024];

int pos;

// switch back to main screen
void browser_abort(void)
{
	screen_switch_to_main();
}

void keymap_browser( int key)
{
	keymap_main(key);

	switch(key)
	{
		case APP_KEY_ESC: 	
			browser_abort();
	
		break;
	}
}

void *browser_go_backward(t_brick *brick)
{
	file_go_backward(path);
	is_built=0;
	brick_release(brick);
	return NULL;
}

void *browser_go_directory(t_brick *brick)
{
	file_go_directory(path,brick->id.name);
	is_built=0;
	brick_release(brick);
	return NULL;
}

void browser_delete_bricks(t_context *C,t_block *b)
{
	t_lst *list=b->bricks;
	t_link *l=list->first;

	if(l)
	{
		for(;l;l=l->next)
		{
			t_brick *brick=l->data;
			scene_delete(C->scene,brick);
		}
	}
	
	lst_cleanup(b->bricks);
}

// delete bricks
void browser_reset(void)
{
	t_context *C=ctx_get();
	t_block *block=BROWSER_BUTTONS->data;
	C->scene->use_tmp_colors=1;
	browser_delete_bricks(C,block);
	C->scene->use_tmp_colors=0;
}

void browser_build_cmd(void)
{
	int p=0;
	char *_path=path->location;

	p=s_append(cmd,"cd ",p);
	p=s_append(cmd,_path,p);
	p=s_append(cmd," && ls",p);
	p=s_append(cmd,NULL,p);
}

void *browser_return_filename(t_brick *brick)
{
	t_context *C=ctx_get();
	char tmp[1024];

	int pos=0;

	pos=s_append(tmp,path->location,pos);
	pos=s_append(tmp,"/",pos);
	pos=s_append(tmp,brick->id.name,pos);
	pos=s_append(tmp,NULL,pos);

	C->event->standby_string=s_allocate(tmp);

	screen_switch_by_name("screen_main");
	C->event->callback();
	brick_release(brick);
	return NULL;
}

void browser_build(void)
{
	t_context *C=ctx_get();
	t_node *node_brick;
	t_brick *brick;

	if(!is_built)
	{
		FILE *fp;
		FILE *file_type;
		
		char input[1024];
		char f_type[1024];

		is_built=1;

		// build cmd
		browser_build_cmd();
		browser_reset();

		fp = popen(cmd,"r");

		// tmp colors
		scene_color_switch_mode(C->scene);

		t_block *block=BROWSER_BUTTONS->data;

		if(fp)
		{
			while (fgets(input, sizeof(input)-1, fp) != NULL)
			{
				char file_name[1024];
				char file_path[1024];
				int pos=0;

				s_convert_newline_endline(file_name,input);

				pos=s_append(file_path,path->location,0);
				pos=s_append(file_path,"/",pos);
				pos=s_append(file_path,file_name,pos);
				pos=s_append(file_path,NULL,pos);

				int p=0;
				char _cmd[1024];

				p=s_append(_cmd,"file '",p);
				p=s_append(_cmd,file_path,p);
				p=s_append(_cmd,"'",p);
				p=s_append(_cmd,NULL,p);

				file_type=popen(_cmd,"r");


				if(file_type)
				{
					while (fgets(f_type,sizeof(f_type)-1, file_type) != NULL)
					{
						char ff[1024];
						char fff[1024];

						s_convert_newline_endline(ff,f_type);
						s_truncate_after(':',fff,ff);


						if(is(fff," directory"))
						{
							node_brick=add_brick_trigger(C,block,file_name,browser_go_directory);
						}
						else
						{
							node_brick=add_brick_trigger(C,block,file_name,browser_return_filename);
						}

						brick=node_brick->data;
						brick->state.use_global_blocking=1;

					}

					pclose(file_type);
				}

			}

			node_brick=add_brick_trigger(C,block,"..",browser_go_backward);
			brick=node_brick->data;
			brick->state.use_global_blocking=1;
		}

		// tmp colors
		scene_color_switch_mode(C->scene);

		pclose(fp);
	}
}

void browser_update(void)
{
	t_block *block=BROWSER_BUTTONS->data;
	block->cls->update(block);
}

void browser_draw(void)
{
	browser_build();
	t_block *block=BROWSER_BUTTONS->data;
	block->state.update_geometry=1;
	block->cls->draw(block);
}

void screen_browser(t_screen *screen)
{
	t_context *C=ctx_get();
	t_camera *camera = C->ui->camera;

	op_camera_switch_2d(C,camera);

	glPushMatrix();
	glLoadIdentity();

	browser_update();
	browser_draw();

	glPopMatrix();
	op_camera_switch_3d(C, camera);
}

void browser_init(void)
{
	if(!is_init)
	{
		// set init
		is_init=1;

		// get current
		char *r = getcwd(default_path,1024);
		if( !r ) printf("getcwd error\n");

		BROWSER_BUTTONS=block_make("block_browser","block");
		t_block *block = BROWSER_BUTTONS->data;
		vset(block->pos,0,0,0);
		path=file_new(default_path);
		file_init(path);
	}
}

t_screen *screen_browser_make( t_context *C)
{
	t_node *node = scene_add( C->scene, dt_screen, "screen_browser");
	t_screen *screen = node->data;

	screen->keymap = keymap_browser;
	screen->draw = screen_browser;

	screen->is_active = 0;
	screen->is_visible = 0;

	lst_add( C->ui->screens, node, "screen_browser");

	// init
	browser_init();

	return screen;
};
