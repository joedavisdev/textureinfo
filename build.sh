set -e
CMAKE_BIN=cmake
if [ -z "$1" ]; then
    CMAKE_BIN=$1
fi
mkdir -p build
cd build
cmake ..
cmake --build .
cd ..
