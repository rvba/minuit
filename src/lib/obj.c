/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "op.h"
#include "node.h"
#include "context.h"
#include "brick.h"
#include "obj.h"
#include "event.h"
#include "term.h"
#include "file.h"
#include "mesh.h"
#include "ctx.h"
#include "object.h"
#include "screen.h"

t_lst *OBJECTS;

typedef enum token_type
{
	token_object,
	token_vertex,
	token_face,
	token_material,
	token_unknown,

}t_token_type;

typedef struct obj
{
	char *name;
	int tot_vert;
	int tot_face;
	int tot_tri;
	int tot_quad;
	float *verts;
	int *quads;
	int *tris;

}t_obj;

void obj_show(t_obj *obj,int show_data)
{
	printf("***************************\n");
	printf("OBJECT:%s\n",obj->name);
	printf("tot_vert:%d\n",obj->tot_vert);
	printf("tot_face:%d\n",obj->tot_face);
	printf("tot_tri:%d\n",obj->tot_tri);
	printf("tot_quad:%d\n",obj->tot_quad);

	if(show_data)
	{
		int i,j;
		if(obj->verts)
		{
			j=0;
			for(i=0;i<obj->tot_vert;i++)
			{
				printf("v: %f %f %f\n",obj->verts[j],obj->verts[j+1],obj->verts[j+2]);
				j+=3;
			}
		}
		if(obj->tris)
		{
			j=0;
			for(i=0;i<obj->tot_tri;i++)
			{
				printf("tri: %d %d %d\n",obj->tris[j],obj->tris[j+1],obj->tris[j+2]);
				j+=3;
			}
		}
		if(obj->quads)
		{
			j=0;
			for(i=0;i<obj->tot_quad;i++)
			{
				printf("quad: %d %d %d %d\n",obj->quads[j],obj->quads[j+1],obj->quads[j+2],obj->quads[j+3]);
				j+=4;
			}
		}
	}
}

void obj_free(t_obj *obj)
{
	free(obj->name);
	if(obj->verts)free(obj->verts);
	if(obj->tris) free(obj->tris);
	if(obj->quads) free(obj->quads);
}

t_obj *obj_new(const char *name)
{
	t_obj *obj = (t_obj *)malloc(sizeof(t_obj));
	obj->name=(char *)malloc(sizeof(char)*(strlen(name)+1));
	strcpy(obj->name,name);
	obj->tot_vert=0;
	obj->tot_face=0;
	obj->tot_tri=0;
	obj->tot_quad=0;
	obj->verts=NULL;
	obj->quads=NULL;
	obj->tris=NULL;

	return obj;
}

void op_export(t_node *node)
{
	obj_export(NULL);
}

void obj_add(const char *object_name,int tot_vert,int tot_face,int tot_quad,int tot_tri)
{
	//printf("adding object:%s\n",object_name);
	t_context *C=ctx_get();
	term_print(C->term,"+ obj");
	t_obj *obj=obj_new(object_name);

	obj->tot_vert=tot_vert;
	obj->tot_face=tot_face;
	obj->tot_quad=tot_quad;
	obj->tot_tri=tot_tri;

	if(tot_vert) obj->verts=(float *)malloc(sizeof(float)*obj->tot_vert*3);
	if(tot_tri) obj->tris=(int *)malloc(sizeof(int)*obj->tot_tri*3);
	if(tot_quad) obj->quads=(int *)malloc(sizeof(int)*obj->tot_quad*4);

	lst_add(OBJECTS,obj,"obj");
}

