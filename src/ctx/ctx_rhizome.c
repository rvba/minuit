/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "node.h"
#include "ctx.h"
#include "scene.h"
#include "app.h"
#include "event.h"
#include "term.h"
#include "block.h"
#include "brick.h"
#include "ui.h"
#include "memory.h"
#include "list.h"
#include "set.h"
#include "rhizome.h"

void ctx_set_exec(struct Context *C)
{
	// Freeze
	if(C->ui->update_links)
	{
		t_link *link;
		C->event->loop_step = 0;

		// For All Sets
		t_lst *lst = scene_lst_get( C->scene, dt_set);
		for(link = lst->first; link; link = link->next)
		{
			t_node *node = ( t_node *) link->data;
			t_set *set = ( t_set *) node->data;
			t_lst *lst = set->blocks;
			t_link *l = lst->first;

			if(l)
			{
				// For All Single Blocks
				for(;l;l=l->next)
				{
					t_block *b;
					b = ( t_block *) l->data;

					// Block Exec
					block_exec(b);
				}
			}

			l = set->rhizomes->first;

			// For All Graphs
			for(;l;l=l->next)
			{
				t_rhizome *rhizome = ( t_rhizome *) l->data;

				// Graph Exec
				rhizome_exec(rhizome);

			}
		}
	}
}

