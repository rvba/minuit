set(
	SRC
	action.c  
	action.h  
	binding.c  
	binding.h  
	data.c  
	data.h  
	datum.c
	datum.h
	event.c  
	event.h  
	file.c  
	file_data.c  
	file.h  
	memory.c  
	memory.h  
	mode.c  
	mode.h  
	option.h
	)

add_library( minuit-lib_data STATIC ${SRC})

