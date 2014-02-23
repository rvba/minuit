/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __FILE_H
#define __FILE_H

#include "util.h"
#include "common.h"

#define PATH_RELATIVE 1
#define PATH_ABSOLUTE 2
#define PATH_LOCAL 3

#define FILE_MAX_DIR 10

#ifdef __cplusplus
extern "C" {
#endif

struct Scene;
struct Lst;

typedef struct Line t_line;
typedef struct File t_file;
typedef struct Word t_word;

// WORD

struct Word
{
	int size;
	char *data;
};

// LINE

struct Line
{
	int size;
	char *data;
	struct Lst *words;
};

// FILE

struct File
{
	t_id id;

	char location[_PATH_];

	char path[_PATH_];
	char ext[_EXT_];

	int is_relative;
	int is_directory;
	int has_extention;
	int path_type;

	int tot_directories;
	char **directories;
	char dirs[FILE_MAX_DIR][_NAME_LONG_];
	char file_name[_NAME_LONG_];
	char dir_path[_PATH_];
	int exists;

	long data_size;
	int tot_line;
	int dir_count;

	FILE *file; // stream
	char *data; // content of file

	struct Lst *lines;
};

// FILE

int file_test( void);
int file_exists(t_file *file);
void file_set_location(t_file *file, const char *path);
void file_build_location(t_file *file);
int file_path_split(t_file *file);
void file_go_directory(t_file *file,char *name);
void file_go_backward(t_file *file);
int word_equal(t_word *word,const char *string);
void word_show(t_word *word);
void line_read_words(t_line *line);
void line_show(t_line *line);
t_line *line_new(void);
t_file *file_rebind(struct Scene *scene, void *ptr);
t_file *file_new(const char *path);
void file_free(t_file *file);
void file_show(t_file *file);

char *file_line_get( t_file *file, int p);

int file_read(t_file *file);
int file_read_lines(t_file *file);

int file_write( t_file *file, const char *data, int size);
//void file_write(t_file *file);

void file_data_add(t_file *file,char *data);
int file_open(t_file *file);
void file_close(t_file *file);
int file_init(t_file *file);
int _file_init( t_file *file);
void file_free(t_file *file);
t_file *file_access( const char *path);
int file_create( const char *path);

#ifdef __cplusplus
}
#endif

#endif
