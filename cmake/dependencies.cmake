MACRO(FIND_GLEW)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_GLEW")
	find_library( GLEW_LIBRARY GLEW PATHS "/usr/lib" "/usr/lib/x86_64-linux-gnu/")
endMACRO()
