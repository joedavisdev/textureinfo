cmake_minimum_required (VERSION 2.8)

set(EXE_NAME textureinfo)
project(${EXE_NAME})

set (INC_FILES
    textureinfo.h
)
set (SRC_FILES
    main.cpp
)
add_executable(${EXE_NAME} ${SRC_FILES} ${INC_FILES})
add_definitions(-std=c++11)
