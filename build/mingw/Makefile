# A Makefile for MinGW-w64.
#
# Used by running mingw32-make.exe
# This assumes all the dependencies are installed in an MSYS2 environment.
#
# The preprocessor -Dmain=SDL_main is needed to give the code access to
# SDL's SDL_main function.

CC=gcc
LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

all: crumbs-release demo-release tootnsploot-release

debug: crumbs-debug demo-debug tootnsploot-debug


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
	$(CC) -g -Dmain=SDL_main $(DEMO_SRC) -o demo -I../.. -L. $(LIBS) -lcrumbs

demo-release:
	$(CC) -Dmain=SDL_main $(DEMO_SRC) -o demo -I../.. -L. -mwindows $(LIBS) -lcrumbs


#=============================================================================
# Toot n Sploot

T=../../tootnsploot
TNS_SRC=$(T)/*.c ../../common/*.c $(T)/entities/*.c $(T)/input/*.c $(T)/scenes/*.c $(T)/util/*.c

tootnsploot-debug:
	$(CC) -g -Dmain=SDL_main $(TNS_SRC) -o tootnsploot -I../.. -L. $(LIBS) -lcrumbs

tootnsploot-release:
	$(CC) -Dmain=SDL_main $(TNS_SRC) -o tootnsploot -I../.. -L. -mwindows $(LIBS) -lcrumbs
