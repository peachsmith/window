# A Makefile for MinGW-w64.
# Used by running mingw32-make.exe
# This assumes all the dependencies are installed in an MSYS2 environment.

CC=gcc
LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

all: crumbs-release demo-release tootnsploot-release


#=============================================================================
# Crumbs

CRUMBS_SRC=../../impl/*.c

crumbs-debug:
	$(CC) -g -fPIC -shared $(CRUMBS_SRC) -DCRUMBS_EXPORTS -o libcrumbs.dll -I../.. $(LIBS)

crumbs-release:
	$(CC) -fPIC -shared $(CRUMBS_SRC) -DCRUMBS_EXPORTS -o libcrumbs.dll -I../.. -mwindows $(LIBS)

#=============================================================================
# Demo

D=../../demo
DEMO_SRC=$(D)/*.c ../../common/*.c $(D)/entities/*.c $(D)/input/*.c $(D)/scenes/*.c $(D)/util/*.c

demo-debug:
	$(CC) -g $(DEMO_SRC) -o demo -I../.. -L. $(LIBS) -lcrumbs

demo-release:
	$(CC) $(DEMO_SRC) -o demo -I../.. -L. -mwindows $(LIBS) -lcrumbs


#=============================================================================
# Toot n Sploot

T=../../tootnsploot
TNS_SRC=$(T)/*.c ../../common/*.c $(T)/entities/*.c $(T)/input/*.c $(T)/scenes/*.c $(T)/util/*.c

tootnsploot-debug:
	$(CC) -g $(TNS_SRC) -o tootnsploot -I../.. -L. $(LIBS) -lcrumbs

tootnsploot-release:
	$(CC) $(TNS_SRC) -o tootnsploot -I../.. -L. -mwindows $(LIBS) -lcrumbs
