/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "file.h"
#include "base.h"
#include "log.h"

//t_file *LOG_FILE = NULL;
t_file *FILE_READ = NULL;
t_file *FILE_WRITE = NULL;

t_file *log_init( const char *path)
{
	t_file *file = NULL;
	if(file_create( path))
	{
		file = file_access( path);
		if( file)
		{
			file_put( file, "LOG\n");
		}
	}
	return file;
}

t_file *log_file( int type)
{
	switch( type)
	{
		case LOG_SAVE:
			if( !FILE_WRITE) FILE_WRITE = log_init( "minuit.save");
			return FILE_WRITE;
			break;

		case LOG_READ:
			if( !FILE_READ) FILE_READ = log_init( "minuit.read");
			return FILE_READ;
			break;
		default:
			printf("Unknown LOG type\n");
			return NULL;
	}
}

void __log__( int status, int type, const char *fmt, ...)
{
	if(status)
	{
		/*
		// Init
		if( !LOG_FILE) log_init();

		t_file *file = LOG_FILE;
		*/
		t_file *file = log_file( type);

		if( file_open( file, "a"))
		{
			va_list ap;
			va_start(ap,fmt);
			vfprintf(file->file,fmt,ap);
			va_end(ap);

			file_close(file);
		}
	}
}
