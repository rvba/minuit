/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
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
#include "brick.h"
#include "plug.h"
#include "object.h"
#include "screen.h"
#include "vector.h"
#include "util.h"
#include "light.h"
#include "set.h"
#include "image.h"
#include "file.h"
#include "rhizome.h"
#include "graph.h"
#include "material.h"
#include "mesh.h"
#include "geometry.h"

#include "ctx.h"

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
int node_make_binding(t_node *node);
int node_make_rhizome(t_node *node);
int node_make_graph(t_node *node);
int node_make_geo(t_node *node);
int node_make_geo_point(t_node *node);
int node_make_geo_edge(t_node *node);
int node_make_geo_array(t_node *node);

char node_name_null[]="node_null";
char node_name_brick[]="node_brick";
char node_name_mesh[]="node_mesh";
char node_name_block[]="node_block";
char node_name_light[]="node_light";
char node_name_object[]="node_object";
char node_name_screen[]="node_screen";
char node_name_file[]="node_file";
char node_name_image[]="node_image";
char node_name_material[]="node_material";
char node_name_list[]="node_list";
char node_name_link[]="node_link";
char node_name_data[]="node_data";
char node_name_texture[]="node_texture";
char node_name_var[]="node_var";
char node_name_option[]="node_option";
char node_name_vlst[]="node_vlst";
char node_name_camera[]="node_camera";
char node_name_dict[]="node_dict";
char node_name_symbol[]="node_symbol";
char node_name_vector[]="node_vector";
char node_name_viewport[]="node_viewport";
char node_name_set[]="node_set";
char node_name_binding[]="node_binding";
char node_name_rhizome[]="node_rhizome";
char node_name_graph[]="node_graph";
char node_name_geo[]="node_geo";
char node_name_geo_point[]="node_geo_point";
char node_name_geo_edge[]="node_geo_edge";
char node_name_geo_array[]="node_geo_array";


int node_equal(t_node *m, t_node *n)
{
	if(m->id == n->id) return 1;
	else return 0;
}


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
		case(nt_binding):return node_name_binding;break;
		case(nt_rhizome):return node_name_rhizome;break;
		case(nt_graph):return node_name_graph;break;
		case(nt_geo):return node_name_geo;break;
		case(nt_geo_point):return node_name_geo_point;break;
		case(nt_geo_edge):return node_name_geo_edge;break;
		case(nt_geo_array):return node_name_geo_array;break;
		default:printf("[ERROR node_name_get] Unknown type %d\n",type);return node_name_null;break;
	}
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

void *cls_node_get_ref_geo_array(t_node *node, const char *ref)
{
	return geo_array_get_ref(node->data, ref);
}

void *cls_node_get_ref(t_node *node, const char *ref)
{
	printf("get ref not implemented\n");
	return NULL;
}

// BUILD


void cls_node_init_brick_ids(t_node *node);

void cls_node_id_add(t_node *node)
{
	t_context *C=ctx_get();
	t_id *id = (t_id *) node->data;

	int scene_id = scene_id_get(C->scene);
	node->id = scene_id;
	id->id = scene_id;

	if(node->cls->type == nt_brick)
	{
		cls_node_init_brick_ids(node);
	}

}

void cls_node_user_add(t_node *node)
{
	t_id *id = (t_id *) node->data;
	id->users++;
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
	void *p = NULL;

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
		case nt_binding:	 p = binding_new(name);
			break;
		case nt_rhizome: 	p = rhizome_new(name);
			break;
		case nt_graph: 		p = graph_new(name);
			break;
		case nt_geo: 		p = geo_new(name);
			break;
		case nt_geo_point:	p = geo_point_new(name);
			break;
		case nt_geo_edge:	p = geo_edge_new(name);
			break;
		case nt_geo_array:	p = geo_array_new(name);
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

	if(node->type != nt_var)
	{
		cls_node_id_add(node);
		cls_node_user_add(node);
	}

}

// DELETE

