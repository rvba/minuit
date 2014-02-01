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

	int tot_directories;
	char **directories;

	long data_size;
	int tot_line;

	FILE *file; // stream
	char *data; // content of file

	struct Lst *lines;
};

// FILE

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
void file_read(t_file *file);
void file_read_lines(t_file *file);
void file_write(t_file *file);
void file_data_add(t_file *file,char *data);
void file_open(t_file *file);
void file_close(t_file *file);
int file_init(t_file *file);
void file_free(t_file *file);

#ifdef __cplusplus
}
#endif

#endif
