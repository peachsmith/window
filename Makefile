# This is the Makefile for Unix-like systems, like MacOS and Linux.
# By default, it uses clang as the compiler, but gcc can be used instead.
# It is assumed that an environment variable called SDL2_HOME is defined
# and that it contains the full path to the installation of SDL.

# The include and lib directories must exist in the SDL2_HOME directory.
INCLUDE_DIR=-I$(SDL2_HOME)/include -I.
LINK_DIR=-L$(SDL2_HOME)/lib

CC=clang # This can be switched out for gcc.
CFLAGS=-Wall -Werror
# example.c main.c input_demo.c entity_demo.c player_demo.c
SRC=*.c demo/*.c
LIBS=-lSDL2 -lSDL2main

all:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(LINK_DIR) $(SRC) $(LIBS)
