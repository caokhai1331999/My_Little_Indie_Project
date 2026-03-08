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

set DIRECTIVES_FLAG=/DDEBUG=1 /DINTERNAL=1 /DON_LITTLE_BEAST=0 /DDISPLAY_TIME=1
set COMPILE_FLAG=/FC /Zi /EHsc
set LIB_= strmiids.lib uuid.lib Kernel32.lib user32.lib gdi32.lib Opengl32.lib assimp-vc143-mtd.lib
set _include=..\include
set glad_src=..\include\glad
rem set PDB_name=

rem Remember exclude out the src files that is used to build dll while building exe
set src_files_for_dll=..\src\exclude_for_debug\*.c*
set dll_name=skeletalAni32
rem set EXPORT_=

set GLFW_INCLUDE_DIR="C:\Users\klove\Downloads\External_Libraries\glfw\glfw-3.4.bin.WIN64\include"

set ASSIMP_DIR="C:\Users\klove\Documents\repos\GLFW2\Vulkan_Learning_Project\assimp\include"

set ASSIMP_LIB="C:\Users\klove\Documents\repos\GLFW2\Vulkan_Learning_Project\assimp\lib\debug"

set FILES_FOR_EXE=..\src\*.c*
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

del animation.obj animator.obj Bone.obj
del skeletalAni32__* animation.exp animation.lib
set dll_name_with_time_=%dll_name%__%hr%_%min%_%sec%__%dd%-%mm%-%yr%
cl /D_USRDLL /D_WINDLL /LD /Zi /EHsc /FD %DIRECTIVES_FLAG% %src_files_for_dll% -I%_include% -I%glad_src% -I%GLFW_INCLUDE_DIR% -I%ASSIMP_DIR% -link /PDB:%dll_name_with_time_%.pdb %LIB_% /LIBPATH:%ASSIMP_LIB% /FORCE:MULTIPLE /IGNORE:4006 /OUT:%dll_name%.dll

rem=========================================

rem ====================================== 

rem del B_shader.obj Camera.obj C_Mesh.obj C_Model.obj handmade.obj main.obj SoundMaker.obj testOpenGL.obj Tile.obj win32Game.obj
rem del win32Game.pdb
rem cl %COMPILE_FLAG% %DIRECTIVES_FLAG% -Fe:"win32Game" %FILES_FOR_EXE% -I%_include% -I%glad_src% -I%GLFW_INCLUDE_DIR% -I%ASSIMP_DIR% -link %LIB_% animation.lib /LIBPATH:%ASSIMP_LIB% /PDB:win32Game.pdb /subsystem:windows /FORCE:MULTIPLE /IGNORE:4006 /ENTRY:WinMainCRTStartup /subsystem:console

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
