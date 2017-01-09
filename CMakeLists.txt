# MINUIT
cmake_minimum_required(VERSION 2.8) 

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
		message(FATAL_ERROR "Don't build in SOURCE directory !")
endif()

###############		 PROJECT #############################

project(Minuit)

###############		 OPTIONS #############################

option(OPT_OPENGL "opengl" ON)
option(OPT_OSC "osc" OFF)
option(OPT_OBJ "obj" OFF)
option(OPT_GLEW "glew" OFF)
option(OPT_SDL "sdl" OFF)
option(OPT_PNG "png" OFF)
option(OPT_JPG "jpg" OFF)
option(OPT_GLEXT "glext" ON)
option(OPT_GLUT "glut" ON)
option(OPT_SHARED "shared" OFF)
option(OPT_FREETYPE "freetype" OFF)


message("SHARED: ${OPT_SHARED}")
if(OPT_SHARED)
	set(LIB_TYPE "SHARED")

else()
	set(LIB_TYPE "STATIC")
endif()

add_definitions(-std=gnu89)

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

###############		 GLEW	#############################

include(cmake/dependencies.cmake)
if(OPT_GLEW)
	FIND_GLEW()
endif()

###############		 SDL	#############################
if(OPT_SDL)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_SDL")
	find_library( SDL_LIBRARY SDL2 PATHS "/usr/lib/i386-linux-gnu")
	include_directories("/usr/local/include/SDL2")
endif()

message("OPENGL ${OPT_OPENGL}")
message("GLUT ${OPT_GLUT}")
message("OSC " ${OPT_OSC})
message("OBJ " ${OPT_OBJ})
message("GLEW ${OPT_GLEW}")
message("SDL ${OPT_SDL}")
message("PNG ${OPT_PNG}")
message("JPG ${OPT_JPG}")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Threads)

if(OPT_GLUT)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_GLUT")
endif()

if(OPT_PNG)
       	find_package(PNG REQUIRED)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_PNG")
endif()

if(OPT_JPG)
	find_library(JPG jpeg PATHS)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_JPG")
endif()

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
	${CMAKE_SOURCE_DIR}/src/lib/brick
	${CMAKE_SOURCE_DIR}/src/lib/core
	${CMAKE_SOURCE_DIR}/src/lib/data
	${CMAKE_SOURCE_DIR}/src/lib/object
	${CMAKE_SOURCE_DIR}/src/lib/screen
	${CMAKE_SOURCE_DIR}/src/umber
	${CMAKE_SOURCE_DIR}/src
	)


set(GPLUSPLUS FALSE)
if(GPLUSPLUS)
	set(CMAKE_C_COMPILER g++)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__cplusplus ")
endif()

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWITH_DEBUG -Wall ")

if(OPT_GLEXT)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_GLEXT")
endif()

if(OPT_OBJ) 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_OBJ")
endif()

if(OPT_OSC) 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_OSC")
endif()

if(OPT_GLUT)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_GLUT")
endif()

if(OPT_FREETYPE)
	find_package(Freetype REQUIRED)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	include_directories(${FREETYPE_INCLUDE_DIR_ft2build})
	include_directories(${FREETYPE_INCLUDE_DIR_freetype2})
	add_definitions(-DHAVE_FREETYPE)
endif()

add_subdirectory(src)

add_executable(
    minuit
    src/minuit.c
)

target_link_libraries(
		minuit 
		minuit-ctx 
		minuit-ops 
		minuit-ui 
		minuit-scene 
		minuit-lib_core
		minuit-lib_object
		minuit-lib_data
		minuit-lib_screen
		minuit-lib_brick
		minuit-draw 
		minuit-app 
		minuit-base
		umber
		)

if(OPT_SDL)
	target_link_libraries(minuit ${SDL_LIBRARY})
endif()

target_link_libraries(minuit ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARY} ${JPG} pthread m)

if(OPT_OSC)
	target_link_libraries(minuit ${LIB_OSC})
endif()

if(OPT_GLEW)
	target_link_libraries(minuit ${GLEW_LIBRARY})
endif()

if(OPT_FREETYPE)
	target_link_libraries(minuit ${FREETYPE_LIBRARIES})
endif()










 
