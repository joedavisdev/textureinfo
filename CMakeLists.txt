cmake_minimum_required (VERSION 3.3.1)

set(EXE_NAME textureinfo)
project(${EXE_NAME})

include_directories("${PROJECT_BINARY_DIR}" ${CMAKE_CURRENT_SOURCE_DIR}/external/pvrSdk)
set (INC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/external/pvrSdk/PixelFormat.h
)
set (SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/external/pvrSdk/PixelFormat.cpp
    main.cpp
)
add_executable(${EXE_NAME} ${SRC_FILES} ${INC_FILES})
add_definitions(-std=c++11)
