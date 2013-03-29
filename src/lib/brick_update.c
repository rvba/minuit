/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"

int is_cloning=0;

void show_buffer(void)
{
	t_context *C=ctx_get();
	int i;

	printf("buffer:");
	for(i=0;i<C->event->buffer_char_counter;i++)
	{
		printf("%c",C->event->buffer_char[i]);
	}

	printf("\n");
}

// CONNECT

void cls_brick_connect(t_brick *self,t_brick *src)
{
	t_context *C=ctx_get();

	char msg[40];
	char *type=data_name_get(self->plug_intern.data_type);
	sprintf(msg,"plug in :%s",type);
	term_print(C->term,msg);

	t_plug *plug_in=&self->plug_in;
	t_plug *plug_out=&src->plug_out;

	// connect bricks
	plug_out->dst=plug_in;
	plug_in->src=plug_out;

	plug_in->is_connected=1;
	plug_out->is_connected=1;

	// versatil: change brick type
	if(self->state.is_versatil)
	{
		if(plug_out->data_type!=plug_in->data_type)
		{
			brick_type_change(self,&src->plug_intern);
		}
	}
}

// DISCONNECT

void cls_brick_disconnect(t_brick *self)
{
	t_context *C=ctx_get();

	C->event->state=event_unlinking;

	t_brick *brick_in=C->ui->brick_in;
	t_plug *plug_in=&brick_in->plug_in;
	t_plug *plug_target=plug_in->src;
	t_brick *brick_out=plug_target->brick;
	t_plug *plug_out=&brick_out->plug_out;

	// set plug state
	plug_in->is_connected=0;
	plug_in->src=NULL;

	plug_out->is_connected=0;
	plug_out->dst=NULL;

	// versatil: change brick type
	if(self->state.is_versatil)
	{
		brick_type_reset(self);
	}


	// change modes
	self->mode=bm_idle;
	brick_out->mode=bm_linking;

	// keep out
	C->ui->brick_out=brick_out;
	// remove in
	C->ui->brick_in=NULL;
}

// MOUSE

int brick_mouse_release(t_brick *brick)
{
	t_context *C=ctx_get();
	if(C->ui->mouse_mode)
	{
		if(C->app->mouse->button_middle==button_released) return 1;
		else return 0;
	}
	else
	{
		if(C->app->mouse->button_left==button_released) return 1;
		else return 0;
	}
}

int brick_mouse_cloning(t_context *C,int mouse_over)
{
	if(C->ui->mouse_mode)
	{
		if(mouse_over && C->app->mouse->button_middle==button_pressed) return 1;
		else return 0;
	}
	else
	{
		if(mouse_over && C->app->mouse->button_left==button_pressed && C->app->keyboard->shift) return 1;
		else return 0;
	}
}

// TRIGGER

void cls_brick_trigger_number(t_brick *brick)
{
	if(brick->state.use_loops == 0)
	{
		t_context *C = ctx_get();
		int frame = C->app->frame;

		if(brick->state.frame_loop != frame)
		{
			brick->state.frame_loop = frame;
			if(brick->action) brick->action(brick);
		}
		else
		{
			brick_set_updated(brick);
		}
	}
	else
	{
		if(brick->action) brick->action(brick);
	}

}

void cls_brick_trigger_switch(t_brick *brick)
{
	brick->action(brick);
}

void cls_brick_trigger_selector(t_brick *brick)
{
	brick->action(brick);
}

void cls_brick_trigger_menu(t_brick *brick)
{
	brick->action(brick);
}

void cls_brick_trigger_operator(t_brick *brick)
{
	brick->action(brick);
	if(brick->mode == bm_triggering)
		brick_release(brick);
}

void cls_brick_trigger_vlst(t_brick *brick)
{
	brick->action(brick);
}

void cls_brick_trigger_lst(t_brick *brick)
{
	brick->action(brick);
}

