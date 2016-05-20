cmake_minimum_required (VERSION 3.0)

set(EXE_NAME textureinfo)
project(${EXE_NAME})

set (INC_FILES
    textureinfo.h
)
set (SRC_FILES
    main.cpp
)
add_executable(${EXE_NAME} ${SRC_FILES} ${INC_FILES})
set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD 11)
