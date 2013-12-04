# UTIL
set(SRC
	common.h
	cpt.h
	cpt.c
	util.c
	util.h
	mem.h
	mem.c
	net.h
	net.c
	rnd.c
	rnd.h
	system.h
	system.c
	str.h
	str.c
	vec.h
	vec.c
	vol.c
	vol.h
	)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_library(minuit-util STATIC ${SRC})
