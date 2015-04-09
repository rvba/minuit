set(
	SRC
	camera.c  
	geometry.c  
	image.c  
	light.c  
	material.c  
	mesh_buffer.c  
	mesh.h            
	object.h  
	texture.c
	camera.h  
	geometry.h  
	image.h  
	light.h  
	material.h  
	mesh.c         
	mesh_transform.c  
	object.c  
	texture.h
	)

if(OPT_OBJ)
set( 	SRC
	${SRC}
	obj.c
	obj.h
	)
endif()

if(OPT_JPG)
set(	SRC
	${SRC}
	image_jpg.c  
	)
endif()

if(OPT_PNG)
set(	SRC
	${SRC}
	image_png.c  
	)
endif()

add_library( minuit-lib_object ${LIB_TYPE} ${SRC})

