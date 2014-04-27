/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "node.h"
#include "scene.h"
#include "log.h"
#include "data.h"
#include "list.h"
#include "util.h"
#include "memory.h"

// STATIC
t_scene *SCENE=NULL;

// tmp idcol
int COLOR[3];

/***	 GET	* **/

// get scene

t_scene *scene_get(void)
{
	return SCENE;
}

void scene_class_add( struct Scene *scene, int type, void *cls)
{
	t_node *node = node_new( dt_undefined);
	node->extra_type = type;
	node->data = cls;
	lst_add( scene->classes, node, "node");
}

void *scene_class_get( struct Scene *scene, int type)
{
	t_link *l;
	t_node *node;
	for( l = scene->classes->first; l; l = l->next)
	{
		node = l->data;
		if( node->extra_type == type)
		{
			return node->data;
		}
	}

	printf("[SCENE] Warning can't find class %d\n", type);

	return NULL;
}

void scene_store(t_scene *scene, int val)
{
	//if( scene->debug_all) printf("scene_store %d\n", val);
	if(val)
	{
		scene->store_stack++;
		scene->store = 1;
	}
	else
	{
		scene->store_stack--;
		if(scene->store_stack ==  0)
			scene->store = 0;
	}

	//if( scene->debug_all) printf("scene_store store=%d\n", scene->store);
}

// get lst

t_lst *scene_lst_get(t_scene *sc,const char *type)
{
	t_lst *lst=NULL;

	if(is(type,"screen"))  lst=sc->screens; 
	else if(is(type,"mesh"))  lst=sc->meshes; 
	else if(is(type,"block"))  lst=sc->blocks; 
	else if(is(type,"brick"))  lst=sc->bricks; 
	else if(is(type,"light"))  lst=sc->lights; 
	else if(is(type,"object"))  lst=sc->objects; 
	else if(is(type,"file"))  lst=sc->files; 
	else if(is(type,"material"))  lst=sc->materials; 
	else if(is(type,"list"))  lst=sc->lists; 
	else if(is(type,"link"))  lst=sc->links; 
	else if(is(type,"texture"))  lst=sc->textures; 
	else if(is(type,"camera"))  lst=sc->cameras; 
	else if(is(type,"dict"))  lst=sc->dicts; 
	else if(is(type,"symbols"))  lst=sc->symbols; 
	else if(is(type,"viewport"))  lst=sc->viewports; 
	else if(is(type,"set"))  lst=sc->sets; 
	else if(is(type,"rhizome"))  lst=sc->rhizomes; 

	if(lst)
	{
		return lst;
	}
	else
	{
		printf("[ERROR scene_lst_get] Unknown type:%s\n",type);
		return NULL;
	}
}

t_node *scene_get_data(t_scene *sc,void *ptr)
{
	t_link *l;
	t_node *n;
	t_data *d;

	for(l=sc->datas->first;l;l=l->next)
	{
		n = ( t_node *) l->data;
		d = ( t_data *) n->data;
		if(d->pointer==ptr) return n;
	}

	printf("[ERROR scene_get_data]\n");
	return NULL;
}

// ptr was mem_malloc'ed, find data var with ptr address

t_node *scene_get_var(t_scene *sc,void *ptr)
{
	t_link *l;
	t_node *n;

	for(l=sc->vars->first;l;l=l->next)
	{
		n = ( t_node *) l->data;
		if(n->id_ptr==ptr) return n;
	}

	printf("[ERROR scene_get_var]\n");
	return NULL;
}

// get node by type/name

t_node *scene_node_get(t_scene *sc,const char *type,const char *name)
{
	t_lst *lst=scene_lst_get(sc,type);

	if(lst)
	{
		return list_find_node_by_name(lst,name);
	}
	else
	{
		printf("[ERROR scene_node_get] Unknown type:%s\n",type);
		return NULL;
	}
}

t_node *scene_node_exists(t_scene *sc,const char *type,const char *name)
{
	t_lst *lst=scene_lst_get(sc,type);

	if(lst)
	{
		return list_find_node_by_name(lst,name);
	}
	else
	{
		return NULL;
	}
}


/***	 COLOR	***/



void scene_color_tmp_reset(t_scene *scene)
{
	bzero(COLOR,3);
}

// switch tmp mode

void scene_color_switch_mode(t_scene *sc)
{
	if(sc->use_tmp_colors)
	{
		sc->use_tmp_colors=0;
	}
	else
	{
		sc->use_tmp_colors=1;
		//bzero(COLOR,3);
		vcp3i( COLOR, sc->color);
	}
}

// set single id col

