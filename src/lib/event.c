/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"

t_event *event_new(void)
{
	t_event *event=(t_event*)malloc(sizeof(t_event));

	bzero(&event->ui,sizeof(t_state_event_ui));

	event->update_connections=0;

	event->state=event_idle;
	event->is_background_button_left_pressed=0;
	event->is_background_button_left_released=0;

	event->ui.is_menu_pressed=0;
	event->ui.is_menu_released=0;
	event->ui.is_menu_show=0;
	event->ui.is_menu_mouse_show=0;
	event->ui.is_menu_fixed = 0;
	event->ui.typing_start = 0;
	event->ui.typing_end = 0;
	event->ui.use_point_global_width = 1;
	event->ui.use_scale = 1;

	event->ui.pan = 0;
	event->ui.zoom = 0;
	event->ui.pan_x = 0;
	event->ui.pan_y = 0;
	event->ui.draw_in_loop = 0;

	event->is_selection=0;

	event->is_brick_transformed=0;
	event->is_brick_pressed=0;
	event->is_brick_moving=0;
	event->is_mouse_over_brick=0;
	event->is_typing=0;
	event->action=NULL;
	event->with_texture=1;
	event->with_polygon_offset=EVENT_WITH_POLYGON_OFFSET;
	event->with_clear=1;
	event->with_normal=1;
	event->with_light=1;
	event->with_depth=1;
	event->with_face=1;
	event->with_point=0;
	event->with_point_id=0;
	event->with_highlight=WITH_HIGHLIGHT;
	event->sketch_with_scale=SKETCH_WITH_SCALE;
	event->with_blend=1;

	event->is_drawing=0;

	event->color[0] = 0;
	event->color[1] = 0;
	event->color[2] = 0;

	event->rec=0;
	event->rec_save=0;
	event->recording=0;
	event->show_term=CTX_SHOW_TERM;

	event->debug_select=CTX_DEBUG_SELECT;
	event->debug_mouse=CTX_DEBUG_MOUSE;
	event->debug_process=DEBUG_PROCESS;
	event->debug_console=DEBUG_CONSOLE;
	event->debug_terminal=DEBUG_TERMINAL;
	event->debug_loop = DEBUG_LOOP;

	event->free_scene=FREE_SCENE;

	vset(event->color_front,1,1,1);
	vset(event->color_back,0,0,0);
	event->color_buffer=0;

	event->last_frame=0;

	bzero(&event->buffer_char[0],20);
	event->buffer_char_counter=0;
	
	event->callback=NULL;
	event->standby_int=0;
	event->standby_char='\0';
	event->standby_string=NULL;
	event->video_record = 0;
	event->video_store = 0;
	event->video_init = 0;
	event->video_frame =0;
	event->video_frame_number =0;
	event->video_stop = 0;
	event->video_stop_call = 0;
	event->video_frames_in_memory = 0;

	event->brick_delete = 0;

	event->switch_plug_in_flow_in = 0;
	event->switch_plug_in_flow_out = 0;
	event->switch_plug_in_open_in = 0;
	event->switch_plug_in_open_out = 0;
	event->switch_plug_in_follow_in = 0;
	event->switch_plug_in_follow_out = 0;

	event->switch_plug_out_flow_in = 0;
	event->switch_plug_out_flow_out = 0;
	event->switch_plug_out_open_in = 0;
	event->switch_plug_out_open_out = 0;
	event->switch_plug_out_follow_in = 0;
	event->switch_plug_out_follow_out = 0;

	event->switch_brick_debug = 0;

	event->camera_rotation = 0;
	event->brick_copying = 0;
	event->brick_cloning = 0;
	event->brick_cloning_start = 0;

	event->loop_step = 0;
	event->load_error = 0;

	return event;
}
