# CORE
set(SRC
	opengl.h
	common.h
	cpt.h
	cpt.c
	lst.h
	lst.c
	mem.h
	mem.c
	net.h
	net.c
	ptr.h
	ptr.c
	rnd.c
	rnd.h
	sys.h
	sys.c
	str.h
	str.c
	vec.h
	vec.c
	vol.c
	vol.h
	util.c
	util.h
	)

add_library(minuit-core STATIC ${SRC})