void cls_brick_trigger_generic(t_brick *brick)
{
	t_context *C = ctx_get();

	t_plug *plug_in=&brick->plug_in;
	t_plug *plug_out = &brick->plug_out;

	// get data from parent 
	if(plug_in->is_connected || plug_out->is_connected)
	{
		brick->action(brick);
		if(brick->mode == bm_triggering)
			brick_release(brick);
	}
	// or get user event
	else
	{
		// always
		if(brick->state.always_trigger)
		{
			brick->action(brick);

			if(brick->mode == bm_triggering)
				brick_release(brick);
		}
		else
		{
			// trigger
			if(brick->state.is_mouse_over)
			{
				// brick pressed
				if(C->app->mouse->button_left==button_pressed && brick->state.is_released)
				{
					brick->state.is_released=0;
					brick->action(brick);
				}
			}

			// release
			if(!brick->state.is_released && C->app->mouse->button_left==button_released)
			{
				// hide menu
				if(C->event->ui.is_menu_mouse_show && !C->ui->fixed_menu)
				{
					ctx_ui_menu_hide(C);
				}

				// release brick
				brick->state.is_released=1;
				brick_release(brick);
			}
		}
	}
}

void cls_brick_trigger_action_default(t_brick *brick)
{
	brick->action(brick);

}

// RELEASE

void brick_release(t_brick *brick)
{
	t_context *C = ctx_get();
	brick->mode=bm_idle;
	C->event->is_brick_transformed=0;
	C->ui->brick_selected=NULL;
}

// UPDATE

int is_vec_stored=0;
float v[3];
float vec[3];


