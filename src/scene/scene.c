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
#include "base.h"
#include "memory.h"

#define CLASS_MAX 128

// STATIC
t_scene *SCENE=NULL;

// Static TMP COLOR
int COLOR[3];

/*
	****************************
	CLASSES
	****************************
*/

// Classes Pointer List
t_node_class *classes[ CLASS_MAX];

// Add a Class Pointer to classes List 
void scene_class_init( t_scene *scene, t_data_type type, t_node_class *cls)
{
	classes[ type] = cls;
}

// Add Extra (external) Class (sc->classes)
void scene_class_add( struct Scene *scene, int type, void *cls)
{
	t_node *node = node_new( dt_undefined);
	node->extra_type = type;
	node->data = cls;
	lst_add( scene->classes, node, "node");
}

// Get a Pointer to a Class
t_node_class *scene_class_get( t_scene *scene, t_data_type type)
{
	return classes[ type];
}

// Get a Pointer to an Externa Class (sc->classes)
void *scene_class_extra_get( struct Scene *scene, int type)
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

/*
	****************************
	STORE
	****************************
*/


// Set Store State ( manage multiple opening/closing states)
void scene_store(t_scene *scene, int val)
{
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
}

/*
	****************************
	GET
	****************************
*/

// Return Scene Pointer
t_scene *scene_get( void)
{
	return SCENE;
}

// Get Lst by Node Type
t_lst *scene_lst_get( t_scene *sc, t_data_type type)
{
	t_node_class *cls = classes[ type];
	return cls->lst;
}

// Retrieve a Node Data 
t_node *scene_get_data( t_scene *sc, void *ptr)
{
	t_link *l;
	t_node *n;
	t_data *d;

	// Get the Data type List
	t_lst *lst = scene_lst_get( sc, dt_data);

	// Compare ptr with data->pointer
	for(l=lst->first;l;l=l->next)
	{
		n = ( t_node *) l->data;
		d = ( t_data *) n->data;
		if(d->pointer==ptr) return n;
	}

	printf("[ERROR scene_get_data]\n");
	return NULL;
}

// Retrieve a Node Var 
t_node *scene_get_var(t_scene *sc,void *ptr)
{
	t_link *l;
	t_node *n;

	t_lst *lst = scene_lst_get( sc, dt_var);
	for(l=lst->first;l;l=l->next)
	{
		n = ( t_node *) l->data;
		if(n->id_ptr==ptr) return n;
	}

	printf("[ERROR scene_get_var]\n");
	return NULL;
}

// Get A Node by Name (With Warning Message) 
t_node *scene_node_get( t_scene *sc, t_data_type type, const char *name)
{
	t_lst *lst=scene_lst_get(sc,type);

	if(lst)
	{
		return list_find_node_by_name(lst,name);
	}
	else
	{
		printf("[ERROR scene_node_get] Unknown type:%s\n", data_name_get( type));
		return NULL;
	}
}

// Get A Node by Name  
t_node *scene_node_exists( t_scene *sc, t_data_type type, const char *name)
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

/*
	****************************
	COLORS ID
	****************************
*/

// Switch between Regular / tmp color mode 
void scene_color_switch_mode(t_scene *sc)
{
	if(sc->use_tmp_colors)
	{
		sc->use_tmp_colors=0;
	}
	else
	{
		sc->use_tmp_colors=1;
		vcp3i( COLOR, sc->color);
	}
}

// Set single id col
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
}

/*
	****************************
	ID
	****************************
*/


int scene_id_get(t_scene *sc)
{
	sc->id++;
	return sc->id;
}

/*
	****************************
	DELETE NODES
	****************************
*/

void scene_node_free( t_scene *sc, t_node *node)
{
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

void scene_free(t_scene *sc)
{
}

/*
	****************************
	LOAD NODES
	****************************
*/

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

/*
	****************************
	ADD NODES
	****************************
*/


// Add Node
t_node *scene_add_node(t_scene *sc,t_data_type type,const char *name)
{
	// New node
	t_node *node = node_new(type);

	// Get Class Pointer
	node->cls = scene_class_get( sc, type);

	// Init List
	if( !node->cls->lst) node->cls->lst = lst_new("lst"); 

	// Build Data 
	if( node->cls->build) node->data = node->cls->build( name);

	// Store Data Address
	node->id_ptr = node->data;

	// Init ID & User
	if(node->type != dt_var)
	{
		cls_node_id_add(node);
		cls_node_user_add(node);
	}

	// Add to (local) Class List
	if(node->cls->lst) lst_add(node->cls->lst,node,name); 

	// Add to (global) Nodes List
	lst_add(sc->nodes,node,name);

	// Set Store State
	if(sc->store) node->store = 1; 

	return node;
}

// Add Node 
t_node *scene_add(t_scene *sc,t_data_type type,const char *name)
{
	ulog((LOG_SCENE,"scene_add %s %s \n",data_name_get(type),name));

	t_node *node = scene_add_node(sc,type,name);

	return node;
}

// Add Data Node 
t_node *scene_add_data( t_scene *sc, const char *type, const char *target, const char *name, void *ptr)
{
	ulog((LOG_SCENE,"scene_add_data %s %s %s\n",type,target,name));

	t_node *node = scene_add_node( sc, dt_data, name);
	t_data *data = ( t_data *) node->data;

	set_name( data->type, type);
	set_name( data->target, target);

	data->pointer = ptr;

	return node;
}

// data node (app_node)(object|)
// retrieve node's struct by struct's id

t_node *scene_add_data_node(t_scene *sc,const char *type,const char *target,const char *name,void *ptr)
{
	// Add Data Node
	t_node *node = scene_add_data( sc, type, target, name, ptr);
	t_data *data = ( t_data *) node->data;

	t_node *n = ( t_node *) ptr;
	data->id_node = n->id;

	return node;
}

void scene_remove_data_node(t_scene *sc,void *ptr)
{
	t_link *l;
	t_node *n;
	t_node *node=NULL;
	t_data *d;

	t_lst *lst = scene_lst_get( sc, dt_data);
	for(l=lst->first;l;l=l->next)
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

void scene_add_data_var(t_scene *sc,const char *name,const char *name_var,int size,void *ptr)
{
	ulog((LOG_SCENE,"scene_add_data_var %s %s %d\n",name,name_var,size));

	// data 
	t_node *node_data=scene_add_data(sc,"dynamic","void",name,ptr);
	t_data *data = ( t_data *) node_data->data;

	// store size
	data->size=size;

	// VAR
	t_node *node_var=scene_add(sc,dt_var,"var");
	node_var->data=ptr;
	node_var->id_ptr=ptr;
	node_var->size = size;
}

/*
	****************************
	NEW SCENE
	****************************
*/

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
	sc->selection_swap = NULL;
	sc->hover=NULL;
	sc->hover_type = dt_null;
	sc->is_ready=1;
	sc->has_generic_viewport = 0;
	sc->store_stack = 0;
	sc->edit_mode = 0;
	sc->debug_all = 0;

	// build lists
	sc->nodes=lst_new("nodes");
	sc->stack=lst_new("stack");
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

	// Fill Classes Pointers
	node_classes_init( scene);
}
