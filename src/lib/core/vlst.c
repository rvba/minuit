/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "base.h"
#include "ctx.h"
#include "node.h"
#include "scene.h"

#include "op.h"
#include "action.h"
#include "event.h"
#include "term.h"
#include "list.h"
#include "data.h"
#include "dict.h"
#include "ui.h"
#include "block.h"
#include "memory.h"
#include "vlst.h"
#include "brick.h"

int vlst_get_size( t_vlst *vlst)
{
	return( vlst->count * vlst->length * vlst->size);
}

void vlst_map( t_vlst *vlst, t_data_type type, float start, float end)
{
	switch( type)
	{
		case( dt_uchar):
			if( vlst->type == dt_float)
			{
				float *src = ( float *) vlst->data;
				int size = vlst_get_size( vlst);
				unsigned char *data = (unsigned char *) mem_malloc( size);
				int count = vlst->count * vlst->length;
				
				int i;
				for( i = 0; i < count ; i++)
				{
					int a = cpt_map_fi( src[i] , 0, 1, 0, 255);
					data[i] = (unsigned char) a;
				}

				mem_free( vlst->data, size);

				vlst->type = dt_uchar;
				vlst->data = data;
			}

			break;
		default: printf("Not Implemented\n");
	}
}

void *vlst_get_pointer(t_vlst *vlst, int indice)
{
	void *ptr = vlst->data;

	switch(vlst->type)
	{
		case(dt_float): return grf_float(ptr, indice); break;
		case(dt_uint): return grf_uint(ptr, indice); break;
		default: 
			printf("[ERR vlst_get_pointer] Unknown type %s\n",data_name_get(vlst->type));
			return NULL;
			break;
	}
}

void vlst_set_data(t_vlst *vlst, void *data, int indice)
{
	void *ptr = vlst->data;

	switch(vlst->type)
	{
		case(dt_float): srf_float(ptr, data, indice); break;
		case(dt_uint): srf_uint(ptr, data,  indice); break;
		case(dt_int): srf_int(ptr, data,  indice); break;
		default: 
			printf("[ERR vlst_set_data] Unknown type %s\n",data_name_get(vlst->type));
			break;
	}
}

void vlst_data_init(t_vlst *vlst, int old_count)
{
	int i,j;
	int indice;

	indice = old_count * vlst->length;

	for(i = old_count; i < vlst->count; i++)
	{
		for(j = 0; j < vlst->length; j++)
		{
			switch(vlst->type)
			{
				case(dt_uint):
					set_uint(grf_uint(vlst->data, indice),0);
					break;

				case(dt_int):
					set_int(grf_int(vlst->data, indice),0);
					break;

				case(dt_float):
					set_float(grf_float(vlst->data, indice),0);
					break;
				default:
					printf("[vlst_data_init] %s not implemented\n", data_name_get(vlst->type));
					break;
			}

			indice++;
		}
	}
}
	
void vlst_change_data( t_vlst *vlst,t_vlst *caller)
{
	t_context *C=ctx_get();

	if(vlst->count != vlst->count_new)
	{
		void *old_ptr=vlst->data;

		// set count
		int old_count = vlst->count;
		vlst->count=vlst->count_new;
		int new_size=(vlst->size)*(vlst->length)*(vlst->count);
		void *new_ptr = NULL;

		if(old_ptr)
		{
			// get data,var
			t_node *node_data=scene_get_data(C->scene,old_ptr);
			t_node *node_var=scene_get_var(C->scene,old_ptr);

			// remove data,var
			scene_node_free(C->scene,node_data);
			scene_node_free(C->scene,node_var);

			// realloc
			new_ptr=realloc(vlst->data,new_size);
		}
		else
		{
			new_ptr = mem_malloc((vlst->size)*(vlst->length)*(vlst->count));
		}

		if(!new_ptr) printf("[ERROR vls_update_data]\n"); 

		// set new ptr
		vlst->data=new_ptr;

		// init
		vlst_data_init(vlst,old_count);

		// add new data,var
		scene_store(C->scene,1);
		scene_add_data_var(C->scene,"vlst_data","v_data",new_size,vlst->data);
		scene_store(C->scene,0);

		// shared vlst (vertex, color , ....)
		if(vlst->is_linked)
		{
			t_vlst *link_vlst = vlst->link;
			link_vlst->count_new = vlst->count;

			if(caller)
			{
				// prevent infinite loop
				if(link_vlst->id.id != caller->id.id)
				{
					vlst_update_data( link_vlst, vlst);
				}
				
			}
			else
			{
				vlst_update_data( link_vlst, vlst);
			}
		}
	}
}

