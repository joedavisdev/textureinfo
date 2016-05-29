set -e
ORIG_DIR=$PWD
CMAKE_BIN=cmake
if [ -n "$1" ]; then
    CMAKE_BIN=$1
fi
echo "DEBUG: CMake binary = $CMAKE_BIN"
BUILD_DIR=build/default
mkdir -p $BUILD_DIR
cd $BUILD_DIR
$CMAKE_BIN $ORIG_DIR
$CMAKE_BIN --build .
cd $ORIG_DIR
 
