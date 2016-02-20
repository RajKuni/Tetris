@echo off

pushd ..\build

set SDLInclude=C:\linker_libs\SDL\include 
set SDLImageInclude=C:\linker_libs\SDL2_image-2.0.1\include

set SDLlib=C:\linker_libs\SDL\sdl_x86
set SDLImagelib=C:\linker_libs\SDL2_image-2.0.1\lib\x86  

set libs= SDL2.lib SDL2main.lib SDL2Test.lib SDL2_image.lib 

cl /EHsc /I %SDLInclude% /I %SDLImageInclude% /Zi /MDd ..\code\Tetris.cpp /link /SUBSYSTEM:CONSOLE /LIBPATH:%SDLlib% /LIBPATH:%SDLImagelib% %libs% 

rem doing set > <filename>.txt will write all enivornment variable paths to file!
rem use echo %<NAME>% to find out environment variable value

popd

rem NOTE: USE Below command to open source along with exe!!!!!!
rem devenv win32_handmade.exe "../handmade/code/handmade.cpp"
rem
rem SO template: devenv <EXE name> <"path and name of the SOURCE"> <Can add more files to open after the exe!>
rem TOOK ME FOREVER TO FIND this : ( 
