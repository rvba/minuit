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

#include "base.h"
#include "umber.h"

#ifdef __cplusplus
extern "C" {
#endif

#define APP_KEY_ESC  27
#define APP_KEY_ENTER 13
#define APP_KEY_TAB 9
#define APP_KEY_BACK 8
#define WIN_X 0  
#define WIN_Y 0
#define APP_DEBUG 0
#define SET_FULLSCREEN  1

#define APP_FILENAME_SAVE 1

struct Lst;
struct Clock;
struct File;

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
	
typedef struct Mouse
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

	int event;
	int motion;
	int motion_passive;
	int button;

	struct Clock *clic_clock;
	struct Clock *release_clock;
}t_mouse;

// WINDOW

typedef struct MNWindow
{
	int change;
	int width;
	int height;
	int fullscreen;
	int width_def;
	int height_def;
	int viewport_width;
	int viewport_height;
}t_window;

// KEYBOARD

typedef struct Keyboard
{
	unsigned char key_pressed;
	int shift;
	int ctrl;
	int alt;
	int special;
	int modifier;
}t_keyboard;

// APP

typedef struct App
{
	int argc;
	char **argv;
	char name[_NAME_];
	char version[_NAME_];
	char app_name[_NAME_];
	char path_home[_PATH_];
	char path_current[_PATH_];
	char path_file[_PATH_];
	char filename[_NAME_LONG_];
	
	void (*main_func)(void);
	void (*x_func)(int argc, char **argv, const char *name);
	void (*x_init)(int argc, char **argv, const char *name);

	int off_screen; //off screen rendering
	int with_glut; // use GLUT
	int debug_keyboard;

	int client;
	int slave;
	int osc_server;
	int osc_client;
	int osc_port;
	
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
	struct Clock *clock;

	int load_file;
	int loaded_file;

	struct Lst *video_frames;
	struct Lst *video_frames_swap;

	int video_offset;
	int video_build;
	int video_limit;

	int quit;
}t_app;

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
void app_gl_move( int x, int y);
void app_screen_switch_fullscreen(t_app *app);
void app_screen_set_fullscreen(t_app *app,int state);

// APP.X

void glx_win(t_app *app);
void glx_main_loop(void);

// APP

void app_print_args( t_app *app);
const char *app_get_arg( t_app *app, int pos);
int app_check_arg( t_app *app, const char *arg);
char *app_get_file_path( t_app *app, int type);

void app_gl_display(void);
void app_swap(t_app *app);
void app_update_viewport(t_app *app);

void app_set_offscreen( t_app *app);
//void app_set_dsp_func(t_app *app,void(*dsp_func)(void));
//void app_set_main_func(t_app *app,void(*func)(void));

int app_free(t_app *app);
void app_launch(t_app *app);
t_app *app_new(int argc,char **argv);
void app_init(t_app *app, const char *name);

void app_clock(t_app *app);
t_app *app_get(void);

#ifdef WITH_GIT
extern const char *git;
#endif

#ifdef HAVE_SDL

int sdl_init( int argc, char **argv);
void sdl_mainloop( t_app *app);
void sdl_swap( t_app *app);


#endif

int app_have_glew( void);

#ifdef __cplusplus
}
#endif


#endif
