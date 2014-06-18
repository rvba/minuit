set(
	SRC
	camera.c  
	geometry.c  
	image.c  
	image_jpg.c  
	light.c  
	material.c  
	mesh_buffer.c  
	mesh.h            
	object.h  
	texture.c
	camera.h  
	geometry.h  
	image.h  
	image_png.c  
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

add_library( minuit-lib_object STATIC ${SRC})