void scene_set_color(t_scene *sc)	
{
	int step = sc->step;
	int *color;

	if(sc->use_tmp_colors)
	{
		color=COLOR;
	}
	else
	{
		color=sc->color;
	}

	while(step>0)
	{
		color[0]++;

	   	if(color[0] > 255)
		{
			color[0] = 0;
			color[1]++;
			if(color[1] > 255)
			{
			     color[1] = 0;
			     color[2]++;
			}
		}
		step--;
	}
}

// get id col

void scene_color_get(t_scene *sc,int *color)
{
	int *col;

	if(sc->use_tmp_colors)
	{
		col=COLOR;
	}
	else
	{
		col=sc->color;
	}

	vseti(color,col[0],col[1],col[2]);

	// increment
	scene_set_color(sc);
}

void scene_switch_edit_mode(t_scene *scene, int state)
{
	if(scene->edit_mode &&! state)
	{
		scene->edit_mode = 0;
	}
	else if(!scene->edit_mode && state)
	{
		scene->edit_mode = 1;
	}

	//scene_color_switch_mode(scene);
}

/***	 ID	***/


int scene_id_get(t_scene *sc)
{
	sc->id++;
	return sc->id;
}

/***	 FREE	***/

void scene_node_free( t_scene *sc, t_node *node)
{
	//if( sc->debug_all) printf("scene_node_free %s \n",data_name_get(node->cls->type));

	// Remove from Lst
	lst_link_delete_by_id( node->cls->lst, node->id);
	lst_link_delete_by_id( sc->nodes, node->id);

	// Free Struct
	node->cls->free(sc,node);

	// Free Node
	mem_free(node, sizeof(t_node));
}


// cleanup (free stack)

void scene_cleanup(t_scene *sc)
{
	// lock scene
	sc->is_ready=0;
	t_link *link;

	for(link=sc->stack->first;link;link=link->next)
	{
		if(link)
		{
			t_node *node = ( t_node *) link->data;
			scene_node_free(sc,node);
		}
	}

	lst_cleanup(sc->stack);

	// unlock scene
	sc->is_ready=1;
}

// delete (add to stack)

int scene_node_delete(t_scene *sc,t_node *node)
{
	if(sc->is_ready)
	{
		lst_add(sc->stack,node,data_name_get(node->type));
		return 1;
	}
	else
	{
		return 0;
	}
}

void scene_delete( t_scene *sc, void *data)
{
	t_id *id = (t_id *) data;
	if( ( id->id > 0)  && ( id->id <= sc->id))
	{
		t_node *node = id->node;
		if( node) scene_node_free( sc, node);
		else printf("[WARNING] scene_delete: node is null\n");
	}
	else
	{
		printf("[WARNING] scene_delete: id (%d) is wrong\n", id->id);
	}
}


// free

void scene_free(t_scene *sc)
{
}


/***	 LOAD	***/

void scene_node_load(t_scene *sc,t_node *node)
{
	ulog((LOG_SCENE,"scene_node_load\n"));

	// INIT NODE && DATA
	node->cls->init(node);

	if(node->type==dt_var)
	{
		lst_add(node->cls->lst,node,data_name_get(node->type));
		lst_add(sc->nodes,node,"var");
	}
	else
	{
		t_id *id = (t_id *) node->data;
		lst_add(sc->nodes, node, id->name);
		// option have no lst
		if(node->cls->lst)
		{
			lst_add(node->cls->lst,node,data_name_get(node->type));
		}
	}
}




/***	ADD	***/



// node

t_node *scene_add_node(t_scene *sc,t_data_type type,const char *name)
{
	// new node
	t_node *node = node_new(type);

	// make node (init cls,size,lst)
	node_init(node,type);

	// build data 
	if( node->cls->build) node->data = node->cls->build( name);

	node->id_ptr = node->data;

	if(node->type != dt_var)
	{
		cls_node_id_add(node);
		cls_node_user_add(node);
	}

	// add to local list
	if(node->cls->lst) lst_add(node->cls->lst,node,name); 

	// add to nodes list
	lst_add(sc->nodes,node,name);

	// store data
	if(sc->store) node->store = 1; 

	return node;
}

// add regular struct

t_node *scene_add(t_scene *sc,t_data_type type,const char *name)
{
	ulog((LOG_SCENE,"scene_add %s %s \n",data_name_get(type),name));

	t_node *node=scene_add_node(sc,type,name);

	return node;
}

// data register (app_data)(app|func)

t_node *scene_add_data(t_scene *sc,const char *type,const char *target,const char *name,void *ptr)
{
	ulog((LOG_SCENE,"scene_add_data %s %s %s\n",type,target,name));
	//printf("scene_add_data %s %s %s\n",type,target,name);

	t_node *node=scene_add_node(sc,dt_data,name);
	t_data *data = ( t_data *) node->data;

	set_name(data->type,type);
	set_name(data->target,target);

	data->pointer=ptr;

	return node;
}

