#
# SRC
#

FLAG= -g -Wall -D$(DEBUG)
INC=-I. -Iutil -Ilib 

MINUIT=app.o scene.o context.o draw.o ui.o node.o git.c git.o

.PHONY:util lib mod
all:util lib mod libminuit.a

#LIBS

util:;cd util && $(MAKE)
lib:;cd lib && $(MAKE)

#MINUIT

minuit.o : minuit.c 
	gcc -c minuit.c $(FLAG) $(INC) $(GFLAG)   

app.o : app.h app.c
	gcc -c app.c $(FLAG) $(INC)   

scene.o: scene.h scene.c
	gcc -c scene.c $(FLAG) $(INC)   

context.o:context.h context.c
	gcc -c context.c $(FLAG) $(INC)      

draw.o : draw.h draw.c
	gcc -c draw.c $(FLAG) $(INC)   

ui.o : ui.h ui.c 
	gcc -c ui.c $(FLAG) $(INC)   

node.o : node.h node.c 
	gcc -c node.c $(FLAG) $(INC)   

git.c:../.git/HEAD ../.git/index
	@echo "const char *git = \"$(shell git rev-parse HEAD)\";" > $@
git.o:git.c
	gcc -c git.c


# LIBMINUIT

libminuit.a:$(MINUIT)
	rm -fr ./libminuit.a
	ar -cr libminuit.a $(MINUIT)  

.PHONY : clean
clean:
	cd util && make clean
	cd lib && make clean
	rm -f ./*.o
	rm -f ./*.a
	@ if [ -e git.c ]; then rm git.c;fi


