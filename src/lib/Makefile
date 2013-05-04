#
# LIB
#

all:liblib.a

obj:=\
	app_keyboard.o\
	app_mouse.o\
	app_window.o\
	app_x.o\
	block.o\
	block_draw.o\
	block_update.o\
	brick.o\
	brick_draw.o\
	brick_act.o\
	brick_update.o\
	brick_cls.o\
	camera.o\
	ctx_camera.o\
	ctx_keyboard.o\
	ctx_links.o\
	ctx_load.o\
	ctx_option.o\
	ctx_process.o\
	ctx_mouse.o\
	ctx_render.o\
	ctx_scene.o\
	ctx_ui.o\
	ctx_video.o\
	data.o\
	dict.o\
	engine.o\
	event.o\
	light.o\
	list.o\
	material.o\
	memory.o\
	mesh.o\
	mesh_buffer.o\
	mesh_transform.o\
	mode.o\
	obj.o \
	object.o\
	object_draw.o\
	op.o\
	op_action.o\
	op_add_brick.o\
	op_add_object.o\
	op_texture.o\
	op_debug.o\
	op_camera.o\
	plug_cls.o\
	screen.o\
	screen_browser.o\
	screen_init.o\
	server.o\
	sketch.o\
	texture.o\
	txt.o\
	term.o\
	vector.o\
	viewport.o\
	vlst.o

ifeq ($(DEBUG),1)
FLAG=-g -Wall -DWITH_DEBUG
else
FLAG= 
endif
INC=-I./ -I../ -I../util  

liblib.a:$(obj)
	rm -f *.a
	ar -cr liblib.a $(obj)


app_mouse.o:../app.h app_mouse.c
	gcc -c app_mouse.c $(FLAG) $(INC)
app_keyboard.o:../app.h app_keyboard.c
	gcc -c app_keyboard.c $(FLAG) $(INC)
app_window.o:../app.h app_window.c
	gcc -c app_window.c $(FLAG) $(INC)
app_x.o:../app.h app_x.c
	gcc -c app_x.c $(FLAG) $(INC)
block.o:block.h block.c
	gcc -c block.c $(FLAG) $(INC)
block_draw.o:block.h block_draw.c 
	gcc -c block_draw.c $(FLAG) $(INC)
block_update.o:block.h block_update.c
	gcc -c block_update.c $(FLAG) $(INC)
brick.o:block.h brick.c
	gcc -c brick.c $(FLAG) $(INC)
brick_act.o:op.h brick_act.c
	gcc -c brick_act.c $(FLAG) $(INC) $(GFLAG)
brick_cls.o:block.h brick_cls.c
	gcc -c brick_cls.c $(FLAG) $(INC)
brick_draw.o:block.h brick_draw.c 
	gcc -c brick_draw.c $(FLAG) $(INC)
brick_update.o:block.h brick_update.c
	gcc -c brick_update.c $(FLAG) $(INC)
camera.o:camera.h
	gcc -c camera.c $(FLAG) $(INC)
ctx_camera.o:../context.h ctx_camera.c 
	gcc -c ctx_camera.c $(FLAG) $(INC)
ctx_links.o:../context.h ctx_links.c
	gcc -c ctx_links.c $(FLAG) $(INC)
ctx_load.o:../context.h ctx_load.c
	gcc -c ctx_load.c $(FLAG) $(INC)
ctx_mouse.o:../context.h ctx_mouse.c
	gcc -c ctx_mouse.c $(FLAG) $(INC)
ctx_option.o:../context.h ctx_option.c
	gcc -c ctx_option.c $(FLAG) $(INC)
ctx_process.o:../context.h ctx_process.c
	gcc -c ctx_process.c $(FLAG) $(INC)
ctx_keyboard.o:../context.h ctx_keyboard.c
	gcc -c ctx_keyboard.c $(FLAG) $(INC)
ctx_ui.o:../context.h ctx_ui.c
	gcc -c ctx_ui.c $(FLAG) $(INC)
