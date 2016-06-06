#include <string>
#include <vector>
#include <map>
#include <ostream>

#include <cstring>
#include <assert.h>
#include <string.h>

//*-------------------------------
// Macros
//-------------------------------*/
#define STRING_ENUM_PAIR(namespace__,enum__) {namespace__::enum__,#enum__}
#define HEADER_PRE_LOAD(file__) \
  if(!file__) return false; \
  file__.seekg(std::ios_base::beg);
#define HEADER_POST_LOAD(file__) \
  file__.close(); \
  return true;

#define ENUM_DEF(enum__, value__) enum__ value__,
#define ENUM_STRING_PAIR(enum__) {enum__,#enum__},
#define ENUM_APPEND_TO_STRING(enum__) \
  if(flags&enum__) output.append(flag_names.find(enum__)->second + "|");
#define ENUM_STRING_LOOKUP(vector__,enum__) \
  vector__.find(enum__)->second == ""?std::to_string(enum__)+" (no string found)":vector__.find(enum__)->second

//*-------------------------------
// Constants
//-------------------------------*/
static const std::string c_empty_string = "-";

//*-------------------------------
// Interfaces
//-------------------------------*/
class IHeader {
public:
  virtual bool LoadHeader(std::ifstream& file, std::string& error_string) = 0;
  virtual std::string ToString() = 0;
  virtual std::string ToCsvString() = 0;
};

//*-------------------------------
// Classes
//-------------------------------*/
namespace PvrLegacyInfo {
  enum PixelFormat {
    #define X(a,b) ENUM_DEF(a,b)
    #include "defs/pvrLegacy/pixel_format.def"
    #undef X
  };
  std::multimap<unsigned int,std::string> pixel_format_names {
    #define X(a,b) ENUM_STRING_PAIR(a)
    #include "defs/pvrLegacy/pixel_format.def"
    #undef X
  };
  enum API {
    #define X(a,b) ENUM_DEF(a,b)
    #include "defs/pvrLegacy/api.def"
    #undef X
  };
  std::multimap<unsigned int,std::string> api_names {
    #define X(a,b) ENUM_STRING_PAIR(a)
    #include "defs/pvrLegacy/api.def"
    #undef X
  };
  enum Flags {
    #define X(a,b) ENUM_DEF(a,b)
    #include "defs/pvrLegacy/flags.def"
    #undef X
  };
  std::multimap<unsigned int,std::string> flag_names {
    #define X(a,b) ENUM_STRING_PAIR(a)
    #include "defs/pvrLegacy/flags.def"
    #undef X
  };
  std::string PrintFlagNames(unsigned int flags) {
    std::string output;
    #define X(a,b) ENUM_APPEND_TO_STRING(a)
    #include "defs/pvrLegacy/flags.def"
    #undef X
    return output;
  };
  const std::uint32_t kPixelTypeMask = 0xff;
  // The old PVR header identifier is the characters 'PVR!', V2 only. Usually ignored...
  const std::uint32_t kIdentifierV2 = 0x21525650;
  const std::uint32_t kHeaderSizeV1 = 44;
  const std::uint32_t kHeaderSizeV2 = 52;
  
