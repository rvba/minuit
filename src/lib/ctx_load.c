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
#include "context.h"
#include "action.h"
#include "app.h"
#include "event.h"
#include "viewport.h"
#include "camera.h"
#include "dict.h"
#include "data.h"
#include "binding.h"
#include "list.h"
#include "ui.h"
#include "block.h"
#include "object.h"
#include "screen.h"
#include "vector.h"
#include "util.h"
#include "light.h"
#include "set.h"
#include "log.h"
#include "memory.h"
#include "term.h"
#include "file.h"

#include "ctx.h"
#include "op.h"

// LISTS

t_lst *NODES;
t_lst *DATA;

int load_error;

// DEBUG

int check(t_node *node,const char *name_data,const char *name_ptr)
{
	if(node)
	{
		ulog((LOG_CHECK,"OK %s:%s\n",name_data,name_ptr));
		return 1;
	}
	else 
	{
		ulog((LOG_CHECK,"[ERROR] Loading %s:%s\n",name_data,name_ptr));
		check_ok=0;
		return 0;
	}
}

void check_init(const char *type,const char *name)
{
	ulog((LOG_CHECK,"  [LOAD STRUCT][%s][%s]\n",type,name));
	check_ok=1;
}

void check_check(const char *type,const char *name)
{
	if(check_ok)
	{
		ulog((LOG_CHECK,"OK[LOAD STRUCT][%s][%s]\n",type,name));
	}
	else 
	{
		ulog((LOG_CHECK,"[ERROR][LOAD STRUCT][%s][%s]\n",type,name));
	}
}

// ID STORE

t_node *tmp_node;

void id_store(t_scene *sc,t_chunk *c,void *data)
{
	t_lst *lst;

	// store node
	if(c->chunk_type==ct_node)
	{
		lst=sc->tmp_node;

		// var node: store tmp node
		if(c->type==nt_var)
		{
			tmp_node=data;
		}
	}
	// store struct
	else
	{
		lst=sc->tmp_data;

		// var data: bind tmp node
		if(c->type==nt_var)
		{
			if(tmp_node)
			{
				tmp_node->data=data;
				tmp_node=NULL;
			}
		}
	}

	// add to LST
	lst_add(lst,data,"data");
}

// FIND

// find chunk 

