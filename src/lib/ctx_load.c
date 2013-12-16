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
#include "vlst.h"
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
#include "brick.h"
#include "rhizome.h"
#include "mesh.h"
#include "geometry.h"
#include "material.h"
#include "texture.h"

#include "ctx.h"
#include "op.h"

// LISTS

t_lst *NODES;
t_lst *DATA;

int load_error;

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
		if(c->type==dt_var)
		{
			tmp_node=data;
		}
	}
	// store struct
	else
	{
		lst=sc->tmp_data;

		// var data: bind tmp node
		if(c->type==dt_var)
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
		if(n->type==dt_data)
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
		n=l->data;	

		// add to scene
		scene_node_load(C->scene,n);
	}

	// load data
	//scene_data_load(sc);

	// Build Graphs
	for(l=C->scene->rhizomes->first;l;l=l->next)
	{
		t_node *node = l->data;
		t_rhizome *rhizome = node->data;
		rhizome_graph_build(rhizome);
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
		n=l->data;
		d=n->data;
		t_id *id = (t_id *) d;

		// DATA
		if(n->type==dt_data)
		{
			d=n->data;

			if(is(d->type,"dynamic"))
			{
				// find var
				t_node *var=find_new(sc,d->pointer);

				if(var)
				{
					void *var_data=var->data;
					n->id_ptr=n->data;
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

				n->id_ptr=n->data;

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
				t_data *data=node->data;

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
				else if(is(data->type,"app_struct"))
				{
					// get internal struct by name
					*ptr=find_struct(data->target, data->id.name);
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
		t_node *node=l->data;
		void *ptr=node->data;

		switch(node->type)
		{
			case(dt_link): 		link_rebind(sc,ptr);break;
			case(dt_list): 		list_rebind(sc,ptr); break;
			case(dt_object): 	object_rebind(sc,ptr); break;
			case(dt_mesh): 		mesh_rebind(sc,ptr); break;
			case(dt_brick): 	brick_rebind(sc,ptr); break;
			case(dt_block): 	block_rebind(sc,ptr); break;
			case(dt_light): 	light_rebind(sc,ptr); break;
			case(dt_material): 	material_rebind(sc,ptr); break;
			case(dt_vlst): 		vlst_rebind(sc,ptr); break;
			case(dt_data): 		data_rebind(sc,ptr);break;
			case(dt_file): 		file_rebind(sc,ptr);break;
			case(dt_texture): 	texture_rebind(sc,ptr);break;
			case(dt_screen): 	screen_rebind(sc,ptr);break;
			case(dt_option): 	option_rebind(sc,ptr);break;
			case(dt_camera): 	camera_rebind(sc,ptr);break;
			case(dt_dict): 		dict_rebind(sc,ptr); break;
			case(dt_symbol): 	symbol_rebind(sc,ptr); break;
			case(dt_vector) : 	vector_rebind(sc,ptr);break; 
			case(dt_viewport) : 	viewport_rebind(sc,ptr);break; 
			case(dt_set) : 		set_rebind(sc,ptr);break; 
			case(dt_binding) : 	binding_rebind(sc,ptr);break; 
			case(dt_rhizome) : 	rhizome_rebind(sc,ptr);break; 
			case(dt_geo) : 		geo_rebind(sc,ptr);break; 
			case(dt_geo_point) : 	geo_point_rebind(sc,ptr);break; 
			case(dt_geo_edge) : 	geo_edge_rebind(sc,ptr);break; 
			case(dt_geo_array) : 	geo_array_rebind(sc,ptr);break; 

			case(dt_var): break;
			default:
				printf("[ERROR load_data] Unknown type %s\n",data_name_get(node->type));
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
		if(n->type==dt_data)
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
		n=l->data;	// node

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
				n->data=ptr;

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
	c=chunk_new(ct_node,dt_null,0,0,NULL);

	ulog((LOG_READ,"File size:%d\n",file_size));
	ulog((LOG_READ,"[0]\n",file_size));

	char version[GIT];

	size_t rsize;

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
		rsize = fread(&magic,sizeof(char),1,file);
		if(rsize != 1) printf("read error 1\n");

		ulog((LOG_READ,"[%d][%d]\t(+%d)\tmagic {%c}\n",limit,(int)ftell(file),(int)sizeof(char),magic));

		// BREAK EOF
		if(magic=='&') break;

		// GIT VERSION
		if(magic==':')
		{
			rsize = fread(version,sizeof(char)*GIT,1,file);
			//if(rsize != (sizeof(char) * GIT)) printf("read error 2\n");
			(void) rsize;
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
			rsize = fread(c,sizeof(t_chunk),1,file);
			(void) rsize;
			//if( rsize != (sizeof(t_chunk) * 1)) printf("read error 3\n");
			ulog((LOG_READ,"[%d]\t(+%d)\tchunk",(int)ftell(file),(int)sizeof(t_chunk)));
			ulog((LOG_READ," %s:%s:%d:%d\n",chunk_type_get(c->chunk_type),data_name_get(c->type),c->size,c->tot));

			// READ DATA

			// MALLOC
			data=mem_malloc(c->size*c->tot);

			// READ
			rsize = fread(data,c->size,c->tot,file);
			(void) rsize;
			//if(rsize != c->size * c->tot) printf("read error 4\n");

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

	// Load Error
	if(load_error)
	{
		C->ui->show_term = 1;
		term_log("WARNING: Load Error");
	}
}


