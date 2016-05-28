# textureinfo

A simple command-line utility for inspecting texture file header data. Currently supports the following container formats:

* PVR
	* v3
	* v1 & v2 (a.k.a. legacy)

# Build health

| Branch  | Status |
| ------: | ------:|
| master  | [![Build Status: master]](https://travis-ci.org/joedavisdev/textureinfo)  |
| develop | [![Build Status: develop]](https://travis-ci.org/joedavisdev/textureinfo) |

[Build Status: master]:https://travis-ci.org/joedavisdev/textureinfo.svg?branch=master
[Build Status: develop]:https://travis-ci.org/joedavisdev/textureinfo.svg?branch=develop

# Building

## Linux & OS X script build
````bash
./build.sh
````

## Manual build

````
mkdir -p build
cd build
cmake ..
cmake --build .
cd ..

````

# Usage

````
usage: textureinfo [--csv] ...

descriptions:
	--csv: Write header information to a CSV file (calling directory)
````
