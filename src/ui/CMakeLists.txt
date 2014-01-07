# UI
set(SRC
	ui.c
	ui.h

	screen_init.c
	screen_intro.c
	screen_browser.c
)

add_library(minuit-ui STATIC ${SRC})
