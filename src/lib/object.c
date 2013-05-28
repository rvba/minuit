 /*
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "op.h"
#include "ctx.h"
#include "mesh.h"
#include "object.h"
#include "camera.h"
#include "list.h"

void object_default(t_node *node){}
void cls_object_link(t_object *self,t_node *target);
void cls_object_show(t_object *object);
void cls_object_init(t_object *object)
{
}

t_object_class object_generic =
{
	.cls_type="object",
	.init=cls_object_init,
	.type="object",
	.link=cls_object_link,
	.show=cls_object_show,
	.draw=cls_object_draw_mesh
};

t_object_class object_light =
{
	.cls_type="object",
	.init=cls_object_init,
	.type="light",
	.link=cls_object_link,
	.show=cls_object_show,
	.draw=cls_object_draw_light
};

t_object_class object_camera =
{
	.cls_type="object",
	.init=cls_object_init,
	.type="camera",
	.link=cls_object_link,
	.show=cls_object_show,
	.draw=cls_object_draw_camera
};

void cls_object_show(t_object *object)
{
	printf("[OBJECT]\n");
	printf("name:[%s]\n",object->name);
	printf("idcol:");vprint3i(object->idcol,'\n');
	printf("loc:");vprint3f(object->loc,'\n');
	printf("rot:");vprint3f(object->rot,'\n');
	printf("size:");vprint3f(object->size,'\n');
}

void cls_object_link(t_object *self,t_node *target)
{
	if(target->type==nt_mesh)
	{
		t_mesh *mesh=target->data;
		self->mesh=mesh;
		target->users++;

	}
	else if(target->type==nt_list)
	{
		t_lst *lst=target->data;
		self->blocks=lst;
		target->users++;
	}
	else if(target->type==nt_camera)
	{
		t_camera *camera=target->data;
		self->data=camera;
		target->users++;
	}
	else printf("[ERROR:cls_object_link] Unknown node type %s",node_name_get(target->type));
}

void object_build(t_object *object,const char *type)
{
	if(is(type,"mesh"))
	{
		object->cls=&object_generic;
	}
	else if(is(type,"light"))
	{
		object->cls=&object_light;
	}
	else if(is(type,"camera"))
	{
		object->cls=&object_camera;
	}
	else
	{
		printf("[ERROR object_build] Unknown type %s\n",type);
	}

	set_name(object->type,type);
}


void object_draw_add(t_node *node,void (* func)(t_node *node))
{
	t_object *object = node->data;
	object->draw=func;
}

void object_method_add(t_node *node,const char method[],void (*func)(t_node *n))
{
	t_object *object = node->data;
	if(is(method,"action"))
	{
		object->action=func;
	}
	if(is(method,"update"))
	{
		object->update=func;
	}
}

void object_data_add(t_node *node,void *ptr)
{
	t_object *object = node->data;
	object->data=ptr;
}

// FREE

void object_free(t_object *object)
{
	t_context *C=ctx_get();
	t_scene *sc=C->scene;

	// free data
	t_node *node=scene_node_get_by_id_global(sc,object->id);
	scene_remove_data_node(sc,node);

	// free ref block 
	if(object->ref)
	{
		// remove from global list
		t_lst *lst = get_target_list(C);
		list_remove_by_ptr(lst,object->ref);
		// free block
		scene_struct_delete(sc,object->ref);
	}

	if(object->mesh) scene_struct_delete(sc,object->mesh);
	if(object->data) scene_struct_delete(sc,object->data);
	if(object->blocks)
	{
		list_cleanup(object->blocks);
		scene_struct_delete(sc,object->blocks);
	}

	free(object);
}

// REF

void *object_get_ref(t_object *object, const char *ref)
{
	void *p;

	if(is(ref,"loc_x"))  p=&object->loc[0]; 
	else if(is(ref,"loc_y"))  	p=&object->loc[1]; 
	else if(is(ref,"loc_z"))  	p=&object->loc[2]; 
	else if(is(ref,"rot_x"))  	p=&object->rot[0]; 
	else if(is(ref,"rot_y"))  	p=&object->rot[1]; 
	else if(is(ref,"rot_z"))  	p=&object->rot[2]; 
	else if(is(ref,"scl_x"))  	p=&object->size[0]; 
	else if(is(ref,"scl_y"))  	p=&object->size[1]; 
	else if(is(ref,"scl_z"))  	p=&object->size[2]; 
	else
	{
		printf("[ERROR object_get_ref] Unknown ref %s \n",ref);
		return NULL;
	}

	return p;
}

// REBIND

t_object *object_rebind(t_scene *sc,void *ptr)
{
	t_object *object=(t_object *)ptr;

	object_build(object,object->type);
	
	rebind(sc,"object","mesh",(void **)&object->mesh);
	rebind(sc,"object","blocks",(void **)&object->blocks);
	rebind(sc,"object","ref",(void **)&object->ref);
	rebind(sc,"object","data",(void **)&object->data);

	object->action=NULL;
	object->update=NULL;

	return object;
}

// ADD

t_node *object_add(const char *type,const char *name)
{
	t_context *C = ctx_get();

	// make object
	t_node *node=object_make(type,name);
	t_object *object=node->data;

	// add data node
	scene_add_data_node(C->scene,"app_node","object",name,node);

	// add global reference
	t_node *node_block=block_make(name,"block");
	t_block *block=node_block->data;
	object->ref=block;

	// add selector
	add_part_selector(C,block,name,node);

	// add to global list
	op_add_global(C,block);

	// outline
	block->state.draw_outline=1;

	return node;
}

// MAKE

t_node *object_make(const char *type,const char *name)
{
	t_context *C = ctx_get();

	// new object
	t_node *node = scene_add(C->scene,nt_object,name);
	t_object *object=node->data;

	// build cls
	object_build(object,type);

	// new list (bricks list)
	t_node *list = scene_add(C->scene,nt_list,"object_blocks");

	// bind list
	object->cls->link(object,list);

	return node;
}

// NEW

t_object *object_new(const char *name)
{
	t_object *object = (t_object *)malloc(sizeof(t_object));

	object->id=0;
	object->id_chunk=0;
	set_name(object->name,name);
	object->users=0;

	vset(object->loc,0,0,0);
	vset(object->rot,0,0,0);
	vset(object->size,1,1,1);
	vseti(object->idcol,0,0,0);
	
	object->mesh=NULL;
	object->data=NULL;
	object->blocks=NULL;
	object->ref=NULL;

	object->is_selected=0;

	object->update=object_default;
	object->action=NULL;
	object->draw=NULL;

	return object;
}
