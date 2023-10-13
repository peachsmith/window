# Makefile for MacOS and Linux.
#
# By default, the CC and LIB_OUTPUT variables are set to MacOS values, but
# they can be swapped out for Linux values.
#
# This file expects the following environment variables to be defined:
#
# SDL2_HOME:
#   directory where SDL2 is installed. This also contains the include
#   directory where all of the SDL2 headers should be.
#
# SDL_IMAGE_HOME:
#   directory where SDL2_image is installed
#
# SDL_TTF_HOME:
#   directory where SDL2_ttf is installed
#
# SDL_MIXER_HOME:
#   directory where SDL2_mixer is installed

# MacOS
CC=clang
LIB_OUTPUT=libcrumbs.dylib

# Linux
#CC=gcc
#LIB_OUTPUT=libcrumbs.so

# The include and lib directories must exist in the SDL2_HOME directory.
INCLUDE_DIR=-I$(SDL2_HOME)/include -I..
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
	$(CC) -g $(DEMO_SRC) -o demo.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs

demo-release:
	$(CC) $(DEMO_SRC) -o demo.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs


#=============================================================================
# Toot n Sploot

T=../tootnsploot
TNS_SRC=$(T)/*.c ../common/*.c $(T)/entities/*.c $(T)/input/*.c $(T)/scenes/*.c $(T)/util/*.c

tootnsploot-debug:
	$(CC) -g $(TNS_SRC) -o tootnsploot.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs

tootnsploot-release:
	$(CC) $(TNS_SRC) -o tootnsploot.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS) -lcrumbs
