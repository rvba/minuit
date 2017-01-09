set(
	SRC
	log.c  
	log.h  
	screen.c  
	screen.h  
	term.c  
	term.h  
	txt.c  
	txt.h  
	viewport.c  
	viewport.h
	)

if(OPT_FREETYPE)
	set( SRC ${SRC} txt_ttf.c)
endif()

add_library( minuit-lib_screen ${LIB_TYPE} ${SRC})