void exe_vlst(t_action *action)
{
	t_dict *args = action->args;
	t_vlst *vlst = dict_pop_data(args,"vlst");
	t_vlst *caller = dict_pop_data(args,"caller");

	vlst_change_data( vlst, caller);

	action->done = 1;
}

void vlst_update_data( t_vlst *vlst,t_vlst *caller)
{
	t_action *action = action_new("action");
	t_dict *dict = dict_make( "args");

	action->act = exe_vlst;
	action->args = dict;

	dict_symbol_add(action->args,"vlst",dt_null,vlst);
	dict_symbol_add(action->args,"caller",dt_null,caller);

	exe_add_action(action);
}

void vlst_float_add(t_vlst *vlst,t_data_type type,void *data)
{
	int count=vlst->count;
	int length=vlst->length; 
	int i,j;

	float *data_dst=vlst->data;

	for(i=0;i<count;i++)
	{
		for(j=0;j<length;j++)
		{
			switch(type)
			{
				case(dt_int): *data_dst += (float)drf_int(data); break;
				case(dt_float): *data_dst += drf_float(data); break;
				default:printf("? ? !!\n");
			}

			data_dst++;
		}
	}
}

void vlst_add_vlst(t_vlst *vlst_dst,t_vlst *vlst_src) 
{
	printf(".........................\n");
	/*
	int i,j;
	int p=0;

	if(vlst_dst->type==vlst_src->type)
	{
		void *data_dst=vlst_dst->data;
		void *data_src=vlst_src->data;

		for(i=0;i<vlst_dst->count;i++)
		{
			for(j=0;j<vlst_dst->length;j++)
			{
				if(vlst_dst->data_type==dt_float)
				{
					float *d=(float *)data_dst;
					float *s=(float *)data_src;

					d[p]+=(s[p]);
				}
				p++;
			}

		}
	}
	else
	{
		printf("[WARNING vlst_add_vlst] vlst are not equal\n");
	}
	*/
}

t_term *term = NULL;

void vlst_info( t_term *t, t_vlst *vlst)
{
	term = t;
	vlst_show( vlst);
	term = NULL;
}

