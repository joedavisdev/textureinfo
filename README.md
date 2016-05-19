# textureinfo
[![Build Status](https://travis-ci.org/joedavisdev/textureinfo.svg?branch=develop)](https://travis-ci.org/joedavisdev/textureinfo)

A simple command-line utility for inspecting texture file header data. Currently supports the following container formats:

* PVR (v3)

# Building

## Linux & OS X
````bash
./build.sh
````

## Manual

````
mkdir -p build
cd build
cmake ..
cmake --build .
cd ..

````

# Instructions

````
usage: textureinfo [--csv] ...

descriptions:
	--csv: Write header information to a CSV file (calling directory)
````