  std::vector<std::string> column_names {
  "Format",
  "Number of bits",
  "Width",
  "Height",
  "Array size",
  "Number of MIP maps",
  "Red mask?",
  "Green mask?",
  "Blue mask?",
  "Alpha mask?",
  "Magic number",
  "Flags",
  "Data size"
  };
}
// PVR Version 1 & 2
class PvrLegacyHeader: public IHeader {
private:
  std::vector<std::string> ToStringArray() {
      std::vector<std::string> output;
      const auto& impl_v1(this->impl_v1_);
      const auto& impl_v2(this->impl_v2_);
      const std::uint32_t pixel_format(impl_v1.pixel_format_flags&PvrLegacyInfo::kPixelTypeMask);
      const std::string pixel_format_string(PvrLegacyInfo::pixel_format_names.find(pixel_format)->second);
      output.push_back(pixel_format_string);
      output.push_back(std::to_string(impl_v1.bit_count));
      output.push_back(std::to_string(impl_v1.width));
      output.push_back(std::to_string(impl_v1.height));
      const std::string num_surfaces_string(
        impl_v2.num_surfaces == 0?std::string("-"):std::to_string(impl_v2.num_surfaces));
      output.push_back(num_surfaces_string);
      output.push_back(std::to_string(impl_v1.mip_map_count+1));
      output.push_back(impl_v1.red_mask==0?"False":"True");
      output.push_back(impl_v1.green_mask==0?"False":"True");
      output.push_back(impl_v1.blue_mask==0?"False":"True");
      output.push_back(impl_v1.alpha_mask==0?"False":"True");
      const std::string magic_number_string(
        impl_v2.magic_number == 0?std::string("-"):std::to_string(impl_v2.magic_number));
      output.push_back(magic_number_string);
      output.push_back(PvrLegacyInfo::PrintFlagNames(impl_v1.pixel_format_flags));
      output.push_back(std::to_string(impl_v1.data_size));
      return output;
    }
public:
  virtual bool LoadHeader(std::ifstream& file, std::string& error_string) {
    HEADER_PRE_LOAD(file)
    std::uint32_t pvr_version;
    file.read(reinterpret_cast<char*>(&pvr_version), sizeof pvr_version);
    if(pvr_version != PvrLegacyInfo::kHeaderSizeV1 && pvr_version != PvrLegacyInfo::kHeaderSizeV2) {
      error_string = "Not a valid legacy (v1 or v2) PVR file";
      return false;
    }
    file.read(reinterpret_cast<char*>(&this->impl_v1_), sizeof(this->impl_v1_));
    if(pvr_version == PvrLegacyInfo::kHeaderSizeV2) {
      file.read(reinterpret_cast<char*>(&this->impl_v2_), sizeof(this->impl_v2_));
    }
    HEADER_POST_LOAD(file)
  }
  virtual std::string ToString() {
    std::string output("");
    const auto& variable_strings(this->ToStringArray());
    for(unsigned int index=0; index<PvrLegacyInfo::column_names.size();++index)
      output.append(PvrLegacyInfo::column_names.at(index) + ": " + variable_strings.at(index) + "\n");
    return output;
  }
  virtual std::string ToCsvString() {
    std::string output("");
    const auto& variable_strings(this->ToStringArray());
    for(const auto& variable_string:variable_strings)
      output.append(variable_string + ',');
    return output;
  }
private:
  #pragma pack(4)
  struct ImplV1 {
    std::uint32_t height;             //!< height of surface to be created
    std::uint32_t width;              //!< width of input surface
    std::uint32_t mip_map_count;      //!< number of mip-map levels requested
    std::uint32_t pixel_format_flags; //!< pixel format flags
    std::uint32_t data_size;          //!< Size of the compress data
    std::uint32_t bit_count;          //!< number of bits per pixel
    std::uint32_t red_mask;           //!< mask for red bit
    std::uint32_t green_mask;         //!< mask for green bits
    std::uint32_t blue_mask;          //!< mask for blue bits
    std::uint32_t alpha_mask;         //!< mask for alpha channel
    ImplV1() {memset(this, 0, sizeof(ImplV1));}
  }impl_v1_;
  #pragma pack(4)
  struct ImplV2 {
    std::uint32_t magic_number; // magic number identifying pvr file
    std::uint32_t num_surfaces; // Number of surfaces present in the pvr
    ImplV2() {memset(this, 0, sizeof(ImplV2));}
  }impl_v2_;
};

namespace PvrV3Info {
  enum CompressedFormat {
    #define X(a,b) ENUM_DEF(a,b)
    #include "defs/pvrV3/compressed_formats.def"
    #undef X
  };
  std::multimap<unsigned int,std::string> compressed_format_names {
    #define X(a,b) ENUM_STRING_PAIR(a)
    #include "defs/pvrV3/compressed_formats.def"
    #undef X
  };
  enum {
    PVRv3 = 0x03525650, //!< PVR format v3 identifier
    PVRv3Reversed = 0x50565203, //!< PVR format v3 reversed identifier

