/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "context.h"
#include "node.h"
#include "brick.h"
#include "camera.h"

// DECLARATIONS

void cls_brick_init(t_brick *brick)
{
	t_context *C=ctx_get();
	brick_init(C->scene,brick);
}

void cls_brick_init_switch( t_brick *brick)
{
	cls_brick_init( brick);
	brick->state = state_brick_switch_default;
}

void cls_brick_init_slider( t_brick *brick)
{
	cls_brick_init( brick);
	brick->state = state_brick_slider_default;
}

void brick_make_trigger(t_brick *brick);
void brick_make_slider(t_brick *brick);
void brick_make_menu(t_brick *brick);
void brick_make_selector(t_brick *brick);
void brick_make_switch(t_brick *brick);

// TRIGGER

t_brick_class brick_trigger = {
	.cls_type="brick",
	.type=bt_trigger,
	.make=brick_make_trigger,
	.draw=brick_draw,
	.update=cls_brick_update,
	.trigger=cls_brick_trigger_trigger,
	.init=cls_brick_init,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	.dispatch=cls_brick_dispatch,
	};

// SLIDER

t_brick_class brick_slider = {
	.cls_type="brick",
	.type=bt_slider,
	.make=brick_make_slider,
	.draw=brick_draw,
	.update=cls_brick_update,
	.trigger=cls_brick_trigger_slider,
	.init=cls_brick_init_slider,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	.dispatch=cls_brick_dispatch,
	};

// MENU

t_brick_class brick_menu = {
	.cls_type="brick",
	.type=bt_menu,
	.make=brick_make_menu,
	.draw=brick_draw,
	.update=cls_brick_update,
	.trigger=cls_brick_trigger_menu,
	.init=cls_brick_init,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	.dispatch=cls_brick_dispatch,
	};

// SELECTOR

t_brick_class brick_selector = {
	.cls_type="brick",
	.type=bt_selector,
	.make=brick_make_selector,
	.draw=brick_draw,
	.update=cls_brick_update,
	.trigger=cls_brick_trigger_selector,
	.init=cls_brick_init,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	.dispatch=cls_brick_dispatch,
	};

// SWITCH

t_brick_class brick_switch = {
	.cls_type="brick",
	.type=bt_switch,
	.make=brick_make_switch,
	.draw=brick_draw,
	.update=cls_brick_update,
	.trigger=cls_brick_trigger_switch,
	.init=cls_brick_init_switch,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	.dispatch=cls_brick_dispatch,
	};


// CLS

t_brick_class *bricks[] = {
	&brick_trigger,
	&brick_slider,
	&brick_menu,
	&brick_selector,
	&brick_switch,
	};

void brick_make_trigger(t_brick *brick)
{
	brick->cls=&brick_trigger;
}

void brick_make_slider(t_brick *brick)
{
	brick->cls=&brick_slider;
}

void brick_make_menu(t_brick *brick)
{
	brick->cls=&brick_menu;
}

void brick_make_selector(t_brick *brick)
{
	brick->cls=&brick_selector;
}

void brick_make_switch(t_brick *brick)
{
	brick->cls=&brick_switch;
}

// INIT

void brick_cls_init(t_brick *brick)
{
	int i;
	int n = sizeof(bricks)/sizeof(bricks[0]);
	int found=0;
	for(i=0;i<n;i++)
	{
		if(brick->type==bricks[i]->type)
		{
			bricks[i]->make(brick);
			found=1;
			break;
		}
	}

	if(!found)printf("[ERROR:brick_cls_init] Unknown brick type %d\n",brick->type);
}