void *find_chunk(t_lst *lst,int _id)
{
	t_link *l;
	for(l=lst->first;l;l=l->next)
	{
		t_generic *g=l->data;
		if(g->id_chunk==_id)
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
		n=l->data;

		// id->ptr match old address ptr
		if(n->data==ptr) 
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
		n=l->data;

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
		n=l->data;

		// data->pointer 
		if(n->type==nt_data)
		{
			t_data *data=n->data;

			if(
				is(data->type,"app_struct")	||
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
			if(n->type==nt_var)
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

// find struct

void *find_struct(const char *target,const char *name)
{
	t_context *C=ctx_get();

	t_node *node = scene_node_get(C->scene,target,name);

	if(node)
	{
		return node->data;
	}
	{
		load_error = 1;
		printf("[ERROR app_get] Unknown target %s\n",name);
		ulog((LOG_REBIND,"[ERROR app_get] Unknown target %s\n",name));
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
		void *ptr = node->cls->get_ref(node,data->name);

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
		printf("[ERROR struct_ref_get] ref %s %s:Can't get node \n",data->target,data->name);
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
		n=l->data;	

		// add to scene
		scene_node_load(C->scene,n);
	}

	// load data
	scene_data_load(sc);
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
		n=l->data;
		d=n->data;
		t_generic *g=(t_generic *)d;

		// DATA
		if(n->type==nt_data)
		{
			d=n->data;

			if(is(d->type,"dynamic"))
			{
				// find var
				t_node *var=find_new(sc,d->pointer);

				if(var)
				{
					void *var_data=var->data;

					// store
					n->id_chunk_self=mem_store(ct_node,n->type,sizeof(t_node),1,n);
					n->id_chunk=mem_store(ct_data,n->type,n->cls->size,1,n->data);
					n->id_ptr=n->data;

					g->id_chunk=n->id_chunk;

					// store
					var->id_chunk_self=mem_store(ct_node,nt_var,sizeof(t_node),1,var);
					var->id_chunk=mem_store(ct_data,nt_var,d->size,1,var_data);
					var->id_ptr=var_data;

					d->pointer=var_data;


				}
				else
				{
					printf("[ERROR load_store] dynamic : can't find node for %s\n",d->name);
					load_error = 1;
				}
			}
			else if(is(d->type,"app_node"))
			{
				// get internal node by id
				t_node *node=find_by_id(sc,d->id_node);
				d->pointer=node;

				n->id_chunk_self=mem_store(ct_node,n->type,sizeof(t_node),1,n);
				n->id_chunk=mem_store(ct_data,n->type,n->cls->size,1,n->data);
				n->id_ptr=n->data;

				g->id_chunk=n->id_chunk;
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

				// store
				n->id_chunk_self=mem_store(ct_node,n->type,sizeof(t_node),1,n);
				n->id_chunk=mem_store(ct_data,n->type,n->cls->size,1,n->data);
				n->id_ptr=n->data;

				g->id_chunk=n->id_chunk;
			}
			else if(is(d->type,"app_struct"))
			{
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
		else if(n->type==nt_var)
		{
			// don't store
			// stored by dynamic data
		}
		else
		{
			// Option
			if(n->type==nt_option)
			{
				// don't store
			}
			// Regular Struct
			else
			{
				// store
				n->id_chunk_self=mem_store(ct_node,n->type,sizeof(t_node),1,n);
				n->id_chunk=mem_store(ct_data,n->type,n->cls->size,1,n->data);
				n->id_ptr=n->data;

				g->id_chunk=n->id_chunk;
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
			if(node->type==nt_data)
			{
				t_data *data=node->data;

				if(is(data->type,"dynamic"))
				{
					// get ptr to memory
					*ptr=data->pointer;
				}
				else if(is(data->type,"app_data"))
				{
					// get register
					*ptr=find_register(data->target,data->name);
				}
				else if(is(data->type,"struct_ref"))
				{
					// get ref
					*ptr=find_ref(sc,data);
				}
				else if(is(data->type,"app_struct"))
				{
					// get internal struct by name
					*ptr=find_struct(data->target,data->name);
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
				*ptr=node->data;
			}
		}
		else
		{
			printf("[ERROR rebind] Can't find data %s %s\n",type,name);
			check_ok=0;
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
		t_node *node=l->data;
		void *ptr=node->data;

		switch(node->type)
		{
			case(nt_link): 		link_rebind(sc,ptr);break;
			case(nt_list): 		lst_rebind(sc,ptr); break;
			case(nt_object): 	object_rebind(sc,ptr); break;
			case(nt_mesh): 		mesh_rebind(sc,ptr); break;
			case(nt_brick): 	brick_rebind(sc,ptr); break;
			case(nt_block): 	block_rebind(sc,ptr); break;
			case(nt_light): 	light_rebind(sc,ptr); break;
			case(nt_material): 	material_rebind(sc,ptr); break;
			case(nt_vlst): 		vlst_rebind(sc,ptr); break;
			case(nt_data): 		data_rebind(sc,ptr);break;
			case(nt_file): 		file_rebind(sc,ptr);break;
			case(nt_texture): 	texture_rebind(sc,ptr);break;
			case(nt_screen): 	screen_rebind(sc,ptr);break;
			case(nt_option): 	option_rebind(sc,ptr);break;
			case(nt_camera): 	camera_rebind(sc,ptr);break;
			case(nt_dict): 		dict_rebind(sc,ptr); break;
			case(nt_symbol): 	symbol_rebind(sc,ptr); break;
			case(nt_vector) : 	vector_rebind(sc,ptr);break; 
			case(nt_viewport) : 	viewport_rebind(sc,ptr);break; 
			case(nt_set) : 		set_rebind(sc,ptr);break; 
			case(nt_binding) : 	binding_rebind(sc,ptr);break; 

			case(nt_var): break;
			default:
				printf("[ERROR load_data] Unknown type %s\n",node_name_get(node->type));
				load_error = 1;
				break;
		}
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
		n=l->data;

		// data
		if(n->type==nt_data)
		{
			d=n->data;

			// dynamic
			if(is(d->type,"dynamic"))
			{
				// find new node 
				t_node *var=find_by_ptr(sc,d->pointer);

				if(var)
				{
					void  *var_data=var->data;

					// get new ptr
					d->pointer=var_data;
				}
				else
				{
					printf("[ERROR load_var] Can't find node for %s\n",d->name);
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
		n=l->data;	// node

		// 0) BIND NODE CLS (MAKE NODE)

		node_init(n,n->type);

		// STORE OLD ID

		n->id_old=n->id;

		// 1) FIND DATA with node's id_chunk 

		void *ptr=NULL;

		// VAR
		if(n->type==nt_var)
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
				n->data=ptr;

			}
			else
			{
				printf("[ERROR load_node] Can't find chunk, node class:%s\n",node_name_get(n->type));
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


void load_read(t_scene *sc,const char *path)
{
	t_context *C=ctx_get();

	void *data; 		// struct memory allocation
	char magic;		// read magic
	int limit=0;		// max size
	#ifdef WITH_DEBUG
	int file_size;		// size of bin
	#endif

	t_chunk *c;

	// FILE
	FILE *file=fopen(path,"r");

	// FILE SIZE
	fseek (file,0,SEEK_END);
	#ifdef WITH_DEBUG
	file_size = (int)ftell(file);
	#endif
	rewind(file);

	// NEW CHUNK
	c=chunk_new(ct_node,nt_null,0,0,NULL);

	ulog((LOG_READ,"File size:%d\n",file_size));
	ulog((LOG_READ,"[0]\n",file_size));

	char version[GIT];

	// READ FILE
	while(1)
	{
		// LIMIT
		if(limit>BIN_MAX)
		{
			printf("[ERROR ctx_load] Max Bin\n]");
			break;
		}

		// MAGIC
		fread(&magic,sizeof(char),1,file);

		ulog((LOG_READ,"[%d][%d]\t(+%d)\tmagic {%c}\n",limit,(int)ftell(file),(int)sizeof(char),magic));

		// BREAK EOF
		if(magic=='&') break;

		// GIT VERSION
		if(magic==':')
		{
			fread(version,sizeof(char)*GIT,1,file);
			if(!is(C->app->git,version))
			{
				/*
				printf("[WARNING] Git versions differs\n");
				printf("[WARNING] file:%s\n",version);
				printf("[WARNING] current:%s\n",C->app->git);
				*/
			}
		}
		else
		{
			// READ CHUNK
			fread(c,sizeof(t_chunk),1,file);
			ulog((LOG_READ,"[%d]\t(+%d)\tchunk",(int)ftell(file),(int)sizeof(t_chunk)));
			ulog((LOG_READ," %s:%s:%d:%d\n",chunk_type_get(c->chunk_type),node_name_get(c->type),c->size,c->tot));

			// READ DATA

			// MALLOC
			data=malloc(c->size*c->tot);

			// READ
			fread(data,c->size,c->tot,file);

			ulog((LOG_READ,"[%d]\t(+%d)\tstruct %s:%s\n",(int)ftell(file),c->size,chunk_type_get(c->chunk_type),c->type));

			// STORE  Nodes + Structs
			id_store(sc,c,data);

			// LIMIT++
			limit++;
		}
	}

	// CLOSE FILE

	fclose(file);
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
	load_read(sc,path);

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

/** RESET **/

	// free NODES & DATAS
	load_free(sc);

	// RESET Scene
	C->scene->selected=NULL;

	// free tmp scene
	scene_free(sc);
	
	// set file path
	file_set_location(C->app->file,path);
	file_init(C->app->file);

	if(load_error)
	{
		term_log("WARNING: Load Error");
	}
}


