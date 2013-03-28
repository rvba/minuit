/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef _OBJ_H
#define _OBJ_H

#include "scene.h"

int obj_export(t_node *node);
void op_export(t_node *node);
void *op_obj_import(t_brick *brick);


#endif
