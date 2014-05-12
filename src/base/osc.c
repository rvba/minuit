/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lo.h"

int osc_server_done = 0;
int osc_debug = 0;
int osc_log = 1;

void osc_error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
}



int osc_handler_generic( const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
	if( osc_debug)
	{
		int i;
		printf("path: <%s>\n", path);
		for (i = 0; i < argc; i++)
		{
			printf("arg %d '%c' ", i, types[i]);
			lo_arg_pp((lo_type)types[i], argv[i]);
			printf("\n");
		}
	}

	return 1;
}

int osc_handler_vector(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
    printf("[OSC SERVER][VECTOR %f %f %f]\n", argv[0]->f, argv[1]->f, argv[2]->f);

    return 0;
}

int osc_handler_int(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
    printf("[OSC SERVER][INT %d]\n", argv[0]->i);

    return 0;
}

int osc_handler_quad(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
    printf("[OSC QUAD][%d %f %f %f %f]\n", argv[0]->i, argv[1]->f,  argv[2]->f, argv[3]->f, argv[4]->f);

    return 0;
}

int osc_handler_quit(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
    osc_server_done = 1;
    printf("[OSC SERVER] exit\n");

    return 0;
}

int osc_server( int port)
{
	char sport[8];

	if( !port)
	{
		printf("[OSC SERVER] No valid port\n");
		return 0;
	}
	else
	{
		snprintf( sport, 8, "%d", port);
		printf("[OSC SERVER] Init server at %d\n", port);
	}

	lo_server_thread st = lo_server_thread_new( sport, osc_error);

	lo_server_thread_add_method(st, NULL, NULL, osc_handler_generic, NULL);
	lo_server_thread_add_method(st, "/a/b/c", "fff", osc_handler_vector, NULL);
	lo_server_thread_add_method(st, "/quit", "", osc_handler_quit, NULL);
	lo_server_thread_add_method(st, "/int", "i", osc_handler_int, NULL);
	lo_server_thread_add_method(st, "/quad", "iffff", osc_handler_quad, NULL);

	lo_server_thread_start(st);

	while ( !osc_server_done)
	{
		usleep(1000);
	}

	lo_server_thread_free(st);

	return 0;
}

void osc_log_print( const char *msg, lo_message lomsg)
{
	int count = lo_message_get_argc( lomsg);
	int i;
	char *type = lo_message_get_types( lomsg);
	lo_arg** arg = lo_message_get_argv( lomsg);

	printf("%s ", msg);

	for( i = 0; i < count ; i++)
	{
		switch( type[i])
		{
			case 's': printf("s"); break;
			case 'i': printf("i"); break;
			case 'f': printf("f"); break;
			case 'l': printf("l"); break;
		}
	}

	printf(" ");

	for( i = 0; i < count ; i++)
	{
		switch( type[i])
		{
			case 's': printf("%s ", &arg[i]->s); break;
			case 'i': printf("%d ", arg[i]->i32); break;
			case 'f': printf("%f ", arg[i]->f); break;
			case 'l': printf("%lu ", arg[i]->i64); break;
		}
	}

	printf("\n");
}

int osc_send( const char *port, const char *msg, const char *format, ...)
{
	lo_address t = lo_address_new(NULL, port);
	lo_message message = lo_message_new();

	va_list args;
	va_start( args, format);

	while( *format != '\0')
	{
		switch( *format)
		{
			case 's': lo_message_add_string( message, va_arg( args, const char *)); break;
			case 'i': lo_message_add_int32( message, va_arg( args, int)); break;
			case 'f': lo_message_add_float( message, va_arg( args, double)); break;
		}

		++format;
	}

	va_end( args);
	

	if( !lo_send_message( t, msg, message))
	{
		printf("OSC error %d: %s\n", lo_address_errno(t),
		lo_address_errstr(t));
	}

	if( osc_log) osc_log_print( msg, message);
	lo_message_free( message);
	lo_address_free( t);

	return 0;
}

int osc_send_ints( const char *port, const char *msg, int count, int *val)
{
	lo_address t = lo_address_new(NULL, port);
	lo_message message = lo_message_new();

	int i;
	for( i = 0; i < count; i++)
	{
		lo_message_add_int32( message, val[i]);
	}

	if( !lo_send_message( t, msg, message))
	{
		printf("OSC error %d: %s\n", lo_address_errno(t),
		lo_address_errstr(t));
	}

	if( osc_log) osc_log_print( msg, message);
	lo_message_free( message);

	return 0;
}

int osc_send_quit( const char *port)
{
	lo_address t = lo_address_new(NULL, port);

	if( !lo_send( t, "/quit" , ""))
	{
		printf("OSC error %d: %s\n", lo_address_errno(t),
		lo_address_errstr(t));
	}

	return 0;
}

int osc_send_vector( const char *port, const char *msg)
{
	lo_address t = lo_address_new(NULL, port);

	if( !lo_send( t, "/a/b/c" , "fff" , 10.0f, 10.0f, 10.0f))
	{
		printf("OSC error %d: %s\n", lo_address_errno(t),
		lo_address_errstr(t));
	}

	return 0;
}

int osc_client( int port)
{
	char sport[8];
	int msg_size = 32;
	char msg[msg_size];
	int get_vector = 0;
	int send_vector = 0;

	if( !port)
	{
		printf("[OSC CLIENT] No valid port\n");
		return 0;
	}
	else
	{
		snprintf( sport, 8, "%d", port);
		printf("[OSC CLIENT] Init server at %d\n", port);
	}

	while( 1)
	{
		if( get_vector)
		{
			printf("vector:");
			get_vector = 0;
			send_vector = 1;
		}
		else
		{
			printf("msg:");
		}

		scanf( "%31s", msg);

		if( is( msg, "q"))
		{
			osc_send_quit( sport);
			return 1;
		}
		else if( is( msg, "vector")) get_vector = 1;

		if( get_vector)
		{
		}
		else if( send_vector)
		{
			osc_send_vector( sport, msg); 
			send_vector = 0;
		}
		else
		{
			printf("quad!!\n");
			osc_send( sport, "/quad", "iffff", 1, .1, .2, .3, .4); 
		}

		usleep(1000);
	}


	return 1;
}






