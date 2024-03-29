# Makefile for MacOS.
#
# This file assumes that SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer are
# installed in the /Library/Frameworks directory.
#
# The preprocessor definition -D_main=_SDL_main is needed to give the code
# access to SDL's SDL_main function.

# MacOS
CC=clang
LIB_OUTPUT=libcrumbs.dylib
MAIN_DEF=-D_main=_SDL_main

INCLUDE_DIR=-I/Library/Frameworks/SDL2.framework/Headers -I../..
LINK_DIR=-F/Library/Frameworks -L.

CFLAGS=-Wall -Werror
LIBS=-framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer

all: crumbs-release demo-release tootnsploot-release

#=============================================================================
# Crumbs

CRUMBS_SRC=../../impl/*.c

crumbs-debug:
	$(CC) $(CFLAGS) -g -fPIC -shared $(CRUMBS_SRC) -DCRUMBS_EXPORTS -o $(LIB_OUTPUT) $(INCLUDE_DIR) $(LINK_DIR) $(LIBS)

crumbs-release:
	$(CC) $(CFLAGS) -fPIC -shared $(CRUMBS_SRC) -DCRUMBS_EXPORTS -o $(LIB_OUTPUT) $(INCLUDE_DIR) $(LINK_DIR) $(LIBS)

#=============================================================================
# Demo

D=../../demo
DEMO_SRC=$(D)/*.c ../../common/*.c $(D)/entities/*.c $(D)/input/*.c $(D)/scenes/*.c $(D)/util/*.c

demo-debug:
	$(CC) -g $(MAIN_DEF) $(DEMO_SRC) -o demo.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs

demo-release:
	$(CC) $(MAIN_DEF) $(DEMO_SRC) -o demo.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs


#=============================================================================
# Toot n Sploot

T=../../tootnsploot
TNS_SRC=$(T)/*.c ../../common/*.c $(T)/entities/*.c $(T)/input/*.c $(T)/scenes/*.c $(T)/util/*.c

tootnsploot-debug:
	$(CC) -g $(MAIN_DEF) $(TNS_SRC) -o tootnsploot.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs

tootnsploot-release:
	$(CC) $(MAIN_DEF) $(TNS_SRC) -o tootnsploot.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs
