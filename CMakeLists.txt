# MINUIT
cmake_minimum_required(VERSION 2.6) 

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
		message(FATAL_ERROR "Don't build in SOURCE directory !")
endif()

###############		 PROJECT #############################

project(Minuit)

###############		 OPTIONS #############################

option(OPT_OSC "osc" ON)
option(OPT_OBJ "obj" OFF)

###############		 LIBS	#############################

SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")

###############		 OSC	#############################
if(OPT_OSC)
	set(PATH_OSC_INC "/usr/include/lo")
	set(PATH_LIB "/usr/lib")

	include_directories(${PATH_OSC_INC})
	find_library(LIB_OSC lo PATHS ${PATH_LIB})
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_OSC")
endif()


message("OSC " ${OPT_OSC})
message("OBJ" ${OPT_OBJ})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)
#find_package(JPEG REQUIRED)
find_package(Threads)

set(GNU_LIB /usr/lib/x86_64-linux-gnu)


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

find_library(JPG jpeg PATHS)

set(GPLUSPLUS FALSE)
if(GPLUSPLUS)
	set(CMAKE_C_COMPILER g++)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__cplusplus ")
endif()

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWITH_DEBUG -Wall ")

if(OPT_OBJ) 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_OBJ")
endif()

add_subdirectory(src)

add_executable(
    minuit
    src/minuit.c
)

target_link_libraries(minuit minuit-ctx minuit-ops minuit-ui minuit-draw minuit-scene minuit-lib minuit-app minuit-base)
#target_link_libraries(minuit ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARY} ${JPEG_LIBRARY} pthread m)
target_link_libraries(minuit ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARY} ${JPG} pthread m)
if(OPT_OSC)
	target_link_libraries(minuit ${LIB_OSC})
endif()








 
