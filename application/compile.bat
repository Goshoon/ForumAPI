@echo off
color 2
setlocal enabledelayedexpansion

::windres resources.rc -O coff -o resources.res
set APPNAME=ForumKK
set CURL=C:/Dependencies/curl/
set FTXUI=C:/Dependencies/ftxui/

set "SOURCES="
set "OBJECTS="
set "INCLUDES="

:: Buscar todos los archivos .cpp en ./Source
for /R ./Source %%f in (*.cpp) do (
    set "SOURCES=!SOURCES! %%f"
    set "OBJECTS=!OBJECTS! %%~nf.o"
)

:: Buscar todas las carpetas de inclusion dentro de ./headers
for /R ./headers /D %%d in (*) do (
    set "INCLUDES=!INCLUDES! -I%%d"
)

:: Compilar todos los archivos a un archivo de objeto
for %%f in (%SOURCES%) do (
    g++ -std=c++17 -c %%f ^
        -I "./headers" %INCLUDES% ^
        -I "%CURL%/include" ^
        -I "%FTXUI%/include" ^
        -o %%~nf.o

    if errorlevel 1 (
        echo Error compiling %%f.
        pause
        exit /b 1
    )
)

:: Conectar todos los archivos al ejecutable
g++ %OBJECTS% ^
  -L "%FTXUI%/lib" ^
  -L "%CURL%/lib" ^
  -lftxui-dom ^
  -lftxui-screen ^
  -lcurl ^
  -o "./bin/%APPNAME%.exe"


pause

echo Compilation succeeded. Running the app...
start ./Bin/%APPNAME%.exe

endlocal
exit
