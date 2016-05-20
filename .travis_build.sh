set -e
# Do some setup
CMAKE_BIN=
echo "DEBUG: TravisCI host OS = $1"
if [ "$1" == "linux" ]; then
bash -c "cd ~/
wget  --no-check-certificate https://cmake.org/files/v3.5/cmake-3.5.2-Linux-x86_64.tar.gz 
tar -xf cmake-3.5.2-Linux-x86_64.tar.gz"
CMAKE_BIN=~/cmake-3.5.2-Linux-x86_64/bin/./cmake
fi
# Run the CMake build commands
bash build.sh $CMAKE_BIN
