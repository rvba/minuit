/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __OPS_H
#define __OPS_H

#include "context.h"

int set_draw_plug;

// OP_BUTTON

t_node *op_add_brick_operator_intern(
				int tot_in,
				int tot_out,
				const char type[],
				const char name[],
				t_node *menu,
				t_node *node,
				void (* func)(t_node *node)
			);

t_node *brick_add_operator(
				int tot_in,
				int tot_out,
				const char type[],
				const char name[],
				t_node *menu,
				t_node *node,
				void *data,
				void (* func)(t_brick *brick)
			);

t_node *brick_add_selector(
				char *name,
				t_node *menu,
				t_node *node_target,
				void *data_target,
				void (* func)(t_brick *brick),
				int selector_length
			);

t_node *	add_menu_process(void);
void 		op_add_menu_node(t_node *node); 
void 		op_add_menu_rc(t_node *node);
t_node *	brick_add_submenu(t_node *source_menu,t_node *submenu,char *name);

void 		op_menu_rc_new(t_node *node,const char name[]);
void 		op_menu_rc_submenu_add(t_node *node, t_node *menu_sub,char *name);

// OP_ADD

void *op_add_set(t_brick *brick);
void *op_new_set(const char *name);
void *		op_new_camera(const char *name);
t_node *	op_new_cube(const char *name);
t_node *	op_add_new_wire_cube(t_node *node);
void 		add_mn(void);
void *		op_add_mn(t_brick *brick);
void *		op_add_camera(t_brick *brick);
void *	 	op_add_camera_main(void);

void 		op_link_object_mesh(t_node *node_object,t_node *node_mesh);

void *		op_camera_rotate_xy(t_brick *brick);
void *		op_camera_rotate_z(t_brick *brick);
void *		op_menu(t_brick *brick);
void 		__op_brick_delete_node(t_brick *brick);
void *		op_brick_add(t_brick *brick);

void *		op_add_default(t_brick *brick);
void *		op_add_light(t_brick *brick);
void *		op_add_cube(t_brick *brick);
void *		op_add_wire_cube(t_brick *brick);
void *		op_add_uv_cube(t_brick *brick);
void *		op_add_uv_plane(t_brick *brick);
void *		op_add_triangle(t_brick *brick);
void *		op_add_quad(t_brick *brick);

// OP_MESH

void 		mesh_object_add(t_node *node,t_node *object);
void 		mesh_line_add(t_mesh *mesh,int *lines,int totline);
void 		mesh_line_cube_add(t_mesh *mesh);
void 		mesh_calc_vertex_array(t_mesh *mesh);
void 		mesh_build_vertex_array(t_mesh *mesh);
void 		mesh_do_quad_colors(t_mesh *mesh,int *col);
void 		mesh_do_tri_colors(t_mesh *mesh,int *col);
void 		mesh_do_quad_normals(t_mesh *mesh);
void 		mesh_do_tri_normals(t_mesh *mesh);
void 		mesh_recalc_normals(t_mesh *mesh);
void 		mesh_show_normals(t_mesh *mesh);
void 		mesh_show_vertices(t_mesh *mesh);
void 		mesh_show_colors(t_mesh *mesh);
void 		mesh_show_faces(t_mesh *mesh);
void 		mesh_show_uv(t_mesh *mesh);
void 		mesh_show(t_mesh *mesh);

t_node *	mesh_join(t_mesh **meshes,int tot_mesh);

int 		op_mesh_vert_copy(t_scene *sc,t_node *target,t_node *source);
int 		op_mesh_face_copy(t_scene *sc,t_node *target,t_node *source);
int 		op_mesh_copy(t_scene *sc,t_node *target,t_node *source);

void 		op_mesh_join_all(t_node *node);
void 		op_mesh_join_selected(t_node *node);
void 		op_mesh_apply(t_node *node);  

// OP_TEXTURE

t_node *	op_texture_add(const char *path);

// OP_ACTION

void 		op_switch_color(t_context *C);
void		op_set_color(t_context *C,int color);
void 		op_save(void);
int 		op_exit(void);
int 		op_quit(t_node *node);
void 		op_export(t_node *node);

void 		screen_start_record(void);
void 		screen_stop_record(void);
void 		screen_capture(const char name[]);
void 		screen_record(void);

// OP_DEBUG

void 		op_debug_mouse(void);
void 		op_debug_process(void);

// OP_UI

int 		op_ui_alphabet_switch(t_node *node);
void 		op_ui_switch_axis(void);


// OP_CAMERA

void 		op_3d_orientation(void);
t_camera *	op_camera_get_current(void);
void 		op_camera_view_top(t_camera *camera);
void 		op_camera_view_axo(t_camera *camera);
void 		op_camera_view_front(t_camera *camera);
void 		op_camera_view_left(t_camera *camera);
void 		op_camera_view_right(t_camera *camera);
void 		op_camera_ortho_rotate(t_camera *camera,int x,int y,int z);
void 		op_camera_switch_view(t_camera *camera);
void 		op_camera_switch_type(t_camera *camera);
void 		op_camera_change_speed(t_camera *camera);
void 		op_camera_translate_key(t_camera *camera,float x,float y);
void 		op_camera_switch_2d(t_context *C, t_camera *camera);
void 		op_camera_switch_3d(t_context *C, t_camera *camera);
void 		op_camera_update(t_context *C, t_camera *camera);
void 		op_camera_frustum_init(t_camera *camera);
void 		op_camera_reset(t_context *C, t_camera *camera);
void 		op_camera_reset_pos(t_camera *camera);
void 		op_camera_rotate(t_context *C, t_camera *camera, float dx, float dy);
void 		op_camera_translate(t_context *C, t_camera *camera);
void 		op_camera_zoom(t_context *C, t_camera *camera, int dir);
void 		op_camera_set_ortho_zoom(t_context *C, t_camera *camera, int i);
void 		op_camera_set_ortho_pan(t_context *C, t_camera *camera);

