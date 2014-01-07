# CTX
set(SRC
	context.c
	context.h
	ctx_camera.c
	ctx_compute.c
	ctx_event.c
	ctx_keyboard.c
	ctx_load.c
	ctx_mouse.c
	ctx_option.c
	ctx_process.c
	ctx_render.c
	ctx_rhizome.c
	ctx_save.c
	ctx_scene.c
	ctx_ui.c
	ctx_video.c
)

add_library(minuit-ctx STATIC ${SRC})
