set -e
CMAKE_BIN=cmake
if [ -n "$1" ]; then
    CMAKE_BIN=$1
fi
echo "DEBUG: CMake binary = $CMAKE_BIN"
mkdir -p build
cd build
$CMAKE_BIN ..
$CMAKE_BIN --build .
cd ..
