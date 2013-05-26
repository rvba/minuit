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


void exe_init(void);

void ctx_switch_record_video(t_context *C);

void ctx_get_selection(t_context *C);


// VIDEO

void ctx_render_video(t_context *C);


#endif 
