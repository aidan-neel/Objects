@echo off
setlocal enabledelayedexpansion

set SRC_DIR=src
set INCLUDE_DIR=include
set BUILD_DIR=build
set OUT_EXE=%BUILD_DIR%\main.exe

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

set FILES=
for /r %SRC_DIR% %%f in (*.cpp) do (
    set FILES=!FILES! "%%f"
)

g++ -std=c++20 -I%INCLUDE_DIR% !FILES! -o %OUT_EXE%
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed.
    exit /b %ERRORLEVEL%
)

echo Running...
%OUT_EXE%
