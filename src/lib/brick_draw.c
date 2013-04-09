/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "ui.h"
#include "txt.h"
#include "context.h"

#define _PRECISION "%-.2f"

int brick_get_width(t_brick *brick)
{
	if(brick->state.use_block_width)
	{
		t_block *block=brick->block;
		return block->width;
	}
	else
	{
		return 0;
	}

}

void brick_build_width(t_brick *brick)
{
	t_txt *txt_name=&brick->txt_name;
	t_txt *txt_data=&brick->txt_data;

	float width_name=txt_name->get_width(txt_name);
	float width_data=txt_name->get_width(txt_data);
	float tot=0;

	// if with value, add value width
	if(brick->state.draw_value)
	{
		tot=width_data;
	}

	if(brick->state.draw_name)
	{
		tot+=width_name;
	}

	// if switch, add more margin
	if(brick->cls->type==bt_switch)
	{
		tot+=20;
	}

	if(brick->state.use_min_width)
	{
		if(tot<brick->geom.min_width)
		{
			tot=brick->geom.min_width;
		}
	}

	brick->geom.width=tot;
}

void brick_build_txt(t_brick *brick)
{
	t_plug *plug_intern=&brick->plug_intern;
	t_data_type type = plug_intern->data_type;

	t_txt *txt_data=&brick->txt_data;

	char txt[30];
	char *_type;

	//-- vector brick is not initialized
	if(plug_intern->data)
	{
		switch(type)
		{
			case dt_float:
				snprintf(txt,30,_PRECISION,drf_float(plug_intern->data));
				txt_data->data_change(txt_data,txt);
				break;

			case dt_int:
				snprintf(txt,30,"%-d",drf_int(plug_intern->data));
				txt_data->data_change(txt_data,txt);
				break;
			case dt_char:
				txt[0]=drf_char(plug_intern->data);
				txt[1]='\0';
				break;

			case dt_selector:
				txt_data->data_change(txt_data,&brick->var.selector_list[brick->var.selector*_NAME_LONG_]);
				break;

			default:
				_type=data_name_get(type);
				txt_data->data_change(txt_data,_type);
				break;
		}
	}
}

void brick_draw_link(t_brick *brick)
{
	t_context *C=ctx_get();

	float width=brick_get_width(brick);
	width+=brick->geom.height*2;

	if(brick->state.draw_plugs)
	{
		t_plug *plug;
		// draw line from plug_out
		plug = &brick->plug_out;

		if(plug->dst) 
		{

			t_block *block=brick->block;
			t_plug *plug_target=plug->dst;
			t_brick *brick_target=plug_target->brick;

			t_block *block_target=brick_target->block;

			// out: end of brick
			float ax= (float)block->pos[0]+ width + (brick->geom.height/2);
			float ay = (float)block->pos[1]+(float)((brick->geom.block_pos*brick->geom.height)+(brick->geom.height/2));

			// in: target
			float bx = (float)block_target->pos[0]-brick->geom.height/2;
			float by = (float)block_target->pos[1]+(float)((brick_target->geom.block_pos*brick->geom.height)+(brick->geom.height/2));

			float v1[3] = {ax,ay,0};
			float v2[3] = {bx,by,0};
			float *color=C->ui->front_color;
			int width=1;
			
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(C->ui->pan_x,C->ui->pan_y,0);
			skt_line(v1,v2,width,color);
			glPopMatrix();
		}
	}
}

void brick_draw_plug_state(t_brick *brick,t_plug *plug,float pos)
{
	float height = brick->geom.height-.2;
	float mid = height / 2;
	float x = pos + brick->geom.height/4;
	float offset = 5;
	float raw = 5;

	float pos_flow_in[3]={x,mid+offset,0};
	float pos_open_in[3]={x,mid,0};
	float pos_follow_in[3]={x,mid-offset,0};

	float pos_flow_out[3]={x+raw,mid+offset,0};
	float pos_open_out[3]={x+raw,mid,0};
	float pos_follow_out[3]={x+raw,mid-offset,0};

	int width = 2;

	float red[3] = {1,0,0};
	float green[3] = {0,1,0};

	float *color_flow_in = red;
	float *color_open_in = red;
	float *color_follow_in = red;

	float *color_flow_out = red;
	float *color_open_out = red;
	float *color_follow_out = red;

	if(plug->flow_in) color_flow_in = green;
	if(plug->open_in) color_open_in = green;
	if(plug->follow_in) color_follow_in = green;

	if(plug->flow_out) color_flow_out = green;
	if(plug->open_out) color_open_out = green;
	if(plug->follow_out) color_follow_out = green;

	skt_point(pos_flow_in,width,color_flow_in); 
	skt_point(pos_open_in,width,color_open_in); 
	skt_point(pos_follow_in,width,color_follow_in); 

	skt_point(pos_flow_out,width,color_flow_out); 
	skt_point(pos_open_out,width,color_open_out); 
	skt_point(pos_follow_out,width,color_follow_out); 
}

void brick_draw_plug(t_brick *brick,int is_out)
{
	t_context *C = ctx_get();
	t_plug *plug;

	float pos[3]={0,0,0};
	float c[3];

	float width=brick_get_width(brick);

	// plug left : translate width

	if(is_out)
	{
		pos[0]=width+brick->geom.height;
		plug=&brick->plug_out;
	}
	else
	{
		plug=&brick->plug_in;
	}

	if(C->draw->mode==mode_selection) 
	{
		// col id
		cconv(c,plug->idcol);
		// draw square
		skt_msh_square(pos,brick->geom.height,c);
	}
	else if(C->ui->show_states)
	{

		if(is_out) brick_draw_plug_state(brick,&brick->plug_out,brick->geom.width+brick->geom.height+brick->geom.height/4);
		else brick_draw_plug_state(brick,&brick->plug_in,0);
	}
}

