/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "scene.h"
#include "ctx.h"
#include "app.h"
#include "event.h"
#include "term.h"
#include "block.h"
#include "brick.h"
#include "ui.h"
#include "memory.h"
#include "list.h"
#include "set.h"

#include "graph.h"


void ctx_graph_update(struct Context *C)
{
	// Freeze
	if(C->ui->update_links)
	{
		t_link *link;
		for(link=C->scene->graphs->first;link;link=link->next)
		{
			t_node *node = link->data;
			t_graph *graph = node->data;
			graph_exec(graph);
		}
	}
}
