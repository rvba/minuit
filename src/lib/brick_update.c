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
#include "action.h"
#include "ctx.h"
#include "app.h"
#include "event.h"
#include "term.h"
#include "ui.h"
#include "dict.h"
#include "list.h"
#include "event.h"
#include "brick.h"
#include "block.h"

#include "graph.h"
#include "set.h"

int is_vec_stored=0;
float v[3];
float vec[3];

// CONNECT

void cls_brick_connect(t_brick *brick_in ,t_brick *brick_out)
{
	t_plug *plug_brick_in = &brick_in->plug_intern;
	t_plug *plug_brick_out = &brick_out->plug_intern;

	plug_brick_in->cls->connect(mode_in, plug_brick_in, plug_brick_out);
	plug_brick_out->cls->connect(mode_out, plug_brick_out, plug_brick_in);
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

	t_plug *plug_intern_in = &brick_in->plug_intern;
	t_plug *plug_intern_out = &brick_out->plug_intern;

	plug_intern_in->cls->disconnect(mode_in , plug_intern_in);
	plug_intern_out->cls->disconnect(mode_out ,plug_intern_out);

	t_block *block_self = brick_in->block;
	t_block *block_target = brick_out->block;

	// Split Graph
	block_graph_split(block_self,plug_in,block_target,plug_target);

	// change modes
	self->mode=bm_idle;
	brick_out->mode=bm_linking;

	// keep out
	C->ui->brick_out=brick_out;
	// remove in
	C->ui->brick_in=NULL;
}

// MOUSE

int brick_release_cloning(t_brick *brick)
{
	t_context *C=ctx_get();
	if(C->app->mouse->button_left == button_released) return 1;
	else return 0;
}

int brick_start_cloning(t_context *C,int mouse_over)
{
	if(
		mouse_over
		&& !C->event->ui.is_menu_show
		&& (C->app->mouse->button_left == button_pressed)
		&& (C->app->keyboard->ctrl || C->app->keyboard->shift)
		)

		return 1;
	else 
		return 0;
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
	}
	else
	{
		if(brick->action) brick->action(brick);
	}

	brick_set_updated(brick);

}

void cls_brick_trigger_switch(t_brick *brick)
{
	brick->action(brick);
	brick_set_updated(brick);
}

void cls_brick_trigger_selector(t_brick *brick)
{
	brick->action(brick);
	brick_set_updated(brick);
}

void cls_brick_trigger_menu(t_brick *brick)
{
	brick->action(brick);
	brick_set_updated(brick);
}

void cls_brick_trigger_operator(t_brick *brick)
{
	brick->action(brick);
	if(brick->mode == bm_triggering)
		brick_release(brick);
	brick_set_updated(brick);
}

void cls_brick_trigger_vlst(t_brick *brick)
{
	brick->action(brick);
	brick_set_updated(brick);
}

void cls_brick_trigger_lst(t_brick *brick)
{
	brick->action(brick);
	brick_set_updated(brick);
}


void cls_brick_trigger_generic(t_brick *brick)
{
	t_context *C = ctx_get();

	/*
	if(brick->state.use_loops == 0)
	{
		int frame = C->app->frame;

		if(brick->state.frame_loop != frame)
		{
			brick->state.frame_loop = frame;
		}
	}
	*/

	t_plug *plug_in=&brick->plug_in;
	t_plug *plug_out = &brick->plug_out;

	// get data from parent 
	if(plug_in->state.is_connected || plug_out->state.is_connected)
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

	brick_set_updated(brick);
}

void cls_brick_trigger_action_default(t_brick *brick)
{
	brick->action(brick);
	brick_set_updated(brick);
}

// RELEASE

void brick_release(t_brick *brick)
{
	t_context *C = ctx_get();
	brick->mode=bm_idle;
	C->event->is_brick_transformed=0;
	C->ui->brick_selected=NULL;
}

// REMOVE

