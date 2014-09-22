/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

 /*
	NODE

	t_node->data		Object,Mesh,Block,Brick,Data,...
 	t_node->id_ptr		Adress of node->data 
	t_node->id_chunk	reference to chunk in MEMORY

	CHUNK

	t_chunk			memory block header 
	t_chunk->id		global MEMORY->id
	t_chunk->pointer	pointer to MEMORY BLOCK (struct)	

*/

#include "option.h"
#include "ctx.h"
#include "action.h"
#include "app.h"
#include "event.h"
#include "viewport.h"
#include "camera.h"
#include "dict.h"
#include "data.h"
#include "binding.h"
#include "list.h"
#include "vlst.h"
#include "ui.h"
#include "block.h"
#include "object.h"
#include "screen.h"
#include "vector.h"
#include "base.h"
#include "light.h"
#include "set.h"
#include "log.h"
#include "memory.h"
#include "term.h"
#include "file.h"
#include "brick.h"
#include "rhizome.h"
#include "mesh.h"
#include "geometry.h"
#include "material.h"
#include "texture.h"
#include "op.h"
#include "sketch.h"
#include "mode.h"

// LISTS

t_lst *NODES;
t_lst *DATA;

int load_error;

// FIND

// find chunk 

void *find_chunk(t_lst *lst,int _id)
{
	t_link *l;
	for(l=lst->first;l;l=l->next)
	{
		t_id *id = (t_id *) l->data;
		if(id->id_chunk == _id)
		{
			return l->data;
		}
	}

	return NULL;
}

// find node new 

t_node *find_new(t_scene *sc,void *ptr)
{
	t_link *l;
	t_node *n;

	// loop over NODES
	for(l=sc->tmp_node->first;l;l=l->next)
	{
		n = ( t_node *) l->data;

		// id->ptr match old address ptr
		if( n->data == ptr) 
		{
			// return node
			return n;
		}
	}

	return NULL;
}

// find id

t_node *find_by_id(t_scene *sc,int _id)
{
	t_link *l;
	t_node *n;

	// loop over tmp_node
	for(l=sc->tmp_node->first;l;l=l->next)
	{
		n = ( t_node *) l->data;

		if(n->id==_id)
		{
			return  n;
		}
	}

	return NULL;
}

// find node 

t_node *find_by_ptr(t_scene *sc,void *ptr)
{
	t_link *l;
	t_node *n;

	// loop over tmp_node
	for(l=sc->tmp_node->first;l;l=l->next)
	{
		n = ( t_node *) l->data;

		// data->pointer 
		if(n->type==dt_data)
		{
			t_data *data = ( t_data *) n->data;

			if(
				is(data->type,"app_data") 	||
				is(data->type,"struct_ref") 	||
				is(data->type,"app_node") 
			)
			{
				if(data->pointer == ptr)
				{
					return n;
				}
			}
		}
		// node->id_ptr
		else
		{
			if(n->type==dt_var)
			{
				if(n->id_ptr==ptr) 
				{
					return n;
				}
			}
			else
			{
				if(n->id_ptr_old==ptr) 
				{
					return n;
				}
			}
		}
	}

	return NULL;
}

// find ref

void *find_ref(t_scene *sc,t_data *data)
{
	// Get Node
	t_node *node=find_by_ptr(sc,data->ref);

	if(node)
	{
		// Get Ref
		void *ptr = node->cls->get_ref(node,data->id.name);

		if(ptr)
		{
			return ptr;
		}
		else
		{
			load_error = 1;
			return NULL;
		}
	}
	else
	{
		printf("[ERROR struct_ref_get] ref %s %s:Can't get node \n", data->target, data->id.name);
		load_error = 1;
		return NULL;
	}
}

// FREE

void load_free(t_scene *sc)
{
	lst_free(sc->tmp_node);
	lst_free(sc->tmp_data);
}


// LOAD SCENE

void load_scene(t_context *C,t_scene *sc)
{
	t_link *l;
	t_node *n;

	// Loop tmp nodes  
	for(l=sc->tmp_node->first;l;l=l->next)
	{
		// node
		n = ( t_node *) l->data;	

		// add to scene
		scene_node_load( C->scene, n);
	}

	// Build Graphs
	t_lst *lst = scene_lst_get( C->scene, dt_rhizome);
	if( lst)
	{
	for(l=lst->first;l;l=l->next)
	{
		t_node *node = ( t_node *) l->data;
		t_rhizome *rhizome = ( t_rhizome *) node->data;
		rhizome_graph_build(rhizome);
	}
	}
}

void load_module( t_context *C, t_scene *sc)
{
	t_link *l;
	t_node *n;

	// Loop tmp nodes  
	for(l=sc->tmp_node->first;l;l=l->next)
	{
		// node
		n = ( t_node *) l->data;	

		if( n->type == dt_module)
		{
			lst_add( C->mode->modules, n->data, "module");
		}
	}
}

