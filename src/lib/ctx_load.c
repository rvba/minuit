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

#include "op.h"

// LISTS

t_lst *NODES;
t_lst *DATA;

// DEBUG

int check(t_node *node,const char *name_data,const char *name_ptr)
{
	if(node)
	{
		log((LOG_CHECK,"OK %s:%s\n",name_data,name_ptr));
		return 1;
	}
	else 
	{
		log((LOG_CHECK,"[ERROR] Loading %s:%s\n",name_data,name_ptr));
		check_ok=0;
		return 0;
	}
}

void check_init(const char *type,const char *name)
{
	log((LOG_CHECK,"  [LOAD STRUCT][%s][%s]\n",type,name));
	check_ok=1;
}

void check_check(const char *type,const char *name)
{
	if(check_ok)
	{
		log((LOG_CHECK,"OK[LOAD STRUCT][%s][%s]\n",type,name));
	}
	else 
	{
		log((LOG_CHECK,"[ERROR][LOAD STRUCT][%s][%s]\n",type,name));
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
		printf("[ERROR app_get] Unknown target %s\n",name);
		log((LOG_REBIND,"[ERROR app_get] Unknown target %s\n",name));
	}

	return NULL;
}

// find ref

void *find_ref(t_scene *sc,t_data *data)
{
	if(is(data->target,"object"))
	{
		t_node *node=find_by_ptr(sc,data->ref);

		if(node)
		{
			void *p;

			t_object *object=node->data;

			// REBIND
			data->ref=object;

			if(is(data->name,"loc_x"))  p=&object->loc[0]; 
			else if(is(data->name,"loc_y"))  	p=&object->loc[1]; 
			else if(is(data->name,"loc_z"))  	p=&object->loc[2]; 
			else if(is(data->name,"rot_x"))  	p=&object->rot[0]; 
			else if(is(data->name,"rot_y"))  	p=&object->rot[1]; 
			else if(is(data->name,"rot_z"))  	p=&object->rot[2]; 
			else if(is(data->name,"scl_x"))  	p=&object->size[0]; 
			else if(is(data->name,"scl_y"))  	p=&object->size[1]; 
			else if(is(data->name,"scl_z"))  	p=&object->size[2]; 
			else
			{
				printf("[ERROR strutc_ref_get object] Unknown name %s \n",data->name);
				return NULL;
			}

			// REBIND
			data->pointer=p;
			// return 
			return p;

		}
		else
		{
			printf("[ERROR struct_ref_get] ref %s %s:Can't get node \n",data->target,data->name);
			return NULL;
		}
	}
	else if(is(data->target,"plug"))
	{
		t_node *node=find_by_ptr(sc,data->ref);

		if(node)
		{
			void *p;

			t_brick *brick=node->data;

			if(is(data->name,"in"))  p=&brick->plug_in; 
			else if(is(data->name,"intern"))  p=&brick->plug_intern; 
			else if(is(data->name,"out"))  p=&brick->plug_out; 
			else
			{
				printf("[ERROR strutc_ref_get object] Unknown name %s \n",data->name);
				return NULL;
			}

			return p;

		}
		else
		{
			printf("[ERROR struct_ref_get] ref %s %s:Can't get node \n",data->target,data->name);
			return NULL;
		}
	}
	else if(is(data->target,"material"))
	{
		t_node *node=find_by_ptr(sc,data->ref);
		if(node)
		{
			t_material *material=node->data;

			void *p;

			// REBIND
			data->ref=material;

			if(is(data->name,"red"))  			p=&material->color[0]; 
			else if(is(data->name,"green"))  		p=&material->color[1]; 
			else if(is(data->name,"blue"))  		p=&material->color[2]; 
			else if(is(data->name,"alpha"))  		p=&material->color[3]; 
			else
			{
				printf("[ERROR strutc_ref_get] Unknown name [%s] \n",data->name);
				return NULL;
			}

			// REBIND
			data->pointer=p;
			// return 
			return p;

		}
		else
		{
			printf("[ERROR strutc_ref_get] ref %s %s:Can't get node \n",data->target,data->name);
			return NULL;
		}
	}
	else if(is(data->target,"mesh"))
	{
		t_node *node=find_by_ptr(sc,data->ref);
		if(node)
		{
			t_mesh *mesh=node->data;

			void *p;

			// REBIND
			data->ref=mesh;

			if(is(data->name,"vertex"))  			p=&mesh->vertex; 
			else
			{
				printf("[ERROR strutc_ref_get] Unknown name [%s] \n",data->name);
				return NULL;
			}

			// REBIND
			data->pointer=p;
			// return 
			return p;

		}
		else
		{
			printf("[ERROR strutc_ref_get] ref %s %s:Can't get node \n",data->target,data->name);
			return NULL;
		}
	}
	else if(is(data->target,"vlst"))
	{
		t_node *node=find_by_ptr(sc,data->ref);
		if(node)
		{
			t_vlst *vlst=node->data;

			void *p;

			// REBIND
			data->ref=vlst;

			if(is(data->name,"count"))  			p=&vlst->count_new; 
			else
			{
				printf("[ERROR strutc_ref_get] Unknown name [%s] \n",data->name);
				return NULL;
			}

			// REBIND
			data->pointer=p;
			// return 
			return p;

		}
		else
		{
			printf("[ERROR strutc_ref_get] ref %s %s:Can't get node \n",data->target,data->name);
			return NULL;
		}
	}
	else
	{
		printf("[ERROR strutc_ref_get] Unknown target %s \n",data->target);
		return NULL;
	}
}


