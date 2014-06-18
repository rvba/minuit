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

add_library( minuit-lib_screen STATIC ${SRC})

