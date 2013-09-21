# SRC
add_subdirectory(lib)
add_subdirectory(util)

set(SRC 
	app.c
	app.h
	common.h
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

include_directories(
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/lib
	${CMAKE_SOURCE_DIR}/src/util
	)
target_link_libraries(minuit-core ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARY} ${JPEG_LIBRARY} pthread minuit-lib minuit-util)

