/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "ui.h"
#include "context.h"
#include "event.h"
#include "sketch.h"
#include "block.h"
#include "brick.h"
#include "list.h"

// update txt && mouse over
void block_update_data(t_block *block)
{
	t_link *link;
	t_brick *brick;

	// loop over bricks
	for(link=block->bricks->first;link;link=link->next)
	{
		brick = link->data;

		// update txt
		if(brick->state.draw_value)
		{
			if(is(brick->name,"camera_main")) printf("block:brick\n");
			brick_build_txt(brick);
		}

		// build width (once)
		if(block->state.update_geometry)
		{
			brick_build_width(brick);
		}
	}
}


// update geometry

void block_update_geometry(t_block *block)
{
	if(block->state.update_geometry)
	{
		t_link *link;

		float block_width=0;
		float width=0;

		// switch state
		block->state.update_geometry=0;

		// reset block height
		block->height=0;

		// loop over bricks
		for(link=block->bricks->first;link;link=link->next)
		{
			t_brick *brick = link->data;

			// Height
			if(brick->state.draw)
				block->height += brick->geom.height;

			// set draw plugs
			if(brick->state.draw_plugs)
			{
				block->state.draw_plugs=1;
			}

			// find max width

			width = brick->geom.width;

			if(width >= block_width)
			{
				block_width=width;
			}
		}

		// store max width
		block->width=block_width;
	}

}

void block_draw_bricks(t_block *block)
{
	t_link *link;
	t_brick *brick;

	int offset=2;

	glPushMatrix();

	// Translate to Block Pos
	glTranslatef(block->pos[0],block->pos[1],0);

	// Loop over Bricks
	for(link=block->bricks->first;link;link=link->next)
	{
		brick = link->data;

		// Draw brick
		brick->cls->draw(brick);

		// If Sub-menu
		if(brick->cls->type==bt_menu)
		{
			if(brick->state.show_menu)
			{
				glPushMatrix();

					// Translate Right
					glTranslatef(((block->width)+offset),0,0);

					// Draw Sub Block
					t_block *menu = brick->menu;
					menu->cls->draw(menu);

				glPopMatrix();
			}
		}

		// Translate Up
		if(brick->state.draw) glTranslatef(0,brick->geom.height,0);
	}

	glPopMatrix();

}

void block_draw_outline(t_block *block)
{
	t_context *C=ctx_get();

	if(block->state.draw_outline)
	{
		int tot=4;
		float *color=C->ui->front_color;
		float points[tot*3];
		float *pos=block->pos;
		int line_width;

		// set line width
		if(block->state.is_mouse_over) line_width=2; 
		else  line_width=1; 

		float width=block->width;

		float a[3]={pos[0], pos[1], 0};
		float b[3]={a[0],a[1] + block->height ,0};
		float c[3]={b[0] + width, b[1], 0};
		float d[3]={c[0], c[1] - block->height ,0};

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

		if(C->event->ui.draw_in_loop)
		{
			glLineStipple(2, 0xAAAA);
			//glEnable(GL_LINE_STIPPLE);
		}

		if(block->state.is_root)
			line_width = 2;

		skt_closedline(points,tot,color,line_width);

		if(C->ui->show_step)
		{
			char order[3];
			float p[3] = {0,0,0};
			float vv[3] = {-10,-10,0};
			vadd(p,a,vv);
			sprintf(order,"%d",block->graph_order);

			t_txt *txt = txt_new(order);
			txt_init(txt,order);
			glPushMatrix();
				glTranslatef(p[0],p[1],p[2]);
				txt->draw(txt);
			glPopMatrix();
			txt_free(txt);
		}

		if(block->graph)
		{

		char order[3];
		float p[3] = {0,0,0};
		float vv[3] = {-10,-10,0};
		vadd(p,a,vv);
		sprintf(order,"%d",block->graph_pos);

		t_txt *txt = txt_new(order);
		txt_init(txt,order);
		glPushMatrix();
			glTranslatef(p[0],p[1],p[2]);
			txt->draw(txt);
		glPopMatrix();
		txt_free(txt);
		}

		if(C->event->ui.draw_in_loop)
		{
			glDisable(GL_LINE_STIPPLE);
		}
	}
}

void cls_block_draw_block(t_block *block)
{
	t_context *C = ctx_get();

	C->event->ui.draw_in_loop = 0;
	C->event->ui.use_point_global_width = 0;

	block_update_data(block);
	block_update_geometry(block);
	block_draw_bricks(block);
	block_draw_outline(block);

	C->event->ui.use_point_global_width = 1;
	C->event->ui.draw_in_loop = 0;
}
	