//void cls_node_del(t_node *node) { t_scene *scene  = scene_get(); scene_node_delete(scene,node);} 
void cls_node_del(void *data) { t_node *node = (t_node *) data; t_scene *scene  = scene_get(); scene_node_delete(scene,node);} 

// LINK

void cls_node_link(t_node *node)
{
}


// FREE

void cls_node_light_free(t_scene *sc,t_node *node)
{
	light_free(node->data);
}

void cls_node_screen_free(t_scene *sc,t_node *node)
{
	screen_free(node->data);
}

void cls_node_file_free(t_scene *sc,t_node *node)
{
	file_free(node->data);
}

void cls_node_image_free(t_scene *sc,t_node *node)
{
	image_free(node->data);
}

void cls_node_material_free(t_scene *sc,t_node *node)
{
	material_free(node->data);
}

void cls_node_option_free(t_scene *sc,t_node *node)
{
	option_free(node->data);
}

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

void cls_node_binding_free(t_scene *sc,t_node *node)
{
	binding_free(node->data);
}

void cls_node_rhizome_free(t_scene *sc,t_node *node)
{
	rhizome_free(node->data);
}

void cls_node_graph_free(t_scene *sc,t_node *node)
{
	graph_free(node->data);
}

void cls_node_geo_free(t_scene *sc,t_node *node)
{
	geo_free(node->data);
}

void cls_node_geo_point_free(t_scene *sc,t_node *node)
{
	geo_point_free(node->data);
}

void cls_node_geo_edge_free(t_scene *sc,t_node *node)
{
	geo_edge_free(node->data);
}

void cls_node_geo_array_free(t_scene *sc,t_node *node)
{
	geo_array_free(node->data);
}

// INIT

// get Scene ID
void cls_node_init_id(t_node *node)
{
	t_context *C=ctx_get();

	int scene_id = scene_id_get(C->scene);
	t_id *id = (t_id *) node->data;

	// node
	node->id = scene_id;
	// data
	id->id = scene_id;
}

void cls_node_init_brick_ids(t_node *node)
{
	t_context *C=ctx_get();

	int id=scene_id_get(C->scene);
	t_brick *brick=node->data;

	// node
	node->id=id;
	// data
	brick->id.id=id;

	brick->plug_in.id.id = scene_id_get(C->scene);
	brick->plug_out.id.id = scene_id_get(C->scene);
}

void cls_node_init_block(t_node *node)
{
	cls_node_init_id(node);
	t_block *block=node->data;
	block->cls->init(block);
}

void cls_node_init_brick(t_node *node)
{
	cls_node_init_brick_ids(node);
	t_brick *brick=node->data;
	brick->cls->init(brick);
}

void cls_node_init_mesh(t_node *node)
{
	cls_node_init_id(node);
	t_mesh *mesh=node->data;
	t_context *C=ctx_get();
	mesh_init(C->scene,mesh);
}

void cls_node_init_object(t_node *node)
{
	cls_node_init_id(node);
	t_object *object=node->data;
	object->cls->init(object);
}

void cls_node_init_var(t_node *node)
{
	// node var has direct data
	// no id
	// no init

	t_context *C=ctx_get();
	int id=scene_id_get(C->scene);
	node->id=id;
}

void cls_node_init_camera(t_node *node)
{
	cls_node_init_id(node);
}

void cls_node_init_dict(t_node *node)
{
	cls_node_init_id(node);
}

void cls_node_init_symbol(t_node *node)
{
	cls_node_init_id(node);
}

void cls_node_init_vector(t_node *node)
{
	cls_node_init_id(node);
	t_vector *vector=node->data;
	vector->cls->init(vector);
}

void cls_node_init_viewport(t_node *node)
{
	cls_node_init_id(node);
}

void cls_node_init_set(t_node *node)
{
	cls_node_init_id(node);
}

void cls_node_init_binding(t_node *node)
{
	cls_node_init_id(node);
}

void cls_node_init_rhizome(t_node *node)
{
	t_rhizome *rhizome = node->data;
	rhizome->roots = NULL;
	cls_node_init_id(node);
	rhizome_init(rhizome);
}

