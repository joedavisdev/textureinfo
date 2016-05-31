# textureinfo

A simple command-line utility for inspecting texture file header data. Currently supports the following container formats:

* PVR
	* v1 & v2 (a.k.a. legacy)
	* v3

# Binaries

| OS (target) |
| :------ |
| [Windows (x86 32-bit)](https://www.dropbox.com/s/muqnxd7ezdi78le/textureinfo.exe?dl=0) |
| [OS X (x86 64-bit)](https://www.dropbox.com/s/invszufmdom2pdc/textureinfo?dl=0) |

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

Print a texture container's header information to stdout. Supported containers:
	* PVR (v1)
	* PVR (v2)
	* PVR (v3)

options:
	--csv: Write header information to a CSV file (calling directory). Disables stdout print
````

# Examples

## Printing to stdout

````
$ ./textureinfo ~/Downloads/PVRTextures/*
========================================
/Users/joe/Downloads/PVRTextures/GnomeWood.pvr
========================================
Container: PVR (v3)
Compressed format: PVRTCI_4bpp_RGBA
Channel name [0]: -
Channel name [1]: -
Channel name [2]: -
Channel name [3]: -
Bits per-pixel [0]: -
Bits per-pixel [1]: -
Bits per-pixel [2]: -
Bits per-pixel [3]: -
Color space: lRGB
Channel type: UnsignedByteNorm
Height: 1024
Width: 1024
Depth: 1
Array size: 1
Number of faces: 1
Number of MIP maps: 11
Flags: 0
Meta data size: 15

========================================
/Users/joe/Downloads/PVRTextures/GnomeWood_legacy.pvr
========================================
Container: PVR (legacy)
Format: GL_PVRTC4
Number of bits: 4
Height: 1024
Width: 1024
Array size: 1
Number of MIP maps: 11
Red mask?: False
Green mask?: False
Blue mask?: False
Alpha mask?: True
Magic number: 559044176
Flags: MIP_MAP|HAS_ALPHA|VERTICAL_FLIP|
Data size: 699136

========================================
/Users/joe/Downloads/PVRTextures/arial_36.pvr
========================================
Container: PVR (v3)
Compressed format: -
Channel name [0]: l
Channel name [1]: a
Channel name [2]: -
Channel name [3]: -
Bits per-pixel [0]: 8
Bits per-pixel [1]: 8
Bits per-pixel [2]: -
Bits per-pixel [3]: -
Color space: lRGB
Channel type: UnsignedByteNorm
Height: 512
Width: 256
Depth: 1
Array size: 1
Number of faces: 1
Number of MIP maps: 10
Flags: 0
Meta data size: 4035
````

## Printing to CSV

````
$ ./textureinfo ~/Downloads/PVRTextures/* --csv
````

````
$ cat pvrLegacy.csv
File name,Format,Number of bits,Height,Width,Array size,Number of MIP maps,Red mask?,Green mask?,Blue mask?,Alpha mask?,Magic number,Flags,Data size,
/Users/joe/Downloads/PVRTextures/GnomeWood_legacy.pvr,GL_PVRTC4,4,1024,1024,1,11,False,False,False,True,559044176,MIP_MAP|HAS_ALPHA|VERTICAL_FLIP|,699136,
````

````
$ cat pvrV3.csv
File name,Compressed format,Channel name [0],Channel name [1],Channel name [2],Channel name [3],Bits per-pixel [0],Bits per-pixel [1],Bits per-pixel [2],Bits per-pixel [3],Color space,Channel type,Height,Width,Depth,Array size,Number of faces,Number of MIP maps,Flags,Meta data size,
/Users/joe/Downloads/PVRTextures/GnomeWood.pvr,PVRTCI_4bpp_RGBA,-,-,-,-,-,-,-,-,lRGB,UnsignedByteNorm,1024,1024,1,1,1,11,0,15,
/Users/joe/Downloads/PVRTextures/arial_36.pvr,-,l,a,-,-,8,8,-,-,lRGB,UnsignedByteNorm,512,256,1,1,1,10,0,4035,
````
