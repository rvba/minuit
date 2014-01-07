# MINUIT
cmake_minimum_required(VERSION 2.6) 

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
		message(FATAL_ERROR "Don't build in SOURCE directory !")
endif()

project(Minuit)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(Threads)

include_directories(
	${CMAKE_SOURCE_DIR}/src/ops
	${CMAKE_SOURCE_DIR}/src/draw
	${CMAKE_SOURCE_DIR}/src/ui
	${CMAKE_SOURCE_DIR}/src/scene
	${CMAKE_SOURCE_DIR}/src/app
	${CMAKE_SOURCE_DIR}/src/ctx
	${CMAKE_SOURCE_DIR}/src/base
	${CMAKE_SOURCE_DIR}/src/lib
	${CMAKE_SOURCE_DIR}/src
	)

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWITH_DEBUG -Wall ")

add_subdirectory(src)

add_executable(
    minuit
    src/minuit.c
)

target_link_libraries(minuit minuit-ctx minuit-ops minuit-ui minuit-draw minuit-scene minuit-lib minuit-app minuit-base)
target_link_libraries(minuit ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARY} ${JPEG_LIBRARY} pthread m)






 
