/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "opengl.h"
#include "base.h"
#include "ui.h"
#include "ctx.h"
#include "event.h"
#include "sketch.h"
#include "block.h"
#include "brick.h"
#include "list.h"
#include "draw.h"
#include "brick.h"

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
		if(brick->brick_state.draw_value)
		{
			if(iseq(brick->id.name,"camera_main")) printf("block:brick\n");
			brick_build_txt(brick);
		}

		// build width (once)
		if(block->block_state.update_geometry)
		{
			brick_build_width(brick);
		}
	}
}


// update geometry

void block_update_geometry(t_block *block)
{
	if(block->block_state.update_geometry)
	{
		t_link *link;

		float block_width=0;
		float width=0;

		// switch state
		block->block_state.update_geometry=0;

		// reset block height
		block->height=0;

		// loop over bricks
		for(link=block->bricks->first;link;link=link->next)
		{
			t_brick *brick = link->data;

			// Height
			if(brick->brick_state.draw)
				block->height += brick->geom.height;

			// set draw plugs
			if(brick->brick_state.draw_plugs)
			{
				block->block_state.draw_plugs=1;
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

t_brick *block_get_first_brick( t_block *block)
{
	t_link *l  = block->bricks->first;
	t_brick *brick = l->data;
	return brick;
}

void block_draw_clone(t_block *block)
{
	t_context *C = ctx_get();
	t_block *clone = block->clone;
	if( block->block_state.draw_clone_link)
	{
	if( clone)
	{
		if( C->draw->mode == mode_draw)
		{
			t_brick *brick = block_get_first_brick( block);
			t_brick *clone_brick = block_get_first_brick( clone);
			float p[3];
			brick_get_geo_in(  p, clone, clone_brick);
			brick_draw_clone_link( block, brick, p);
		}
	}
	}
}


/*
void brick_draw_link(t_brick *brick)
{
	t_context *C = ctx_get();
	if( C->draw->mode == mode_draw)
	{
		t_block *block = brick->block;
		if( block->block_state.connecting && brick->brick_state.connecting)
		{
			t_context *C = ctx_get();
			float p[3]={0,0,0};

			ctx_get_mouse_pos( C, p);
			brick_draw_connection_line( block, brick, p, 0);

		}
		else if(brick->brick_state.draw_plugs)
		{
			t_plug *plug_out = &brick->plug_out;

			if(plug_out->dst) 
			{
				t_plug *plug_target=plug_out->dst;
				t_brick *brick_target=plug_target->brick;

				t_block *block_target=brick_target->block;

				float p[3];
				brick_get_geo_in(  p, block_target, brick_target);
				brick_draw_connection_line( block, brick, p, 1);
			}
		}
	}
}
*/









void block_draw_bricks(t_block *block)
{
	t_link *link;
	t_brick *brick;

	glPushMatrix();

	// Translate to Block Pos
	glTranslatef(block->pos[0],block->pos[1],0);

	// Loop over Bricks
	for(link=block->bricks->first;link;link=link->next)
	{
		brick = link->data;

		// Draw brick
		brick->cls->draw(brick);

		// Translate Up or Down
		if(brick->brick_state.draw) glTranslatef(0,brick->geom.height * block->up ,0);
	}

	glPopMatrix();

}

void block_draw_bar(t_block *block)
{
	t_link *link;
	t_brick *brick;
	float offset = 5;

	glPushMatrix();
	glTranslatef(block->pos[0],block->pos[1],0);
	for(link=block->bricks->first;link;link=link->next)
	{
		brick = link->data;
		brick->cls->draw(brick);
		if(brick->brick_state.draw) glTranslatef(brick->geom.width + offset,0,0);
	}
	glPopMatrix();
}

void block_menu_draw_bricks(t_block *block)
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
			if(brick->brick_state.show_menu)
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
		if(brick->brick_state.draw) glTranslatef(0,brick->geom.height,0);
	}

	glPopMatrix();

}

void block_draw_outline(t_block *block)
{
	t_context *C=ctx_get();

	if(block->block_state.draw_outline)
	{
		int tot=4;
		float *color=C->ui->front_color;
		float points[tot*3];
		float *pos=block->pos;
		int line_width;

		line_width = 1;

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

		if(block->block_state.is_mouse_over)
			line_width = 2;

		// Rectangle
		skt_closedline(points,tot,color,line_width);

		if(C->ui->show_step)
		{
			char order[3];
			float p[3] = {0,0,0};
			float vv[3] = {-10,-10,0};
			vadd(p,a,vv);
			sprintf(order,"%d",block->rhizome_order);

			t_txt *txt = txt_new(order);
			glPushMatrix();
				glTranslatef(p[0],p[1],p[2]);
				txt->draw(txt);
			glPopMatrix();
			txt_free(txt);
		}

		// Rhizome Position
		if(C->ui->show_rhizome_order)
		{
			if(block->rhizome)
			{
				char order[3];
				float p[3] = {0,0,0};
				float vv[3] = {-10,-10,0};
				vadd(p,a,vv);
				sprintf(order,"%d",block->rhizome_pos);

				glPushMatrix();
					glTranslatef(p[0],p[1],p[2]);
					if(block->block_state.is_root)
					{
						t_txt *root = txt_new("root");
						root->draw(root);
						txt_free(root);
					}
					else
					{
						t_txt *txt = txt_new(order);
						txt->draw(txt);
						txt_free(txt);
					}
				glPopMatrix();
			}
		}

	}
}

void cls_block_draw_menu(t_block *block)
{
	block_update_data(block);
	block_update_geometry(block);
	block_menu_draw_bricks(block);
	block_draw_outline(block);
}

void cls_block_draw_block(t_block *block)
{
	block_update_data(block);
	block_update_geometry(block);
	block_draw_bricks(block);
	block_draw_outline(block);
	block_draw_clone(block);
}

void cls_block_draw_bar(t_block *block)
{
	block_update_data(block);
	block_update_geometry(block);
	block_draw_bar(block);
	block_draw_outline(block);
}
	
