#
# UTIL
#

ifeq ($(DEBUG),1)
FLAG=-g -Wall -DWITH_DEBUG
else
FLAG=
endif

INC=-I./ -I../ -I../lib 

all:libutil.a

obj=\
	u_string.o\
	u_vector.o\
	u_calc.o\
	u_main.o\
	u_rand.o\
	u_sort.o\
	u_volume.o\
	u_off_screen.o\
	util.o\
	u_client.o\
	u_system.o\
	u_lst.o\
	u_socket.o\
	u_img.o\
	u_img_jpg.o\
	u_img_png.o

u_img.o:util.h u_img.c
	gcc -c u_img.c $(FLAG) $(INC)
u_img_jpg.o:util.h u_img_jpg.c
	gcc -c u_img_jpg.c $(FLAG) $(INC)
u_img_png.o:util.h u_img_png.c
	gcc -c u_img_png.c $(FLAG) $(INC)
u_socket.o:util.h u_socket.c
	gcc -c u_socket.c $(FLAG) $(INC)
u_lst.o:util.h u_lst.c
	gcc -c u_lst.c $(FLAG) $(INC)
u_system.o:util.h u_system.c
	gcc -c u_system.c $(FLAG) $(INC)
u_client.o:util.h u_client.c
	gcc -c u_client.c $(FLAG) $(INC)
u_main.o: u_main.c
	gcc -c u_main.c $(FLAG) $(INC) 
u_vector.o : util.h u_vector.c
	gcc -c u_vector.c $(FLAG) $(INC) 
u_calc.o : util.h u_calc.c
	gcc -c u_calc.c $(FLAG) $(INC) 
u_string.o: util.h u_string.c
	gcc -c u_string.c $(FLAG) $(INC) 
u_rand.o: util.h u_rand.c
	gcc -c u_rand.c $(FLAG) $(INC)
u_sort.o:util.h u_sort.c
	gcc -c u_sort.c $(FLAG) $(INC)
u_volume.o:util.h u_volume.c
	gcc -c u_volume.c $(FLAG) $(INC)
u_off_screen.o:util.h u_off_screen.c
	gcc -c u_off_screen.c $(FLAG) $(INC)
util.o: util.h util.c
	gcc -c util.c -o util.o $(FLAG) $(INC)

libutil.a:$(obj)
	rm -f *.a
	ar -rcs libutil.a $(obj)

.PHONY : clean
clean:
	rm -f ./*.o
	rm -f ./*.a
	rm -f ./util
