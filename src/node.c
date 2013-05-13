/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"

int node_make_mesh(t_node *node);
int node_make_block(t_node *node);
int node_make_brick(t_node *node);
int node_make_light(t_node *node);
int node_make_object(t_node *node);
int node_make_screen(t_node *node);
int node_make_file(t_node *node);
int node_make_image(t_node *node);
int node_make_material(t_node *node);
int node_make_list(t_node *node);
int node_make_link(t_node *node);
int node_make_data(t_node *node);
int node_make_texture(t_node *node);
int node_make_var(t_node *node);
int node_make_option(t_node *node);
int node_make_vlst(t_node *node);
int node_make_camera(t_node *node);
int node_make_dict(t_node *node);
int node_make_symbol(t_node *node);
int node_make_vector(t_node *node);
int node_make_viewport(t_node *node);
int node_make_set(t_node *node);

char *node_name_null="node_null";
char *node_name_brick="node_brick";
char *node_name_mesh="node_mesh";
char *node_name_block="node_block";
char *node_name_light="node_light";
char *node_name_object="node_object";
char *node_name_screen="node_screen";
char *node_name_file="node_file";
char *node_name_image="node_image";
char *node_name_material="node_material";
char *node_name_list="node_list";
char *node_name_link="node_link";
char *node_name_data="node_data";
char *node_name_texture="node_texture";
char *node_name_var="node_var";
char *node_name_option="node_option";
char *node_name_vlst="node_vlst";
char *node_name_camera="node_camera";
char *node_name_dict="node_dict";
char *node_name_symbol="node_symbol";
char *node_name_vector="node_vector";
char *node_name_viewport="node_viewport";
char *node_name_set="node_set";

char *node_name_get(t_node_type type)
{
	switch(type)
	{
		case(nt_null):return node_name_null;break;
		case(nt_brick):return node_name_brick;break;
		case(nt_mesh):return node_name_mesh;break;
		case(nt_block):return node_name_block;break;
		case(nt_light):return node_name_light;break;
		case(nt_object):return node_name_object;break;
		case(nt_screen):return node_name_screen;break;
		case(nt_file):return node_name_file;break;
		case(nt_image):return node_name_image;break;
		case(nt_material):return node_name_material;break;
		case(nt_list):return node_name_list;break;
		case(nt_link):return node_name_link;break;
		case(nt_data):return node_name_data;break;
		case(nt_texture):return node_name_texture;break;
		case(nt_var):return node_name_var;break;
		case(nt_option):return node_name_option;break;
		case(nt_vlst):return node_name_vlst;break;
		case(nt_camera):return node_name_camera;break;
		case(nt_dict):return node_name_dict;break;
		case(nt_symbol):return node_name_symbol;break;
		case(nt_vector):return node_name_vector;break;
		case(nt_viewport):return node_name_viewport;break;
		case(nt_set):return node_name_set;break;
		default:printf("[ERROR node_name_get] Unknown type %d\n",type);return node_name_null;break;
	}
}

int dlink(const char *type,void *ptr)
{
	t_generic *g=(t_generic *)ptr;
	g->users--;
	if(g->users<=0) 
	{
	}
	return 0;
}

// GET REF

void *cls_node_get_ref_object(t_node *node, const char *ref)
{
	return object_get_ref(node->data, ref);
}

void *cls_node_get_ref_brick(t_node *node, const char *ref)
{
	return brick_get_ref(node->data, ref);
}

void *cls_node_get_ref_material(t_node *node, const char *ref)
{
	return material_get_ref(node->data, ref);
}

void *cls_node_get_ref_mesh(t_node *node, const char *ref)
{
	return mesh_get_ref(node->data, ref);
}

void *cls_node_get_ref_vlst(t_node *node, const char *ref)
{
	return vlst_get_ref(node->data, ref);
}

void *cls_node_get_ref_camera(t_node *node, const char *ref)
{
	return camera_get_ref(node->data, ref);
}

void *cls_node_get_ref(t_node *node, const char *ref)
{
	printf("get ref not implemented\n");
	return NULL;
}

// BUILD

void cls_node_id_add(t_node *node)
{
	t_context *C=ctx_get();
	t_generic *g=(t_generic *)node->data;

	int id=scene_id_get(C->scene);
	node->id=id;
	g->id=id;
}

