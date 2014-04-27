/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
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
#include "texture.h"
#include "mesh.h"
#include "geometry.h"
#include "memory.h"
#include "node.h"

void var_rebind( struct Scene *scene, void *ptr)
{
}

int node_equal(t_node *m, t_node *n)
{
	if(m->id == n->id) return 1;
	else return 0;
}

// GET REF

void *cls_node_get_ref_viewport(t_node *node, const char *ref)
{
	return viewport_get_ref( (t_viewport *) node->data, ref);
}

void *cls_node_get_ref_object(t_node *node, const char *ref)
{
	return object_get_ref( (t_object *) node->data, ref);
}

void *cls_node_get_ref_brick(t_node *node, const char *ref)
{
	return brick_get_ref( ( t_brick *) node->data, ref);
}

void *cls_node_get_ref_material(t_node *node, const char *ref)
{
	return material_get_ref( ( t_material *) node->data, ref);
}

void *cls_node_get_ref_mesh(t_node *node, const char *ref)
{
	return mesh_get_ref( ( t_mesh *) node->data, ref);
}

void *cls_node_get_ref_vlst(t_node *node, const char *ref)
{
	return vlst_get_ref( ( t_vlst *) node->data, ref);
}

void *cls_node_get_ref_camera(t_node *node, const char *ref)
{
	return camera_get_ref( ( t_camera *) node->data, ref);
}

