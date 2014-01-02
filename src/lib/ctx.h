/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __CTX_H
#define __CTX_H

struct Context;
struct Scene;
struct Node;
struct Brick;
struct Block;
struct Plug;
struct Lst;
struct Ui;
struct Event;

// CTX_EVENT

void 		ctx_event( struct Context *C);

// CTX_LOAD

void load_file(struct Context *C,const char *path);
void *ctx_get_pointer(void *ptr);
void rebind(struct Scene *sc,const char *type,const char *name,void **ptr);
void load_last(struct Context *C);

// CTX_SAVE

void save_to_file( struct Context *C);
void save_file(struct Context *C);
void save_file_increment(struct Context *C);


// CTX_KEYBOARD

void show_sets(struct Context *C);
void switch_sets(struct Context *C);
void keymap_main( int key);
void keymap_generic( int key);
void ctx_keyboard(struct Context *C);

// CTX_MOUSE

int is_mouse_over_brick(struct Context *C,struct Brick *brick);

int is_mouse_over_background(struct Context *C);
int is_mouse_over_link_out(struct Context *C,struct Brick *brick);
int is_mouse_over_link_in(struct Context *C,struct Brick *brick);

int is_mouse_over_plug(struct Context *C,struct Plug *plug);
int is_mouse_over(struct Context *C,int *c);
void ctx_set_mouse_over(struct Context *C);
void ctx_get_mouse_pos(struct Context *C,float *r);

void ctx_mouse( struct Context *C);
inline int ui_left_clic( struct Ui *ui);

// CTX_UI

void ctx_ui_freeze(struct Context *C);
void ctx_ui_switch_show_step(struct Context *C);
void ctx_ui_switch_show_states(struct Context *C);
void ctx_block_store(struct Context *C,struct Node *node);
void ctx_ui_exec(struct Context *C);
void ctx_ui_menu_hide(struct Context *C);
void ctx_ui_links_update(struct Context *C);


void ctx_ui_keyboard_set( struct Context *C, struct Event *event);
void ctx_ui_mouse_set( struct Context *C, struct Event *event);
void ctx_ui_mouse_motion( struct Context *C, int x, int y);
void ctx_ui( struct Context *C);
void ctx_ui_init( struct Context *C);

// CTX_SCENE

void ctx_scene_selection(struct Context *C, struct Node *node, int state);
void ctx_scene_clear_selections(struct Context *C);
void ctx_scene_set_selected(struct Context *C, struct Node *node);
void ctx_scene(struct Context *C);
struct Node *ctx_scene_hover( struct Context *C, t_data_type type);


// CTX_RENDER

void ctx_render(struct Context *C);
void ctx_render_scene(struct Context *C);

// CTX_LINKS.C

struct Lst *block_branch_src_get(struct Context *C,struct Block *block);
void ctx_links_term_cleanup(struct Context *C);
void block_branch_get(struct Lst *lst,struct Block *block);
void ctx_links_loop(struct Context *C);
void ctx_links_update(struct Context *C);
void ctx_links_reset(struct Context *C,struct Lst *lst);

struct Lst *ctx_links_lst_get(void);

// CTX_CAMERA.C

void ctx_camera(struct Context *C);

// CONTEXT.C

void context_store_action(struct Context *C,struct Node *node);
void context_read_buffer(struct Context *C); 
void context_update_background(struct Context *C); 


void exe_init(void);
void ctx_switch_record_video(struct Context *C);
void ctx_get_selection(struct Context *C);


// VIDEO

void ctx_render_video(struct Context *C);

// CTX_OPTION

void 		option_save(struct Context *C);
void 		load_option(struct Context *C,struct Scene *sc);

// CTX_GRAPH

void ctx_set_exec(struct Context *C);

// CTX_COMPUTE

void ctx_compute(struct Context *C);

void *ctx_set_compute(void *data);
void ctx_event_cleanup( struct Context *C);

#endif 