void cls_brick_update(t_brick *brick)
{
	t_context *C = ctx_get();
	t_brick_mode mode = brick->mode;


	t_plug *plug_in = &brick->plug_in;

	int button_left=C->app->mouse->button_left;
	int button_right=C->app->mouse->button_right;
	int button_middle=C->app->mouse->button_middle;
	int mouse_over = is_mouse_over_brick(C,brick);
	int brick_clic=0;
	int button_middle_clic=0;

	float mouse_pos[3];
	vset(mouse_pos,0,0,0);


	if(mouse_over && button_left==button_pressed && (brick->state.is_mouse_over_plug_in==0) && (brick->state.is_mouse_over_plug_out==0)) brick_clic=1;
	if(mouse_over && button_middle==button_pressed) button_middle_clic=1;

	// IDLE

	if(!brick->state.is_idle)
	{
		if(C->app->mouse->button_left==button_released)
		{
			brick->state.is_idle=0;
		}
	}
		
	// MODES

	if(!C->event->ui.pan)
	{

	switch(mode)
	{
		case bm_idle:

			// STORE BRICK_IN
			if(C->event->is_brick_transformed)
			{

				if(is_mouse_over_plug(C,&brick->plug_in) && button_left==button_pressed)
				{
					C->ui->brick_in=brick;
				}

			}

			if(!C->event->is_brick_transformed)
			{
				// START MOVING
				if(mouse_over && button_right==button_pressed) 
				{
					if(!C->event->ui.is_menu_mouse_show)
					{
						C->event->is_brick_transformed=1;
						C->ui->brick_selected=brick;
						brick->mode=bm_moving;
					}
				}
				// START CLONING
				else if(brick_mouse_cloning(C,mouse_over))
				{
					if(is_cloning)
					{
						C->event->is_brick_transformed=1;
						C->ui->brick_selected=brick;
						brick->mode=bm_moving;
					}
					else
					{
						if(!C->event->ui.is_menu_mouse_show)
						{
							C->event->is_brick_transformed=1;
							C->ui->brick_selected=brick;
							brick->mode=bm_cloning;
						}
					}
				}

				// START LINKING
				else if(is_mouse_over_plug(C,&brick->plug_out) && button_left==button_pressed)
				{
					if(!C->event->ui.is_menu_mouse_show)
					{
						C->event->is_brick_transformed=1;
						C->ui->brick_selected=brick;
						brick->mode=bm_linking;
						C->ui->brick_out=brick;

						char msg[40];
						char *type=data_name_get(brick->plug_intern.data_type);
						sprintf(msg,"plug out:%s",type);
						term_print(C->term,msg);

					}
				}

				// START UNLINKING
				else if(is_mouse_over_plug(C,&brick->plug_in) && button_left==button_pressed)
				{
					C->ui->brick_in=brick;

					if(plug_in->is_connected)
					{
						if(!C->event->ui.is_menu_mouse_show)
						{
							C->event->is_brick_transformed=1;
							C->ui->brick_selected=brick;
							brick->mode=bm_unlinking;
						}
					}
				}

				// START TYPING
				else if(button_middle_clic)
				{
				}

				// START TRIGGER 
				else 
				{
					if(brick->state.is_clicable)
					{
						if(brick_clic && !plug_in->is_connected)
						{
							if(brick->state.use_brick_blocking)
							{
								if(brick->state.is_idle)
								{
									brick->state.is_idle=0;
									C->event->is_brick_transformed=1;
									C->ui->brick_selected=brick;
									brick->mode=bm_triggering;
								}
							}
							else if(brick->state.use_global_blocking)
							{
								if(C->app->mouse->button_left_is_ready)
								{
									C->app->mouse->button_left_is_ready=0;
									C->event->is_brick_transformed=1;
									C->ui->brick_selected=brick;
									brick->mode=bm_triggering;
								}
							}
							else
							{
								C->event->is_brick_transformed=1;
								C->ui->brick_selected=brick;
								brick->mode=bm_triggering;
							}
						}
					}
				}
			}

			break;

		case bm_moving:

			// cloning
			if(is_cloning)
			{
				// release
				if(brick_mouse_release(brick))
				{
					is_vec_stored=0;
					is_cloning=0;
					brick_release(brick);
				}

				t_block *block=brick->block;
				float *block_pos=block->pos;
				ctx_get_mouse_pos(C,mouse_pos);

				if(!is_vec_stored)
				{
					is_vec_stored=1;
					ctx_ui_menu_hide(C);
					vsub(vec,block_pos,mouse_pos);
				}
				else
				{
					ctx_ui_menu_hide(C);
					vadd(v,mouse_pos,vec);
					vcp(block_pos,v);
				}
			}
			// moving
			else
			{
				// release
				if(button_right==button_released)
				{
					is_vec_stored=0;
					brick_release(brick);
				}
				// move
				else
				{
					t_block *block=brick->block;
					float *block_pos=block->pos;
					ctx_get_mouse_pos(C,mouse_pos);

					if(!is_vec_stored)
					{
						is_vec_stored=1;
						ctx_ui_menu_hide(C);
						vsub(vec,block_pos,mouse_pos);
					}
					else
					{
						ctx_ui_menu_hide(C);
						vadd(v,mouse_pos,vec);
						vcp(block_pos,v);
					}
				
				}
			}

			break;

		case bm_cloning:

			if(!is_cloning)
			{

				t_block *clone_block=block_clone(brick->block);
				t_brick *clone_brick=clone_block->bricks->first->data;

				clone_brick->state.is_mouse_over=1;
				clone_block->state.is_mouse_over=0;
				clone_brick->mode=bm_moving;

				brick_release(brick);
				C->event->is_brick_transformed=1;

				is_cloning=1;
			}

			break;


		case bm_linking:
			
			// release linking
			if(button_left==button_released)
			{
				// connect
				if(C->ui->brick_in)
				{
					t_brick *brick_in=C->ui->brick_in;
					t_plug *plug_in=&brick_in->plug_in;
					if(!plug_in->src)
					{
						t_brick *brick_in = C->ui->brick_in;
						t_brick *brick_out = C->ui->brick_out;

						brick_in->cls->connect(brick_in,brick_out);
					}

				}

				// reset

				C->ui->brick_out=NULL;
				C->ui->brick_in=NULL;

				C->event->is_drawing=0;

				C->event->state=event_idle;
				brick->state.is_linking=0;

				// release
				brick_release(brick);

			}
			else
			{
				t_brick *brick_out=C->ui->brick_out;
				t_plug *plug_out=&brick_out->plug_out;

				// if plug out isn't yet plugged
				if(!plug_out->is_connected)
				{
					// init linking
					if(!brick->state.is_linking)
					{
						brick->state.is_linking=1;

						ctx_get_mouse_pos(C,mouse_pos);

						t_block *block=brick->block;

						float width = brick_get_width(brick);
						width+=brick->geom.height*2;

						float x=block->pos[0];
						float y=block->pos[1];

						float pos_x= x	+  width ;
						float pos_y= y + (brick->geom.block_pos*brick->geom.height) +(brick->geom.height/2);

						C->event->start_x=pos_x; 
						C->event->start_y=pos_y;


						C->event->is_drawing=1;
						C->event->state=event_linking;
					}
				}

				// else release
				else
				{
					brick->mode=bm_idle;
					C->event->is_brick_transformed=0;
				}
			}

			break;
	
		case bm_unlinking:
			brick->cls->disconnect(brick);
			break;

		case bm_triggering:
			brick->cls->trigger(brick);
			break;

		case bm_typing:
			break;

	}
	}
}