void cls_node_init_graph(t_node *node)
{
	cls_node_init_id(node);
}

void cls_node_init_geo(t_node *node)
{
	cls_node_init_id(node);
}

void cls_node_init_geo_point(t_node *node)
{
	cls_node_init_id(node);
}

void cls_node_init_geo_edge(t_node *node)
{
	cls_node_init_id(node);
}

void cls_node_init_geo_array(t_node *node)
{
	cls_node_init_id(node);
}


// CLASSES

t_node_class mesh= {
	.type=nt_mesh,
	.size=sizeof( t_mesh),
	.lst=NULL,
	.make=node_make_mesh,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_mesh,
	.free=cls_node_mesh_free,
	.get_ref = cls_node_get_ref_mesh,
};

t_node_class block= {
	.type=nt_block,
	.size=sizeof( t_block),
	.lst=NULL,
	.make=node_make_block,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_block,
	.free=cls_node_block_free,
	.get_ref = cls_node_get_ref,
};

t_node_class brick= {
	.type=nt_brick,
	.size=sizeof( t_brick),
	.lst=NULL,
	.make=node_make_brick,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_brick,
	.free=cls_node_brick_free,
	.get_ref = cls_node_get_ref_brick,
};

t_node_class light= {
	.type=nt_light,
	.size=sizeof( t_light),
	.lst=NULL,
	.make=node_make_light,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_light_free,
	.get_ref = cls_node_get_ref,
};

t_node_class object= {
	.type=nt_object,
	.size=sizeof( t_object),
	.lst=NULL,
	.make=node_make_object,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_object_free,
	.get_ref = cls_node_get_ref_object,
};

t_node_class screen= {
	.type=nt_screen,
	.size=sizeof( t_screen),
	.lst=NULL,
	.make=node_make_screen,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_screen_free,
	.get_ref = cls_node_get_ref,
};

t_node_class file= {
	.type=nt_file,
	.size=sizeof( t_file),
	.lst=NULL,
	.make=node_make_file,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_file_free,
	.get_ref = cls_node_get_ref,
};

t_node_class image= {
	.type=nt_image,
	.size=sizeof( t_image),
	.lst=NULL,
	.make=node_make_image,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_image_free,
	.get_ref = cls_node_get_ref,
};

t_node_class material= {
	.type=nt_material,
	.size=sizeof( t_material),
	.lst=NULL,
	.make=node_make_material,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_material_free,
	.get_ref = cls_node_get_ref_material,
};

t_node_class list= {
	.type=nt_list,
	.size=sizeof( t_lst),
	.lst=NULL,
	.make=node_make_list,
	.build=cls_node_build,
	.link=cls_node_link,
	//.del=cls_node_del,
	.del = cls_lst_delete,
	.init=cls_node_init_id,
	.free=cls_node_list_free,
	.get_ref = cls_node_get_ref,
};

t_node_class _link_= {
	.type=nt_link,
	.size=sizeof( t_link),
	.lst=NULL,
	.make=node_make_link,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_link_free,
	.get_ref = cls_node_get_ref,
};

t_node_class data= {
	.type=nt_data,
	.size=sizeof( t_data),
	.lst=NULL,
	.make=node_make_data,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_data_free,
	.get_ref = cls_node_get_ref,
};

t_node_class texture= {
	.type=nt_texture,
	.size=sizeof( t_texture),
	.lst=NULL,
	.make=node_make_texture,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
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
	.free=cls_node_var_free,
	.get_ref = cls_node_get_ref,
};

t_node_class option= {
	.type=nt_option,
	.size=sizeof( t_option),
	.lst=NULL,
	.make=node_make_option,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_option_free,
	.get_ref = cls_node_get_ref,
};

t_node_class vlst= {
	.type=nt_vlst,
	.size=sizeof( t_vlst),
	.lst=NULL,
	.make=node_make_vlst,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_vlst_free,
	.get_ref = cls_node_get_ref_vlst,
};