void *cls_node_get_ref_geo_array(t_node *node, const char *ref)
{
	return geo_array_get_ref( ( t_geo_array *) node->data, ref);
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

	if(node->cls->type == dt_brick)
	{
		cls_node_init_brick_ids(node);
	}

	id->node = node;
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
	/*
	void *p = NULL;

	switch(node->type)
	{
		case dt_mesh:		p = mesh_new(name);
			break;
		case dt_block:		p = block_new(name);
			break;
		case dt_brick:		p = brick_new(name);
			break;
		case dt_light:		p = light_new(name);
			break;
		case dt_object:		p = object_new(name);
			break;
		case dt_screen:		p = screen_new(name);
			break;
		case dt_file:		p = file_new(name);
			break;
		case dt_image:		p = image_new(name);
			break;
		case dt_material:	p = material_new(name);
			break;
		case dt_list:		p = lst_new(name);
			break;
		case dt_lst:		p = lst_new(name);
			break;
		case dt_link:		p = link_new(name);
			break;
		case dt_data:		p = data_new(name);
			break;
		case dt_texture:	p = texture_new(name);
			break;
		case dt_option:		p = option_new(name);
			break;
		case dt_vlst:		p = vlst_new(name);
			break;
		case dt_camera:		p = camera_new(name);
			break;
		case dt_dict:		p = dict_new(name);
			break;
		case dt_symbol: 	p = symbol_new(name);
			break;
		case dt_vector: 	p = vector_new(name);
			break;
		case dt_viewport: 	p = viewport_new(name);
			break;
		case dt_set:	 	p = set_new(name);
			break;
		case dt_binding:	 p = binding_new(name);
			break;
		case dt_rhizome: 	p = rhizome_new(name);
			break;
		case dt_graph: 		p = graph_new(name);
			break;
		case dt_geo: 		p = geo_new(name);
			break;
		case dt_geo_point:	p = geo_point_new(name);
			break;
		case dt_geo_edge:	p = geo_edge_new(name);
			break;
		case dt_geo_array:	p = geo_array_new(name);
			break;

		case dt_null: 
			printf("[WARNING node_cls_build] null node\n");
			break;
		case dt_var: 
			cls_node_build_var(node,name);
			break;
		case dt_datum:		p = datum_new( name);
			break;

		default:
			printf("[ERROR cls_node_build] Unkown type %s\n", data_name_get( node->type));
			break;
	}
	*/

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
	camera_free(node->data);
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

void cls_node_datum_free(t_scene *sc,t_node *node)
{
	datum_free(node->data);
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
	t_scene *scene = scene_get();
	if( scene) scene->has_generic_viewport = 1;
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
	geo_init( node->data);
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
	geo_array_init( node->data);
}

void cls_node_init_datum(t_node *node)
{
	cls_node_init_id(node);
	datum_init( node->data);
}


// CLASSES

t_node_class mesh= {
	.type=dt_mesh,
	.size=sizeof( t_mesh),
	.lst=NULL,
	.build=mesh_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_mesh,
	.free=cls_node_mesh_free,
	.get_ref = cls_node_get_ref_mesh,
	.rebind = mesh_rebind,
};

t_node_class block= {
	.type=dt_block,
	.size=sizeof( t_block),
	.lst=NULL,
	.build=block_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_block,
	.free=cls_node_block_free,
	.get_ref = cls_node_get_ref,
	.rebind = block_rebind,
};

t_node_class brick= {
	.type=dt_brick,
	.size=sizeof( t_brick),
	.lst=NULL,
	.build=brick_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_brick,
	.free=cls_node_brick_free,
	.get_ref = cls_node_get_ref_brick,
	.rebind = brick_rebind,
};

t_node_class light= {
	.type=dt_light,
	.size=sizeof( t_light),
	.lst=NULL,
	.build=light_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_light_free,
	.get_ref = cls_node_get_ref,
	.rebind = light_rebind,
};

t_node_class object= {
	.type=dt_object,
	.size=sizeof( t_object),
	.lst=NULL,
	.build=object_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_object_free,
	.get_ref = cls_node_get_ref_object,
	.rebind = object_rebind,
};

t_node_class screen= {
	.type=dt_screen,
	.size=sizeof( t_screen),
	.lst=NULL,
	.build=screen_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_screen_free,
	.get_ref = cls_node_get_ref,
	.rebind = screen_rebind,
};

t_node_class file= {
	.type=dt_file,
	.size=sizeof( t_file),
	.lst=NULL,
	.build=file_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_file_free,
	.get_ref = cls_node_get_ref,
	.rebind = file_rebind,
};

t_node_class image= {
	.type=dt_image,
	.size=sizeof( t_image),
	.lst=NULL,
	.build=image_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_image_free,
	.get_ref = cls_node_get_ref,
	.rebind = image_rebind,
};

t_node_class material= {
	.type=dt_material,
	.size=sizeof( t_material),
	.lst=NULL,
	.build=material_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_material_free,
	.get_ref = cls_node_get_ref_material,
	.rebind = material_rebind,
};

t_node_class list= {
	.type=dt_list,
	.size=sizeof( t_lst),
	.lst=NULL,
	.build=lst_new,
	.link=cls_node_link,
	//.del=cls_node_del,
	.del = cls_list_delete,
	.init=cls_node_init_id,
	.free=cls_node_list_free,
	.get_ref = cls_node_get_ref,
	.rebind = list_rebind,
};

t_node_class _link_= {
	.type=dt_link,
	.size=sizeof( t_link),
	.lst=NULL,
	.build=link_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_link_free,
	.get_ref = cls_node_get_ref,
	.rebind = link_rebind,
};

t_node_class data= {
	.type=dt_data,
	.size=sizeof( t_data),
	.lst=NULL,
	.build=data_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_data_free,
	.get_ref = cls_node_get_ref,
	.rebind = data_rebind,
};

t_node_class texture= {
	.type=dt_texture,
	.size=sizeof( t_texture),
	.lst=NULL,
	.build=texture_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_texture_free,
	.get_ref = cls_node_get_ref,
	.rebind = texture_rebind,
};

t_node_class var= {
	.type=dt_var,
	.size=0,
	.lst=NULL,
	.build=NULL,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_var,
	.free=cls_node_var_free,
	.get_ref = cls_node_get_ref,
	.rebind = var_rebind,
};

t_node_class option= {
	.type=dt_option,
	.size=sizeof( t_option),
	.lst=NULL,
	.build=option_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_option_free,
	.get_ref = cls_node_get_ref,
	.rebind = var_rebind,
};

t_node_class vlst= {
	.type=dt_vlst,
	.size=sizeof( t_vlst),
	.lst=NULL,
	.build=vlst_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_vlst_free,
	.get_ref = cls_node_get_ref_vlst,
	.rebind = vlst_rebind,
};

t_node_class camera= {
	.type=dt_camera,
	.size=sizeof( t_camera),
	.lst=NULL,
	.build=camera_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_camera,
	.free=cls_node_camera_free,
	.get_ref = cls_node_get_ref_camera,
	.rebind = camera_rebind,
};

t_node_class dict= {
	.type=dt_dict,
	.size=sizeof( t_dict),
	.lst=NULL,
	.build=dict_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_id,
	.free=cls_node_dict_free,
	.get_ref = cls_node_get_ref,
	.rebind = dict_rebind,
};

t_node_class symbol= {
	.type=dt_symbol,
	.size=sizeof( t_symbol),
	.lst=NULL,
	.build=symbol_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_symbol,
	.free=cls_node_symbol_free,
	.get_ref = cls_node_get_ref,
	.rebind = symbol_rebind,
};

t_node_class vector= {
	.type=dt_vector,
	.size=sizeof( t_vector),
	.lst=NULL,
	.build=vector_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_vector,
	.free=cls_node_vector_free,
	.get_ref = cls_node_get_ref,
	.rebind = vector_rebind,
};

t_node_class viewport= {
	.type=dt_viewport,
	.size=sizeof( t_viewport),
	.lst=NULL,
	.build=viewport_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_viewport,
	.free=cls_node_viewport_free,
	.get_ref = cls_node_get_ref_viewport,
	.rebind = viewport_rebind,
};

t_node_class set = {
	.type=dt_set,
	.size=sizeof( t_set),
	.lst=NULL,
	.build=set_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_set,
	.free=cls_node_set_free,
	.get_ref = cls_node_get_ref,
	.rebind = set_rebind,
};

t_node_class binding = {
	.type=dt_binding,
	.size=sizeof( t_binding),
	.lst=NULL,
	.build=binding_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_binding,
	.free=cls_node_binding_free,
	.get_ref = cls_node_get_ref,
	.rebind = binding_rebind,
};

t_node_class rhizome = {
	.type=dt_rhizome,
	.size=sizeof( t_rhizome),
	.lst=NULL,
	.build=rhizome_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_rhizome,
	.free=cls_node_rhizome_free,
	.get_ref = cls_node_get_ref,
	.rebind = rhizome_rebind,
};

t_node_class graph = {
	.type=dt_graph,
	.size=sizeof( t_graph),
	.lst=NULL,
	.build=graph_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_graph,
	.free=cls_node_graph_free,
	.get_ref = cls_node_get_ref,
	.rebind = graph_rebind,
};

t_node_class geo = {
	.type=dt_geo,
	.size=sizeof( t_geo),
	.lst=NULL,
	.build=geo_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_geo,
	.free=cls_node_geo_free,
	.get_ref = cls_node_get_ref,
	.rebind = geo_rebind,
};

t_node_class geo_point = {
	.type=dt_geo_point,
	.size=sizeof( t_geo_point),
	.lst=NULL,
	.build=geo_point_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_geo_point,
	.free=cls_node_geo_point_free,
	.get_ref = cls_node_get_ref,
	.rebind = geo_point_rebind,
};

t_node_class geo_edge = {
	.type=dt_geo_edge,
	.size=sizeof( t_geo_edge),
	.lst=NULL,
	.build=geo_edge_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_geo_edge,
	.free=cls_node_geo_edge_free,
	.get_ref = cls_node_get_ref,
	.rebind = geo_edge_rebind,
};

t_node_class geo_array = {
	.type=dt_geo_array,
	.size=sizeof( t_geo_array),
	.lst=NULL,
	.build=geo_array_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_geo_array,
	.free=cls_node_geo_array_free,
	.get_ref = cls_node_get_ref_geo_array,
	.rebind = geo_array_rebind,
};

t_node_class datum = {
	.type=dt_datum,
	.size=sizeof( t_datum),
	.lst=NULL,
	.build=datum_new,
	.link=cls_node_link,
	.del=cls_node_del,
	.init=cls_node_init_datum,
	.free=cls_node_datum_free,
	.get_ref = cls_node_get_ref,
	.rebind = datum_rebind,
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
				&datum,
			};


void cls_node_lst_set( t_scene *sc, t_node *node)
{
	switch( node->cls->type)
	{
		case( dt_mesh): node->cls->lst = sc->meshes; break;
		case( dt_block): node->cls->lst = sc->blocks; break;
		case( dt_brick): node->cls->lst = sc->bricks; break;
		case( dt_light): node->cls->lst = sc->lights; break;
		case( dt_object): node->cls->lst = sc->objects; break;
		case( dt_screen): node->cls->lst = sc->screens; break;
		case( dt_file): node->cls->lst = sc->files; break;
		case( dt_image): node->cls->lst = sc->images; break;
		case( dt_material): node->cls->lst = sc->materials; break;
		case( dt_list): node->cls->lst = sc->lists; break;
		case( dt_link): node->cls->lst = sc->links; break;
		case( dt_texture): node->cls->lst = sc->textures; break;
		case( dt_option): node->cls->lst = NULL; break;
		case( dt_vlst): node->cls->lst = sc->vlst; break;
		case( dt_camera): node->cls->lst = sc->cameras; break;
		case( dt_dict): node->cls->lst = sc->dicts; break;
		case( dt_symbol): node->cls->lst = sc->symbols; break;
		case( dt_vector): node->cls->lst = sc->vectors; break;
		case( dt_binding): node->cls->lst = sc->bindings; break;
		case( dt_rhizome): node->cls->lst = sc->rhizomes; break;
		case( dt_graph): node->cls->lst = sc->graphs; break;
		case( dt_viewport): node->cls->lst = sc->viewports; break;
		case( dt_set): node->cls->lst = sc->sets; break;
		case( dt_null): break;
		case( dt_data):node->cls->lst = sc->datas; break;
		case( dt_var): node->cls->lst = sc->vars; break;
		case( dt_geo): node->cls->lst = sc->geos; break;
		case( dt_geo_point): node->cls->lst = sc->geos; break;
		case( dt_geo_edge): node->cls->lst = sc->geos; break;
		case( dt_geo_array): node->cls->lst = sc->geos; break;
		case( dt_datum): node->cls->lst = sc->datums; break;
		case( dt_undefined): break;
		default: printf("[ERROR cls_node_lst_set] Unkown type %s\n", data_name_get( node->cls->type)); break;
	}
}

void cls_node_cls_set( t_node *node)
{
	t_scene *scene;

	switch( node->type)
	{
		case( dt_mesh): node->cls = &mesh; break;
		case( dt_block): node->cls = &block; break;
		case( dt_brick): node->cls = &brick; break;
		case( dt_light): node->cls = &light; break;
		case( dt_object): node->cls = &object; break;
		case( dt_screen): node->cls = &screen; break;
		case( dt_file): node->cls = &file; break;
		case( dt_image): node->cls = &image; break;
		case( dt_material): node->cls = &material; break;
		case( dt_list): node->cls = &list; break;
		case( dt_lst): node->cls = &list; break;
		case( dt_link): node->cls = &_link_; break;
		case( dt_texture): node->cls = &texture; break;
		case( dt_option): node->cls = &option; break;
		case( dt_vlst): node->cls = &vlst; break;
		case( dt_camera): node->cls = &camera; break;
		case( dt_dict): node->cls = &dict; break;
		case( dt_symbol): node->cls = &symbol; break;
		case( dt_vector): node->cls = &vector; break;
		case( dt_binding): node->cls = &binding; break;
		case( dt_rhizome): node->cls = &rhizome; break;
		case( dt_graph): node->cls = &graph; break;
		case( dt_viewport): node->cls = &viewport; break;
		case( dt_set): node->cls = &set; break;
		case( dt_null): node->cls = NULL;break;
		case( dt_data): node->cls = &data; break;
		case( dt_var): node->cls = &var; break;
		case( dt_geo): node->cls = &geo ; break;
		case( dt_geo_point): node->cls = &geo_point; break;
		case( dt_geo_edge): node->cls = &geo_edge; break;
		case( dt_geo_array): node->cls = &geo_array; break;
		case( dt_datum): node->cls = &datum; break;
		case( dt_undefined):
			scene = scene_get();
			node->cls = scene_class_get( scene, node->extra_type);
			break;
		default: printf("[ERROR cls_node_cls_set] Unkown type %s\n", data_name_get( node->type)); break;
	}
}


void cls_node_make( t_scene *sc, t_node *node, t_data_type type)
{
	node->type = type;
	cls_node_cls_set( node);
	cls_node_lst_set( sc, node);
}


// INIT 

void cls_node_init(t_node *node,t_data_type type)
{
	t_scene *sc = ctx_scene_get();
	cls_node_make( sc, node, type);
}

void node_init(t_node *node,t_data_type type)
{
	cls_node_init(node,type);
}

// NEW

t_node *node_new(t_data_type type)
{
	t_node *node = (t_node *) mem_malloc(sizeof(t_node));

	node->id=0;
	node->id_old=0;
	node->id_chunk=0;
	node->id_chunk_self=0;
	node->id_ptr=NULL;

	node->data=NULL;
	node->type=type;
	node->extra_type = 0;
	node->store = 0;
	node->size = 0;

	return node;
}