// data node (app_node)(object|)
// retrieve node's struct by struct's id

t_node *scene_add_data_node(t_scene *sc,const char *type,const char *target,const char *name,void *ptr)
{
	ulog((LOG_SCENE,"scene_add_node %s %s %s\n",type,target,name));

	t_node *node=scene_add_node(sc,dt_data,name);
	t_data *data = ( t_data *) node->data;

	set_name(data->type,type);
	set_name(data->target,target);

	data->pointer=ptr;

	t_node *n=(t_node *)ptr;
	data->id_node=n->id;

	return node;
}

void scene_remove_data_node(t_scene *sc,void *ptr)
{
	t_link *l;
	t_node *n;
	t_node *node=NULL;
	t_data *d;

	for(l=sc->datas->first;l;l=l->next)
	{
		n = ( t_node *) l->data;
		d = ( t_data *) n->data;

		if(d->pointer==ptr)
		{
			node=n;
			break;
		}
	}

	if(node)
	{
		scene_node_free(sc,node);
	}
	else
	{
		printf("[ERROR scene_remove_data_node] Can't find node\n");
	}

}

// data ref (struct_ref)(object|)(loc_x|)

t_node *scene_add_ref(t_scene *sc,const char *type,const char *target,const char *name,void *ptr,void *ref)
{
	ulog((LOG_SCENE,"scene_add_ref %s %s %s\n",type,target,name));

	t_node *node=scene_add_node(sc,dt_data,name);
	t_data *data = ( t_data *) node->data;

	set_name(data->type,type); 
	set_name(data->target,target);

	data->pointer=ptr;
	data->ref=ref;

	return node;
}

void scene_remove_ref(t_scene *sc,void *ptr)
{
	scene_remove_data_node(sc,ptr);
}

// data var (name)(name)(size)(ptr)
int node_garbage = 0;

void scene_add_data_var(t_scene *sc,const char *name,const char *name_var,int size,void *ptr)
{
	ulog((LOG_SCENE,"scene_add_data_var %s %s %d\n",name,name_var,size));

	// data 
	t_node *node_data=scene_add_data(sc,"dynamic","void",name,ptr);
	if( node_garbage) node_data->garbage = 1;
	t_data *data = ( t_data *) node_data->data;

	// store size
	data->size=size;

	// VAR
	t_node *node_var=scene_add(sc,dt_var,"var");
	if( node_garbage) node_var->garbage = 1;
	node_var->data=ptr;
	node_var->id_ptr=ptr;
	node_var->size = size;
}

/***	INIT	***/

t_scene *scene_new(void)
{
	// new scene
	t_scene *sc = (t_scene *)mem_malloc(sizeof(t_scene));

	// init
	sc->id=1;
	sc->store=0;
	sc->step=1;
	vset3i(sc->color,0,0,0);
	vset3i(COLOR,0,0,0);
	sc->use_tmp_colors=0;
	sc->selected=NULL;
	sc->selection = NULL;
	sc->hover=NULL;
	sc->hover_type = dt_null;
	sc->is_ready=1;
	sc->has_generic_viewport = 0;
	sc->store_stack = 0;
	sc->edit_mode = 0;
	sc->debug_all = 0;

	// build lists
	sc->nodes=lst_new("nodes");
	sc->blocks=lst_new("blocks");	
	sc->bricks=lst_new("bricks");	
	sc->textures=lst_new("textures");	
	sc->materials=lst_new("materials");
	sc->files=lst_new("files");	
	sc->meshes=lst_new("meshes");	
	sc->cameras=lst_new("cameras");	
	sc->lights=lst_new("lights");	
	sc->objects=lst_new("objects");
	sc->screens=lst_new("screens");
	sc->lists=lst_new("lists");
	sc->links=lst_new("links");
	sc->datas=lst_new("datas");
	sc->vars=lst_new("vars");
	sc->images=lst_new("images");
	sc->stack=lst_new("stack");
	sc->selections=lst_new("selections");
	sc->vlst=lst_new("vlst");
	sc->dicts=lst_new("dicts");
	sc->symbols=lst_new("symbols");
	sc->vectors=lst_new("vectors");
	sc->viewports=lst_new("viewports");
	sc->sets=lst_new("sets");
	sc->bindings=lst_new("bindings");
	sc->rhizomes=lst_new("rhizomes");
	sc->graphs=lst_new("graphs");
	sc->geos=lst_new("geos");
	sc->datums = lst_new("datums");
	sc->classes = lst_new("classes");

	sc->tmp_colors=lst_new("tmp_colors");

	// load file
	sc->tmp_node=NULL;
	sc->tmp_data=NULL;
	
	return sc;
}

void scene_init(t_scene *scene)
{
	// Store Localy
	SCENE = scene;
}
