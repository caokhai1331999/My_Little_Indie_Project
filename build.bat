@echo off
mkdir build
pushd build

set hr=%time:~0,2%
set hr=%hr: =0%
set min=%time:~3,2%
set min=%min: =0%
set sec=%time:~6,2%
set sec=%sec: =0%

set dd=%date:~-7,2%
set dd=%dd: =0%
set mm=%date:~-10,2%
set mm=%mm: =0%
set yr=%date:~-4,4%
set yr=%yr: =0%

set DIRECTIVES_FLAG=/DDEBUG=1 /DINTERNAL=1 /DSTD_140=1 /DDISPLAY_TIME=1
set COMPILE_FLAG=/FC /Zi /EHsc /MT
rem new flag /MT
set LIB_= strmiids.lib uuid.lib Kernel32.lib user32.lib gdi32.lib Opengl32.lib assimp-vc143-mtd.lib
set _include=..\include
set glad_include=..\include\glad
rem set PDB_name=

rem Remember exclude out the src files that is used to build dll while building exe
set src_files_for_dll=..\src\exclude_for_debug\*.cpp
set glad_src=..\src\glad\*.c
set dll_name=light32
rem set EXPORT_=

set GLFW_INCLUDE_DIR="C:\Users\klove\Downloads\External_Libraries\glfw\glfw-3.4.bin.WIN64\include"

set ASSIMP_DIR="C:\Users\klove\Documents\repos\GLFW2\Vulkan_Learning_Project\assimp\include"

set ASSIMP_LIB="C:\Users\klove\Documents\repos\GLFW2\Vulkan_Learning_Project\assimp\lib\debug"

set FILES_FOR_EXE=..\src\*.cpp

del gl.obj wgl.obj glad.obj glad_wgl.obj

rem ..\SoundMaker.cpp  ..\GUIDs.cpp
rem Mmdevapi.dll Audioses.dll /VERBOSE:LIB 
rem remember to add these to use address sanitizer /EHsc /fsanitize=address
rem -fsanitize=address -DEBUG  uuid.lib ..\gl_extensions.cpp
rem for %%f in (..\*.cpp) do (
rem     cl /FC /Zi -Fe:"win32Game" %%f -I%INCLUDE_% -link %LIB_% -DEBUG /FORCE:MULTIPLE /IGNORE:4006
rem  )

rem Let alone the hot Loading code later
rem delete pre-pdb files
rem=========================================

rem del Camera.obj Light.obj entity.obj
rem del %dll_name%__* light32.exp light32.lib
rem set dll_name_with_time_=%dll_name%__%hr%_%min%_%sec%__%dd%-%mm%-%yr%
rem cl /LD /FD %COMPILE_FLAG% %DIRECTIVES_FLAG% %src_files_for_dll% %glad_src% -I%_include% -I%glad_include% -I%GLFW_INCLUDE_DIR% -I%ASSIMP_DIR% -link /PDB:%dll_name_with_time_%.pdb %LIB_% /LIBPATH:%ASSIMP_LIB% /FORCE:MULTIPLE /IGNORE:4006 /IMPLIB:%dll_name%.lib /OUT:%dll_name%.dll
 
rem ====================================== 

del B_shader.obj C_Model.obj C_Mesh.obj handmade.obj main.obj SoundMaker.obj testOpenGL.obj Tile.obj win32Game.obj Tile.obj animation.obj animator.obj bone.obj
del win32Game.pdb win32Game__*
set exe_name_with_time=win32Game__%hr%_%min%_%sec%__%dd%-%mm%-%yr%
cl %COMPILE_FLAG% %DIRECTIVES_FLAG% -Fe:"win32Game" %FILES_FOR_EXE% %glad_src% -I%_include% -I%glad_include% -I%GLFW_INCLUDE_DIR% -I%ASSIMP_DIR% -link %LIB_% /LIBPATH:%ASSIMP_LIB% /PDB:%exe_name_with_time%.pdb /subsystem:windows /FORCE:MULTIPLE /IGNORE:4006 /ENTRY:WinMainCRTStartup /subsystem:console

rem=======================================

rem if %ERRORLEVEL% EQU 0 (
rem    @echo Announce: " compilation succeeded (^ w ^) "
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
