# This is a Makefile for use by NMAKE from the MSVC build tools.
# It produces a file called a.exe.
# The necessary SDL dll files must be present in the same directory as the
# executable in order for it to run.

# The include and lib directories must exist in the SDL2_HOME directory.
INCLUDE_DIR=/I$(SDL2_HOME)\include /I..
LINK_DIR=/LIBPATH:"$(SDL2_HOME)\lib"

CC=cl
CFLAGS=/W3 /WX
DEMO_SRC= ..\*.c ..\impl\*.c ..\demo\main.c ..\demo\demo.c ..\common\*.c ..\demo\entities\*.c ..\demo\input\*.c ..\demo\scenes\*.c ..\demo\util\*.c
TNS_SRC= ..\*.c ..\impl\*.c ..\tootnsploot\main.c ..\tootnsploot\tootnsploot.c ..\common\*.c ..\tootnsploot\entities\*.c ..\tootnsploot\input\*.c ..\tootnsploot\scenes\*.c ..\tootnsploot\util\*.c
LIBS="SDL2.lib" "SDL2main.lib" "SDL2_ttf.lib" "SDL2_image.lib" "SDL2_mixer.lib" "kernel32.lib" "user32.lib" "shell32.lib"

demo-debug:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(DEMO_SRC) /link $(LIBS) $(LINK_DIR) /SUBSYSTEM:CONSOLE -out:demo.exe

demo-release:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(DEMO_SRC) /link $(LIBS) $(LINK_DIR) /SUBSYSTEM:WINDOWS -out:demo.exe

tootnsploot-debug:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(TNS_SRC) /link $(LIBS) $(LINK_DIR) /SUBSYSTEM:CONSOLE -out:tootnsploot.exe

tootnsploot-release:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(TNS_SRC) /link $(LIBS) $(LINK_DIR) /SUBSYSTEM:WINDOWS -out:tootnsploot.exe
