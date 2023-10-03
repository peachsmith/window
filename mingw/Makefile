# A Makefile for MinGW-w64.
# Used by running mingw32-make.exe
# This assumes all the dependencies are installed in an MSYS2 environment.

SRC=../*.c ../impl/*.c ../demo/main.c ../demo/demo.c ../demo/util/*.c ../demo/menu/*.c ../demo/dialog/*.c ../demo/entities/*.c ../demo/input/*.c ../demo/collision/*.c ../demo/scenes/*.c ../demo/font/*.c ../demo/texture/*.c ../demo/audio/*.c

all:
	gcc $(SRC) -I.. -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

debug:
	gcc -g $(SRC) -I.. -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
