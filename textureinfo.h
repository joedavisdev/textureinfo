#include <string>
#include <vector>
#include <map>

#include <cstring>
#include <assert.h>

//*-------------------------------
// Global functions
//-------------------------------*/
static const void AppendCsvifiedRow(std::string& property, const std::string& value) {
  property.append(value + ',');
}

//*-------------------------------
// Macros
//-------------------------------*/
#define APPEND_FORMATTED_ROW_RAW(string__,value_name__,value__) \
  string__.append(value_name__ + ": " + value__ + "\n");
#define APPEND_FORMATTED_ROW(string__,value_name__,value__) \
  APPEND_FORMATTED_ROW_RAW(string__,value_name__,std::to_string(value__))
#define STRING_ENUM_PAIR(namespace__,enum__) {namespace__::enum__,#enum__}

//*-------------------------------
// Constants
//-------------------------------*/
static const std::string c_empty_string = "-";

//*-------------------------------
// Interfaces
//-------------------------------*/
class IHeader {
  virtual bool LoadHeader(std::ifstream& file, std::string& error_string) = 0;
  virtual std::string ToString() = 0;
  virtual std::string ToCsvString() = 0;
};

//*-------------------------------
// Classes
//-------------------------------*/
namespace PVRCompressedPixelFormat {
enum Enum
{
  PVRTCI_2bpp_RGB,
  PVRTCI_2bpp_RGBA,
  PVRTCI_4bpp_RGB,
  PVRTCI_4bpp_RGBA,
  PVRTCII_2bpp,
  PVRTCII_4bpp,
  ETC1,
  DXT1,
  DXT2,
  DXT3,
  DXT4,
  DXT5,

  //These formats are identical to some DXT formats.
  BC1 = DXT1,
  BC2 = DXT3,
  BC3 = DXT5,

  //These are currently unsupported:
  BC4,
  BC5,
  BC6,
  BC7,

  //These are supported
  UYVY,
  YUY2,
  BW1bpp,
  SharedExponentR9G9B9E5,
  RGBG8888,
  GRGB8888,
  ETC2_RGB,
  ETC2_RGBA,
  ETC2_RGB_A1,
  EAC_R11,
  EAC_RG11,

