# A Makefile for MinGW-w64.
# Used by running mingw32-make.exe
# This assumes all the dependencies are installed in an MSYS2 environment.

all: demo-release tootnsploot-release

IMPL_SRC=../*.c ../impl/*.c

#=============================================================================
# Demo

D=../demo
DEMO_SRC=$(IMPL_SRC) $(D)/*.c ../common/*.c $(D)/entities/*.c $(D)/input/*.c $(D)/scenes/*.c $(D)/util/*.c

demo-debug:
	gcc -g $(DEMO_SRC) -o demo -I.. -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

demo-release:
	gcc $(DEMO_SRC) -o demo -I.. -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer


#=============================================================================
# Toot n Sploot

T=../tootnsploot
TNS_SRC=$(IMPL_SRC) $(T)/*.c ../common/*.c $(T)/entities/*.c $(T)/input/*.c $(T)/scenes/*.c $(T)/util/*.c

tootnsploot-debug:
	gcc -g $(TNS_SRC) -o tootnsploot -I.. -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

tootnsploot-release:
	gcc $(TNS_SRC) -o tootnsploot -I.. -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
