#
# ROOT 
#

LIBS:= -lminuit -llib -lutil -lpng12 -ljpeg -lz -llo -lGLEW -lGLU -lGL -lglut -lpthread  
INC:= -Isrc -Isrc/util -Isrc/lib 
DIR:= -Lsrc -Lsrc/util -Lsrc/lib 

WITH_LIBLO=1
DEBUG=__DEBUG__
#VERSION=$(`echo git log -n 1 | head -n 1`)

export WITH_LIBLO
export DEBUG
#export VERSION


# TARGETS
#all:gitversion.c app tags
all:app tags

#gitversion.c:.git/HEAD .git/index
#	@echo "const char *gitversion = \"$(shell git rev-parse HEAD)\";" > $@

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
	#rm ./gitversion.c
	cd ./src && $(MAKE) clean
	rm -f ./minuit
	rm -f ./tags
 