void brick_draw_out(t_brick *brick)
{
	if(brick->state.draw_plugs)
	{
		brick_draw_plug(brick,1);
	}
}

void brick_draw_in(t_brick *brick)
{
	if(brick->state.draw_plugs)
	{
		brick_draw_plug(brick,0);
	}
}

void brick_draw_outline(t_brick *brick)
{
	t_context *C=ctx_get();

	if(
		(brick->state.draw_outline && C->draw->mode==mode_draw)
		|| C->ui->show_brick_step
		) 
	{
		float pos[3]={0,0,0};
		int tot=4;
		float width = brick_get_width(brick);
		float height = brick->geom.height;
		int line_width=1;

		// add plugs width
		if(brick->state.draw_plugs)
		{
			width+=((brick->geom.height)*2);
		}

		// set line width
		if(brick->state.is_mouse_over)
		{
			 line_width=2; 
		}

		// build points

		float points[tot*3];

		float a[3]={pos[0],pos[1],0};
		float b[3]={a[0],a[1]+height,0};
		float c[3]={b[0]+width,b[1],0};
		float d[3]={c[0],c[1]-height,0};

		points[0]=a[0];
		points[1]=a[1];
		points[2]=a[2];

		points[3]=b[0];
		points[4]=b[1];
		points[5]=b[2];

		points[6]=c[0];
		points[7]=c[1];
		points[8]=c[2];

		points[9]=d[0];
		points[10]=d[1];
		points[11]=d[2];

		//  draw polyline

		float *color;
		float red[3] = {1,0,0};
		float green[3] = {0,1,0};
		float yellow[3]={1,1,0};

		if(C->ui->show_brick_step)
		{
			if(brick->state.is_root) line_width = 4;

			if(brick->state.is_current)
			{
				color=yellow;
				line_width=4;

			}
			else
			{
				if(brick->plug_intern.is_updated) color = green;
				else color = red;
			}
			
			skt_closedline(points,tot,color,line_width);
		}
		else
		{
			if(brick->state.is_mouse_over) 
			{
				skt_closedline(points,tot,C->ui->front_color,line_width);
				skt_closedline_filled(points,tot,C->ui->back_color,line_width);
			}
			else
			{
				skt_closedline(points,tot,C->ui->front_color,line_width);
			}
		}
	}
}

void brick_draw_body(t_brick *brick)
{
	t_context *C = ctx_get();

	// selection mode only
	if(C->draw->mode==mode_selection) 
	{
		// colors
		float lcol[3]; // left color
		float rcol[3]; // right color
		cconv(lcol,brick->idcol_left);
		cconv(rcol,brick->idcol_right);

		float width = brick_get_width(brick);
		float height = brick->geom.height;

		// split half
		width /= 2;	

		float a[3];    // right corner
		a[0]=0;
		a[1]=0;
		a[2]=0;

		// if plug : translate left
		if(brick->state.draw_plugs)
		{
			a[0]=brick->geom.height;
		}
		
		// draw half
		skt_msh_rectangle(a,width,height,lcol);

		// translate left
		a[0]+=width;

		// draw half
		skt_msh_rectangle(a,width,height,rcol);
	}
}

void brick_draw_switch_val(t_brick *brick)
{
	t_context *C = ctx_get();
	float *color=C->ui->front_color;

	if(C->draw->mode==mode_draw)
	{
		float vec[3]={10,7.5,0};

		int *d=brick->plug_intern.data;

		// off
		if(*d==0)
		{
			skt_line_rectangle(vec,5,5,1,color);
		}
		// on
		else
		{
			skt_msh_rectangle(vec,5,5,color);
		}
	}
}

void brick_draw_txt(t_brick *brick)
{
	t_context *C = ctx_get();

	if(C->draw->mode==mode_draw)
	{
		glPushMatrix();

			// margin
			if(brick->state.draw_plugs)
			{
				glTranslatef(brick->geom.height+1,0,0);
			}

			// switch
			if(brick->cls->type==bt_switch)
			{
				brick_draw_switch_val(brick);
				glTranslatef(25,0,0);
			}

			//margin

			if(!brick->state.draw_plugs)
			{
				glTranslatef(brick->geom.margin,0,0);
			}

			// vertical align (middle)
			glTranslatef(0,5,0);

			// draw NAME
			if(brick->state.draw_name)
			{
				brick->txt_name.draw(&brick->txt_name);
				glTranslatef(brick->txt_name.width,0,0);
				glTranslatef(5,0,0);
			}

			// draw DATA
			if(brick->state.draw_value || C->ui->show_step)
			{
				if(brick->txt_data.name)
					brick->txt_data.draw(&brick->txt_data);
			}

		glPopMatrix();
	}
}

void brick_draw(t_brick *brick)
{
	int draw=1;
	if(is(brick->name,"object"))
	{
		t_context *C=ctx_get();
		if(C->scene->selected)
		{
			t_node *node=C->scene->selected;
			if(node->type==nt_object)
			{
			}
			else
			{
				draw=0;
			}
		}
		else
		{
			draw=0;
		}
	}

	if(draw)
	{
		brick_draw_txt(brick);
		brick_draw_in(brick);
		brick_draw_body(brick);
		brick_draw_out(brick);
		brick_draw_link(brick);
		brick_draw_outline(brick);
	}
}
