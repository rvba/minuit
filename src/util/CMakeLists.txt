# UTIL
set(SRC
	network.h
	system.h
	u_calc.c
	u_client.c
	u_off_screen.c
	u_rand.c
	u_server.c
	u_socket.c
	u_string.c
	u_system.c
	util.c
	util.h
	u_vector.c
	u_volume.c
	)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_library(minuit-util STATIC ${SRC})