ctx_scene.o:../context.h ctx_scene.c
	gcc -c ctx_scene.c $(FLAG) $(INC)
ctx_render.o:../context.h ctx_render.c
	gcc -c ctx_render.c $(FLAG) $(INC)
ctx_video.o:../context.h ctx_video.c
	gcc -c ctx_video.c $(FLAG) $(INC)
data.o:data.h data.c
	gcc -c data.c $(FLAG) $(INC)
dict.o:dict.h dict.c
	gcc -c dict.c $(FLAG) $(INC)
engine.o:engine.h 
	gcc -c engine.c $(FLAG) $(INC)
event.o:event.h
	gcc -c event.c $(FLAG) $(INC)
light.o:light.h light.c
	gcc -c light.c $(FLAG) $(INC)
list.o:list.h list.c
	gcc -c list.c $(FLAG) $(INC)
material.o:material.h material.c
	gcc -c material.c $(FLAG) $(INC)
memory.o:memory.h memory.c
	gcc -c memory.c $(FLAG) $(INC)
mode.o:mode.h mode.c
	gcc -c mode.c $(FLAG) $(INC)
mesh.o:mesh.h mesh.c
	gcc -c mesh.c $(FLAG) $(INC)
mesh_transform.o:mesh.h mesh_transform.c
	gcc -c mesh_transform.c $(FLAG) $(INC)
mesh_buffer.o:mesh.h mesh_buffer.c
	gcc -c mesh_buffer.c $(FLAG) $(INC)
obj.o:obj.h obj.c
	gcc -c obj.c $(FLAG) $(INC)
object.o: object.h object.c
	gcc -c object.c $(FLAG) $(INC)
object_draw.o:op.h object_draw.c 
	gcc -c object_draw.c $(FLAG) $(INC) $(GFLAG)
op.o:op.h op.c
	gcc -c op.c $(FLAG) $(INC) $(GFLAG)
op_action.o:op.h op_action.c
	gcc -c op_action.c $(FLAG) $(INC) $(GFLAG)
op_add_brick.o:op.h op_add_brick.c
	gcc -c op_add_brick.c $(FLAG) $(INC) $(GFLAG)
op_add_object.o:op.h op_add_object.c
	gcc -c op_add_object.c $(FLAG) $(INC) $(GFLAG)
op_camera.o:op.h op_camera.c
	gcc -c op_camera.c $(FLAG) $(INC) $(GFLAG)
op_debug.o:op.h op_debug.c 
	gcc -c op_debug.c $(FLAG) $(INC) $(GFLAG)
op_texture.o:op.h op_texture.c
	gcc -c op_texture.c $(FLAG) $(INC)
plug_cls.o:block.h plug_cls.c
	gcc -c plug_cls.c $(FLAG) $(INC)
server.o:server.h server.c
	gcc -c server.c $(FLAG) $(INC)
screen.o:screen.h screen.c 
	gcc -c screen.c $(FLAG) $(INC)
screen_browser.o:op.h screen_browser.c 
	gcc -c screen_browser.c $(FLAG) $(INC)
screen_init.o:op.h screen_init.c 
	gcc -c screen_init.c $(FLAG) $(INC)
sketch.o: sketch.h sketch.c
	gcc -c sketch.c $(FLAG) $(INC)   
texture.o:texture.h texture.c
	gcc -c texture.c $(FLAG) $(INC)
txt.o:txt.h txt.c
	gcc -c txt.c $(FLAG) $(INC)   
term.o:term.h term.c
	gcc -c term.c $(FLAG) $(INC)
vector.o:vector.h vector.c
	gcc -c vector.c $(FLAG) $(INC)
vlst.o:list.h vlst.c
	gcc -c vlst.c $(FLAG) $(INC)
viewport.o:viewport.h viewport.c
	gcc -c viewport.c $(FLAG) $(INC)

.PHONY:clean
clean:
	rm -f ./*.o
	rm -f ./*.a

