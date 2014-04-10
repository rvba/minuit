# OPS

set(SRC
	op.c
	op.h
	op_action.c
	op_add_brick.c
	op_add_object.c
	op_texture.c
	op_camera.c
	op_register.c
	op_ui.c
)

add_library(minuit-ops STATIC ${SRC})

