# APP

set(SRC 
	app.c
	app.h
	app_keyboard.c
	app_mouse.c
	app_window.c
)

add_library(minuit-app STATIC ${SRC})


