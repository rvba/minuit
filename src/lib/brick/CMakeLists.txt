set(
	SRC
	block.c
	block.h
	brick_act.c
	brick.c
	brick_cls.c
	brick_exe.c
	brick.h
	plug_cls.c 
	plug.h
	rhizome.c 
	rhizome.h
	set.c 
	set.h
	)

add_library(minuit-lib_brick ${LIB_TYPE} ${SRC})