  //Invalid value
  NumCompressedPFs
};
};
std::multimap<unsigned int,std::string> PVRCompressedPixelFormatNames {
STRING_ENUM_PAIR(PVRCompressedPixelFormat,PVRTCI_2bpp_RGB),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,PVRTCI_2bpp_RGBA),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,PVRTCI_4bpp_RGB),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,PVRTCI_4bpp_RGBA),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,PVRTCII_2bpp),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,PVRTCII_4bpp),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,ETC1),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,DXT1),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,DXT2),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,DXT3),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,DXT4),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,DXT5),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,BC1),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,BC2),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,BC3),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,BC4),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,BC5),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,BC6),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,BC7),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,UYVY),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,YUY2),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,BW1bpp),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,SharedExponentR9G9B9E5),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,RGBG8888),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,GRGB8888),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,ETC2_RGB),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,ETC2_RGBA),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,ETC2_RGB_A1),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,EAC_R11),
STRING_ENUM_PAIR(PVRCompressedPixelFormat,EAC_RG11)
};
std::vector<std::string> PvrV3HeaderVarNames {
  "Flags",
  "Compressed format",
  "Channel name [0]",
  "Channel name [1]",
  "Channel name [2]",
  "Channel name [3]",
  "Bits per-pixel [0]",
  "Bits per-pixel [1]",
  "Bits per-pixel [2]",
  "Bits per-pixel [3]",
  "Color space",
  "Channel type",
  "Height",
  "Width",
  "Depth",
  "Number of surfaces",
  "Number of faces",
  "Number of MIP maps",
  "Meta data size"
};
class PvrV3Header: IHeader {
public:
  virtual bool LoadHeader(std::ifstream& file, std::string& error_string) {
    if(!file) return false;
    std::uint32_t pvr_version;
    file.read(reinterpret_cast<char*>(&pvr_version), sizeof pvr_version);
    if(pvr_version == PvrV3Header::PVRv3) {}
    else if(pvr_version == PvrV3Header::PVRv3Reversed) {
      error_string = "Endianess does not matches host";
      return false;
    }
    else {
      error_string = "Not a valid PVR v3 file";
      return false;
    }
    file.read(reinterpret_cast<char*>(&this->impl_), sizeof(this->impl_));
    file.close();
    return true;
  };
  virtual std::string ToString() {
    std::string out_string("");
    const auto& impl(this->impl_);
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[0],impl.flags)
    if(impl.pixel_format.u32[1] == 0) {
      APPEND_FORMATTED_ROW_RAW(out_string,PvrV3HeaderVarNames[1],
        PVRCompressedPixelFormatNames.find(impl.pixel_format.u8[0])->second)
      for(unsigned int index = 0; index < 8; index++)
        APPEND_FORMATTED_ROW_RAW(out_string,PvrV3HeaderVarNames[2+index],c_empty_string)
      }
    else {
      APPEND_FORMATTED_ROW_RAW(out_string,PvrV3HeaderVarNames[1],c_empty_string)
      // Channel names
      for(unsigned int index = 0; index < 4; index++) {
        // Convert unsigned int pulled from the header into a literal character
        char value(c_empty_string[0]);
        std::sscanf((char*)&impl.pixel_format.u8[index],"%c",&value);
        APPEND_FORMATTED_ROW_RAW(out_string,PvrV3HeaderVarNames[2+index],value)
      }
      // Bits per-pixel
      for(unsigned int index = 4; index < 8; index++)
        if(impl.pixel_format.u8[index]==0)
          APPEND_FORMATTED_ROW_RAW(out_string,PvrV3HeaderVarNames[2+index],c_empty_string)
        else
          APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[2+index],impl.pixel_format.u8[index])
    }
    APPEND_FORMATTED_ROW_RAW(out_string,PvrV3HeaderVarNames[10],
      ColorSpaceNames.find(impl.color_space)->second)
    APPEND_FORMATTED_ROW_RAW(out_string,PvrV3HeaderVarNames[11],
      VariableTypeNames.find(impl.channel_type)->second)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[12],impl.height)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[13],impl.width)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[14],impl.depth)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[15],impl.num_surfaces)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[16],impl.num_faces)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[17],impl.mip_map_count)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[18],impl.meta_data_size)
    return out_string;
  };
  virtual std::string ToCsvString() {
    std::string csv_string("");
    const auto& impl(this->impl_);
    AppendCsvifiedRow(csv_string,std::to_string(impl.flags));
    if(impl.pixel_format.u32[1] == 0) {
      AppendCsvifiedRow(csv_string,
        PVRCompressedPixelFormatNames.find(impl.pixel_format.u8[0])->second);
      for(unsigned int index = 0; index < 8; index++)
        AppendCsvifiedRow(csv_string,c_empty_string);
      }
    else {
      AppendCsvifiedRow(csv_string,c_empty_string);
      // Channel names
      for(unsigned int index = 0; index < 4; index++) {
        // Convert unsigned int pulled from the header into a literal character
        char value(c_empty_string[0]);
        std::sscanf((char*)&impl.pixel_format.u8[index],"%c",&value);
        AppendCsvifiedRow(csv_string,std::string(&value));
      }
      // Bits per-pixel
      for(unsigned int index = 4; index < 8; index++)
        if(impl.pixel_format.u8[index]==0)
          AppendCsvifiedRow(csv_string,c_empty_string);
        else
          AppendCsvifiedRow(csv_string,std::to_string(impl.pixel_format.u8[index]));
    }
    AppendCsvifiedRow(csv_string,std::to_string(impl.color_space));
    AppendCsvifiedRow(csv_string,std::to_string(impl.channel_type));
    AppendCsvifiedRow(csv_string,std::to_string(impl.height));
    AppendCsvifiedRow(csv_string,std::to_string(impl.width));
    AppendCsvifiedRow(csv_string,std::to_string(impl.depth));
    AppendCsvifiedRow(csv_string,std::to_string(impl.num_surfaces));
    AppendCsvifiedRow(csv_string,std::to_string(impl.num_faces));
    AppendCsvifiedRow(csv_string,std::to_string(impl.mip_map_count));
    AppendCsvifiedRow(csv_string,std::to_string(impl.meta_data_size));
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
    std::multimap<unsigned int,std::string> ColorSpaceNames {
      STRING_ENUM_PAIR(PvrV3Header,lRGB),
      STRING_ENUM_PAIR(PvrV3Header,sRGB)
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
    std::multimap<unsigned int,std::string> VariableTypeNames {
      STRING_ENUM_PAIR(PvrV3Header,UnsignedByteNorm),
      STRING_ENUM_PAIR(PvrV3Header,SignedByteNorm),
      STRING_ENUM_PAIR(PvrV3Header,UnsignedByte),
      STRING_ENUM_PAIR(PvrV3Header,SignedByte),
      STRING_ENUM_PAIR(PvrV3Header,UnsignedShortNorm),
      STRING_ENUM_PAIR(PvrV3Header,SignedShortNorm),
      STRING_ENUM_PAIR(PvrV3Header,UnsignedShort),
      STRING_ENUM_PAIR(PvrV3Header,SignedShort),
      STRING_ENUM_PAIR(PvrV3Header,UnsignedIntegerNorm),
      STRING_ENUM_PAIR(PvrV3Header,SignedIntegerNorm),
      STRING_ENUM_PAIR(PvrV3Header,UnsignedInteger),
      STRING_ENUM_PAIR(PvrV3Header,SignedInteger),
      STRING_ENUM_PAIR(PvrV3Header,SignedFloat),
      STRING_ENUM_PAIR(PvrV3Header,Float),
      STRING_ENUM_PAIR(PvrV3Header,UnsignedFloat)
    };
    union PixelFormatUnion {
      std::uint64_t u64;
      std::uint32_t u32[2];
      std::uint8_t u8[8];
    };
    #pragma pack(4)
    struct Impl {
      std::uint32_t flags;          //!< Various format flags.
      PixelFormatUnion pixel_format;//!< The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
      std::uint32_t color_space;
      std::uint32_t channel_type;
      std::uint32_t height;         //!< Height of the texture.
      std::uint32_t width;          //!< Width of the texture.
      std::uint32_t depth;          //!< Depth of the texture. (Z-slices)
      std::uint32_t num_surfaces;   //!< Number of members in a Texture Array.
      std::uint32_t num_faces;      //!< Number of faces in a Cube Map. May be a value other than 6.
      std::uint32_t mip_map_count;  //!< Number of MIP Maps in the texture - NB: Includes top level.
      std::uint32_t meta_data_size; //!< Size of the accompanying meta data.
      Impl() {memset(this, 0, sizeof(Impl));}
    }impl_;
};
