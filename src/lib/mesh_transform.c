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
#include "memory.h"

//static int debug=0;

/*
t_node *mesh_join(t_mesh **meshes,int tot_mesh)
{
	int n_verts=0;
	int n_faces=0;
	int n_quads=0;
	int n_tris=0;

	int offset=0;
	int pos=0;
	int i;

	// count v & f
	for(i=0;i<tot_mesh;i++)
	{
		int f=0;
		n_verts += meshes[i]->var.tot_vertex;
		n_faces += meshes[i]->var.tot_face;

		for(f=0;f<meshes[i]->var.tot_face;f++) // totface
		{
			if(meshes[i]->faces[f].type == t_quad)
				n_quads++;
			else if(meshes[i]->faces[f].type == t_tri)
				n_tris++;
		}
	}

	// // VERTICES

	float *vertices = (float *)mem_malloc(sizeof(float)*n_verts*3);

	pos=0;

	for(i=0;i<tot_mesh;i++)
	{
		int t;
		for(t=0;t<meshes[i]->var.tot_vertex;t++)
		{

			vertices[pos] = meshes[i]->verts[t].co[0];
			vertices[pos+1] = meshes[i]->verts[t].co[1];
			vertices[pos+2] = meshes[i]->verts[t].co[2];
			meshes[i]->var.offset = offset;

			pos+=3;
		}

		offset += meshes[i]->var.tot_vertex;
	}

	// // QUADS
	
	int *quads=NULL;

	if(n_quads>0)
	{
		if(debug)printf("QUADS\n");
		quads = (int *)mem_malloc(sizeof(int)*n_quads*4);

		pos=0;

		for(i=0;i<tot_mesh;i++)
		{
			int t;
			for(t=0;t<meshes[i]->var.tot_face;t++)
			{
				if(meshes[i]->faces[t].type == t_quad)
				{
					int o = meshes[i]->var.offset;

					quads[pos] = meshes[i]->faces[t].v1 +o;
					quads[pos+1] = meshes[i]->faces[t].v2 +o;
					quads[pos+2] = meshes[i]->faces[t].v3 +o;
					quads[pos+3] = meshes[i]->faces[t].v4 +o;

					pos+=4;
				}
			}
		}
	}

	// // TRIS

	int *tris=NULL;
	
	if(n_tris>0)
	{
		if(debug)printf("TRIS\n");
		tris = (int *)mem_malloc(sizeof(int)*n_tris*3);

		pos=0;

		for(i=0;i<tot_mesh;i++)
		{
			int t;
			for(t=0;t<meshes[i]->var.tot_face;t++)
			{
				if(meshes[i]->faces[t].type == t_tri)
				{
					int o = meshes[i]->var.offset;

					tris[pos] = meshes[i]->faces[t].v1 +o;
					tris[pos+1] = meshes[i]->faces[t].v2 +o;
					tris[pos+2] = meshes[i]->faces[t].v3 +o;

					pos+=3;
				}
			}
		}
	}

	t_node *node = mesh_make("joined m",n_verts,n_faces,n_quads,n_tris,vertices,quads,tris);

	return node;
}

int op_mesh_vert_copy(t_scene *sc,t_node *target,t_node *source)
{

	t_mesh *target_mesh = target->data;
	t_mesh *source_mesh = source->data;

	target_mesh->var.tot_vertex=source_mesh->var.tot_vertex;
	target_mesh->verts = (t_vertex*)mem_malloc(sizeof(t_vertex)*target_mesh->var.tot_vertex);
	
	int i;
	for(i=0;i<target_mesh->var.tot_vertex;i++)
	{
		target_mesh->verts[i].co[0] = source_mesh->verts[i].co[0];
		target_mesh->verts[i].co[1] = source_mesh->verts[i].co[1];
		target_mesh->verts[i].co[2] = source_mesh->verts[i].co[2];
	}
	return 1;
}

int op_mesh_face_copy(t_scene *sc,t_node *target,t_node *source)
{
	t_mesh *target_mesh = target->data;
	t_mesh *source_mesh = source->data;

	target_mesh->var.tot_face = source_mesh->var.tot_face;
	target_mesh->state.has_tri = source_mesh->state.has_tri;
	target_mesh->state.has_quad = source_mesh->state.has_quad;
	target_mesh->var.tot_quad_face = source_mesh->var.tot_quad_face;
	target_mesh->var.tot_tri_face = source_mesh->var.tot_tri_face;

	target_mesh->faces = (t_face*)mem_malloc(sizeof(t_face)*target_mesh->var.tot_face);
	
	int i;
	for(i=0;i<target_mesh->var.tot_face;i++)
	{
		target_mesh->faces[i].type = source_mesh->faces[i].type;
		target_mesh->faces[i].v1 = source_mesh->faces[i].v1;
		target_mesh->faces[i].v2 = source_mesh->faces[i].v2;
		target_mesh->faces[i].v3 = source_mesh->faces[i].v3;
		target_mesh->faces[i].v4 = source_mesh->faces[i].v4;
	}
	return 1;
}

int op_mesh_copy(t_scene *sc,t_node *target,t_node *source)
{
	op_mesh_vert_copy(sc,target,source);
	op_mesh_face_copy(sc,target,source);

	mesh_build_vertex_array(target->data);

	t_mesh *mesh = target->data;

	if(mesh->state.has_quad)
	{
		mesh_do_quad_normals(target->data);
		mesh_do_quad_colors(target->data,mesh->idcol);
	}
	if(mesh->state.has_tri)
	{

		mesh_do_tri_normals(target->data);
		mesh_do_tri_colors(target->data,mesh->idcol);
	}

	return 1;
}

void op_mesh_join_all(t_node *node)
{
	t_context *C = ctx_get();
	t_node *n;
	t_node *result;
	t_link *link;

	t_node **nodes = (t_node **)mem_malloc(sizeof(t_node*)*C->scene->meshes->tot);
	t_mesh **meshes = (t_mesh **)mem_malloc(sizeof(t_mesh*)*C->scene->meshes->tot);

	// join all meshes

	int i=0;
	for(link=C->scene->meshes->first;link;link=link->next)	
	{
		n=link->data;
		op_mesh_apply(n);
		nodes[i] = n;
		meshes[i]=n->data;
		i++;
	}

	result = mesh_join(meshes,C->scene->meshes->tot);

	int id = result->id;
	int j;

	// remove meshes

	for(j=0;j<i;j++)	
	{
		t_node *tmp = nodes[j];
		if(tmp->id != id)
			scene_node_delete(C->scene,tmp);
	}

	// free
	mem_free(nodes);
	mem_free(meshes);
	
}
*/

void op_mesh_apply(t_node *node)
{
	/*
	t_mesh *mesh = node->data;

	float vector[3]={node->shell->loc[0],node->shell->loc[1],node->shell->loc[2]}; 
	
	int i;
	for (i=0;i<mesh->var.tot_vertex;i++)
	{
		float r[3];
		vadd(r,mesh->verts[i].co,vector);

		mesh->verts[i].co[0] = r[0];
		mesh->verts[i].co[1] = r[1];
		mesh->verts[i].co[2] = r[2];
	}

	node->shell->loc[0]=0;
	node->shell->loc[1]=0;
	node->shell->loc[2]=0;

	mesh_build_vertex_array(node->data);
	*/

	printf("[Out Of Order]\n");

}	