// STORE
/*
	add to MEMORY
*/

void load_store(t_scene *sc)
{
	t_link *l;
	t_node *n;
	t_data *d;

	for(l=sc->tmp_node->first;l;l=l->next)
	{
		n = ( t_node *) l->data;
		d = ( t_data *) n->data;
		t_id *id = (t_id *) d;

		// DATA
		if(n->type==dt_data)
		{
			d = ( t_data *) n->data;

			if(is(d->type,"dynamic"))
			{
				// find var
				t_node *var=find_new(sc,d->pointer);

				if(var)
				{
					void *var_data = var->data;
					n->id_ptr = n->data;
					id->id_chunk = n->id_chunk;
					var->id_ptr=var_data;
					d->pointer=var_data;
				}
				else
				{
					printf("[ERROR load_store] dynamic : can't find node for %s\n", d->id.name);
					load_error = 1;
				}
			}
			else if(is(d->type,"app_node"))
			{
				// get internal node by id
				t_node *node=find_by_id(sc,d->id_node);
				d->pointer=node;

				n->id_ptr = n->data;

				id->id_chunk = n->id_chunk;
			}
			else if(is(d->type,"struct_ref"))
			{
				// get new object reference
				void *p = find_ref(sc,d);

				// get new pointer
				t_node *node=find_by_ptr(sc,d->ref);
				void *data=node->data;

				// set new pointers
				d->ref=data;
				d->pointer=p;

				n->id_ptr=n->data;

				id->id_chunk = n->id_chunk;
			}
			else if(is(d->type,"app_data"))
			{
			}
			else
			{
				printf("[ERROR load_store] Unknown type (%s)\n",d->type);
				load_error = 1;
			}
		}

		// ELSE
		else if(n->type==dt_var)
		{
			// don't store
			// stored by dynamic data
		}
		else
		{
			// Option
			if(n->type==dt_option)
			{
				// don't store
			}
			// Regular Struct
			else
			{
				n->id_ptr=n->data;
				id->id_chunk = n->id_chunk;
			}
		}
	}
}

// REBIND
/*
	Relink pointers with new adresses
*/


void rebind(t_scene *sc,const char *type,const char *name,void **ptr)
{
	if(*ptr)
	{
		// GET NODE BY *PTR
		t_node *node=find_by_ptr(sc,*ptr);

		if(node)
		{
			// [INDIRECT DATA]
			if(node->type==dt_data)
			{
				t_data *data = ( t_data *) node->data;

				if(is(data->type,"dynamic"))
				{
					// get ptr to memory
					*ptr=data->pointer;
				}
				else if(is(data->type,"app_data"))
				{
					// get register
					*ptr=find_register(data->target, data->id.name);
				}
				else if(is(data->type,"struct_ref"))
				{
					// get ref
					*ptr=find_ref(sc,data);
				}
				else if(is(data->type,"app_node"))
				{
					// get internal node by id
					t_node *n=find_by_id(sc,data->id_node);
					*ptr=n;
				}
				else
				{
					printf("[ERROR rebind] Unknown type %s\n",data->type);
					load_error = 1;
				}
			}
			// [DIRECT DATA] t_struct
			else
			{
				*ptr = node->data;
			}
		}
		else
		{
			t_id *id = (t_id *) *ptr;
			printf("[ERROR rebind] Can't find data %s %s\n",type,name);
			printf("[ERROR rebind] ID: %d %s\n",id->id, id->name);
			load_error = 1;
		}
	}
}

// LOAD DATA
// Init struct classes & Rebind
/*
	Loop over DATA -> rebind
*/

void load_rebind(t_scene *sc)
{
	t_link *l;

	// Loop over tmp_data
	for(l=sc->tmp_node->first;l;l=l->next)
	{
		t_node *node = ( t_node *) l->data;
		void *ptr = node->data;

		node->cls->rebind( sc, ptr);
	}
}

// LOAD VAR
// rebind external vars

void load_var(t_scene *sc)
{
	t_link *l;
	t_node *n;
	t_data *d;

	// Loop over tmp_node
	for(l=sc->tmp_node->first;l;l=l->next)
	{
		n = ( t_node *) l->data;

		// data
		if(n->type==dt_data)
		{
			d = ( t_data *) n->data;

			// dynamic
			if(is(d->type,"dynamic"))
			{
				// find new node 
				t_node *var=find_by_ptr(sc,d->pointer);

				if(var)
				{
					void  *var_data = var->data;

					// get new ptr
					d->pointer=var_data;
				}
				else
				{
					printf("[ERROR load_var] Can't find node for %s\n", d->id.name);
					load_error = 1;
				}
			}
		}
	}
}

// LOAD NODE
// bind node->data with node's id_chunk
/*
	loop NODES
		BIND node->data (node->id_chunk == data->id_chunk) 
		BIND node->cls with [id chunk type]
*/