void vlst_show(t_vlst *vlst)
{
	t_context *C = ctx_get();
	if(vlst)
	{
		int i;
		int count=vlst->count;
		int length=vlst->length;
		void *dat=vlst->data;

		if(C->event->debug_terminal)
			printf("[VLST] type:%s name:%s count:%d length:%d\n", data_name_get(vlst->type), vlst->id.name, count, length);
		else 
			printf("C->event->debug_terminal must be on !\n");
		if(C->event->debug_console)
			term_log("[VLST] type:%s name:%s count:%d length:%d", data_name_get(vlst->type), vlst->id.name, count, length);

		if( term) term_l( term, "[VLST] type:%s name:%s count:%d length:%d", data_name_get(vlst->type), vlst->id.name, count, length);

		if(dat)
		{
			if(vlst->type == dt_float)
			{
				if(vlst->length == 3)
				{
					float *d=(float *)dat;
					for(i=0;i<count;i++)
					{
						if(C->event->debug_terminal)
							printf("[%d] %f %f %f\n",i,d[0],d[1],d[2]);
						if(C->event->debug_console)
							term_log("[%d] %f %f %f",i,d[0],d[1],d[2]);
						if( term)
							term_l( term, "[%d] %f %f %f",i,d[0],d[1],d[2]);

						d+=length;
					}
				}
				else if(vlst->length == 4)
				{
					float *d=(float *)dat;
					for(i=0;i<count;i++)
					{
						if(C->event->debug_terminal)
							printf("[%d] %f %f %f %f\n",i,d[0],d[1],d[2],d[3]);
						if(C->event->debug_console)
							term_log("[%d] %f %f %f %f",i,d[0],d[1],d[2],d[3]);
						if( term)
							term_l( term, "[%d] %f %f %f %f",i,d[0],d[1],d[2],d[3]);

						d+=length;
					}
				}
			}

			else if(vlst->type == dt_uint)
			{
				if(vlst->length == 2)
				{
					unsigned int *d=(unsigned int *)dat;
					for(i=0;i<count;i++)
					{
						if(C->event->debug_terminal)
							printf("[%d] %u %u \n",i,d[0],d[1]);
						if(C->event->debug_console)
							term_log("[%d] %u %u",i,d[0],d[1]);
						if( term)
							term_l( term, "[%d] %u %u",i,d[0],d[1]);

						d+=length;
					}
				}
				else if(vlst->length == 3)
				{
					unsigned int *d=(unsigned int *)dat;
					for(i=0;i<count;i++)
					{
						if(C->event->debug_terminal)
							printf("[%d] %u %u %u \n",i,d[0],d[1],d[2]);
						if(C->event->debug_console)
							term_log("[%d] %u %u %u",i,d[0],d[1],d[2]);
						if( term)
							term_l( term, "[%d] %u %u %u",i,d[0],d[1],d[2]);

						d+=length;
					}
				}
				else if(vlst->length == 4)
				{
					unsigned int *d=(unsigned int *)dat;
					for(i=0;i<count;i++)
					{
						if(C->event->debug_terminal)
							printf("[%d] %u %u %u %u\n",i,d[0],d[1],d[2],d[3]);
						if(C->event->debug_console)
							term_log("[%d] %u %u %u %u",i,d[0],d[1],d[2],d[3]);
						if( term)
							term_l( term, "[%d] %u %u %u %u",i,d[0],d[1],d[2],d[3]);
						d+=length;
					}
				}
			}
			else if(vlst->type == dt_int)
			{
				if(vlst->length == 4)
				{
					int *d=(int *)dat;
					for(i=0;i<count;i++)
					{
						if(C->event->debug_terminal)
							printf("[%d] %d %d %d %d\n",i,d[0],d[1],d[2],d[3]);
						if(C->event->debug_console)
							term_log("[%d] %d %d %d %d",i,d[0],d[1],d[2],d[3]);
						if( term)
							term_l( term, "[%d] %d %d %d %d",i,d[0],d[1],d[2],d[3]);
						d+=length;
					}
				}

				else if(vlst->length == 2)
				{
					int *d=(int *)dat;
					for(i=0;i<count;i++)
					{
						if(C->event->debug_terminal)
							printf("[%d] %d %d \n",i,d[0],d[1]);
						if(C->event->debug_console)
							term_log("[%d] %d %d",i,d[0],d[1]);
						if( term)
							term_l( term, "[%d] %d %d",i,d[0],d[1]);
						d+=length;
					}
				}
			}
		}
		else
		{
			printf("NO DATA\n");
		}
	}
}

void _loop_int(void *dst,void *src,int count,int size)
{
	int i;
	int *s=(int *)src;
	int *d=(int *)dst;

	int p=0;

	for(i=0;i<count;i++)
	{
		if(size>=1) d[p+0]=s[p+0];
		if(size>=2) d[p+1]=s[p+1];
		if(size>=3) d[p+2]=s[p+2];
		if(size>=4) d[p+3]=s[p+3];

		p+=size;
	}
}

void _loop_float(void *dst,void *src,int count,int size)
{

	if(src)
	{
		int i;
		float *s=(float *)src;
		float *d=(float *)dst;

		int p=0;

		for(i=0;i<count;i++)
		{
			if(size>=1) d[p+0]=s[p+0];
			if(size>=2) d[p+1]=s[p+1];
			if(size>=3) d[p+2]=s[p+2];
			if(size>=4) d[p+3]=s[p+3];

			p+=size;
		}
	}
	else
	{
		int i;
		float *d=(float *)dst;

		int p=0;

		float pp=.0001;

		for(i=0;i<count;i++)
		{
			if(size>=1) d[p+0]+=rnd_range(0,100)*pp;
			if(size>=2) d[p+1]+=rnd_range(0,100)*pp;
			if(size>=3) d[p+2]+=rnd_range(0,100)*pp;
			if(size>=4) d[p+3]+=rnd_range(0,100)*pp;

			p+=size;
		}
	}
}