void cls_node_user_add(t_node *node)
{
	t_generic *g = (t_generic *) node->data;
	g->users++;
}

void cls_node_build_var(t_node *node,const char *name)
{
	t_context *C=ctx_get();

	// get node's ID only
	int id=scene_id_get(C->scene);
	node->id=id;
}

void cls_node_build(t_node *node,const char *name)
{
	void *p;

	switch(node->type)
	{
		case nt_mesh:		p = mesh_new(name);
			break;
		case nt_block:		p = block_new(name);
			break;
		case nt_brick:		p = brick_new(name);
			break;
		case nt_light:		p = light_new(name);
			break;
		case nt_object:		p = object_new(name);
			break;
		case nt_screen:		p = screen_new(name);
			break;
		case nt_file:		p = file_new(name);
			break;
		case nt_image:		p = image_new(name);
			break;
		case nt_material:	p = material_new(name);
			break;
		case nt_list:		p = lst_new(name);
			break;
		case nt_link:		p = link_new(name);
			break;
		case nt_data:		p = data_new(name);
			break;
		case nt_texture:	p = texture_new(name);
			break;
		case nt_option:		p = option_new(name);
			break;
		case nt_vlst:		p = vlst_new(name);
			break;
		case nt_camera:		p = camera_new(name);
			break;
		case nt_dict:		p = dict_new(name);
			break;
		case nt_symbol: 	p = symbol_new(name);
			break;
		case nt_vector: 	p = vector_new(name);
			break;
		case nt_viewport: 	p = viewport_new(name);
			break;
		case nt_set:	 	p = set_new(name);
			break;

		case nt_null: 
			printf("[WARNING node_cls_build] null node\n");
			break;
		case nt_var: 
			cls_node_build_var(node,name);
			break;

		default:
			printf("[ERROR cls_node_build] Unkown type\n");
			break;
	}

	node->data=p;
	node->id_ptr=node->data;
	cls_node_id_add(node);
	cls_node_user_add(node);

}

// SELECT

void cls_node_del(t_node *node) { t_scene *scene  = scene_get(); scene_node_delete(scene,node);} 
void cls_node_set_state_selected(t_node *node,int i) {}
void cls_node_light_set_state_selected(t_node *node,int state)
{
	t_light *light=node->data;
	light->is_selected=state;
}
void cls_node_mesh_set_state_selected(t_node *node,int state)
{
	t_mesh *mesh=node->data;
	mesh->state.is_selected=state;
}

void cls_node_object_set_state_selected(t_node *node,int state)
{
	t_object *object=node->data;
	object->is_selected=state;
}

// MOUSE OVER

int cls_node_is_mouse_over(t_node *node)
{
	return 0;
};