// FREE

void load_free(t_scene *sc)
{
	lst_free(sc->tmp_node);
	lst_free(sc->tmp_data);
}

// SAVE

void load_last(t_context *C)
{
	t_link *link;

	t_file *file = file_new("minuit.save");
	file_init(file);
	if(file_exists(file))
	{
		file_read(file);
		file_read_lines(file);
		if(file->lines->first)
		{
			link = file->lines->first;
			t_line *line = link->data;
			line_read_words(line);
			t_link *l = line->words->first;
			if(l)
			{
				t_word *word = l->data;
				char *name = word->data;
				if(is(name,"void"))
				{
					load_file(C,"minuit.mn");
				}
				else
				{
					load_file(C,word->data);
				}
			}
		}
	}
	else
	{
		C->event->callback=add_mn;
		screen_switch_by_name("screen_browser");
	}
}

void save_file(t_context *C)
{
	t_file *file = C->app->file;

	char *name = file->name;

	if(is(name,"void"))
	{
		set_name(file->name,"minuit.mn");
		file_build_location(file);
	}

	option_save(C);
	mem_write(file->location);
}

void save_file_increment(t_context *C)
{
	t_file *file = C->app->file;

	file_path_split(file);

	int i,j;
	char *name = file->name;
	int length = strlen(name);
	int last_char = length - 1; // starts from 0 !
	int max_indice = 10;
	char indice[max_indice];
	bzero(indice,max_indice);

	if(!isdigit(name[last_char]))
	{
		char number[4] = "-01";

		strcat(file->name,number);
		set_name(file->name,name);
		file_build_location(file);

		save_file(C);
	}
	else
	{
		j = 0;
		for(i = 0; i < length ; i++)
		{
			if(isdigit(name[i]))
			{
				if(j < max_indice)
				{
					indice[j] = name[i];
					j++;
				}
			}

		}

		if(j)
		{
			int old_indice = atoi(indice);

			int new_indice = ++old_indice;

			char new_name[_PATH_];
			char new_name_indice[10];

			if(new_indice < 10)
			{
				sprintf(new_name_indice,"0%d",new_indice);
			}
			else
			{
				sprintf(new_name_indice,"%d",new_indice);
			}


			for(i = 0; i < length ; i++)
			{
				if(isdigit(name[i]))
				{
					break;
				}
				else
				{
					new_name[i] = name[i];
				}
			}

			strcat(new_name,new_name_indice);

			set_name(file->name,new_name);
			file_build_location(file);

			save_file(C);
		}
	}
}

// LOAD SCENE