void _loop_unsigned_int(void *dst,void *src,int count,int size)
{
	int i;
	unsigned int *s=(unsigned int *)src;
	unsigned int *d=(unsigned int *)dst;

	int p=0;

	for(i=0;i<count;i++)
	{
		if(size>=1) d[p+0]=s[p+0];
		if(size>=2) d[p+1]=s[p+1];
		if(size>=3) d[p+2]=s[p+2];
		if(size>=4) d[p+3]=s[p+3];

		p+=size;
	}
}

void _loop_unsigned_char(void *dst,void *src,int count,int size)
{
	int i;
	unsigned char *s=(unsigned char *)src;
	unsigned char *d=(unsigned char *)dst;

	int p=0;

	for(i=0;i<count;i++)
	{
		if(size>=1) d[p+0]=s[p+0];
		if(size>=2) d[p+1]=s[p+1];
		if(size>=3) d[p+2]=s[p+2];
		if(size>=4) d[p+3]=s[p+3];

		p+=size;
	}
}

void _face_int(void *src,void *dst,int count,int size)
{
	int i;
	int *d=(int *)dst;

	int offset=0;

	for(i=0;i<count;i++)
	{
		if(size>=1) d[0]=offset;
		if(size>=2) d[1]=offset+1;
		if(size>=3) d[2]=offset+2;
		if(size>=4) d[3]=offset+3;

		d+=size;
		offset+=size;
	}
}

void _face_float(void *src,void *dst,int count,int size)
{
	printf("Errrr\n");
	/*
	*
	*
	*/
}

void _face_unsigned_int(void *src,void *dst,int count,int size)
{
	int i;
	unsigned int *d=(unsigned int *)dst;

	int offset=0;

	for(i=0;i<count;i++)
	{
		if(size>=1) d[0]=(unsigned int)offset;
		if(size>=2) d[1]=(unsigned int)offset+1;
		if(size>=3) d[2]=(unsigned int)offset+2;
		if(size>=4) d[3]=(unsigned int)offset+3;

		d+=size;
		offset+=size;
	}
}

void vlst_color_3f(t_vlst *vlst,int *color)
{
	if(vlst)
	{
		float r=((float)color[0])/255.0f;
		float g=((float)color[1])/255.0f;
		float b=((float)color[2])/255.0f;

		int i;
		float *d=(float *)vlst->data;

		int pos=0;

		for(i=0;i<vlst->count;i++)
		{

			d[pos]=r;
			d[pos+1]=g;
			d[pos+2]=b;

			pos+=3;
		}
	}
}

void vlst_normal_3f(t_vlst *dst,t_vlst *vertex,t_vlst *face)
{
	int i;
	int pos=0;
	int _pos=0;

	float *v=(float *)vertex->data;
	float *d=(float *)dst->data;

	for(i=0;i<face->count;i++)
	{
		float v1[3]={v[pos],v[pos+1],v[pos+2]};
		float v2[3]={v[pos+3],v[pos+4],v[pos+5]};
		float v3[3]={v[pos+6],v[pos+7],v[pos+8]};

		float result[3];

		calc_normals(v1,v2,v3,result);

		int k;
		for(k=0;k<3;k++)
		{
			d[_pos+0]=result[0];
			d[_pos+1]=result[1];
			d[_pos+2]=result[2];

			_pos+=3;
		}

		pos+=9; // 4 vertices
	}	
}

void vlst_normal_4f(t_vlst *dst,t_vlst *vertex,t_vlst *face)
{
	int i;
	int pos=0;
	int _pos=0;

	float *v=(float *)vertex->data;
	float *d=(float *)dst->data;

	for(i=0;i<face->count;i++)
	{
		float v1[3]={v[pos],v[pos+1],v[pos+2]};
		float v2[3]={v[pos+3],v[pos+4],v[pos+5]};
		float v3[3]={v[pos+6],v[pos+7],v[pos+8]};

		float result[3];

		calc_normals(v1,v2,v3,result);

		int k;
		for(k=0;k<4;k++)
		{
			d[_pos+0]=result[0];
			d[_pos+1]=result[1];
			d[_pos+2]=result[2];

			_pos+=3;
		}

		pos+=12; // 4 vertices
	}	
}

