@echo Pls wait for some settings
@echo  ()     ()
@echo   (- 3 -)
@echo   (@) (@)
@echo  (@)   (@)
@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
@echo Copying project file for backup purpose!! * o 0
@echo off
runemacs
backup.bat
set path="W:\misc";%path%
sh.exe backupOnline.sh
@echo DONE!!!