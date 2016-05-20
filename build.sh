CMAKE_BIN=cmake
if [-z $1]
    CMAKE_BIN=$1
fi
set -e
mkdir -p build
cd build
cmake ..
cmake --build .
cd ..
