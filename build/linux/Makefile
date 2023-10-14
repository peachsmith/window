# Makefile for Linux.
#
# The preprocessor definition -D_start=_SDL_main is needed to give the code
# access to SDL's SDL_main function.

CC=gcc
LIB_OUTPUT=libcrumbs.so
MAIN_DEF=-D_start=_SDL_main

# The include and lib directories must exist in the SDL2_HOME directory.
INCLUDE_DIR=-I$(SDL2_HOME)/include -I../..
LINK_DIR=-L$(SDL2_HOME)/lib -L$(SDL_IMAGE_HOME)/lib -L$(SDL_TTF_HOME)/lib -L$(SDL_MIXER_HOME)/lib -L.

CFLAGS=-Wall -Werror
LIBS=-lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

all: crumbs-release demo-release tootnsploot-release

#=============================================================================
# Crumbs

CRUMBS_SRC=../impl/*.c

crumbs-debug:
	$(CC) $(CFLAGS) -g -fPIC -shared $(CRUMBS_SRC) -DCRUMBS_EXPORTS -o $(LIB_OUTPUT) $(INCLUDE_DIR) $(LINK_DIR) $(LIBS)

crumbs-release:
	$(CC) $(CFLAGS) -fPIC -shared $(CRUMBS_SRC) -DCRUMBS_EXPORTS -o $(LIB_OUTPUT) $(INCLUDE_DIR) $(LINK_DIR) $(LIBS)

#=============================================================================
# Demo

D=../demo
DEMO_SRC=$(D)/*.c ../common/*.c $(D)/entities/*.c $(D)/input/*.c $(D)/scenes/*.c $(D)/util/*.c

demo-debug:
	$(CC) -g $(MAIN_DEF) $(DEMO_SRC) -o demo.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs

demo-release:
	$(CC) $(MAIN_DEF) $(DEMO_SRC) -o demo.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs


#=============================================================================
# Toot n Sploot

T=../tootnsploot
TNS_SRC=$(T)/*.c ../common/*.c $(T)/entities/*.c $(T)/input/*.c $(T)/scenes/*.c $(T)/util/*.c

tootnsploot-debug:
	$(CC) -g $(MAIN_DEF) $(TNS_SRC) -o tootnsploot.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs

tootnsploot-release:
	$(CC) $(MAIN_DEF) $(TNS_SRC) -o tootnsploot.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs
