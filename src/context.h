/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __CONTEXT_H
#define __CONTEXT_H

#include "app.h"
#include "sketch.h"
#include "term.h"
#include "draw.h"
#include "engine.h"
#include "event.h"
#include "camera.h"
#include "mode.h"
#include "server.h"
#include "dict.h"

#define ZOOM 1
#define PAN_X 0
#define PAN_Y 0

#define WITH_HIGHLIGHT 0
#define GLFONT 1
#define FREE_SCENE 0
#define DEBUG_PROCESS 0
#define DEBUG_CONSOLE 1
#define DEBUG_TERMINAL 0
#define DEBUG_LOOP 0

#define CTX_SHOW_TERM 1
#define CTX_FIXED_BUTTON 0
#define CTX_DEBUG_MOUSE 0
#define CTX_DEBUG_SELECT 0
#define EVENT_WITH_POLYGON_OFFSET 0

#define SKETCH_WITH_SCALE 1

typedef struct Context t_context;
typedef struct Option t_option;
typedef struct Action t_action;

// CONTEXT

struct Context
{
	t_app *app;
	t_mode *mode;
	t_engine *engine;
	t_scene *scene;
	t_event *event;
	t_ui *ui;
	t_draw *draw;
	t_skt *skt;
	t_term *term;
	t_lst *terms;
	t_server *server;
};

struct Action
{
	char name[_NAME_];

	void (* act)(t_dict *args);
	t_dict *args;
};

struct Option
{
	int id;
	short users;
	char name[_NAME_];

	// app

	int window_width_def;
	int window_height_def;

	int window_width;
	int window_height;

	float timer_add_low;

	// skecth

	float skt_color[3];
	float skt_point_color[3];
	int skt_line_width;
	int skt_point_width;
	float skt_point_size;
	int skt_with_scale;
	float skt_scale;

	// draw

	int draw_background_color[4];
	float draw_front_color[4];
	int draw_with_ui;
	int draw_with_clear;
	int draw_with_normal;
	int draw_with_texture;
	int draw_with_material;
	int draw_with_point;
	int draw_with_point_id;
	int draw_with_face;
	int draw_with_highlight;
	int draw_with_light;
	int draw_with_depth;
	int draw_with_polygon_offset;
	int draw_with_alpha;
	int draw_with_grid;
	int draw_img_with_alpha;
	int draw_divx;
	int draw_divy;
	int draw_show_axis_world;
	int draw_lights;

	int fullscreen;
	int color;

};

// OPTION

void option_free(t_option *option);
t_option *option_new(const char *name);
t_option *option_rebind(t_scene *scene, void *ptr);

// ACTION

void action_free(t_action *action);
t_action *action_new(const char *name);
void exe_add_action(t_action *action);


// CTX_LOAD

int check_ok;
int check_op_ok;
void check_init(const char *type,const char *name);
void check_check(const char *type,const char *name);
int check(t_node *node,const char *name_data,const char *name_ptr);
void load_file(t_context *C,const char *path);
void *ctx_get_pointer(void *ptr);
void rebind(t_scene *sc,const char *type,const char *name,void **ptr);
void load_last(t_context *C);

// CTX_SAVE

void save_file(t_context *C);
void save_file_increment(t_context *C);


// CTX_KEYBOARD

void switch_sets(t_context *C);
void keymap_main(unsigned char key);
void keymap_generic(unsigned char key);
void ctx_keyboard(t_context *C);

// CTX_MOUSE

int is_mouse_over_brick(t_context *C,t_brick *brick);

int is_mouse_over_background(t_context *C);
int is_mouse_over_link_out(t_context *C,t_brick *brick);
int is_mouse_over_link_in(t_context *C,t_brick *brick);

int is_mouse_over_plug(t_context *C,t_plug *plug);
int is_mouse_over(t_context *C,int *c);
void ctx_set_mouse_over(t_context *C);
void ctx_get_mouse_pos(t_context *C,float *r);

// CTX_UI

void ctx_ui_freeze(t_context *C);
void ctx_ui_switch_show_step(t_context *C);
void ctx_ui_switch_show_states(t_context *C);
void ctx_block_store(t_context *C,t_node *node);
void ctx_ui_exec(t_context *C);
void ctx_ui_menu_hide(t_context *C);
void ctx_ui_links_update(t_context *C);
void ctx_ui(t_context *C);

// CTX_SCENE

void ctx_scene_clear_selections(t_context *C);
void ctx_scene_set_selected(t_context *C,void *data);
void ctx_scene(t_context *C);

// CTX_PROCESS

void *ctx_compute(void *data);
void *ctx_thread_main(void *data);
void ctx_thread_init(t_context *C);
t_process *process_new(char *name,void*(* func)(void *data));
t_process *process_add(t_context *C,char *name,void*(* func)(void *ptr));
void process_launch(t_process *process);

// CTX_RENDER

void ctx_render(t_context *C);
void ctx_render_scene(t_context *C);

// CTX_LINKS.C

t_lst *block_branch_src_get(t_context *C,t_block *block);
void ctx_links_term_cleanup(t_context *C);
void block_branch_get(t_lst *lst,t_block *block);
void ctx_links_loop(t_context *C);
void ctx_links_update(t_context *C);
void ctx_links_reset(t_context *C,t_lst *lst);

t_lst *ctx_links_lst_get(void);

// CTX_CAMERA.C

void ctx_camera(t_context *C);

// CONTEXT.C

void context_store_action(t_context *C,t_node *node);
void context_read_buffer(t_context *C); 
void context_update_background(t_context *C); 

t_module *ctx_module_get(t_context *C,char *name);
void ctx_module_add(t_context *C,char *name,void *data);

// INIT

t_context *ctx_get(void);
t_context *ctx_init(int argc,char **argv);
void exe_init(void);
void brick_remove(t_dict *args);

void ctx_switch_record_video(t_context *C);

void ctx_get_selection(t_context *C);


// VIDEO

void ctx_render_video(t_context *C);

#endif
