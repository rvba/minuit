# BASE
set(SRC
	opengl.h
	common.h
	cpt.h
	cpt.c
	clock.h
	clock.c
	lst.h
	lst.c
	mem.h
	mem.c
	eng.h
	eng.c
	proc.h
	proc.c
	net.h
	net.c
	ptr.h
	ptr.c
	rnd.c
	rnd.h
	srv.c
	srv.h
	shm.h
	shm.c
	sock.h
	sock.c
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

if(OPT_OSC)
set(SRC ${SRC} osc.c osc.h)
endif()

add_library(minuit-base STATIC ${SRC})
