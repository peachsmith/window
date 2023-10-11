# A Makefile for MinGW-w64.
# Used by running mingw32-make.exe
# This assumes all the dependencies are installed in an MSYS2 environment.

#=============================================================================
# Demo


DEMO_SRC=../*.c ../impl/*.c ../demo/main.c ../demo/demo.c \
../common/*.c \
../demo/entities/*.c \
../demo/input/*.c \
../demo/scenes/*.c \
../demo/util/*.c

demo-debug:
	gcc -g $(DEMO_SRC) -o demo -I.. -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

demo-release:
	gcc $(DEMO_SRC) -o demo -I.. -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer


#=============================================================================
# Toot n Sploot

TNS_SRC=../*.c ../impl/*.c ../tootnsploot/main.c ../tootnsploot/tootnsploot.c \
../common/*.c \
../tootnsploot/entities/*.c \
../tootnsploot/input/*.c \
../tootnsploot/scenes/*.c \
../tootnsploot/util/*.c

tootnsploot-debug:
	gcc -g $(TNS_SRC) -o tootnsploot -I.. -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

tootnsploot-release:
	gcc $(TNS_SRC) -o tootnsploot -I.. -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
