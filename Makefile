# This is the Makefile for Unix-like systems, like MacOS and Linux.
# By default, it uses clang as the compiler, but gcc can be used instead.
# It is assumed that an environment variable called SDL2_HOME is defined
# and that it contains the full path to the installation of SDL.

# The include and lib directories must exist in the SDL2_HOME directory.
INCLUDE_DIR=-I$(SDL2_HOME)/include -I.
LINK_DIR=-L$(SDL2_HOME)/lib -L$(SDL_TTF_HOME)/lib -L$(SDL_IMAGE_HOME)/lib

CC=clang # This can be switched out for gcc.
CFLAGS=-Wall -Werror
SRC=*.c impl/*.c demo/main.c demo/demo.c demo/util/*.c demo/menu/*.c demo/dialog/*.c demo/entities/*.c demo/input/*.c demo/collision/*.c demo/scenes/*.c demo/font/*.c demo/texture/*.c
TESTSRC=*.c impl/*.c demo/test.c demo/entities/*.c demo/input/*.c demo/collision/*.c demo/scenes/*.c
LIBS=-lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image

all:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(LINK_DIR) $(SRC) $(LIBS)


.PHONY: test
test:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(LINK_DIR) $(TESTSRC) $(LIBS) -o test