    // PVR header flags.
    CompressedFlag = (1 << 0), //!< Compressed format flag
    PremultipliedFlag = (1 << 1), //!< Premultiplied flag
    SizeOfHeader = 52
  };
  enum ColorSpace {
    #define X(a,b) ENUM_DEF(a,b)
    #include "defs/pvrV3/colour_spaces.def"
    #undef X
  };
  std::multimap<unsigned int,std::string> color_space_names {
    #define X(a,b) ENUM_STRING_PAIR(a)
    #include "defs/pvrV3/colour_spaces.def"
    #undef X
  };
  enum VariableType {
    #define X(a,b) ENUM_DEF(a,b)
    #include "defs/pvrV3/variable_types.def"
    #undef X
  };
  std::multimap<unsigned int,std::string> variable_type_names {
    #define X(a,b) ENUM_STRING_PAIR(a)
    #include "defs/pvrV3/variable_types.def"
    #undef X
  };
  
  std::vector<std::string> column_names {
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
    "Width",
    "Height",
    "Depth",
    "Array size",
    "Number of faces",
    "Number of MIP maps",
    "Flags",
    "Meta data size"
  };
};
class PvrV3Header: public IHeader {
private:
  std::vector<std::string> ToStringArray() {
    std::vector<std::string> output;
    const auto& impl(this->impl_);
    // If the last 16-bits are empty, it's a compressed format
    if(impl.pixel_format.u32[1] == 0) {
        output.push_back(PvrV3Info::compressed_format_names.find(impl.pixel_format.u8[0])->second);
      for(unsigned int index = 0; index < 8; index++)
        output.push_back(c_empty_string);
      }
    else {
      output.push_back(c_empty_string);
      // Channel names
      for(unsigned int index = 0; index < 4; index++) {
        // Convert unsigned int pulled from the header into a literal character
        char value_char(impl.pixel_format.u8[index]);
        if(!value_char) value_char = c_empty_string[0];
        const std::string value((char*)&value_char,1);
        output.push_back(value);
      }
      // Bits per-pixel
      for(unsigned int index = 4; index < 8; index++)
        if(impl.pixel_format.u8[index]==0)
          output.push_back(c_empty_string);
        else
          output.push_back(std::to_string(impl.pixel_format.u8[index]));
    }
    output.push_back(PvrV3Info::color_space_names.find(impl.color_space)->second);
    output.push_back(PvrV3Info::variable_type_names.find(impl.channel_type)->second);
    output.push_back(std::to_string(impl.width));
    output.push_back(std::to_string(impl.height));
    output.push_back(std::to_string(impl.depth));
    output.push_back(std::to_string(impl.num_surfaces));
    output.push_back(std::to_string(impl.num_faces));
    output.push_back(std::to_string(impl.mip_map_count));
    output.push_back(std::to_string(impl.flags));
    output.push_back(std::to_string(impl.meta_data_size));
    return output;
  }
public:
  virtual bool LoadHeader(std::ifstream& file, std::string& error_string) {
    HEADER_PRE_LOAD(file)
    std::uint32_t pvr_version;
    file.read(reinterpret_cast<char*>(&pvr_version), sizeof pvr_version);
    if(pvr_version == PvrV3Info::PVRv3) {}
    else if(pvr_version == PvrV3Info::PVRv3Reversed) {
      error_string = "Endianess does not matches host";
      return false;
    }
    else {
      error_string = "Not a valid PVR v3 file";
      return false;
    }
    file.read(reinterpret_cast<char*>(&this->impl_), sizeof(this->impl_));
    HEADER_POST_LOAD(file)
  }
  virtual std::string ToString() {
    std::string output("");
    const auto& variable_strings(this->ToStringArray());
    for(unsigned int index=0; index<PvrV3Info::column_names.size();++index)
      output.append(PvrV3Info::column_names.at(index) + ": " + variable_strings.at(index) + "\n");
    return output;
  }
  virtual std::string ToCsvString() {
    std::string output("");
    const auto& variable_strings(this->ToStringArray());
    for(const auto& variable_string:variable_strings)
      output.append(variable_string + ',');
    return output;
  }
private:
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

namespace KTXInfo {
  static const std::uint8_t kIdentifier[] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };
  // Reference number to verify endianness of a file
  static const std::uint32_t kEndianReference = 0x04030201;
  // Expected size of a header in file
  static const std::uint32_t kExpectedHeaderSize = 64;
  // Identifier for the orientation meta data
  static const std::uint8_t kOrientationMetaDataKey[] = "KTXOrientation";
  
