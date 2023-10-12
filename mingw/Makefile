# A Makefile for MinGW-w64.
# Used by running mingw32-make.exe
# This assumes all the dependencies are installed in an MSYS2 environment.

all: crumbs-release demo-release tootnsploot-release

#=============================================================================
# Crumbs

IMPL_SRC=../impl/*.c

crumbs-debug:
	gcc -g -fPIC -shared $(IMPL_SRC) -DCRUMBS_EXPORTS -o libcrumbs.dll -I.. -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

crumbs-release:
	gcc -fPIC -shared $(IMPL_SRC) -DCRUMBS_EXPORTS -o libcrumbs.dll -I.. -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#=============================================================================
# Demo

D=../demo
DEMO_SRC=$(D)/*.c ../common/*.c $(D)/entities/*.c $(D)/input/*.c $(D)/scenes/*.c $(D)/util/*.c

demo-debug:
	gcc -g $(DEMO_SRC) -o demo -I.. -L. -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lcrumbs

demo-release:
	gcc $(DEMO_SRC) -o demo -I.. -L. -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lcrumbs


#=============================================================================
# Toot n Sploot

T=../tootnsploot
TNS_SRC=$(T)/*.c ../common/*.c $(T)/entities/*.c $(T)/input/*.c $(T)/scenes/*.c $(T)/util/*.c

tootnsploot-debug:
	gcc -g $(TNS_SRC) -o tootnsploot -I.. -L. -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lcrumbs

tootnsploot-release:
	gcc $(TNS_SRC) -o tootnsploot -I.. -L. -mwindows -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lcrumbs