t_node_class camera= {
	.type=nt_camera,
	.size=sizeof( t_camera),
	.lst=NULL,
	.make=node_make_camera,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_camera,
	.free=cls_node_camera_free,
	.get_ref = cls_node_get_ref_camera,
};

t_node_class dict= {
	.type=nt_dict,
	.size=sizeof( t_dict),
	.lst=NULL,
	.make=node_make_dict,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_dict_free,
	.get_ref = cls_node_get_ref,
};

t_node_class symbol= {
	.type=nt_symbol,
	.size=sizeof( t_symbol),
	.lst=NULL,
	.make=node_make_symbol,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_symbol,
	.free=cls_node_symbol_free,
	.get_ref = cls_node_get_ref,
};

t_node_class vector= {
	.type=nt_vector,
	.size=sizeof( t_vector),
	.lst=NULL,
	.make=node_make_vector,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_vector,
	.free=cls_node_vector_free,
	.get_ref = cls_node_get_ref,
};

t_node_class viewport= {
	.type=nt_viewport,
	.size=sizeof( t_viewport),
	.lst=NULL,
	.make=node_make_viewport,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_viewport,
	.free=cls_node_viewport_free,
	.get_ref = cls_node_get_ref,
};

t_node_class set = {
	.type=nt_set,
	.size=sizeof( t_set),
	.lst=NULL,
	.make=node_make_set,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_set,
	.free=cls_node_set_free,
	.get_ref = cls_node_get_ref,
};

t_node_class binding = {
	.type=nt_binding,
	.size=sizeof( t_binding),
	.lst=NULL,
	.make=node_make_binding,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_binding,
	.free=cls_node_binding_free,
	.get_ref = cls_node_get_ref,
};

t_node_class rhizome = {
	.type=nt_rhizome,
	.size=sizeof( t_rhizome),
	.lst=NULL,
	.make=node_make_rhizome,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_rhizome,
	.free=cls_node_rhizome_free,
	.get_ref = cls_node_get_ref,
};

t_node_class graph = {
	.type=nt_graph,
	.size=sizeof( t_graph),
	.lst=NULL,
	.make=node_make_graph,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_graph,
	.free=cls_node_graph_free,
	.get_ref = cls_node_get_ref,
};

t_node_class geo = {
	.type=nt_geo,
	.size=sizeof( t_geo),
	.lst=NULL,
	.make=node_make_geo,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_geo,
	.free=cls_node_geo_free,
	.get_ref = cls_node_get_ref,
};

t_node_class geo_point = {
	.type=nt_geo_point,
	.size=sizeof( t_geo_point),
	.lst=NULL,
	.make=node_make_geo_point,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_geo_point,
	.free=cls_node_geo_point_free,
	.get_ref = cls_node_get_ref,
};

t_node_class geo_edge = {
	.type=nt_geo_edge,
	.size=sizeof( t_geo_edge),
	.lst=NULL,
	.make=node_make_geo_edge,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_geo_edge,
	.free=cls_node_geo_edge_free,
	.get_ref = cls_node_get_ref,
};

t_node_class geo_array = {
	.type=nt_geo_array,
	.size=sizeof( t_geo_array),
	.lst=NULL,
	.make=node_make_geo_array,
	.build=cls_node_build,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_geo_array,
	.free=cls_node_geo_array_free,
	.get_ref = cls_node_get_ref_geo_array,
};


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
				&binding,
				&rhizome,
				&graph,
				&geo,
				&geo_point,
				&geo_edge,
				&geo_array,
			};


