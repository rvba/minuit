# OPS

set(SRC
	op_action.c
	op_add_brick.c
	op_add_object.c
	op.c

	op.h
	op_texture.c

	op_camera.c
	op_debug.c

)

add_library(minuit-ops STATIC ${SRC})

