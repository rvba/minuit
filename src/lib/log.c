/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "file.h"
#include "util.h"
#include "log.h"

t_file *LOG_FILE;

void log_init(void)
{
	/*
	t_file *file = file_new("minuit.log");
	file_open(file);
	fprintf(file->file,"LOG\n");
	file_close(file);
	LOG_FILE=file;
	*/
}

void __log__( int status, const char *fmt, ...)
{
	if(status)
	{
		t_file *file=LOG_FILE;
		file->file=fopen(file->location,"a");

		va_list ap;
		va_start(ap,fmt);
		vfprintf(file->file,fmt,ap);
		va_end(ap);

		file_close(file);
	}
}
