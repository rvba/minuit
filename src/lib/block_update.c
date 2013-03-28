/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"

void cls_block_update(t_block *block);

void cls_block_generic_update(t_block *self)
{
}

void cls_block_block_update(t_block *block)
{
	t_context *C = ctx_get();
	t_brick *brick;
	t_link *link;

	int is_mouse_over=0;

	// loop over all bricks
	for(link=block->bricks->first;link;link=link->next)
	{
		brick=link->data;
		//XXX tmp for cloning
		if(!C->event->is_brick_transformed)
		{
			// MOUSE OVER
			if(is_mouse_over_brick(C,brick))
			{
				brick->state.is_mouse_over=1;
				is_mouse_over=1;
			}
			else
			{
				brick->state.is_mouse_over=0;
			}
		}

		// UPDATE BUTTON 
		brick->cls->update(brick);
	}

	if(is_mouse_over) block->state.is_mouse_over=1;
	else block->state.is_mouse_over=0;
}

void cls_block_menu_update(t_block *block)
{
	t_context *C = ctx_get();

	if(C->ui->show_menu)  block->state.is_show=1; 
	if(block->state.is_show) cls_block_update(block);
}

void block_store(t_block *block,t_brick *brick)
{
	brick->state.show_menu=1;
	block->selected=brick;
	block->submenu=brick; //???
}

void block_unstore(t_block *block)
{
	t_link *link;
	t_brick *brick;

	for(link=block->bricks->first;link;link=link->next)
	{
		brick=link->data;

		if(brick->cls->type==bt_menu)
		{
			// HIDE
			brick->state.show_menu=0;

			// recusrive
			if(brick->menu)
			{
				block_unstore(brick->menu);
			}
		}
	}

	block->submenu=NULL;
}

void cls_block_ref_update(t_block *block)
{
	t_context *C = ctx_get();
	t_brick *brick;
	t_link *link;

	int is_mouse_over=0;

	// loop over all bricks
	for(link=block->bricks->first;link;link=link->next)
	{
		brick=link->data;
		// MOUSE OVER
		if(is_mouse_over_brick(C,brick))
		{
			brick->state.is_mouse_over=1;
			is_mouse_over=1;
		}
		else
		{
			 brick->state.is_mouse_over=0;
		}

		// UPDATE BUTTON 
		brick->cls->update(brick);
	}

	if(is_mouse_over) block->state.is_mouse_over=1;
	else block->state.is_mouse_over=0;
}

void cls_block_update(t_block *block)
{
	t_context *C = ctx_get();
	t_brick *brick;
	t_link *link;

	int is_mouse_over=0;

	// hide previous menu
	if(block->state.is_show && !C->ui->show_menu)
	{
		block_unstore(block);
		block->selected=NULL;
		block->state.is_show=0;
		C->event->is_brick_transformed=0;
	}
	// OR update
	else
	{
		// loop over all bricks
		for(link=block->bricks->first;link;link=link->next)
		{
			brick=link->data;
			if(is_mouse_over_brick(C,brick))
			{
				 brick->state.is_mouse_over=1;
				is_mouse_over=1;
			}
			else 
			{
				brick->state.is_mouse_over=0;
			}

			// UPDATE BRICK 
			brick->cls->update(brick);

			// if BRICK is a MENU 
			if(brick->cls->type == bt_menu)
			{
				// if mouse over
				if(brick->state.is_mouse_over)
				{
					// check STORED menu

					t_brick *brick_menu=block->selected;

					if(brick_menu)
					{
						t_block *block_menu=brick->menu;

						// if a submenu is stored
						if(block->submenu)
						{
							t_brick *brick_submenu=block->submenu;

							if(!is(brick_submenu->name,brick->name))
							{
								block_unstore(block);
								block_store(block,brick);
							}
						}
						else
						{
							block_store(block,brick);
						}

						// update block_menu

						if(block_menu)
						{
							cls_block_update(block_menu);
						}
							
					}
					// no previous selected_menu
					else
					{
						block_store(block,brick);
					}
				}

				// mouse not over
				else
				{
					// UPDTAE MENU
					t_block *m=brick->menu;
					//XXX
					if(m) cls_block_update(m);
				}
			}
		}

		if(is_mouse_over) block->state.is_mouse_over=1;
		else block->state.is_mouse_over=0;
	}
}






