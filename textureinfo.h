#include <string>
#include <assert.h>

#define APPEND_CSVIFIED_VALUE(string__,value__) string__.append(std::to_string(value__) + ',');
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
class BaseHeader {
protected:
  std::string file_name;
};
class PvrV3Header: IHeader, BaseHeader {
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
    file.read(reinterpret_cast<char*>(&this->impl_),
      sizeof(this->impl_));
    file.close();
    return true;
  };
  virtual std::string ToString() {
    assert(0);
    return "";
  };
  virtual std::string ToCsvString() {
    std::string csv_string("");
    const auto& impl(this->impl_);
    csv_string.append(this->file_name);csv_string.append(",");
    APPEND_CSVIFIED_VALUE(csv_string,impl.flags)
    for(unsigned int index = 0; index < 8; index++)
//        APPEND_CSVIFIED_VALUE(csv_string,impl.pixel_format.chars[index]) //TODO: FIX!
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
    union PixelFormatImpl {
      std::uint64_t data;
      std::uint8_t chars[8];
    };
	#pragma pack(4)
    struct Impl {
      std::uint32_t flags;          //!< Various format flags.
      std::uint64_t pixel_format; //!< The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
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