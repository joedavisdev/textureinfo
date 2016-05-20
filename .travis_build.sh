set -e
# Do some setup
CMAKE_BIN=
if [[ "$1" == "linux" ]]; then
bash -c "cd ~/
wget  --no-check-certificate https://cmake.org/files/v3.5/cmake-3.5.2-Linux-x86_64.tar.gz 
tar -xf cmake-3.5.2-Linux-x86_64.tar.gz"
CMAKE_BIN=~/cmake-3.5.2-Linux-x86_64/bin/./cmake
fi
# Run the CMake build commands
sh build.sh $CMAKE_BIN
