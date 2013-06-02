cmake_minimum_required(VERSION 2.6) 

project(Minuit)
 
file(
    GLOB_RECURSE
    source_files
    src/*
    src/lib/*
    src/util/*
)

add_executable(
    minuit
    ${source_files}
)

include_directories(src src/lib src/util)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${PNG_INCLUDE_DIR} ${JPEG_INCLUDE_DIRS})

target_link_libraries(minuit ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARY} ${JPEG_LIBRARY})

LINK_DIRECTORIES(${OPENGL_LIBRARIES})

set(CMAKE_C_FLAGS "-DWITH_DEBUG -g -Wall")
 