char *		render_null;
char *		render_camera;
char *		render_fbo;
char *		render_tiled;


// OP_NODE

int 		op_node_delete(t_node *brick_node);
int 		op_node_apply(t_node *brick_node);
int 		op_node_show(t_node *brick_node);
int 		op_node_transform(t_node *brick_node);
int 		op_mesh_update(t_node *node);
int 		op_apply(t_node *node);
int 		op_apply_all(void);

// OP_C

void 		op_init(t_context *C);
void 		op_brick_delete_node(t_node *n);

// OP_ADD_BRICK

t_lst *		get_target_list(t_context *C);

t_node *	add_brick_submenu(t_context *C,t_node *menu,t_node *submenu,const char *name);
t_node *	add_brick_submenu_contextual(t_context *C,t_node *menu,t_node *submenu,const char *name,t_node_type context);
t_node *	add_brick_selector(t_context *C,t_block *block,const char *name,void *data_target,int length);
t_node *	add_brick(t_context *C,t_block *block,const char *name,const char *type,const char *data_type,void *data_target);
t_node *	add_brick_slider_int(t_context *C,t_block *block,const char *name,void *data_target);
t_node *	add_brick_slider_float(t_context *C,t_block *block,const char *name,void *data_target);
t_node *	add_brick_slider_float_custom(t_context *C,t_block *block,const char *name,void *data_target,void *(*f)(t_brick *b));
t_node *	add_brick_slider_int_custom(t_context *C,t_block *block,const char *name,void *data_target,void *(*f)(t_brick *b));
t_node *	add_brick_label(t_context *C,t_block *block,const char *name);
t_node *	add_brick_switch(t_context *C,t_block *block,const char *name,void *data_target);
t_node *	add_brick_trigger(t_context *C,t_block *block,const char *name,void *(*f)(t_brick *b));

t_node *	add_trigger(t_context *C,const char *name,void*(* f)(t_brick *brick));
t_node *	add_trigger_always(t_context *C,const char *name,void*(* f)(t_brick *brick));
t_node *	add_clone(t_context *C);
t_node *	add_maths(t_context *C,const char *name);
t_node *	add_loop(t_context *C);
t_node *	add_switch(t_context *C,const char *name,void *data);

t_node *	add_switch_custom(t_context *C,const char *name,void *data,void *(* f)(t_brick *brick));
t_node *	add_label(t_context *C,const char *name);
t_node *	add_slider_float(t_context *C,const char *name,void *target_data);
t_node *	add_slider_int(t_context *C,const char *name,void *target_data);
t_node *	add_slider_int_custom(t_context *C,const char *name,void *target_data,void*(* f)(t_brick *b));
t_node *	add_slider_int_positive(t_context *C,const char *name,void *target_data);
t_node *	add_operator_double(t_context *C,const char *type);
t_node *	add_operator_single(t_context *C,const char *type,void *(*f)(t_brick *brick));
t_node *	add_multiplier(t_context *C,const char *type);
t_node *	add_slider_float_custom(t_context *C,const char *name,void *(*f)(t_brick *brick));
t_node *	add_slider_float_special(t_context *C,const char *name,void *(*f)(t_brick *brick));
t_node *	add_slider_int_special(t_context *C,const char *name,void *f(t_brick *brick));
t_node *	add_slider_char(t_context *C,const char *name,void *target_data);

t_node *	add_slider_object(t_context *C,const char *name);
t_node *	add_pointer_object(t_context *C,const char *name);
t_node *	add_slider_camera(t_context *C,const char *name);


t_node *	add_part_label(t_context *C,t_block *block,const char *name);
t_node *	add_part_slider_float(t_context *C,t_block *block,const char *name,void *data_target);
t_node *	add_part_trigger(t_context *C,t_block *block,const char *name,void *(*f)(t_brick *b));
t_node *	add_part_selector(t_context *C,t_block *block,const char *name,t_node *node);
t_node *	add_part_lst(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr);
t_node *	add_part_vlst(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr);
t_node *	add_part_pointer(t_context *C,t_block *block,t_data_type type,const char *name,void *ptr);
t_node *	add_loop_get(t_context *C);
t_node *	add_get(t_context *C);
t_node *	add_for(t_context *C);
t_node *	add_vector(t_context *C);
t_node *	add_stack(t_context *C);
t_node *	add_plusplus(t_context *C);

t_node *	add_const(t_context *C);
void *		op_const(t_brick *brick);


void 		option_save(t_context *C);
void 		load_option(t_context *C,t_scene *sc);
void *		find_register(const char *target,const char *name);


void 		op_add_global(t_context *C,t_block *block);
t_node *	add_pipe(t_context *C);
void 		*op_mod(t_brick *brick);
void *		op_float(t_brick *brick);
void *		op_int(t_brick *brick);
void op_3d_orientation(void);

t_lst *get_target_list(t_context *C);

#endif
