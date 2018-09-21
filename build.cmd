cd /d %~dp0 
@echo off
SET path=%path%;E:\nil\apps\nim\dist\mingw64\bin
SET MINGW=E:\nil\apps\nim\dist\mingw64

mingw32-make  -R -C .build/projects/gmake-mingw-gcc config=release64 verbose=1
