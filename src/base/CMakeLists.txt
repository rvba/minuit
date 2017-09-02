# BASE
set(SRC
	opengl.h
	cpt.h
	cpt.c
	clock.h
	clock.c
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
	vec.h
	vec.c
	vol.c
	vol.h
	)

if(OPT_OSC)
set(SRC ${SRC} osc.c osc.h)
endif()

add_library(minuit-base ${LIB_TYPE} ${SRC})
