# UI
set(SRC
	ui.c
	ui.h

	screen_init.c
	screen_intro.c
	screen_browser.c
	screen_bricks_menu.c
	screen_generic.c
	screen_main.c
	screen_sets.c
	screen_log.c
)

add_library(minuit-ui STATIC ${SRC})