  enum GLEnum {
    #define X(a,b) ENUM_DEF(a,b)
    #include "defs/ktx/gl.def"
    #undef X
  };
  std::multimap<unsigned int,std::string> gl_enum_names {
    #define X(a,b) ENUM_STRING_PAIR(a)
    #include "defs/ktx/gl.def"
    #undef X
  };
  
  std::vector<std::string> column_names {
    "glFormat",
    "glInternalFormat",
    "glBaseInternalFormat",
    "glType",
    "glType size",
    "Width",
    "Height",
    "Depth",
    "Array size",
    "Number of faces",
    "Number of MIP maps",
    "Bytes of key value data",
    "Endianness"
  };
};
class KTXHeader: public IHeader {
public:
  virtual bool LoadHeader(std::ifstream& file, std::string& error_string){
    HEADER_PRE_LOAD(file)
    std::uint8_t identifier[12];
    file.read(reinterpret_cast<char*>(&identifier), sizeof identifier);
    if(memcmp(identifier,KTXInfo::kIdentifier,sizeof(KTXInfo::kIdentifier)) !=0) {
      error_string = "Header does not contain a valid KTX identifier";
      return false;
    }
    file.read(reinterpret_cast<char*>(&this->impl_), sizeof(this->impl_));
    HEADER_POST_LOAD(file)
  }
  std::vector<std::string> ToStringArray() {
    std::vector<std::string> output;
    const auto& impl(this->impl_);
    std::string gl_format_string("0 (compressed format)");
    if(impl.gl_format != 0) gl_format_string = ENUM_STRING_LOOKUP(KTXInfo::gl_enum_names,impl.gl_format);
    output.push_back(gl_format_string);
    std::string gl_internal_format_string(ENUM_STRING_LOOKUP(KTXInfo::gl_enum_names,impl.gl_internal_format));
    output.push_back(gl_internal_format_string);
    std::string gl_base_internal_format_string(ENUM_STRING_LOOKUP(KTXInfo::gl_enum_names,impl.gl_base_internal_format));
    output.push_back(gl_base_internal_format_string);
    std::string gl_type_string("0 (compressed format)");
    if(impl.gl_type != 0) gl_type_string = ENUM_STRING_LOOKUP(KTXInfo::gl_enum_names,impl.gl_type);
    output.push_back(gl_type_string);
    output.push_back(std::to_string(impl.gl_type_size));
    output.push_back(std::to_string(impl.pixel_width));
    output.push_back(std::to_string(impl.pixel_height));
    output.push_back(std::to_string(impl.pixel_depth));
    output.push_back(std::to_string(impl.number_of_array_elements));
    output.push_back(std::to_string(impl.number_of_faces));
    output.push_back(std::to_string(impl.number_of_mipmap_levels));
    output.push_back(std::to_string(impl.bytes_of_key_value_data));
    output.push_back(std::to_string(impl.endianness));
    return output;
  }
  virtual std::string ToString() {
    std::string output("");
    const auto& variable_strings(this->ToStringArray());
    for(unsigned int index=0; index<KTXInfo::column_names.size();++index)
      output.append(KTXInfo::column_names.at(index) + ": " + variable_strings.at(index) + "\n");
    return output;
  }
  virtual std::string ToCsvString() {
    std::string output("");
    const auto& variable_strings(this->ToStringArray());
    for(const auto& variable_string:variable_strings)
      output.append(variable_string + ',');
    return output;
  }
private:
  #pragma pack(4)
  struct Impl {
    std::uint32_t endianness;
    std::uint32_t gl_type;
    std::uint32_t gl_type_size;
    std::uint32_t gl_format;
    std::uint32_t gl_internal_format;
    std::uint32_t gl_base_internal_format;
    std::uint32_t pixel_width;
    std::uint32_t pixel_height;
    std::uint32_t pixel_depth;
    std::uint32_t number_of_array_elements;
    std::uint32_t number_of_faces;
    std::uint32_t number_of_mipmap_levels;
    std::uint32_t bytes_of_key_value_data;
    Impl() {memset(this, 0, sizeof(Impl));}
  }impl_;
};
