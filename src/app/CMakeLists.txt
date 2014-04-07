# APP

set(SRC 
	app.c
	app.h
	app_keyboard.c
	app_mouse.c
	app_window.c
)

if(OPT_SDL)

set(SRC
	${SRC}
	app_sdl.c
)

endif()

add_library(minuit-app STATIC ${SRC})


