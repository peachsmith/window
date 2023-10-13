# This is a Makefile for use by NMAKE from the MSVC build tools.
# It produces a file called a.exe.
# The necessary SDL dll files must be present in the same directory as the
# executable in order for it to run.

# The include and lib directories must exist in the SDL2_HOME directory.
INCLUDE_DIR=/I$(SDL2_HOME)\include /I..\..
LINK_DIR=/LIBPATH:"$(SDL2_HOME)\lib" /LIBPATH:"."

CC=cl
CFLAGS=/W3 /WX
LIBS="SDL2.lib" "SDL2main.lib" "SDL2_ttf.lib" "SDL2_image.lib" "SDL2_mixer.lib" "kernel32.lib" "user32.lib" "shell32.lib"

all: crumbs-release demo-release tootnsploot-release clean

clean:
	del *.obj

#=============================================================================
# Crumbs

CRUMBS_SRC=..\..\impl\*.c

CRUMBS_DEBUG_CFLAGS=/JMC /permissive- /ifcOutput "./" /GS /W3 /Zc:wchar_t /ZI /Gm- /Od /sdl /Fd"vc143.pdb" /Zc:inline /fp:precise /D "_DEBUG" /D "CRUMBS_EXPORTS" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /RTC1 /Gd /LDd /FC /EHsc /nologo /Fo"./" /Fp"x64\Debug\crumbs.pch" /diagnostics:column 
CRUMBS_DEBUG_LFLAGS=/OUT:"crumbs.dll" /MANIFEST /NXCOMPAT /PDB:"crumbs.pdb" /DYNAMICBASE $(LIBS) /IMPLIB:"crumbs.lib" /DEBUG /DLL /MACHINE:X64 /INCREMENTAL /SUBSYSTEM:WINDOWS /MANIFESTUAC:NO /ManifestFile:"crumbs.dll.intermediate.manifest" /LTCGOUT:"crumbs.iobj" /ERRORREPORT:PROMPT /ILK:"crumbs.ilk" /NOLOGO /TLBID:1 

CRUMBS_CFLAGS=/permissive- /ifcOutput "./" /GS /GL /W3 /Gy /Zc:wchar_t /Zi /Gm- /O2 /sdl /Zc:inline /fp:precise /D "NDEBUG" /D "CRUMBS_EXPORTS" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_UNICODE" /D "UNICODE" /errorReport:prompt /WX- /Zc:forScope /Gd /Oi /LD /FC /EHsc /nologo /Fo"./" /diagnostics:column 
CRUMBS_LFLAGS=/OUT:"crumbs.dll" /LTCG:incremental /NXCOMPAT /DYNAMICBASE $(LIBS) /IMPLIB:"crumbs.lib" /DEBUG /DLL /MACHINE:X64 /OPT:REF /SUBSYSTEM:WINDOWS /OPT:ICF /ERRORREPORT:PROMPT /NOLOGO /TLBID:1

crumbs-debug:
	$(CC) $(CRUMBS_DEBUG_CFLAGS) $(INCLUDE_DIR) $(CRUMBS_SRC) /link $(LINK_DIR) $(CRUMBS_DEBUG_LFLAGS)

crumbs-release:
	$(CC) $(CRUMBS_CFLAGS) $(INCLUDE_DIR) $(CRUMBS_SRC) /link $(LINK_DIR) $(CRUMBS_LFLAGS)
	del *.obj
	del crumbs.iobj
	del crumbs.ipdb
	del *.pdb
	del crumbs.exp


#=============================================================================
# Demo

D=..\..\demo
DEMO_SRC=$(D)\main.c $(D)\demo.c ..\..\common\*.c $(D)\entities\*.c $(D)\input\*.c $(D)\scenes\*.c $(D)\util\*.c

demo-debug:
	$(CC) /JMC $(CFLAGS) /D "_DEBUG" $(INCLUDE_DIR) $(DEMO_SRC) /link $(LIBS) "crumbs.lib" $(LINK_DIR) /SUBSYSTEM:CONSOLE -out:demo.exe

demo-release:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(DEMO_SRC) /link $(LIBS) "crumbs.lib" $(LINK_DIR) /SUBSYSTEM:WINDOWS -out:demo.exe

#=============================================================================
# Toot n Sploot

T=..\..\tootnsploot
TNS_SRC=$(T)\main.c $(T)\tootnsploot.c ..\..\common\*.c $(T)\entities\*.c $(T)\input\*.c $(T)\scenes\*.c $(T)\util\*.c

tootnsploot-debug:
	$(CC) /JMC $(CFLAGS) /D "_DEBUG" $(INCLUDE_DIR) $(TNS_SRC) /link $(LIBS) "crumbs.lib" $(LINK_DIR) /SUBSYSTEM:CONSOLE -out:tootnsploot.exe

tootnsploot-release:
	$(CC) $(CFLAGS) $(INCLUDE_DIR) $(TNS_SRC) /link $(LIBS) "crumbs.lib" $(LINK_DIR) /SUBSYSTEM:WINDOWS -out:tootnsploot.exe