void _op_obj_import(void)
{
	t_context *C=ctx_get();
	char *filename=C->event->standby_string;

	if(filename)
	{
		// init list
		OBJECTS=lst_new("lst");

		// parse file
		t_file *file = file_new(filename);
		file_init(file);

		free(C->event->standby_string);
		//C->event->standby_function=NULL;
		C->event->callback=NULL;

		file_read(file);
		file_read_lines(file);

		//parse words
		t_link *link;
		t_link *l;
		t_word *word;

		for(link=file->lines->first;link;link=link->next)
		{
			t_line *line = link->data;
			line_read_words(line);
		}

		// parse tokens
		int object_start;
		int line_object;
		int is_face;
		//int tot_object;
		int tot_vert;
		int tot_face;
		int tot_tri;
		int tot_quad;
		int tot_indice;

		char *object_name;

		//tot_object=0;
		tot_face=0;
		tot_quad=0;
		tot_tri=0;
		object_start=0;
		
		for(link=file->lines->first;link;link=link->next)
		{
			// LINE
			t_line *line = link->data;

			// RESET
			is_face=0;

			for(l=line->words->first;l;l=l->next)
			{
				// WORD

				word=l->data;

				if(word_equal(word,"o"))
				{
					if(object_start)
					{
						obj_add(object_name,tot_vert,tot_face,tot_quad,tot_tri);

						tot_vert=0;
						tot_face=0;
						tot_quad=0;
						tot_tri=0;
					
						free(object_name);
					}
					else
					{
						object_start=1;
					}

					tot_vert=0;
					line_object=1;
				}
				else if(line_object)
				{
					object_name=(char *)malloc(sizeof(char)*(strlen(word->data)+1));
					strcpy(object_name,word->data);
					line_object=0;
				}
				else if(word_equal(word,"v"))
				{
					tot_vert++;
				}
				else if(word_equal(word,"f"))
				{
					tot_face++;
					is_face=1;
					tot_indice=0;
				}
				else if(is_face)
				{
					tot_indice++;
				}
				else if(word_equal(word,"usemtl"))
				{
				}
				else if(word_equal(word,"s"))
				{
				}
			}

			if(is_face)
			{
				if(tot_indice==4)
				{
					tot_quad++;
				}
				else
				{
					tot_tri++;
				}
			}
		}

		// add last object
		obj_add(object_name,tot_vert,tot_face,tot_quad,tot_tri);

		// vars
		int is_data=0;
		int indice_vertex=0;
		int indice_face=0;
		int cursor_tri=0;
		int cursor_quad=0;
		int global_cursor=0;
		int tmp_global_cursor=0;
		int face[4];

		object_start=0;

		t_token_type token;
		t_link *link_object;
		t_obj *obj;

		for(link=file->lines->first;link;link=link->next)
		{
			// LINE

			t_line *line = link->data;
			
			// reset
			is_data=0;
			indice_face=0;

			for(l=line->words->first;l;l=l->next)
			{
				// WORD

				word=l->data;

				if(word_equal(word,"o"))
				{
					token=token_object;

					if(object_start)
					{
						if(link_object->next) link_object=link_object->next;
						obj=link_object->data;
						// global cursor
						global_cursor+=tmp_global_cursor;
						tmp_global_cursor=obj->tot_vert;
					}
					//first
					else
					{
						link_object=OBJECTS->first;
						obj=link_object->data;
						object_start=1;
						tmp_global_cursor=obj->tot_vert;
					}

					indice_vertex=0;
					indice_face=0;
					cursor_tri=0;
					cursor_quad=0;
				}
				else if(word_equal(word,"v"))
				{
					token=token_vertex;
				}
				else if(word_equal(word,"f"))
				{
					token=token_face;
				}
				else if(word_equal(word,"usemtl"))
				{
					token=token_material;
				}
				else if(word_equal(word,"s"))
				{
					token=token_unknown;
				}
				else
				{
					is_data=1;
				}

				if(is_data)
				{
					if(token==token_vertex)
					{
						obj->verts[indice_vertex]=atof(word->data);
						indice_vertex++;
					}
					else if(token==token_face)
					{
						face[indice_face]=atoi(word->data);
						indice_face++;
					}
				}
			}

			// store face indice
			if(token==token_face)
			{
				int i;
				if(indice_face==3)
				{
					for(i=0;i<3;i++)
					{
						obj->tris[cursor_tri]=face[i]-global_cursor-1;
						cursor_tri++;
					}
				}
				else
				{
					for(i=0;i<4;i++)
					{
						obj->quads[cursor_quad]=face[i]-global_cursor-1;
						cursor_quad++;
					}
				}
			}
		}

		
		// add objects to scene

		C->scene->store=1;

		for(link=OBJECTS->first;link;link=link->next)
		{
			t_obj *obj = link->data;

			// new mesh
			t_node *node_mesh=mesh_make(
						obj->name,
						//"me_obj",
						obj->tot_vert,
						obj->tot_face,
						obj->tot_quad,
						obj->tot_tri,
						obj->verts,
						obj->quads,
						obj->tris);


			// new object
			t_node *node_object=object_add("mesh",obj->name);

			// link
			t_object *object=node_object->data;
			object->cls->link(object,node_mesh);
		}

		C->scene->store=0;

		// free obj

		for(link=OBJECTS->first;link;link=link->next)
		{
			t_obj *obj = link->data;
			obj_free(obj);
		}
	}
}

void *op_obj_import(t_brick *brick)
{
	t_context *C=ctx_get();
	C->event->callback=_op_obj_import;
	screen_switch_by_name("screen_browser");

	return NULL;
}

int obj_export(t_node *node)
{
	/*
	t_context *C = ctx_get();
	t_node *n;
	t_link *l;

	FILE *file = fopen("./data/out.obj","w");

	fprintf(file,"#obj\n");

	int id=0;
	int uv=0;

	for(l=C->scene->meshes->first;l;l=l->next)
	{
		n=l->data;
		int tmp=0;
		op_mesh_apply(n);
		t_mesh *mesh = n->data;

		fprintf(file,"o mesh.%d\n",id);

		int i;	
		
		t_vertex *v = mesh->verts;
		t_face *f = mesh->faces;

		for(i=0;i<mesh->var.tot_vertex;i++)
		{
			fprintf(file,"v %f %f %f \n",v[i].co[0],v[i].co[1],v[i].co[2]); // 
			tmp+=1;
		}

		for(i=0;i<mesh->var.tot_face;i++)
		{
			if(f[i].type == t_quad)
			{
				fprintf(file,"f %d %d %d %d \n",f[i].v1+1+uv,f[i].v2+1+uv,f[i].v3+1+uv,f[i].v4+1+uv); // 
			}	
			else if (f[i].type == t_tri)
			{
				fprintf(file,"f %d %d %d\n",f[i].v1+1+uv,f[i].v2+1+uv,f[i].v3+1+uv); // 
			}
		}

		id++;
		uv+=tmp;

	}

	fclose(file);
	printf("obj exported\n");

	*/
	return 1;
}




