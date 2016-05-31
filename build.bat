@echo off
set ORIG_DIR= %CD%
set CMAKE_BIN=cmake

set BUILD_DIR=build\default
mkdir %BUILD_DIR%
cd %BUILD_DIR%
%CMAKE_BIN% %ORIG_DIR%
%CMAKE_BIN% --build .
cd %ORIG_DIR%