void cls_node_lst_set( t_scene *sc, t_node *node)
{
	switch( node->cls->type)
	{
		case( nt_mesh): node->cls->lst = sc->meshes; break;
		case( nt_block): node->cls->lst = sc->blocks; break;
		case( nt_brick): node->cls->lst = sc->bricks; break;
		case( nt_light): node->cls->lst = sc->lights; break;
		case( nt_object): node->cls->lst = sc->objects; break;
		case( nt_screen): node->cls->lst = sc->screens; break;
		case( nt_file): node->cls->lst = sc->files; break;
		case( nt_image): node->cls->lst = sc->images; break;
		case( nt_material): node->cls->lst = sc->materials; break;
		case( nt_list): node->cls->lst = sc->lists; break;
		case( nt_link): node->cls->lst = sc->links; break;
		case( nt_texture): node->cls->lst = sc->textures; break;
		case( nt_option): node->cls->lst = NULL; break;
		case( nt_vlst): node->cls->lst = sc->vlst; break;
		case( nt_camera): node->cls->lst = sc->cameras; break;
		case( nt_dict): node->cls->lst = sc->dicts; break;
		case( nt_symbol): node->cls->lst = sc->symbols; break;
		case( nt_vector): node->cls->lst = sc->vectors; break;
		case( nt_binding): node->cls->lst = sc->bindings; break;
		case( nt_rhizome): node->cls->lst = sc->rhizomes; break;
		case( nt_graph): node->cls->lst = sc->graphs; break;
		case( nt_viewport): node->cls->lst = sc->viewports; break;
		case( nt_set): node->cls->lst = sc->sets; break;
		case( nt_null): break;
		case( nt_data):node->cls->lst = sc->datas; break;
		case( nt_var): node->cls->lst = sc->vars; break;
		case( nt_geo): node->cls->lst = sc->geos; break;
		case( nt_geo_point): node->cls->lst = sc->geos; break;
		case( nt_geo_edge): node->cls->lst = sc->geos; break;
		case( nt_geo_array): node->cls->lst = sc->geos; break;
		default: printf("[ERROR cls_node_lst_set] Unkown type %s\n", node_name_get( node->cls->type)); break;
	}
}

void cls_node_cls_set( t_node *node)
{
	switch( node->type)
	{
		case( nt_mesh): node->cls = &mesh; break;
		case( nt_block): node->cls = &block; break;
		case( nt_brick): node->cls = &brick; break;
		case( nt_light): node->cls = &light; break;
		case( nt_object): node->cls = &object; break;
		case( nt_screen): node->cls = &screen; break;
		case( nt_file): node->cls = &file; break;
		case( nt_image): node->cls = &image; break;
		case( nt_material): node->cls = &material; break;
		case( nt_list): node->cls = &list; break;
		case( nt_link): node->cls = &_link_; break;
		case( nt_texture): node->cls = &texture; break;
		case( nt_option): node->cls = &option; break;
		case( nt_vlst): node->cls = &vlst; break;
		case( nt_camera): node->cls = &camera; break;
		case( nt_dict): node->cls = &dict; break;
		case( nt_symbol): node->cls = &symbol; break;
		case( nt_vector): node->cls = &vector; break;
		case( nt_binding): node->cls = &binding; break;
		case( nt_rhizome): node->cls = &rhizome; break;
		case( nt_graph): node->cls = &graph; break;
		case( nt_viewport): node->cls = &viewport; break;
		case( nt_set): node->cls = &set; break;
		case( nt_null): node->cls = NULL;break;
		case( nt_data): node->cls = &data; break;
		case( nt_var): node->cls = &var; break;
		case( nt_geo): node->cls = &geo ; break;
		case( nt_geo_point): node->cls = &geo_point; break;
		case( nt_geo_edge): node->cls = &geo_edge; break;
		case( nt_geo_array): node->cls = &geo_array; break;
		default: printf("[ERROR cls_node_cls_set] Unkown type %s\n", node_name_get( node->cls->type)); break;
	}
}


void cls_node_make( t_scene *sc, t_node *node, t_node_type type)
{
	node->type = type;
	cls_node_cls_set( node);
	cls_node_lst_set( sc, node);
}


// INIT 

void cls_node_init(t_node *node,t_node_type type)
{
	t_scene *sc = ctx_scene_get();
	cls_node_make( sc, node, type);
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

void id_init(t_id *id, const char *name)
{
	id->id = 0;
	id->id_chunk = 0;
	id->users = 0;
	set_name(id->name, name);
	id->store = 0;
}


