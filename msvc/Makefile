# This is a Makefile for use by NMAKE from the MSVC build tools.
# It produces a file called a.exe.
# The necessary SDL dll files must be present in the same directory as the
# executable in order for it to run.

all: a.exe

objects:
	cl /c /I"$(SDL2_HOME)\include" ..\main.c
	cl /c /I"$(SDL2_HOME)\include" ..\example.c

a.exe: objects
	link main.obj example.obj "SDL2.lib" "SDL2main.lib" "kernel32.lib" "user32.lib" "shell32.lib" /LIBPATH:"$(SDL2_HOME)\lib" /SUBSYSTEM:CONSOLE -out:a.exe