void vlst_normal_4f_direct(t_vlst *dst,t_vlst *vertex,t_vlst *quads)
{
	int i,n,j;
	float vectors[3*4];
	int *q=quads->data;
	float *v=vertex->data;
	float *normal=dst->data;

	// calc normals
	j=0;
	for(i=0;i<quads->count;i++)
	{
		for(n=0;n<4;n++)
		{
			vectors[(n*3)+0]=v[(q[j]*3)];
			vectors[(n*3)+1]=v[(q[j]*3)+1];
			vectors[(n*3)+2]=v[(q[j]*3)+2];
			j++;
		}

		calc_normals(vectors,vectors+3,vectors+6,normal+(i*3));
	}
}

void vlst_vertex(const char *type,t_vlst *dst,t_vlst *vertex,t_vlst *face)
{
	int i;
	int *f=face->data;
	float *v=(float *)vertex->data;
	float *d=(float *)dst->data;

	int j=0;
	int pos=0;

	for(i=0;i<face->count;i++)
	{
		if(face->length>=1)
		{
			d[pos+0] = v[(f[j+0]*3)+0];
			d[pos+1] = v[(f[j+0]*3)+1];
			d[pos+2] = v[(f[j+0]*3)+2];
		}
		if(face->length>=2)
		{
			d[pos+3] = v[(f[j+1]*3)+0];
			d[pos+4] = v[(f[j+1]*3)+1];
			d[pos+5] = v[(f[j+1]*3)+2];
		}
		if(face->length>=3)
		{
			d[pos+6] = v[(f[j+2]*3)+0];
			d[pos+7] = v[(f[j+2]*3)+1];
			d[pos+8] = v[(f[j+2]*3)+2];
		}
		if(face->length>=4)
		{
			d[pos+9]  = v[(f[j+3]*3)+0];
			d[pos+10] = v[(f[j+3]*3)+1];
			d[pos+11] = v[(f[j+3]*3)+2];
		}

		pos+=(face->length*3);
		j+=face->length;

		//printf("done\n");
	}
}

// COPY

t_vlst *vlst_copy( t_vlst *vlst_src)
{
	t_vlst *vlst = vlst_new( vlst_src->id.name);

	vlst->count = vlst_src->count;
	vlst->length = vlst_src->length;
	vlst->size = vlst_src->size;
	vlst->count_new = vlst_src->count_new;
	vlst->need_update = vlst_src->need_update;
	vlst->is_linked = vlst_src->is_linked;
	vlst->link = vlst_src->link;
	vlst->type = vlst_src->type;

	size_t size = vlst->size * vlst->length * vlst->count;
	vlst->data = mem_malloc(size);
	memcpy( vlst->data, vlst_src->data, size);

	return vlst;
}

void vlst_add_data(t_vlst *vlst,void *ptr)
{
	t_data_type t=vlst->type;

	if (t == dt_int) _loop_int(vlst->data,ptr,vlst->count,vlst->length);
	else if	(t == dt_float) _loop_float(vlst->data,ptr,vlst->count,vlst->length);
	else if	(t == dt_uint) _loop_unsigned_int(vlst->data,ptr,vlst->count,vlst->length);
	else if	(t == dt_uchar) _loop_unsigned_char(vlst->data,ptr,vlst->count,vlst->length);
	else 	printf("[ERROR vlst_add_data] Unknown type %d\n",t);
}

void vlst_copy_data(t_vlst *dst,t_vlst *src)
{
	if(src->type==dst->type)
	{
		t_data_type t=src->type;

		if (t == dt_int) _loop_int(dst->data,src->data,src->count,src->length);
		else if	(t == dt_float) _loop_float(dst->data,src->data,src->count,src->length);
		else if	(t == dt_uint) _loop_unsigned_int(dst->data,src->data,src->count,src->length);
		else 	printf("[ERROR vlst_copy] Unknown type %d\n",t);
	}
	else
	{
		printf("[ERROR vlst_copy]Vlists don't match\n");
	}
}

// BUILD

void vlst_build(t_vlst *vlst,void *ptr)
{
	t_data_type t = vlst->type;

	if (t == dt_int) _face_int(ptr,vlst->data,vlst->count,vlst->length);
	else if	(t == dt_float) _face_float((float *)vlst->data,(float *)ptr,vlst->count,vlst->length);
	else if	(t == dt_uint) _face_unsigned_int(ptr,vlst->data,vlst->count,vlst->length);
	else 	printf("[ERROR _copy] Unknown type %d\n",t);
}

// MAKE

