mkdir build
pushd build

del *.obj *.pdb

set LIB_=user32.lib gdi32.lib opengl32.lib
set INCLUDE_=..\*.h
rem remember to add these to use address sanitizer /EHsc /fsanitize=address
@echo off
rem -fsanitize=address -DEBUG
rem for %%f in (..\*.cpp) do (
rem     cl /FC /Zi -Fe:"win32Game" %%f -link user32.lib gdi32.lib
rem  )
    cl /FC /Zi -Fe:"win32Game"  ..\handmade.cpp ..\win32Game.cpp -I%INCLUDE_% -link %LIB_%
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
