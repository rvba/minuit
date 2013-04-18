#
# ROOT 
#

LIBS:= -lminuit -llib -lutil -lpng12 -ljpeg -lz -lGLU -lGL -lglut -lpthread  
INC:= -Isrc -Isrc/util -Isrc/lib 
DIR:= -Lsrc -Lsrc/util -Lsrc/lib 

WITH_LIBLO=1
DEBUG=0

export WITH_LIBLO
export DEBUG

# TARGETS
all:app tags

# MINUIT
app:
	cd src && $(MAKE)
	gcc src/minuit.c -o minuit $(INC) $(DIR) $(LIBS)


# CTAGS
tags:
	ctags -R * 

# INSTALL
DESTDIR=
BINDIR=usr/bin
.PHONY : install
install:
	install -d -m 0755 -o root -g root $(DESTDIR)/$(BINDIR)
	install    -m 0755 -o root -g root minuit  $(DESTDIR)/$(BINDIR)

# CLEAN
.PHONY : clean

clean:
	cd ./src && $(MAKE) clean
	rm -f ./minuit
	rm -f ./tags
 

