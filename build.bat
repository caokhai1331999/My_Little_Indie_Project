@echo off
mkdir build
pushd build

del .\src\*.obj
del *.pdb > NUL 2> NUL

set DIRECTIVES_FLAG=/D:DEBUG=1 /D:INTERNAL=1
set COMPILE_FLAG=/FC /Zi /EHsc
set LIB_= strmiids.lib uuid.lib Kernel32.lib user32.lib gdi32.lib Opengl32.lib assimp-vc143-mtd.lib
set _include=..\include
set glad_src=..\include\glad
rem set PDB_name=
rem set dll_name=win32.dll
set GLFW_INCLUDE_DIR="C:\Users\klove\Downloads\External_Libraries\glfw\glfw-3.4.bin.WIN64\include"

set ASSIMP_DIR="C:\Users\klove\Documents\repos\GLFW2\Vulkan_Learning_Project\assimp\include"

set ASSIMP_LIB="C:\Users\klove\Documents\repos\GLFW2\Vulkan_Learning_Project\assimp\lib\debug"

set FILES=..\src\*.c*
rem ..\SoundMaker.cpp  ..\GUIDs.cpp
rem Mmdevapi.dll Audioses.dll /VERBOSE:LIB 
rem remember to add these to use address sanitizer /EHsc /fsanitize=address
rem -fsanitize=address -DEBUG  uuid.lib ..\gl_extensions.cpp
rem for %%f in (..\*.cpp) do (
rem     cl /FC /Zi -Fe:"win32Game" %%f -I%INCLUDE_% -link %LIB_% -DEBUG /FORCE:MULTIPLE /IGNORE:4006
rem  )

rem Let alone the hot Loading code later

rem cl /LD ..\win32Game.cpp ..\main.cpp %COMPILE_FLAG% %DIRECTIVES_FLAG%  -I%_include% -I%glad_src% -link %LIB_% -DEBUG /subsystem:windows /FORCE:MULTIPLE /IGNORE:4006 /PDB:%PDB_name% %EXPORT_% /DEBUG/OUT:%dll_name%

cl %COMPILE_FLAG% %DIRECTIVES_FLAG% -Fe:"win32Game" %FILES% -I%_include% -I%glad_src% -I%GLFW_INCLUDE_DIR% -I%ASSIMP_DIR% -link %LIB_% /LIBPATH:%ASSIMP_LIB% -DEBUG /subsystem:windows /FORCE:MULTIPLE /IGNORE:4006
/subsystem:console
rem if %ERRORLEVEL% EQU 0 (
rem    @echo Announce: " compilation succeeded (^ w ^) "
rem ) else (
rem    @echo                                  @   @          
rem    @echo Announce: " compilation failed \_(-_-)_/ "
rem )

popd


rem mkdir build
rem pushd build

rem @echo off

rem for %%f in (..\*.cpp ..\*.h) do (
rem     cl /FC /Zi /EHsc /fsanitize=address -Fe:"win32Game" %%f -link user32.lib gdi32.lib
rem )

rem if %ERRORLEVEL% EQU 0 (
rem     @echo Compilation succeeded (^ w ^)
rem ) else (
rem     @echo                      
rem     @echo Compilation failed \_(-_-)_/
rem )

rem popd