int cls_node_object_is_mouse_over(t_node *node)
{
	t_context *C=ctx_get();
	t_object *object=node->data;
	t_mesh *mesh=object->mesh;

	if(mesh)
	{
		if(is_mouse_over(C,mesh->idcol))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int cls_node_brick_is_mouse_over(t_node *node)
{
	t_context *C=ctx_get();
	t_brick *brick=node->data;

	if(is_mouse_over_brick(C,brick))
	{
		brick->state.is_mouse_over=1;
		return 1;
	}
	else
	{
		brick->state.is_mouse_over=0;
		return 0;
	}
}

// LINK

void cls_node_link(t_node *node)
{
}


// FREE

void cls_node_light_free(t_scene *sc,t_node *node) {}
void cls_node_screen_free(t_scene *sc,t_node *node) {}
void cls_node_file_free(t_scene *sc,t_node *node) {}
void cls_node_image_free(t_scene *sc,t_node *node) {}
void cls_node_material_free(t_scene *sc,t_node *node) {}
void cls_node_option_free(t_scene *sc,t_node *node) {}

void cls_node_texture_free(t_scene *sc,t_node *node)
{
	texture_free(node->data);
}

void cls_node_vector_free(t_scene *sc,t_node *node)
{
	vector_free(node->data);
}

void cls_node_list_free(t_scene *sc,t_node *node)
{
	list_free(node->data);
}

void cls_node_camera_free(t_scene *sc,t_node *node)
{
	free(node->data);
}

void cls_node_mesh_free(t_scene *sc,t_node *node)
{
	mesh_free(node->data);
}

void cls_node_brick_free(t_scene *sc,t_node *node)
{
	brick_free(node->data);
}

void cls_node_block_free(t_scene *sc,t_node *node)
{
	block_free(node->data);
}

void cls_node_object_free(t_scene *sc,t_node *node)
{
	object_free(node->data);
}

void cls_node_data_free(t_scene *sc,t_node *node)
{
	data_free(node->data);
}

void cls_node_vlst_free(t_scene *sc,t_node *node)
{
	vlst_free(node->data); 
}

void cls_node_link_free(t_scene *sc,t_node *node)
{
	_link_free(node->data); 
}

void cls_node_dict_free(t_scene *sc,t_node *node)
{
	dict_free(node->data);
}

void cls_node_symbol_free(t_scene *sc,t_node *node)
{
	symbol_free(node->data);
}

void cls_node_var_free(t_scene *sc,t_node *node)
{
	// freed elsewhere
}

void cls_node_viewport_free(t_scene *sc,t_node *node)
{
	viewport_free(node->data);
}

void cls_node_set_free(t_scene *sc,t_node *node)
{
	set_free(node->data);
}

// INIT

// get Scene ID
void cls_node_init_generic(t_node *node)
{
	t_context *C=ctx_get();

	int id=scene_id_get(C->scene);
	t_generic *g=(t_generic *)node->data;

	// node
	node->id=id;
	// data
	g->id=id;
}

void cls_node_init_block(t_node *node)
{
	cls_node_init_generic(node);
	t_block *block=node->data;
	block->cls->init(block);
}

void cls_node_init_brick(t_node *node)
{
	cls_node_init_generic(node);
	t_brick *brick=node->data;
	brick->cls->init(brick);
}

void cls_node_init_mesh(t_node *node)
{
	cls_node_init_generic(node);
	t_mesh *mesh=node->data;
	t_context *C=ctx_get();
	mesh_init(C->scene,mesh);
}

void cls_node_init_object(t_node *node)
{
	cls_node_init_generic(node);
	t_object *object=node->data;
	object->cls->init(object);
}

void cls_node_init_var(t_node *node)
{
	// node var has direct data
	// no generic id
	// no init

	t_context *C=ctx_get();
	int id=scene_id_get(C->scene);
	node->id=id;
}

void cls_node_init_camera(t_node *node)
{
	cls_node_init_generic(node);
}

void cls_node_init_dict(t_node *node)
{
	cls_node_init_generic(node);
}

void cls_node_init_symbol(t_node *node)
{
	cls_node_init_generic(node);
}

void cls_node_init_vector(t_node *node)
{
	cls_node_init_generic(node);
	t_vector *vector=node->data;
	vector->cls->init(vector);
}

void cls_node_init_viewport(t_node *node)
{
	cls_node_init_generic(node);
}

void cls_node_init_set(t_node *node)
{
	cls_node_init_generic(node);
}

// CLASSES

t_node_class mesh= {
	.type=nt_mesh,
	.size=0,
	.lst=NULL,
	.make=node_make_mesh,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_mesh,
	.set_state_selected=cls_node_mesh_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_mesh_free,
	.get_ref = cls_node_get_ref_mesh,
};

t_node_class block= {
	.type=nt_block,
	.size=0,
	.lst=NULL,
	.make=node_make_block,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_block,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_block_free,
	.get_ref = cls_node_get_ref,
};

t_node_class brick= {
	.type=nt_brick,
	.size=0,
	.lst=NULL,
	.make=node_make_brick,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_brick,
	.is_mouse_over=cls_node_brick_is_mouse_over,
	.free=cls_node_brick_free,
	.get_ref = cls_node_get_ref_brick,
};

t_node_class light= {
	.type=nt_light,
	.size=0,
	.lst=NULL,
	.make=node_make_light,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_light_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_light_free,
	.get_ref = cls_node_get_ref,
};

t_node_class object= {
	.type=nt_object,
	.size=0,
	.lst=NULL,
	.make=node_make_object,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_object_set_state_selected,
	.is_mouse_over=cls_node_object_is_mouse_over,
	.free=cls_node_object_free,
	.get_ref = cls_node_get_ref_object,
};

t_node_class screen= {
	.type=nt_screen,
	.size=0,
	.lst=NULL,
	.make=node_make_screen,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_screen_free,
	.get_ref = cls_node_get_ref,
};

t_node_class file= {
	.type=nt_file,
	.size=0,
	.lst=NULL,
	.make=node_make_file,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_file_free,
	.get_ref = cls_node_get_ref,
};

t_node_class image= {
	.type=nt_image,
	.size=0,
	.lst=NULL,
	.make=node_make_image,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_image_free,
	.get_ref = cls_node_get_ref,
};

t_node_class material= {
	.type=nt_material,
	.size=0,
	.lst=NULL,
	.make=node_make_material,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_material_free,
	.get_ref = cls_node_get_ref_material,
};

t_node_class list= {
	.type=nt_list,
	.size=0,
	.lst=NULL,
	.make=node_make_list,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_list_free,
	.get_ref = cls_node_get_ref,
};

t_node_class _link_= {
	.type=nt_link,
	.size=0,
	.lst=NULL,
	.make=node_make_link,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_link_free,
	.get_ref = cls_node_get_ref,
};

t_node_class data= {
	.type=nt_data,
	.size=0,
	.lst=NULL,
	.make=node_make_data,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_data_free,
	.get_ref = cls_node_get_ref,
};

t_node_class texture= {
	.type=nt_texture,
	.size=0,
	.lst=NULL,
	.make=node_make_texture,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_texture_free,
	.get_ref = cls_node_get_ref,
};

t_node_class var= {
	.type=nt_var,
	.size=0,
	.lst=NULL,
	.make=node_make_var,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_var,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_var_free,
	.get_ref = cls_node_get_ref,
};

t_node_class option= {
	.type=nt_option,
	.size=0,
	.lst=NULL,
	.make=node_make_option,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_option_free,
	.get_ref = cls_node_get_ref,
};

t_node_class vlst= {
	.type=nt_vlst,
	.size=0,
	.lst=NULL,
	.make=node_make_vlst,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_vlst_free,
	.get_ref = cls_node_get_ref_vlst,
};

t_node_class camera= {
	.type=nt_camera,
	.size=0,
	.lst=NULL,
	.make=node_make_camera,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_camera,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_camera_free,
	.get_ref = cls_node_get_ref_camera,
};

t_node_class dict= {
	.type=nt_dict,
	.size=0,
	.lst=NULL,
	.make=node_make_dict,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_generic,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_dict_free,
	.get_ref = cls_node_get_ref,
};

t_node_class symbol= {
	.type=nt_symbol,
	.size=0,
	.lst=NULL,
	.make=node_make_symbol,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_symbol,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_symbol_free,
	.get_ref = cls_node_get_ref,
};

t_node_class vector= {
	.type=nt_vector,
	.size=0,
	.lst=NULL,
	.make=node_make_vector,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_vector,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_vector_free,
	.get_ref = cls_node_get_ref,
};

t_node_class viewport= {
	.type=nt_viewport,
	.size=0,
	.lst=NULL,
	.make=node_make_viewport,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_viewport,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_viewport_free,
	.get_ref = cls_node_get_ref,
};

t_node_class set = {
	.type=nt_set,
	.size=0,
	.lst=NULL,
	.make=node_make_set,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_set,
	.set_state_selected=cls_node_set_state_selected,
	.is_mouse_over=cls_node_is_mouse_over,
	.free=cls_node_set_free,
	.get_ref = cls_node_get_ref,
};


// MAKE

int node_make_mesh(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&mesh;
	node->cls->size=sizeof(t_mesh);
	node->cls->lst=C->scene->meshes;
	node->type = nt_mesh;
	return 1;
}

int node_make_block(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&block;
	node->cls->size=sizeof(t_block);
	node->cls->lst=C->scene->blocks;
	node->type = nt_block;
	return 1;
}

int node_make_brick(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&brick;
	node->cls->size=sizeof(t_brick);
	node->cls->lst=C->scene->bricks;
	return 1;
}

int node_make_light(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&light;
	node->cls->size=sizeof(t_light);
	node->cls->lst=C->scene->lights;
	node->type = nt_light;
	return 1;
}

int node_make_object(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&object;
	node->cls->size=sizeof(t_object);
	node->cls->lst=C->scene->objects;
	return 1;
}

int node_make_screen(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&screen;
	node->cls->size=sizeof(t_screen);
	node->cls->lst=C->scene->screens;
	node->type = nt_screen;
	return 1;
}

int node_make_file(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&file;
	node->cls->size=sizeof(t_file);
	node->cls->lst=C->scene->files;
	return 1;
}

int node_make_image(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&image;
	node->cls->size=sizeof(t_image);
	node->cls->lst=C->scene->images;
	node->type = nt_image;
	return 1;
}

int node_make_material(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&material;
	node->cls->size=sizeof(t_material);
	node->cls->lst=C->scene->materials;
	return 1;
}

int node_make_list(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&list;
	node->cls->size=sizeof(t_lst);
	node->cls->lst=C->scene->lists;
	node->type = nt_list;
	return 1;
}

int node_make_link(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&_link_;
	node->cls->size=sizeof(t_link);
	node->cls->lst=C->scene->links;
	return 1;
}

int node_make_texture(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&texture;
	node->cls->size=sizeof(t_texture);
	node->cls->lst=C->scene->textures;
	node->type = nt_texture;
	return 1;
}

int node_make_data(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&data;
	node->cls->size=sizeof(t_data);
	node->cls->lst=C->scene->datas;
	node->type = nt_data;
	return 1;
}

int node_make_var(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&var;
	node->cls->size=0; // variable,dynamic size
	node->cls->lst=C->scene->vars;
	node->type = nt_var;
	return 1;
}

int node_make_option(t_node *node)
{
	node->cls=&option;
	node->cls->size=sizeof(t_option);
	node->cls->lst=NULL;
	node->type=nt_option;
	return 1;
}

int node_make_vlst(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&vlst;
	node->cls->size=sizeof(t_vlst);
	node->cls->lst=C->scene->vlst;
	node->type = nt_vlst;
	return 1;
}

int node_make_camera(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&camera;
	node->cls->size=sizeof(t_camera);
	node->cls->lst=C->scene->cameras;
	node->type = nt_camera;
	return 1;
}

int node_make_dict(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&dict;
	node->cls->size=sizeof(t_dict);
	node->cls->lst=C->scene->dicts;
	node->type = nt_dict;
	return 1;
}

int node_make_symbol(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&symbol;
	node->cls->size=sizeof(t_symbol);
	node->cls->lst=C->scene->symbols;
	node->type = nt_symbol;
	return 1;
}

int node_make_vector(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&vector;
	node->cls->size=sizeof(t_vector);
	node->cls->lst=C->scene->vectors;
	node->type = nt_vector;
	return 1;
}

int node_make_viewport(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&viewport;
	node->cls->size=sizeof(t_viewport);
	node->cls->lst=C->scene->viewports;
	node->type = nt_viewport;
	return 1;
}

int node_make_set(t_node *node)
{
	t_context *C=ctx_get();
	node->cls=&set;
	node->cls->size=sizeof(t_set);
	node->cls->lst=C->scene->sets;
	node->type = nt_set;
	return 1;
}

// TYPES

t_node_class *nodes[] = {
				&mesh,
				&block,
				&brick,
				&light,
				&object,
				&screen,
				&file,
				&image,
				&material,
				&list,
				&_link_,
				&data,
				&texture,
				&var,
				&option,
				&vlst,
				&camera,
				&dict,
				&symbol,
				&vector,
				&viewport,
				&set,
			};


// INIT 

void cls_node_init(t_node *node,t_node_type type)
{
	int i;
	int done=0;
	int n = sizeof(nodes)/sizeof(nodes[0]);

	for(i=0;i<n;i++)
	{
		if(type==nodes[i]->type)
		{
			nodes[i]->make(node);
			done=1;
		}
	}

	if(!done) 
	{
		printf("[ERROR:node_make] Unknown node type [%s]\n",node_name_get(type));
	}

}

void node_init(t_node *node,t_node_type type)
{
	cls_node_init(node,type);
}

// NEW

t_node *node_new(t_node_type type)
{
	t_node *node = (t_node *)malloc(sizeof(t_node));

	node->id=0;
	node->id_old=0;
	node->id_chunk=0;
	node->id_chunk_self=0;
	node->id_ptr=NULL;

	node->data=NULL;
	node->type=type;

	return node;
}