void load_scene(t_context *C,t_scene *sc)
{
	t_link *l;
	t_node *n;

	// Loop tmp nodes  
	for(l=sc->tmp_node->first;l;l=l->next)
	{
		n=l->data;	// node

		// ADD TO SCENE
		scene_node_load(C->scene,n);

		if(n->type==nt_camera)
		{
			t_generic *g=(t_generic *)n->data;

			if(is(g->name,"main_camera"))
			{
				t_camera *camera=n->data;
				t_context *C=ctx_get();
				C->camera=camera;
			}
		}
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

			// global
			if(is(g->name,"global"))
			{
			}
			else if(is(d->type,"dynamic"))
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
				}
			}
			else if(is(d->type,"app_node"))
			{

				n->id_chunk_self=mem_store(ct_node,n->type,sizeof(t_node),1,n);
				n->id_chunk=mem_store(ct_data,n->type,n->cls->size,1,n->data);
				n->id_ptr=n->data;

				g->id_chunk=n->id_chunk;

			}
			else if(is(d->type,"struct_ref"))
			{

				if(is(d->target,"object"))
				{
				}
				else if(is(d->target,"material"))
				{
				}
				else if(is(d->target,"mesh"))
				{
				}

				else if(is(d->target,"vlst"))
				{
				}
				else if(is(d->target,"plug"))
				{
					t_node *node=find_by_ptr(sc,d->ref);

					if(node)
					{
						void *p;

						t_brick *brick=node->data;

						// REBIND
						d->ref=brick;

						if(is(d->name,"in"))  p=&brick->plug_in; 
						else if(is(d->name,"intern"))  p=&brick->plug_intern; 
						else if(is(d->name,"out"))  p=&brick->plug_out; 
						//else if(is(d->name,"parent"))  p=&brick->plug_intern.parent; 
						else
						{
							printf("[ERROR strutc_ref_get object] Unknown name %s \n",d->name);
						}

						// REBIND
						d->pointer=p;
					}
					else
					{
						printf("[ERROR load_store plug] can't find node\n");
					}
				

				}
				else
				{
					printf("[ERROR load_store] Unknown target %s\n",d->target);
				}


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
			if(n->type==nt_option)
			{
				// don't store
			}
			else
			{

				// global list

				d=n->data;

				if(is(g->name,"global"))
				{
				}
				else if(is(g->name,"glink"))
				{
				}

				// regular struct

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

					// store !
					data->pointer=n;
				}
				else
				{
					printf("[ERROR rebind] Unknown type %s\n",data->type);
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
			t_context *C = ctx_get();
			C->event->load_error = 1;
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
			case(nt_link): link_rebind(sc,ptr);break;
			case(nt_list): lst_rebind(sc,ptr); break;
			case(nt_object): object_rebind(sc,ptr); break;
			case(nt_mesh): mesh_rebind(sc,ptr); break;
			case(nt_brick): brick_rebind(sc,ptr); break;
			case(nt_block): block_rebind(sc,ptr); break;
			case(nt_light): light_rebind(sc,ptr); break;
			case(nt_material): material_rebind(sc,ptr); break;
			case(nt_vlst): vlst_rebind(sc,ptr); break;
			case(nt_data): break;
			case(nt_file): break;
			case(nt_texture): break;
			case(nt_screen): break;
			case(nt_var): break;
			case(nt_option):break;
			case(nt_camera): break;
			case(nt_dict):dict_rebind(sc,ptr); break;
			case(nt_symbol): symbol_rebind(sc,ptr); break;
			case(nt_vector) : vector_rebind(sc,ptr);break; 
			default: printf("[ERROR load_data] Unknown type %s\n",node_name_get(node->type));break;
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
					//t_data *var_data=var->data;
					void  *var_data=var->data;

					// get new ptr
					d->pointer=var_data;
				}
				else
				{
					printf("[ERROR load_var] Can't find node for %s\n",d->name);
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
	// Bind new ptr data to node->data
	for(l=sc->tmp_node->first;l;l=l->next)
	{
		n=l->data;	// node

		// 0) BIND NODE CLS (MAKE NODE)

		//printf("load %s\n",node_name_get(n->type));

		node_init(n,n->type);

		// STORE OLD ID

		n->id_old=n->id;

		// 1) FIND DATA with node's id_chunk 

		void *ptr=NULL;

		if(n->type==nt_var)
		{
			// don't
			//n->data=NULL;
		}
		else
		{
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
	int file_size;		// size of bin

	t_chunk *c;

	// FILE
	FILE *file=fopen(path,"r");

	// FILE SIZE
	fseek (file,0,SEEK_END);
	file_size = (int)ftell(file);
	rewind(file);

	// NEW CHUNK
	c=chunk_new(ct_node,nt_null,0,0,NULL);

	log((LOG_READ,"File size:%d\n",file_size));
	log((LOG_READ,"[0]\n",file_size));

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

		log((LOG_READ,"[%d][%d]\t(+%d)\tmagic {%c}\n",limit,(int)ftell(file),(int)sizeof(char),magic));

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
			log((LOG_READ,"[%d]\t(+%d)\tchunk",(int)ftell(file),(int)sizeof(t_chunk)));
			log((LOG_READ," %s:%s:%d:%d\n",chunk_type_get(c->chunk_type),node_name_get(c->type),c->size,c->tot));

			// READ DATA

			// MALLOC
			data=malloc(c->size*c->tot);

			// READ
			fread(data,c->size,c->tot,file);

			log((LOG_READ,"[%d]\t(+%d)\tstruct %s:%s\n",(int)ftell(file),c->size,chunk_type_get(c->chunk_type),c->type));

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
// read .mn file
// add to current scene

void load_file(t_context *C,const char *path)
{

/** INIT **/

	// create a new scene
	t_scene *sc=scene_init();

	// init tmp lists
	sc->tmp_node=lst_new("tmp_node");
	sc->tmp_data=lst_new("tmp_data");

	// remove object camera

	t_node *node_object_camera=scene_node_get(C->scene,"object","main_camera");
	scene_node_free(C->scene,node_object_camera);

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

	if(C->event->load_error)
	{
		term_log("WARNING: Load Error");
	}
}


