#include <string>
#include <assert.h>

#include "PixelFormat.h"

#define APPEND_CSVIFIED_VALUE(string__,value__) string__.append(std::to_string(value__) + ',');
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
    APPEND_CSVIFIED_VALUE(csv_string,(int)this->pixel_format.isIrregularFormat())
    APPEND_CSVIFIED_VALUE(csv_string,this->pixel_format.getPixelTypeId())
    for(unsigned int index = 0; index < 8; index++)
        APPEND_CSVIFIED_VALUE(csv_string,this->pixel_format.getPixelTypeChar()[index])
    APPEND_CSVIFIED_VALUE(csv_string,this->color_space)
    APPEND_CSVIFIED_VALUE(csv_string,this->channel_type)
    APPEND_CSVIFIED_VALUE(csv_string,this->height)
    APPEND_CSVIFIED_VALUE(csv_string,this->width)
    APPEND_CSVIFIED_VALUE(csv_string,this->depth)
    APPEND_CSVIFIED_VALUE(csv_string,this->num_surfaces)
    APPEND_CSVIFIED_VALUE(csv_string,this->num_faces)
    APPEND_CSVIFIED_VALUE(csv_string,this->mip_map_count)
    APPEND_CSVIFIED_VALUE(csv_string,this->meta_data_size)
    return csv_string;
  };
private:
    enum {
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
    unsigned int flags;           //!< Various format flags.
    pvr::PixelFormat pixel_format;//!< The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
    ColorSpaceEnum color_space;
    VariableTypeEnum channel_type;
    unsigned int height;          //!< Height of the texture.
    unsigned int width;           //!< Width of the texture.
    unsigned int depth;           //!< Depth of the texture. (Z-slices)
    unsigned int num_surfaces;    //!< Number of members in a Texture Array.
    unsigned int num_faces;       //!< Number of faces in a Cube Map. Maybe be a value other than 6.
    unsigned int mip_map_count;   //!< Number of MIP Maps in the texture - NB: Includes top level.
    unsigned int meta_data_size;  //!< Size of the accompanying meta data.
};