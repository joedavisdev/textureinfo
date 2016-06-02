# textureinfo

A simple command-line utility for inspecting texture file header data. Currently supports the following container formats:

* KTX
* PVR
	* v1 & v2 (a.k.a. legacy)
	* v3

# Binaries

| OS (target) |
| :------ |
| [Windows (x86 32-bit)](https://www.dropbox.com/s/muqnxd7ezdi78le/textureinfo.exe?dl=0) |
| [OS X (x86 64-bit)](https://www.dropbox.com/s/invszufmdom2pdc/textureinfo?dl=0) |
| [Linux (x86 64-bit)](https://www.dropbox.com/s/w822vy8ftsj5zc4/textureinfo?dl=0) |

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
version: 1.1.0
usage: textureinfo [--csv] ...

Print a texture container's header information to stdout. Supported containers:
	* PVR (v1)
	* PVR (v2)
	* PVR (v3)
	* KTX

options:
	--csv: Write header information to a CSV file (calling directory). Disables stdout print
````

# Examples

## Printing to stdout

````
$ ./textureinfo ~/Downloads/PVRTextures/GnomeWood.*
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
Width: 1024
Height: 1024
Depth: 1
Array size: 1
Number of faces: 1
Number of MIP maps: 11
Flags: 0
Meta data size: 15

========================================
/Users/joe/Downloads/PVRTextures/GnomeWood.ktx
========================================
Container: KTX
glFormat: 0
glInternalFormat: 35842
glBaseInternalFormat: 6408
glType: 0
glType size: 1
Width: 1024
Height: 1024
Depth: 0
Array size: 0
Number of faces: 1
Number of MIP maps: 11
Bytes of key value data: 32
Endianness: 67305985
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

````
$ cat ktx.csv
File name,glFormat,glInternalFormat,glBaseInternalFormat,glType,glType size,Width,Height,Depth,Array size,Number of faces,Number of MIP maps,Bytes of key value data,Endianness,
/Users/joe/Downloads/PVRTextures/GnomeWood.ktx,0,35842,6408,0,1,1024,1024,0,0,1,11,32,67305985,
````
