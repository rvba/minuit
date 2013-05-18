/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/osmesa.h>
#include <unistd.h>
#include <pthread.h>
#include <jpeglib.h>
#include <png.h>
#include <sys/stat.h>
#include <sys/resource.h>

#define PI 3.14159655f

#define TIME_LIMIT 0.05
#define PROCESS_LIMIT 0.01
#define SOCKET_BUFFER 2024

#define _NAME_ 64
#define _NAME_LONG_ 128
#define _PATH_ 150
#define _EXT_ 10
#define _STATE_ 20
#define _LIST_ 10

#define BIN_MAX 10000

#ifdef WITH_LIBLO
#include "liblo.h"
#endif


typedef struct Clock t_clock;
typedef struct MN_Socket t_socket;
typedef struct Link t_link;
typedef struct Lst t_lst;
typedef struct Nameval Nameval;
typedef struct Image t_image;
typedef struct Buffer t_buffer;

// BUFFER

struct Buffer
{
	char name[_NAME_];
	char type[_NAME_];
	int size_buffer;
	int size_chunk;

	void *data;
};

// IMAGE

struct Image
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	int width;
	int height;
	int bpp; //byte per pixel
	GLenum format;

	unsigned char *data;
};

// LINK

struct Link
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	t_link *next;
	t_link *prev;
	t_link *parent;
	t_link *child;

	void *data;
};

// LST

struct Lst
{
	int id;
	int id_chunk;
	short users;
	char name[_NAME_];

	t_link *first;
	t_link *last;
	t_link *current;

	int tot;
};

// SOCKET

struct MN_Socket
{
	int socket;
	int sockfd;
	int newsockfd;
	int portno;
	char buffer[SOCKET_BUFFER];

	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	struct hostent *server;
	socklen_t clilen;

	void (* connect)(t_socket *socket,int port);
	void (* print)(char *msg);
};


// CLOCK

struct Clock
{
	float freq;
	int factor;
	double delta;
	float limit; 
	struct timeval before; 
	struct timeval now;

	int framerate;
	struct timeval frame_before; 
	struct timeval frame_now;

	struct timeval start;
	unsigned long start_time; //milliseconds

	unsigned int msec;
	int sec;
	int min;
};


// NAMEVAL

struct Nameval
{
	char *name;
	int value;
};

// UTIL

void set_name(char *dst,const char *src);
void set_name_long(char *dst,const char *src);
void set_path(char *dst,const char *src);
inline int is(const char a[],const char b[]);
void clock_init(t_clock *clock);
void clock_free(t_clock *clock);
t_clock *clock_new(void);

// IMG

void image_free(t_image *image);
t_image *image_new(const char *name);
t_image *img_read_jpg(char* Name);
void img_save_jpg(int width,int height,const char name[]);
void img_save_jpg_highres(int width,int height,const char name[],unsigned char *image);
t_image *img_read_png(char *filename);
int img_save_png(int alpha,int width,int height,unsigned char *bitmap, const char name[]);
int img_save_png_hd(int alpha,int width,int height, const char name[],unsigned char *bitmap);

// SOCKET

int socket_listen(t_socket *sock);
void socket_connect(t_socket *socket,int port);
t_socket *socket_new(void);

// SYSTEM

void sys_get_cd(const char *cmd);
int sys_get_hostname(char* data);

// CLIENT

void slave(void);
int tcp_client(void);

// LST

t_link *lst_link_find_by_name(t_lst *lst, const char *name);
void lst_link_delete_by_name(t_lst *lst, const char *name);

void 		lst_show(t_lst *lst);
t_link *	lst_add(t_lst *lst,void *data,const char *name);
void 		lst_add_lst(t_lst *dst, t_lst *src);
void		lst_link_remove(t_lst *lst,t_link *link);
void 		lst_delete_all(t_lst *lst);
void 		lst_link_delete(t_lst *lst,t_link *link);
t_lst *		lst_copy(t_lst *lst);
t_lst *		lst_new(const char *name);
void 		lst_free(t_lst *lst);
void 		lst_cleanup(t_lst *lst);
void 		lst_push_back(t_lst *lst,t_link *link);

void lst_remove_by_ptr(t_lst *lst,void *ptr);
void 		link_free(t_link *link);

t_link *link_new(const char *name);
void link_free(t_link *link);


// STRING

void s_raw_cp(char *src,char *dst);
char *s_allocate(const char *data);
void s_truncate_after(char letter,char *target,char *src);
void s_convert_newline_endline(char *target,char *src);
void s_remove_newline(char *target,char *src);
void s_write_time(char *string);
int s_append(char *dest,char *src,int pos);
void s_store(char *target,char source,int pos);

// TERM

int u_read_int(void);
char u_read_char(void);
void print_args(int argc,char **argv);

// VECTOR

void calc_normals(float *v1,float *v2,float *v3,float *result);
inline void vprint3i(int *v,char end);
inline void vprint3f(float *v,char end);
inline float *vnew(float x,float y,float z);
inline void vprint(float v[3]);
inline void vcp(float t[3],float s[3]);
inline void vcp3f(float t[3],float s[3]);
inline void vcpi(int t[3],int s[3]);
inline void vset3i(int v[3],int a,int b,int c);
inline void vcp3i(int t[3],int s[3]);
inline void vset4i(int *v,int a,int b,int c,int d);
inline void vset3f(float *v,float a,float b,float c);
inline void vset4f(float *v,float a,float b,float c,float d);
inline void vseti(int v[3],int a,int b,int c);
inline void vset(float v[3],float a,float b,float c);
inline void vsub(float r[3],float a[3],float b[3]);
inline void vadd(float r[3],float a[3],float b[3]);
inline void vmul(float v[3],float i);
inline void vcross(float r[3],float a[3],float b[3]);
inline float vdot(float a[3],float b[3]);
inline float vmag(float v[3]);
inline void vnormalize(float v[3]);
inline void cnorm(float c[3],float o[3]);
inline void cconv(float c[3],int o[3]);

// VOLUMES

int mesh_cube_lines[24];
int mesh_cube_tot_lines;
float cube_verts[8*3];
int cube_quads[6*4];
float vertices[72]; 
unsigned int faces[24];
float quad_vert[12]; 
int uvlst[48];
unsigned quad_face[4];
float plane_v[12];
unsigned int plane_f[4];
int plane_uv[8];

// NET

int net_connect(void);

// CALC

float calc_normalize(int n,int range);
float *calc_circle(float center[3],double radius,int divisions);

// RAND

int u_randn(int n);
int u_randrange(int start,int end);

// SORT

int u_lookup(char *word,char *array[]);



inline void set_float(void *_ptr,float i);
inline void set_int(void *_ptr,int i);

inline void flow_int_int(void *_dst, void *_src);
inline void flow_int_float(void *_dst, void *_src);
inline void flow_float_float(void *_dst, void *_src);
inline void flow_float_int(void *_dst, void *_src);

inline void negate_int(void *_dst);
inline void negate_float(void *_dst);


inline int drf_int(void *ptr);
inline int drf_uint(void *ptr);
inline float drf_float(void *ptr);
inline char drf_char(void *ptr);
inline char *drf_string(void *ptr);

void sys_git_get(void);

void img_save_video(int width, int height, const char *name,unsigned char *image);

size_t get_memory_usage(void);

#endif
