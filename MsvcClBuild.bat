@echo off
mkdir build
pushd build

del *.obj
del *.pdb > NUL 2> NUL

set DIRECTIVES_FLAG=/D:DEBUG=1 /D:INTERNAL=1
set COMPILE_FLAG=/FC /Zi
set LIB_=user32.lib gdi32.lib uuid.lib strmiids.lib opengl32.lib 
set INCLUDE_=..\*.h
set FILES= ..\main.cpp ..\handmade.cpp ..\win32Game.cpp
rem remember to add these to use address sanitizer /EHsc /fsanitize=address
rem -fsanitize=address -DEBUG
rem for %%f in (..\*.cpp) do (
rem     cl /FC /Zi -Fe:"win32Game" %%f -I%INCLUDE_% -link %LIB_% -DEBUG /FORCE:MULTIPLE /IGNORE:4006
rem  )
    cl %COMPILE_FLAG% %DIRECTIVES_FLAG% -Fe:"win32Game" %FILES% -I%INCLUDE_% -link %LIB_% -DEBUG /subsystem:windows /FORCE:MULTIPLE /IGNORE:4006
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
