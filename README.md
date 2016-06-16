# textureinfo

A simple command-line utility for inspecting texture file header data. Currently supports the following container formats:

* DDS
* KTX
* PVR
	* v1 & v2 (a.k.a. legacy)
	* v3

# Build health

| Branch  | Status |
| ------: | ------:|
| master  | [![Build Status: master]](https://travis-ci.org/joedavisdev/textureinfo)  |
| develop | [![Build Status: develop]](https://travis-ci.org/joedavisdev/textureinfo) |

[Build Status: master]:https://travis-ci.org/joedavisdev/textureinfo.svg?branch=master
[Build Status: develop]:https://travis-ci.org/joedavisdev/textureinfo.svg?branch=develop

# Building

## Dependencies

* Cmake 3.0 or better

## Linux & OS X script build
````
./build.sh
````

## Windows script build
````
./build.bat
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
$ ./textureinfo --help
version: 1.2
usage: textureinfo [--csv] ...

Print a texture container's header information to stdout. Supported containers:
	* PVR (v3)
	* PVR (legacy)
	* KTX
	* DDS

options:
	--csv: Write header information to a CSV file (calling directory). Disables stdout print
````

# Examples

## Printing to stdout

````
$ ./textureinfo ~/Downloads/Textures/*
========================================
/Users/joe/Downloads/Textures/GnomeWood.pvr
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
/Users/joe/Downloads/Textures/GnomeWood_legacy.pvr
========================================
Container: PVR (legacy)
Format: GL_PVRTC4
Number of bits: 4
Width: 1024
Height: 1024
Array size: 1
Number of MIP maps: 11
Red mask?: False
Green mask?: False
Blue mask?: False
Alpha mask?: True
Magic number: 559044176
Flags: MIP_MAP|HAS_ALPHA|VERTICAL_FLIP
Data size: 699136

========================================
/Users/joe/Downloads/Textures/arial_36.pvr
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
Width: 256
Height: 512
Depth: 1
Array size: 1
Number of faces: 1
Number of MIP maps: 10
Flags: 0
Meta data size: 4035

========================================
/Users/joe/Downloads/Textures/GnomeWood.ktx
========================================
Container: KTX
glFormat: 0 (compressed format)
glInternalFormat: COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
glBaseInternalFormat: GL_RGBA
glType: 0 (compressed format)
glType size: 1
Width: 1024
Height: 1024
Depth: 0
Array size: 0
Number of faces: 1
Number of MIP maps: 11
Bytes of key value data: 32
Endianness: 67305985

========================================
/Users/joe/Downloads/Textures/GnomeWoodRGBA5551.ktx
========================================
Container: KTX
glFormat: GL_RGBA
glInternalFormat: GL_RGB5_A1
glBaseInternalFormat: GL_RGBA
glType: GL_UNSIGNED_SHORT_5_5_5_1
glType size: 2
Width: 1024
Height: 1024
Depth: 0
Array size: 0
Number of faces: 1
Number of MIP maps: 11
Bytes of key value data: 32
Endianness: 67305985

========================================
/Users/joe/Downloads/Textures/GnomeWood.dds
========================================
Container: DDS
Pixel format (size): 32
Pixel format (flags): e_fourCC
Pixel format (fourcc): PTC4
Pixel format (bit count): 0
Pixel format (red mask): False
Pixel format (blue mask): False
Pixel format (green mask): False
Pixel format (alpha mask): False
Size: 124
Flags: e_Capabilities|e_height|e_width|e_pixelFormat|e_mipMapCount|e_linearSize
Width: 1024
Height: 1024
Depth: 1
Number of MIP maps: 11
Pitch or linear size: 699136
Capabilities (1): e_complex|e_texture|e_mipMaps
Capabilities (2):
Capabilities (3): 0
Capabilities (4): 0
Reserved: 0:0:0:0:0:0:0:0:0:0:0
Reserved (2): 0
DX10: Format:
DX10: Dimension:
DX10: Array size: 0
DX10: Flags (1):
DX10: Flags (2):
````

## Printing to CSV

````
$ ./textureinfo ~/Downloads/Textures/* --csv
````

````
$ cat dds.csv
File name,Pixel format (size),Pixel format (flags),Pixel format (fourcc),Pixel format (bit count),Pixel format (red mask),Pixel format (blue mask),Pixel format (green mask),Pixel format (alpha mask),Size,Flags,Width,Height,Depth,Number of MIP maps,Pitch or linear size,Capabilities (1),Capabilities (2),Capabilities (3),Capabilities (4),Reserved,Reserved (2),DX10: Format,DX10: Dimension,DX10: Array size,DX10: Flags (1),DX10: Flags (2),
/Users/joe/Downloads/Textures/GnomeWood.dds,32,e_fourCC,PTC4,0,False,False,False,False,124,e_Capabilities|e_height|e_width|e_pixelFormat|e_mipMapCount|e_linearSize,1024,1024,1,11,699136,e_complex|e_texture|e_mipMaps,,0,0,0:0:0:0:0:0:0:0:0:0:0,0,,,0,,,
````

````
$ cat ktx.csv
File name,glFormat,glInternalFormat,glBaseInternalFormat,glType,glType size,Width,Height,Depth,Array size,Number of faces,Number of MIP maps,Bytes of key value data,Endianness,
/Users/joe/Downloads/Textures/GnomeWood.ktx,0 (compressed format),COMPRESSED_RGBA_PVRTC_4BPPV1_IMG,GL_RGBA,0 (compressed format),1,1024,1024,0,0,1,11,32,67305985,
/Users/joe/Downloads/Textures/GnomeWoodRGBA5551.ktx,GL_RGBA,GL_RGB5_A1,GL_RGBA,GL_UNSIGNED_SHORT_5_5_5_1,2,1024,1024,0,0,1,11,32,67305985,
````

````
$ cat pvrV3.csv
File name,Compressed format,Channel name [0],Channel name [1],Channel name [2],Channel name [3],Bits per-pixel [0],Bits per-pixel [1],Bits per-pixel [2],Bits per-pixel [3],Color space,Channel type,Width,Height,Depth,Array size,Number of faces,Number of MIP maps,Flags,Meta data size,
/Users/joe/Downloads/Textures/GnomeWood.pvr,PVRTCI_4bpp_RGBA,-,-,-,-,-,-,-,-,lRGB,UnsignedByteNorm,1024,1024,1,1,1,11,0,15,
/Users/joe/Downloads/Textures/arial_36.pvr,-,l,a,-,-,8,8,-,-,lRGB,UnsignedByteNorm,256,512,1,1,1,10,0,4035,
````

````
$ cat pvrLegacy.csv
File name,Format,Number of bits,Width,Height,Array size,Number of MIP maps,Red mask?,Green mask?,Blue mask?,Alpha mask?,Magic number,Flags,Data size,
/Users/joe/Downloads/Textures/GnomeWood_legacy.pvr,GL_PVRTC4,4,1024,1024,1,11,False,False,False,True,559044176,MIP_MAP|HAS_ALPHA|VERTICAL_FLIP,699136,
````
