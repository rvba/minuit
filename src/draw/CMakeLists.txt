# DRAW
set(SRC
	draw.c
	draw.h
	draw_brick.c
	draw_block.c
	draw_light.c
	draw_object.c
	draw_mesh.c
	draw_mesh_va.c
	draw_mesh_im.c
	draw_point_va.c
	sketch.c
	sketch.h
)

add_library(minuit-draw ${LIB_TYPE} ${SRC})
