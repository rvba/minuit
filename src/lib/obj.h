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

struct Node;
struct Brick;

int obj_export(struct Node *node);
void op_export(struct Node *node);
void *op_obj_import(struct Brick *brick);


#endif
