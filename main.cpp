#include <string>
#include <cstdio>
#include <assert.h>

#include "PixelFormat.h"

#define APPEND_CSVIFIED_VALUE(string__,value__) string__.append(std::to_string(value__) + ',');

//*-------------------------------
// Global functions
//-------------------------------*/
std::string GetFilenameExt(std::string filename) {
  std::string ext_name("");
  std::string::size_type index = filename.rfind('.');
  if(index != std::string::npos)
    ext_name = filename.substr(index+1);
  return ext_name;
}

//*-------------------------------
// Interfaces
//-------------------------------*/
class IPrint {
  virtual std::string ToString() = 0;
  virtual std::string ToCsvString() = 0;
};

//*-------------------------------
// Classes
//-------------------------------*/
class PvrTexHeader: IPrint {
public:
  std::string ToString() {
    assert(0);
    return "";
  };
  std::string ToCsvString() {
    std::string csv_string("");
    APPEND_CSVIFIED_VALUE(csv_string,this->flags)
    APPEND_CSVIFIED_VALUE(csv_string,(int)this->pixelFormat.isIrregularFormat())
    APPEND_CSVIFIED_VALUE(csv_string,this->pixelFormat.getPixelTypeId())
    for(unsigned int index = 0; index < 8; index++)
        APPEND_CSVIFIED_VALUE(csv_string,this->pixelFormat.getPixelTypeChar()[index])
    APPEND_CSVIFIED_VALUE(csv_string,this->colorSpace)
    APPEND_CSVIFIED_VALUE(csv_string,this->channelType)
    APPEND_CSVIFIED_VALUE(csv_string,this->height)
    APPEND_CSVIFIED_VALUE(csv_string,this->width)
    APPEND_CSVIFIED_VALUE(csv_string,this->depth)
    APPEND_CSVIFIED_VALUE(csv_string,this->numberOfSurfaces)
    APPEND_CSVIFIED_VALUE(csv_string,this->numberOfFaces)
    APPEND_CSVIFIED_VALUE(csv_string,this->mipMapCount)
    APPEND_CSVIFIED_VALUE(csv_string,this->metaDataSize)
    return csv_string;
  };
private:
    enum
    {
      PVRv3 = 0x03525650, //!< PVR format v3 identifier
      PVRv3Reversed = 0x50565203, //!< PVR format v3 reversed identifier

      // PVR header flags.
      CompressedFlag = (1 << 0), //!< Compressed format flag
      PremultipliedFlag = (1 << 1), //!< Premultiplied flag
      SizeOfHeader = 52
    };
    enum ColorSpaceEnum {
      lRGB,
      sRGB,
      NumSpaces
    };
    enum VariableTypeEnum {
      UnsignedByteNorm,
      SignedByteNorm,
      UnsignedByte,
      SignedByte,
      UnsignedShortNorm,
      SignedShortNorm,
      UnsignedShort,
      SignedShort,
      UnsignedIntegerNorm,
      SignedIntegerNorm,
      UnsignedInteger,
      SignedInteger,
      SignedFloat,
      Float = SignedFloat,
      UnsignedFloat,
      NumVarTypes
    };
    unsigned int flags;            //!< Various format flags.
    pvr::PixelFormat pixelFormat;      //!< The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
    ColorSpaceEnum colorSpace;
    VariableTypeEnum channelType;
    unsigned int height;           //!< Height of the texture.
    unsigned int width;            //!< Width of the texture.
    unsigned int depth;            //!< Depth of the texture. (Z-slices)
    unsigned int numberOfSurfaces; //!< Number of members in a Texture Array.
    unsigned int numberOfFaces;    //!< Number of faces in a Cube Map. Maybe be a value other than 6.
    unsigned int mipMapCount;      //!< Number of MIP Maps in the texture - NB: Includes top level.
    unsigned int metaDataSize;     //!< Size of the accompanying meta data.
};
//*-------------------------------
// main
//-------------------------------*/
int main (int argc, char *argv[]) {
  for(unsigned int index = 1; index < argc; index++) {
    std::string ext_name(GetFilenameExt(argv[index]));
    if(ext_name == "pvr")
      printf("DEBUG: File name: %s. ext name: %s\n",argv[index], ext_name.c_str());
  }
}