
# The SDL2_HOME variable is an environment variable where SDL2 was installed.
INCLUDE_DIR=-I$(SDL2_HOME)/include
LINK_DIR=-L$(SDL2_HOME)/lib

all:
	clang -Wall -Werror $(INCLUDE_DIR) $(LINK_DIR) main.c example.c -lSDL2 -lSDL2main
