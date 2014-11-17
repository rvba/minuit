set(
	SRC
	array.c
	array.h
	dict.c  
	dict.h  
	graph.c  
	graph.h  
	list.c  
	list.h  
	vector.c  
	vector.h  
	vlst.c  
	vlst.h
	)

add_library( minuit-lib_core STATIC ${SRC})

