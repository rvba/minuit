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

add_subdirectory(src)

set(CMAKE_C_FLAGS "-DWITH_DEBUG -g -Wall")

add_executable(
    minuit
    minuit.c
)

target_link_libraries(minuit minuit-util minuit-lib minuit-core)




 
