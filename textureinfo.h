#include <string>
#include <vector>
#include <map>
#include <assert.h>

//*-------------------------------
// Macros
//-------------------------------*/
#define APPEND_FORMATTED_ROW_RAW(string__,value_name__,value__) \
  string__.append(value_name__ + ": " + value__ + "\n");
#define APPEND_FORMATTED_ROW(string__,value_name__,value__) \
  APPEND_FORMATTED_ROW_RAW(string__,value_name__,std::to_string(value__))
#define APPEND_CSVIFIED_VALUE(string__,value__) \
  string__.append(std::to_string(value__) + ',');

//*-------------------------------
// Interfaces
//-------------------------------*/
class IHeader {
  virtual bool LoadHeader(std::ifstream& file, std::string& file_name) = 0;
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
#define PVR_COMPRESSED_FORMAT_NAME(enum__) {PVRCompressedPixelFormat::enum__,#enum__}
std::multimap<unsigned int, std::string> PVRCompressedPixelFormatNames {
PVR_COMPRESSED_FORMAT_NAME(PVRTCI_2bpp_RGB),
PVR_COMPRESSED_FORMAT_NAME(PVRTCI_2bpp_RGBA),
PVR_COMPRESSED_FORMAT_NAME(PVRTCI_4bpp_RGB),
PVR_COMPRESSED_FORMAT_NAME(PVRTCI_4bpp_RGBA),
PVR_COMPRESSED_FORMAT_NAME(PVRTCII_2bpp),
PVR_COMPRESSED_FORMAT_NAME(PVRTCII_4bpp),
PVR_COMPRESSED_FORMAT_NAME(ETC1),
PVR_COMPRESSED_FORMAT_NAME(DXT1),
PVR_COMPRESSED_FORMAT_NAME(DXT2),
PVR_COMPRESSED_FORMAT_NAME(DXT3),
PVR_COMPRESSED_FORMAT_NAME(DXT4),
PVR_COMPRESSED_FORMAT_NAME(DXT5),
PVR_COMPRESSED_FORMAT_NAME(BC1),
PVR_COMPRESSED_FORMAT_NAME(BC2),
PVR_COMPRESSED_FORMAT_NAME(BC3),
PVR_COMPRESSED_FORMAT_NAME(BC4),
PVR_COMPRESSED_FORMAT_NAME(BC5),
PVR_COMPRESSED_FORMAT_NAME(BC6),
PVR_COMPRESSED_FORMAT_NAME(BC7),
PVR_COMPRESSED_FORMAT_NAME(UYVY),
PVR_COMPRESSED_FORMAT_NAME(YUY2),
PVR_COMPRESSED_FORMAT_NAME(BW1bpp),
PVR_COMPRESSED_FORMAT_NAME(SharedExponentR9G9B9E5),
PVR_COMPRESSED_FORMAT_NAME(RGBG8888),
PVR_COMPRESSED_FORMAT_NAME(GRGB8888),
PVR_COMPRESSED_FORMAT_NAME(ETC2_RGB),
PVR_COMPRESSED_FORMAT_NAME(ETC2_RGBA),
PVR_COMPRESSED_FORMAT_NAME(ETC2_RGB_A1),
PVR_COMPRESSED_FORMAT_NAME(EAC_R11),
PVR_COMPRESSED_FORMAT_NAME(EAC_RG11)
};
std::vector<std::string> PvrV3HeaderVarNames {
  "Flags",
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
  virtual bool LoadHeader(std::ifstream& file, std::string& file_name) {
    if(!file) return false;
    std::uint32_t pvr_version;
    file.read(reinterpret_cast<char*>(&pvr_version), sizeof pvr_version);
    if(pvr_version == PvrV3Header::PVRv3) {}
    else if(pvr_version == PvrV3Header::PVRv3Reversed) {
      printf("DEBUG: %s endianess does not matches host\n",file_name.c_str());
      assert(0); // TODO: Handle endianess
    }
    else {
      printf("DEBUG: %s is not a valid PVR v3 file\n",file_name.c_str());
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
    if(impl.pixel_format.u32[1] == 0)
      APPEND_FORMATTED_ROW_RAW(out_string,std::string("Compressed format"),
        PVRCompressedPixelFormatNames.find(impl.pixel_format.u8[0])->second)
    else {
      // Channel names
      for(unsigned int index = 0; index < 4; index++) {
        // Convert unsigned int pulled from the header into a literal character
        char value('-');
        std::sscanf((char*)&impl.pixel_format.u8[index],"%c",&value);
        APPEND_FORMATTED_ROW_RAW(out_string,PvrV3HeaderVarNames[1+index],value)
      }
      // Bits per-pixel
      for(unsigned int index = 4; index < 8; index++)
        if(impl.pixel_format.u8[index]==0)
          APPEND_FORMATTED_ROW_RAW(out_string,PvrV3HeaderVarNames[1+index],'-')
        else
          APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[1+index],impl.pixel_format.u8[index])
    }
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[9],impl.color_space)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[10],impl.channel_type)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[11],impl.height)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[12],impl.width)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[13],impl.depth)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[14],impl.num_surfaces)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[15],impl.num_faces)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[16],impl.mip_map_count)
    APPEND_FORMATTED_ROW(out_string,PvrV3HeaderVarNames[17],impl.meta_data_size)
    return out_string;
  };
  virtual std::string ToCsvString() {
    std::string csv_string("");
    const auto& impl(this->impl_);
    APPEND_CSVIFIED_VALUE(csv_string,impl.flags)
    for(unsigned int index = 0; index < 8; index++)
        APPEND_CSVIFIED_VALUE(csv_string,impl.pixel_format.u8[index])
    APPEND_CSVIFIED_VALUE(csv_string,impl.color_space)
    APPEND_CSVIFIED_VALUE(csv_string,impl.channel_type)
    APPEND_CSVIFIED_VALUE(csv_string,impl.height)
    APPEND_CSVIFIED_VALUE(csv_string,impl.width)
    APPEND_CSVIFIED_VALUE(csv_string,impl.depth)
    APPEND_CSVIFIED_VALUE(csv_string,impl.num_surfaces)
    APPEND_CSVIFIED_VALUE(csv_string,impl.num_faces)
    APPEND_CSVIFIED_VALUE(csv_string,impl.mip_map_count)
    APPEND_CSVIFIED_VALUE(csv_string,impl.meta_data_size)
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