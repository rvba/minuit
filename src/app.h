/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef _APP_H
#define _APP_H

#include "util.h"
#include "file.h"

#define APP_KEY_ESC  27
#define APP_KEY_ENTER 13
#define APP_KEY_TAB 9
#define APP_KEY_BACK 8
#define WIN_X 0  
#define WIN_Y 0
#define APP_DEBUG 0
#define SET_FULLSCREEN  1

#define GIT 128

struct Lst;

typedef struct App t_app;
typedef struct Mouse t_mouse;
typedef struct Window t_window;
typedef struct Keyboard t_keyboard;

enum BUT
{
	button_clic,
	button_clic_db,
	button_pressed,
	button_released,
	button_idle,
};

enum WHEEL
{
	wheel_up,
	wheel_down,
	wheel_idle,
};

typedef enum MouseButton
{
	mouse_left,
	mouse_right,
	mouse_middle,
	mouse_wheel,

}MouseButton;

// MOUSE
	
struct Mouse
{
	int x; // current x 
	int y; // curent y

	int last_x; // last mouse clic position 
	int last_y;

	int delta_x; // x - last_x
	int delta_y;

	int dx; // instant delta (frame)
	int dy;

	int sign_x; // direction of dx 
	int sign_y;

	int button_left; 
	int button_middle;
	int button_right;
	int wheel;

	int dbclic;
	int lgclic;

	int is_moving;
	int button_left_is_ready;

	t_clock *clic_clock;
	t_clock *release_clock;
};

// WINDOW

struct Window
{
	int change;
	int width;
	int height;
	int fullscreen;
	int width_def;
	int height_def;
	int viewport_width;
	int viewport_height;
};

// KEYBOARD

struct Keyboard
{
	unsigned char key_pressed;
	int shift;
	int ctrl;
	int alt;
	int special;
};

// APP

struct App
{
	int argc;
	char **argv;
	char name[_NAME_];

	char git[GIT];
	
	void (*main_func)(void);

	int off_screen; //off screen rendering
	int with_glut; // use GLUT
	int debug_keyboard;

	int client;
	int slave;
	
	int frame;
	float timer;
	float timer_add;
	float timer_add_low;
	int timer_count;
	int loop;
	int buffer; // single/double

	t_mouse  *mouse;
	t_window *window;
	t_keyboard *keyboard;
	t_clock *clock;

	int load_file;
	int loaded_file;
	char *file_path;

	t_file *file;
	struct Lst *video_frames;
	struct Lst *video_frames_swap;

	int video_offset;
	int video_build;
	int video_limit;
};

int app_get_frame(void);

// KEYBAORD

void app_gl_special(int key,int x,int y);
void app_gl_keyboard(unsigned char key,int x,int y);
void app_gl_keyboard_special(t_app *app);
t_keyboard *keyboard_new(void);

// MOUSE

void set_mouse_delta(t_app *app,int x,int y);
void app_mouse_store(t_app *app,int x,int y);
void app_mouse_reset(t_app *app);
void app_gl_mouse(int brick,int state,int x,int y);
void app_gl_passive_motion(int x,int y);
void app_gl_motion(int x,int y);
t_mouse *mouse_new(void);

// PROCESS

void app_sleep(t_app *app);
void app_gl_idle(void);

// WINDOW

t_window *window_new(int with_glut);
void app_gl_reshape(int width,int height);
void app_screen_switch_fullscreen(t_app *app);
void app_screen_set_fullscreen(t_app *app,int state);

// APP.X

void glx_win(t_app *app);
void glx_main_loop(void);

// APP

void app_gl_display(void);
void app_swap(t_app *app);
void app_update_viewport(t_app *app);

void app_set_dsp_func(t_app *app,void(*dsp_func)(void));
void app_set_main_func(t_app *app,void(*func)(void));

int app_free(t_app *app);
void app_launch(t_app *app);
t_app *app_new(int argc,char **argv);
void app_init(t_app *app);

void app_clock(t_app *app);
t_app *app_get(void);

#ifdef WITH_GIT
extern const char *git;
#endif


#endif