t_vlst *vlst_make( const char *name, t_data_type type, int length, int count, void *data)
{
	t_context *C=ctx_get();

	t_node *node_vlst=scene_add(C->scene,dt_vlst,name);
	t_vlst *vlst=node_vlst->data;

	vlst->type = type;
	vlst->length = length;
	vlst->count=count;
	vlst->count_new=vlst->count;

	if(type == dt_int) vlst->size = sizeof(int);
	else if(type == dt_uint) vlst->size = sizeof(unsigned int);
	else if(type == dt_float) vlst->size = sizeof(float);
	else if(type == dt_uchar) vlst->size = sizeof(unsigned char);
	else printf("[ERROR vlst_make] Unknown type %s\n",data_name_get(type));

	if( !data && length && count)
	{
		vlst->data=mem_malloc((vlst->size)*(vlst->length)*(vlst->count));
		if(C->scene->store)
		{
			 scene_add_data_var(C->scene,"vlst_data","v_data",((vlst->size)*(vlst->length)*(vlst->count)),vlst->data);
		}
	}
	else
	{
		vlst->data = data;
	}

	if(C->ui->add_bricks && C->ui->add_bricks_data)
	{
		t_node *node_block=add_block(C,"vlst");
		t_block *block=node_block->data;
		add_part_vlst(C,block,dt_vlst,"vlst",vlst);
		vlst->ref = block;
	}


	return vlst;
}

// CLONE

/*
t_vlst *vlst_duplicate(t_vlst *vlst)
{
	t_vlst *vlst_new = vlst_make(vlst->id.name, vlst->type, vlst->length, vlst->count, NULL);
	vlst_copy(vlst_new,vlst->data);

	return vlst_new;
}
*/

// INIT

void vlst_init(t_vlst *vlst)
{
	int size=vlst->count*vlst->length*vlst->size;
	bzero(vlst->data,size);
}

// CLONE

/*
t_vlst *vlst_clone(t_vlst *vlst)
{
	if(vlst)
	{
		t_vlst *clone = vlst_new(vlst->id.name);

		clone->count = vlst->count;
		clone->length = vlst->length;
		clone->size = vlst->size;
		clone->count_new = vlst->count_new;
		clone->need_update = vlst->need_update;
		clone->is_linked = vlst->is_linked;

		clone->link = NULL; //XXX
		clone->type = vlst->type;

		size_t size = sizeof(vlst->type)*vlst->length*vlst->count;
		clone->data = mem_malloc(size);
		memcpy(clone->data,vlst->data,size);

		return clone;
	}
	else
	{
		return NULL;
	}
}
*/

// REF

void *vlst_get_ref(t_vlst *vlst, const char *ref)
{
	void *p;

	if(iseq(ref,"count"))  			p=&vlst->count_new; 
	else
	{
		printf("[ERROR vlst_get_ref] Unknown ref [%s] \n",ref);
		return NULL;
	}

	return p;
}


// REBIND

void vlst_rebind(t_scene *sc,void *ptr)
{
	t_vlst *vlst=(t_vlst *)ptr;

	// REBIND

	rebind(sc,"vlst","data",(void **)&vlst->data);
	rebind(sc,"vlst","link",(void **)&vlst->link);
}

// NEW

void *vlst_new(const char *name)
{
	t_vlst *vlst=(t_vlst *)mem_malloc(sizeof(t_vlst));

	id_init(&vlst->id, name);

	vlst->type=dt_null;
	vlst->count=0;
	vlst->count_new=0;
	vlst->data=NULL;
	vlst->need_update=0;
	vlst->is_linked=0;
	vlst->link=NULL;
	vlst->ref = NULL;
	vlst->type = dt_null;
	vlst->type_target = dt_null;

	return vlst;
}

void vlst_free(t_vlst *vlst)
{
	t_context *C = ctx_get();
	int l = vlst->count * vlst->length * vlst->size;
	if(vlst->data) mem_free(vlst->data, l);
	if(vlst->ref) scene_delete( C->scene, vlst->ref);
	mem_free( vlst, sizeof( t_vlst));
}

void vlst_delete( t_vlst *vlst)
{
	t_scene *sc = ctx_scene_get();
	if( vlst->id.store)  scene_delete( sc, vlst); 
	else  vlst_free( vlst); 
}