void load_node(t_scene *sc)
{
	t_link *l;
	t_node *n;

	// Loop over tmp_node
	for(l=sc->tmp_node->first;l;l=l->next)
	{
		n = ( t_node *) l->data;	// node

		// 0) BIND NODE CLS (MAKE NODE)

		node_init(n,n->type);

		// STORE OLD ID

		n->id_old=n->id;

		// 1) FIND DATA with node's id_chunk 

		void *ptr=NULL;

		// VAR
		if(n->type==dt_var)
		{
			// don't
		}
		// REGULAR NODE
		else
		{
			// Find Chunk
			ptr=find_chunk(sc->tmp_data,n->id_chunk); 

			if(ptr)
			{

				// 2) *** BIND *** node->data

				// store old ptr
				n->id_ptr_old=n->id_ptr;

				// bind
				n->data = ( t_node *) ptr;

				t_id *id = (t_id *) n->data;
				id->node = n;

			}
			else
			{
				printf("[ERROR load_node] Can't find chunk, node class:%s\n",data_name_get(n->type));
				load_error = 1;

			}
		}

	}
}

// READ DATA
// transform raw memory to new allocated memory by type
/*
	while magic not [&]
		read chunk
			family	(node/data)
			type,size,tot,length

		allocate data* size*tot
		read data
		id_store (lst family (NODE/DATA))
*/


void mod_load( void)
{
	t_context *C = ctx_get();
	skt_load( C);
}

void load_settings( t_context *C)
{
	t_module *module = mode_module_get( C->mode, "load");
	void (* f)( void) =  module->data;
	f();
}

// LOAD

void load_file(t_context *C,const char *path)
{

/** INIT **/

	// Init Error
	load_error = 0;

	// create a new scene
	t_scene *sc=scene_new();

	// init tmp lists
	sc->tmp_node=lst_new("tmp_node");
	sc->tmp_data=lst_new("tmp_data");

/** READ **/
	
	// read file, allocate memory, store in id
	mem_read( sc, path);

/** BIND **/

	// bind node->data, init node->cls
	load_node(sc);

	// bind dynamic data
	load_var(sc);

	// rebind structs
	load_rebind(sc);

/** STORE **/

	// add loaded data to memory
	load_store(sc);

/** LOAD **/

	// load options
	load_option(C,sc);

	// add to scene 
	load_scene(C,sc);

	load_module( C, sc);

	// Load Settings
	load_settings( C);

/** RESET **/

	// free NODES & DATAS
	load_free(sc);

	// RESET Scene
	C->scene->selected=NULL;

	// free tmp scene
	scene_free(sc);
	
	// Load Error
	if(load_error)
	{
		C->ui->show_term = 1;
		term_log("WARNING: Load Error");
	}
}

void load_file_from_path( t_context *C, const char *filename)
{
	load_file( C, filename);
	set_name_long( C->app->path_file, filename);
	t_file *f = file_new( filename);
	file_init( f);
	char new_name[_NAME_LONG_];
	bzero( new_name, _NAME_LONG_);
	set_name_long( new_name, f->file_name);
	s_cat( new_name, ".mn", _NAME_LONG_);
	set_name_long( C->app->filename, new_name);
	file_free( f);
}

void load_last( t_context *C)
{
	char *path = app_get_file_path( C->app, APP_FILENAME_SAVE);
	char *filename = NULL;
	t_file *file = NULL;

	if( sys_file_exists( path))
	{
		file = file_access( path);
	}

	if( file)
	{
		filename = file_line_get( file, 0);
		if( filename)
		{
			if( is( filename, "void")) filename = NULL;
		}
		else
		{
			printf("[LOAD] Can't get filename\n");
		}
	}

	if( filename)
	{
		/*
		load_file( C, filename);
		set_name_long( C->app->path_file, filename);
		t_file *f = file_new( filename);
		file_init( f);
		char new_name[_NAME_LONG_];
		bzero( new_name, _NAME_LONG_);
		set_name_long( new_name, f->file_name);
		s_cat( new_name, ".mn", _NAME_LONG_);
		set_name_long( C->app->filename, new_name);
		file_free( f);
		*/
		load_file_from_path( C, filename);
	}
	else
	{
		C->event->callback = add_mn;
		screen_switch_by_name("screen_browser");
	}
}

void add_mn(void)
{
	t_context *C=ctx_get();

	t_file *file = file_new( C->app->path_file);
	file_init(file);

	if(is(file->ext,"mn"))
	{
		/*
		load_file( C, file->path);
		set_name_long( C->app->filename, file->file_name);
		s_cat( C->app->filename, ".mn", _NAME_LONG_);
		*/
		load_file_from_path( C, file->path);
	}
	else
	{
		printf("Not a mn file\n");
	}

	file_free( file);
}


