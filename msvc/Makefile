# This is a Makefile for use by NMAKE from the MSVC build tools.
# It produces a file called a.exe.
# The necessary SDL dll files must be present in the same directory as the
# executable in order for it to run.

# The include and lib directories must exist in the SDL2_HOME directory.
INCLUDE_DIR=/I$(SDL2_HOME)\include /I..
LINK_DIR=/LIBPATH:"$(SDL2_HOME)\lib"

CC=cl
CFLAGS=/W3 /WX
# ..\main.c ..\example.c ..\input_demo.c ..\entity_demo.c ..\player_demo.c
SRC= ..\*.c ..\demo\main.c ..\demo\demo.c ..\demo\entities\*.c ..\demo\input\*.c ..\demo\collision\*.c ..\demo\scenes\*.c
TESTSRC= ..\*.c ..\demo\test.c ..\demo\entities\*.c ..\demo\input\*.c ..\demo\collision\*.c ..\demo\scenes\*.c
LIBS="SDL2.lib" "SDL2main.lib" "kernel32.lib" "user32.lib" "shell32.lib"

all:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(SRC) /link $(LIBS) $(LINK_DIR) /SUBSYSTEM:CONSOLE -out:a.exe

.PHONY: test
test:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(TESTSRC) /link $(LIBS) $(LINK_DIR) /SUBSYSTEM:CONSOLE -out:test.exe