void brick_remove(t_dict *args)
{
	t_context *C = ctx_get();

	t_brick *brick = dict_pop_data(args,"brick");
	t_block *block = brick->block;

	if(!block_is_connected("in",block) && !block_is_connected("out",block))
	{
		//t_lst *lst = get_target_list(C);
		t_set *set = get_current_set(C);
		t_lst *lst = set->blocks;

		list_remove_by_ptr(lst,block);
		scene_struct_delete(C->scene,block);
	}
}


// UPDATE


void cls_brick_update(t_brick *brick)
{
	t_context *C = ctx_get();
	t_brick_mode mode = brick->mode;

	t_plug *plug_in = &brick->plug_in;
	t_plug *plug_out = &brick->plug_out;

	int button_left=C->app->mouse->button_left;
	int button_right=C->app->mouse->button_right;
	int mouse_over = is_mouse_over_brick(C,brick);
	int brick_clic=0;

	float mouse_pos[3];
	vset(mouse_pos,0,0,0);


	if(	 mouse_over && button_left==button_pressed
		&& (brick->state.is_mouse_over_plug_in==0)
		&& (brick->state.is_mouse_over_plug_out==0)
		)
		
		brick_clic=1;

	// IDLE

	if(!brick->state.is_idle)
	{
		if(C->app->mouse->button_left==button_released)
		{
			brick->state.is_idle=0;
		}
	}
		
	// DELETE

	if(mouse_over && C->event->brick_delete) 
	{
		C->event->brick_delete = 0;

		t_action *action = action_new("action");

		action->act = brick_remove;

		t_node *node_dict = dict_add("args");
		t_dict *dict = node_dict->data;
		action->args = dict;

		dict_symbol_add(action->args,"brick",dt_null,brick);

		exe_add_action(action);
	}

	// PLUGS
	t_event *event = C->event;

	if(mouse_over)
	{
		     if(event->switch_plug_in_flow_in)		plug_in->state.flow_in = switch_int(plug_in->state.flow_in);
		else if(event->switch_plug_in_open_in)		plug_in->state.open_in = switch_int(plug_in->state.open_in);
		else if(event->switch_plug_in_follow_in)	plug_in->state.follow_in  = switch_int(plug_in->state.follow_in);
		else if(event->switch_plug_in_flow_out)		plug_in->state.flow_out  = switch_int(plug_in->state.flow_out);
		else if(event->switch_plug_in_open_out)		plug_in->state.open_out  = switch_int(plug_in->state.open_out);
		else if(event->switch_plug_in_follow_out)	plug_in->state.follow_out  = switch_int(plug_in->state.follow_out);

		else if(event->switch_plug_out_flow_in)		plug_out->state.flow_in  = switch_int(plug_out->state.flow_in);
		else if(event->switch_plug_out_open_in)		plug_out->state.open_in  = switch_int(plug_out->state.open_in);
		else if(event->switch_plug_out_follow_in)	plug_out->state.follow_in  = switch_int(plug_out->state.follow_in);
		else if(event->switch_plug_out_flow_out)	plug_out->state.flow_out  = switch_int(plug_out->state.flow_out);
		else if(event->switch_plug_out_open_out)	plug_out->state.open_out  = switch_int(plug_out->state.open_out);
		else if(event->switch_plug_out_follow_out)	plug_out->state.follow_out  = switch_int(plug_out->state.follow_out);

		else if(event->switch_brick_debug)		
		{
			if(brick->state.debug)
			{
				term_log("%s: debug off",brick->name);
				brick->state.debug = 0;
			}
			else
			{
				term_log("%s: debug on",brick->name);
				brick->state.debug = 1;
			}
		}
	}

	// Debug

	if(brick->state.is_mouse_over && brick->state.debug)
	{
		t_plug *plug = &brick->plug_intern;
		term_log("%d) brick %s",C->app->frame, brick->name);
		plug_debug(plug);
	}


	// MODES

	if(!C->event->ui.pan && !C->event->camera_rotation && !C->event->ui.zoom)
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
					// START CLONING
					if(brick_start_cloning(C,mouse_over))
					{
						// start moving
						if(C->event->brick_cloning_start)
						{
							C->event->is_brick_transformed=1;
							C->ui->brick_selected=brick;
							brick->mode=bm_moving;
						}
						// start cloning
						else
						{
							if(!C->event->ui.is_menu_mouse_show)
							{
								C->event->is_brick_transformed=1;
								C->ui->brick_selected=brick;
								brick->mode=bm_cloning;

								// Copy or Clone
								if(C->app->keyboard->shift)
									C->event->brick_copying = 1;
								else if(C->app->keyboard->ctrl)
									C->event->brick_cloning = 1;
							}
						}
					}
					// START MOVING
					else if(mouse_over && button_right==button_pressed) 
					{
						if(!C->event->ui.is_menu_mouse_show)
						{
							C->event->is_brick_transformed=1;
							C->ui->brick_selected=brick;
							brick->mode=bm_moving;
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
						}
					}

					// START UNLINKING
					else if(is_mouse_over_plug(C,&brick->plug_in) && button_left==button_pressed)
					{
						C->ui->brick_in=brick;

						if(plug_in->state.is_connected)
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
					else if(mouse_over && (button_left == button_pressed) && C->app->keyboard->alt)
					{
						if(!C->event->ui.is_menu_mouse_show)
						{
							C->event->is_brick_transformed=1;
							C->ui->brick_selected=brick;
							brick->mode=bm_typing;
							C->event->ui.typing_start = 1;
						}
					}

					// START TRIGGER 
					else 
					{
						if(brick->state.is_clicable)
						{
							if(brick_clic)
							{
								int trigger;

								if(plug_in->state.is_connected)
								{
									if(plug_in->state.flow_in == 0)
										trigger = 1;
									else
										trigger = 0;
								}
								else
								{
									trigger = 1;
								}

								if(trigger && brick->state.use_brick_blocking)
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
				if(C->event->brick_cloning_start)
				{
					// release
					if(brick_release_cloning(brick))
					{
						is_vec_stored=0;
						C->event->brick_cloning_start=0;
						C->event->brick_copying = 0;
						C->event->brick_cloning = 0;
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

						if(block->state.is_moveable)
						{
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
				}

				break;

			case bm_cloning:

				if(!C->event->brick_cloning_start)
				{
					t_block *clone_block;

					if(C->event->brick_cloning)
						clone_block = block_dupli(brick->block);
					else
						clone_block = block_copy(brick->block);

					t_brick *clone_brick=clone_block->bricks->first->data;

					clone_brick->state.is_mouse_over=1;
					clone_block->state.is_mouse_over=0;
					clone_brick->mode=bm_moving;

					brick_release(brick);
					C->event->is_brick_transformed=1;

					C->event->brick_cloning_start=1;
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
					if(!plug_out->state.is_connected)
					{
						// init linking
						if(!brick->state.is_linking)
						{
							brick->state.is_linking=1;

							ctx_get_mouse_pos(C,mouse_pos);

							t_block *block=brick->block;

							float width = brick_get_width(brick);
							//width+=brick->geom.height*2;

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

				//release
				if(C->event->ui.typing_start && C->event->ui.typing_end)
				{
					brick->mode=bm_idle;
					C->event->is_brick_transformed=0;
					C->event->is_typing=0;
					bzero(&C->event->buffer_char[0],20);
					C->event->buffer_char_counter=0;
					C->event->ui.typing_start = 0;
					C->event->ui.typing_end = 0;
				}
				// change data
				else
				{
					t_plug *plug = &brick->plug_intern;

					if(plug->data_type==dt_int)
					{
						int *dt=plug->data;
						*dt=atoi(C->event->buffer_char);
					}
					else if(plug->data_type==dt_float)
					{
						float *dt=plug->data;
						*dt=atof(C->event->buffer_char);
					}
				}

				break;
		}
	}
}

