# CTX
set(SRC
	ctx.h
	ctx.c
	ctx_browser.c
	ctx_block.c
	ctx_block_menu.c
	ctx_brick.c
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

add_library(minuit-ctx ${LIB_TYPE} ${SRC})
