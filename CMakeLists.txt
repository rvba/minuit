# MINUIT
cmake_minimum_required(VERSION 2.6) 

message("[M]INUIT\n[I]s\n[N]ot a\n[U]ser\n[I]nterface\n[T]oolkit")
message("SOURCE_DIR\t" ${CMAKE_SOURCE_DIR})
message("BINARY_DIR\t" ${CMAKE_BINARY_DIR})

project(Minuit)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(Threads)

include_directories(${CMAKE_SOURCE_DIR}/src)

include_directories(
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/src/lib
	${CMAKE_SOURCE_DIR}/src/util
	)

add_subdirectory(src)

set(CMAKE_C_FLAGS "-DWITH_DEBUG -g -Wall")

add_executable(
    minuit
    minuit.c
)

target_link_libraries(minuit minuit-core  minuit-lib minuit-util)
target_link_libraries(minuit ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARY} ${JPEG_LIBRARY} pthread)






 
