/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "ctx.h"
#include "node.h"
#include "brick.h"
#include "camera.h"

#include "scene.h"
#include "event.h"
#include "base.h"




void cls_brick_connect(t_brick *brick_in ,t_brick *brick_out)
{
	t_plug *plug_brick_in = &brick_in->plug_intern;
	t_plug *plug_brick_out = &brick_out->plug_intern;

	if( brick_in->plug_in.state.can_connect)
	{

	// first Out
	plug_brick_out->cls->connect(mode_out, plug_brick_out, plug_brick_in);
	// Then In
	plug_brick_in->cls->connect(mode_in, plug_brick_in, plug_brick_out);
	}
}


void cls_brick_disconnect(t_brick *self)
{
	t_brick *brick_in = self;
	t_plug *plug_target = brick_in->plug_in.src;
	if( plug_target)
	{
		t_brick *brick_out = plug_target->brick;
		t_plug *plug_out = &brick_out->plug_intern;
		t_plug *plug_in = &brick_in->plug_intern;

		plug_out->cls->disconnect(mode_out ,plug_out);
		plug_in->cls->disconnect(mode_in , plug_in);

		// Split Graph
		brick_rhizome_split(brick_in, brick_out);
	}
}

// DECLARATIONS

void cls_brick_init(t_brick *brick)
{
	t_context *C=ctx_get();
	brick_init(C->scene,brick);
}

void cls_brick_init_switch( t_brick *brick)
{
	cls_brick_init( brick);
	brick->state = NULL;
}

void cls_brick_init_slider( t_brick *brick)
{
	cls_brick_init( brick);
	brick->state = NULL;
}

void brick_make_trigger(t_brick *brick);
void brick_make_slider(t_brick *brick);
void brick_make_menu(t_brick *brick);
void brick_make_selector(t_brick *brick);
void brick_make_switch(t_brick *brick);
void brick_make_bar(t_brick *brick);
void brick_make_label(t_brick *brick);

// TRIGGER

t_brick_class brick_trigger = {
	.cls_type="brick",
	.type=bt_trigger,
	.make=brick_make_trigger,
	.draw=brick_draw,
	.init=cls_brick_init,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	};

// SLIDER

t_brick_class brick_slider = {
	.cls_type="brick",
	.type=bt_slider,
	.make=brick_make_slider,
	.draw=brick_draw,
	.init=cls_brick_init_slider,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	};

// MENU

t_brick_class brick_menu = {
	.cls_type="brick",
	.type=bt_menu,
	.make=brick_make_menu,
	.draw=brick_draw,
	.init=cls_brick_init,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	};

// SELECTOR

t_brick_class brick_selector = {
	.cls_type="brick",
	.type=bt_selector,
	.make=brick_make_selector,
	.draw=brick_draw,
	.init=cls_brick_init,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	};

// SWITCH

t_brick_class brick_switch = {
	.cls_type="brick",
	.type=bt_switch,
	.make=brick_make_switch,
	.draw=brick_draw,
	.init=cls_brick_init_switch,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	};

// BAR

t_brick_class brick_bar = {
	.cls_type="brick",
	.type=bt_bar,
	.make=brick_make_bar,
	.draw=brick_draw_bar,
	.init=cls_brick_init_switch,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	};

// LABEL

t_brick_class brick_label = {
	.cls_type="brick",
	.type=bt_label,
	.make=brick_make_label,
	.draw=brick_draw,
	.init=cls_brick_init,
	.connect=cls_brick_connect,
	.disconnect=cls_brick_disconnect,
	};


// CLS

t_brick_class *bricks[] = {
	&brick_trigger,
	&brick_slider,
	&brick_menu,
	&brick_selector,
	&brick_switch,
	&brick_bar,
	&brick_label,
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

void brick_make_bar(t_brick *brick)
{
	brick->cls=&brick_bar;
}

void brick_make_label(t_brick *brick)
{
	brick->cls=&brick_label;
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
