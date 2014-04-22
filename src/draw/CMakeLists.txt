# DRAW
set(SRC
	draw.c
	draw.h
	draw_brick.c
	draw_block.c
	sketch.c
	sketch.h
)

add_library(minuit-draw STATIC ${SRC})
