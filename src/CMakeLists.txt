# SRC
add_subdirectory(lib)
add_subdirectory(util)

set(SRC 
	app.c
	app.h
	context.c
	context.h
	draw.c
	draw.h
	node.c
	node.h
	opengl.h
	scene.c
	scene.h
	ui.c
	ui.h
)

add_library(minuit-core STATIC ${SRC})


