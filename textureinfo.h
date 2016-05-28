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
#define HEADER_PRE_LOAD(file__) \
  if(!file__) return false; \
  file__.seekg(std::ios_base::beg);
#define HEADER_POST_LOAD(file__) \
  file__.close(); \
  return true;

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
namespace PvrLegacyProps {
  enum PixelFormat {
    // MGL Formats
    MGL_ARGB_4444 = 0x00,
    MGL_ARGB_1555,
    MGL_RGB_565,
    MGL_RGB_555,
    MGL_RGB_888,
    MGL_ARGB_8888,
    MGL_ARGB_8332,
    MGL_I_8,
    MGL_AI_88,
    MGL_1_BPP,
    MGL_VY1UY0,
    MGL_Y1VY0U,
    MGL_PVRTC2,
    MGL_PVRTC4,
    // OpenGL Formats
    GL_RGBA_4444 = 0x10,
    GL_RGBA_5551,
    GL_RGBA_8888,
    GL_RGB_565,
    GL_RGB_555,
    GL_RGB_888,
    GL_I_8,
    GL_AI_88,
    GL_PVRTC2,
    GL_PVRTC4,
    GL_BGRA_8888,
    GL_A_8,
    GL_PVRTCII4,
    GL_PVRTCII2,
    // DirectX 9 and Earlier Formats
    D3D_DXT1 = 0x20,
    D3D_DXT2,
    D3D_DXT3,
    D3D_DXT4,
    D3D_DXT5,
    D3D_RGB_332,
    D3D_AL_44,
    D3D_LVU_655,
    D3D_XLVU_8888,
    D3D_QWVU_8888,
    D3D_ABGR_2101010,
    D3D_ARGB_2101010,
    D3D_AWVU_2101010,
    D3D_GR_1616,
    D3D_VU_1616,
    D3D_ABGR_16161616,
    D3D_R16F,
    D3D_GR_1616F,
    D3D_ABGR_16161616F,
    D3D_R32F,
    D3D_GR_3232F,
    D3D_ABGR_32323232F,
    // Ericsson Texture Compression formats
    e_etc_RGB_4BPP,
    //More DirectX 9 Formats
    D3D_A8 = 0x40,
    D3D_V8U8,
    D3D_L16,
    D3D_L8,
    D3D_AL_88,
    D3D_UYVY,
    D3D_YUY2,
    // DirectX 10+ Formats
    DXGI_R32G32B32A32_FLOAT = 0x50,
    DXGI_R32G32B32A32_UINT,
    DXGI_R32G32B32A32_SINT,
    DXGI_R32G32B32_FLOAT,
    DXGI_R32G32B32_UINT,
    DXGI_R32G32B32_SINT,
    DXGI_R16G16B16A16_FLOAT,
    DXGI_R16G16B16A16_UNORM,
    DXGI_R16G16B16A16_UINT,
    DXGI_R16G16B16A16_SNORM,
    DXGI_R16G16B16A16_SINT,
    DXGI_R32G32_FLOAT,
    DXGI_R32G32_UINT,
    DXGI_R32G32_SINT,
    DXGI_R10G10B10A2_UNORM,
    DXGI_R10G10B10A2_UINT,
    DXGI_R11G11B10_FLOAT,
    DXGI_R8G8B8A8_UNORM,
    DXGI_R8G8B8A8_UNORM_SRGB,
    DXGI_R8G8B8A8_UINT,
    DXGI_R8G8B8A8_SNORM,
    DXGI_R8G8B8A8_SINT,
    DXGI_R16G16_FLOAT,
    DXGI_R16G16_UNORM,
    DXGI_R16G16_UINT,
    DXGI_R16G16_SNORM,
    DXGI_R16G16_SINT,
    DXGI_R32_FLOAT,
    DXGI_R32_UINT,
    DXGI_R32_SINT,
    DXGI_R8G8_UNORM,
    DXGI_R8G8_UINT,
    DXGI_R8G8_SNORM,
    DXGI_R8G8_SINT,
    DXGI_R16_FLOAT,
    DXGI_R16_UNORM,
    DXGI_R16_UINT,
    DXGI_R16_SNORM,
    DXGI_R16_SINT,
    DXGI_R8_UNORM,
    DXGI_R8_UINT,
    DXGI_R8_SNORM,
    DXGI_R8_SINT,
    DXGI_A8_UNORM,
    DXGI_R1_UNORM,
    DXGI_R9G9B9E5_SHAREDEXP,
    DXGI_R8G8_B8G8_UNORM,
    DXGI_G8R8_G8B8_UNORM,
    DXGI_BC1_UNORM,
    DXGI_BC1_UNORM_SRGB,
    DXGI_BC2_UNORM,
    DXGI_BC2_UNORM_SRGB,
    DXGI_BC3_UNORM,
    DXGI_BC3_UNORM_SRGB,
    DXGI_BC4_UNORM,				// unimplemented
    DXGI_BC4_SNORM,				// unimplemented
    DXGI_BC5_UNORM,				// unimplemented
    DXGI_BC5_SNORM,				// unimplemented
    // OpenVG
    VG_sRGBX_8888  = 0x90,
    VG_sRGBA_8888,
    VG_sRGBA_8888_PRE,
    VG_sRGB_565,
    VG_sRGBA_5551,
    VG_sRGBA_4444,
    VG_sL_8,
    VG_lRGBX_8888,
    VG_lRGBA_8888,
    VG_lRGBA_8888_PRE,
    VG_lL_8,
    VG_A_8,
    VG_BW_1,
    VG_sXRGB_8888,
    VG_sARGB_8888,
    VG_sARGB_8888_PRE,
    VG_sARGB_1555,
    VG_sARGB_4444,
    VG_lXRGB_8888,
    VG_lARGB_8888,
    VG_lARGB_8888_PRE,
    VG_sBGRX_8888,
    VG_sBGRA_8888,
    VG_sBGRA_8888_PRE,
    VG_sBGR_565,
    VG_sBGRA_5551,
    VG_sBGRA_4444,
    VG_lBGRX_8888,
    VG_lBGRA_8888,
    VG_lBGRA_8888_PRE,
    VG_sXBGR_8888,
    VG_sABGR_8888,
    VG_sABGR_8888_PRE,
    VG_sABGR_1555,
    VG_sABGR_4444,
    VG_lXBGR_8888,
    VG_lABGR_8888,
    VG_lABGR_8888_PRE,
    NumPixelTypes,
    InvalidType = 0xffffffff
  };
  std::multimap<unsigned int,std::string> pixel_format_names {
    STRING_ENUM_PAIR(PixelFormat,MGL_ARGB_4444),
    STRING_ENUM_PAIR(PixelFormat,MGL_ARGB_1555),
    STRING_ENUM_PAIR(PixelFormat,MGL_RGB_565),
    STRING_ENUM_PAIR(PixelFormat,MGL_RGB_555),
    STRING_ENUM_PAIR(PixelFormat,MGL_RGB_888),
    STRING_ENUM_PAIR(PixelFormat,MGL_ARGB_8888),
    STRING_ENUM_PAIR(PixelFormat,MGL_ARGB_8332),
    STRING_ENUM_PAIR(PixelFormat,MGL_I_8),
    STRING_ENUM_PAIR(PixelFormat,MGL_AI_88),
    STRING_ENUM_PAIR(PixelFormat,MGL_1_BPP),
    STRING_ENUM_PAIR(PixelFormat,MGL_VY1UY0),
    STRING_ENUM_PAIR(PixelFormat,MGL_Y1VY0U),
    STRING_ENUM_PAIR(PixelFormat,MGL_PVRTC2),
    STRING_ENUM_PAIR(PixelFormat,MGL_PVRTC4),
    STRING_ENUM_PAIR(PixelFormat,GL_RGBA_4444),
    STRING_ENUM_PAIR(PixelFormat,GL_RGBA_5551),
    STRING_ENUM_PAIR(PixelFormat,GL_RGBA_8888),
    STRING_ENUM_PAIR(PixelFormat,GL_RGB_565),
    STRING_ENUM_PAIR(PixelFormat,GL_RGB_555),
    STRING_ENUM_PAIR(PixelFormat,GL_RGB_888),
    STRING_ENUM_PAIR(PixelFormat,GL_I_8),
    STRING_ENUM_PAIR(PixelFormat,GL_AI_88),
    STRING_ENUM_PAIR(PixelFormat,GL_PVRTC2),
    STRING_ENUM_PAIR(PixelFormat,GL_PVRTC4),
    STRING_ENUM_PAIR(PixelFormat,GL_BGRA_8888),
    STRING_ENUM_PAIR(PixelFormat,GL_A_8),
    STRING_ENUM_PAIR(PixelFormat,GL_PVRTCII4),
    STRING_ENUM_PAIR(PixelFormat,GL_PVRTCII2),
    STRING_ENUM_PAIR(PixelFormat,D3D_DXT1),
    STRING_ENUM_PAIR(PixelFormat,D3D_DXT2),
    STRING_ENUM_PAIR(PixelFormat,D3D_DXT3),
    STRING_ENUM_PAIR(PixelFormat,D3D_DXT4),
    STRING_ENUM_PAIR(PixelFormat,D3D_DXT5),
    STRING_ENUM_PAIR(PixelFormat,D3D_RGB_332),
    STRING_ENUM_PAIR(PixelFormat,D3D_AL_44),
    STRING_ENUM_PAIR(PixelFormat,D3D_LVU_655),
    STRING_ENUM_PAIR(PixelFormat,D3D_XLVU_8888),
    STRING_ENUM_PAIR(PixelFormat,D3D_QWVU_8888),
    STRING_ENUM_PAIR(PixelFormat,D3D_ABGR_2101010),
    STRING_ENUM_PAIR(PixelFormat,D3D_ARGB_2101010),
    STRING_ENUM_PAIR(PixelFormat,D3D_AWVU_2101010),
    STRING_ENUM_PAIR(PixelFormat,D3D_GR_1616),
    STRING_ENUM_PAIR(PixelFormat,D3D_VU_1616),
    STRING_ENUM_PAIR(PixelFormat,D3D_ABGR_16161616),
    STRING_ENUM_PAIR(PixelFormat,D3D_R16F),
    STRING_ENUM_PAIR(PixelFormat,D3D_GR_1616F),
    STRING_ENUM_PAIR(PixelFormat,D3D_ABGR_16161616F),
    STRING_ENUM_PAIR(PixelFormat,D3D_R32F),
    STRING_ENUM_PAIR(PixelFormat,D3D_GR_3232F),
    STRING_ENUM_PAIR(PixelFormat,D3D_ABGR_32323232F),
    STRING_ENUM_PAIR(PixelFormat,e_etc_RGB_4BPP),
    STRING_ENUM_PAIR(PixelFormat,D3D_A8),
    STRING_ENUM_PAIR(PixelFormat,D3D_V8U8),
    STRING_ENUM_PAIR(PixelFormat,D3D_L16),
    STRING_ENUM_PAIR(PixelFormat,D3D_L8),
    STRING_ENUM_PAIR(PixelFormat,D3D_AL_88),
    STRING_ENUM_PAIR(PixelFormat,D3D_UYVY),
    STRING_ENUM_PAIR(PixelFormat,D3D_YUY2),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32G32B32A32_FLOAT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32G32B32A32_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32G32B32A32_SINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32G32B32_FLOAT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32G32B32_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32G32B32_SINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16G16B16A16_FLOAT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16G16B16A16_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16G16B16A16_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16G16B16A16_SNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16G16B16A16_SINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32G32_FLOAT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32G32_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32G32_SINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R10G10B10A2_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R10G10B10A2_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R11G11B10_FLOAT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8G8B8A8_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8G8B8A8_UNORM_SRGB),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8G8B8A8_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8G8B8A8_SNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8G8B8A8_SINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16G16_FLOAT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16G16_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16G16_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16G16_SNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16G16_SINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32_FLOAT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R32_SINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8G8_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8G8_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8G8_SNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8G8_SINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16_FLOAT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16_SNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R16_SINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8_UINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8_SNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8_SINT),
    STRING_ENUM_PAIR(PixelFormat,DXGI_A8_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R1_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R9G9B9E5_SHAREDEXP),
    STRING_ENUM_PAIR(PixelFormat,DXGI_R8G8_B8G8_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_G8R8_G8B8_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_BC1_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_BC1_UNORM_SRGB),
    STRING_ENUM_PAIR(PixelFormat,DXGI_BC2_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_BC2_UNORM_SRGB),
    STRING_ENUM_PAIR(PixelFormat,DXGI_BC3_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_BC3_UNORM_SRGB),
    STRING_ENUM_PAIR(PixelFormat,DXGI_BC4_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_BC4_SNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_BC5_UNORM),
    STRING_ENUM_PAIR(PixelFormat,DXGI_BC5_SNORM),
    STRING_ENUM_PAIR(PixelFormat,VG_sRGBX_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_sRGBA_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_sRGBA_8888_PRE),
    STRING_ENUM_PAIR(PixelFormat,VG_sRGB_565),
    STRING_ENUM_PAIR(PixelFormat,VG_sRGBA_5551),
    STRING_ENUM_PAIR(PixelFormat,VG_sRGBA_4444),
    STRING_ENUM_PAIR(PixelFormat,VG_sL_8),
    STRING_ENUM_PAIR(PixelFormat,VG_lRGBX_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_lRGBA_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_lRGBA_8888_PRE),
    STRING_ENUM_PAIR(PixelFormat,VG_lL_8),
    STRING_ENUM_PAIR(PixelFormat,VG_A_8),
    STRING_ENUM_PAIR(PixelFormat,VG_BW_1),
    STRING_ENUM_PAIR(PixelFormat,VG_sXRGB_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_sARGB_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_sARGB_8888_PRE),
    STRING_ENUM_PAIR(PixelFormat,VG_sARGB_1555),
    STRING_ENUM_PAIR(PixelFormat,VG_sARGB_4444),
    STRING_ENUM_PAIR(PixelFormat,VG_lXRGB_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_lARGB_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_lARGB_8888_PRE),
    STRING_ENUM_PAIR(PixelFormat,VG_sBGRX_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_sBGRA_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_sBGRA_8888_PRE),
    STRING_ENUM_PAIR(PixelFormat,VG_sBGR_565),
    STRING_ENUM_PAIR(PixelFormat,VG_sBGRA_5551),
    STRING_ENUM_PAIR(PixelFormat,VG_sBGRA_4444),
    STRING_ENUM_PAIR(PixelFormat,VG_lBGRX_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_lBGRA_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_lBGRA_8888_PRE),
    STRING_ENUM_PAIR(PixelFormat,VG_sXBGR_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_sABGR_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_sABGR_8888_PRE),
    STRING_ENUM_PAIR(PixelFormat,VG_sABGR_1555),
    STRING_ENUM_PAIR(PixelFormat,VG_sABGR_4444),
    STRING_ENUM_PAIR(PixelFormat,VG_lXBGR_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_lABGR_8888),
    STRING_ENUM_PAIR(PixelFormat,VG_lABGR_8888_PRE)
  };
  enum API {
    ApiOGLES = 1,
    ApiOGLES2,
    ApiD3DM,
    ApiOGL,
    ApiDX9,
    ApiDX10,
    ApiOVG,
    ApiMGL
  };
  std::multimap<unsigned int,std::string> api_names {
    STRING_ENUM_PAIR(API,ApiOGLES),
    STRING_ENUM_PAIR(API,ApiOGLES2),
    STRING_ENUM_PAIR(API,ApiD3DM),
    STRING_ENUM_PAIR(API,ApiOGL),
    STRING_ENUM_PAIR(API,ApiDX9),
    STRING_ENUM_PAIR(API,ApiDX10),
    STRING_ENUM_PAIR(API,ApiOVG),
    STRING_ENUM_PAIR(API,ApiMGL)
  };
  enum Flags {
    MIP_MAP         = (1 << 8),  // Texture has MIP Map levels
    BUMP_MAP        = (1 << 10), // Texture has normals encoded for a bump map
    CUBE_MAP        = (1 << 12), // Texture is a cubemap/skybox
    VOLUME_TEXTURE  = (1 << 14), // Texture is a 3D texture
    HAS_ALPHA       = (1 << 15), // Texture has transparency
    VERTICAL_FLIP   = (1 << 16)  // Texture is vertically flipped
  };
  std::multimap<unsigned int,std::string> flag_names {
    STRING_ENUM_PAIR(Flags,MIP_MAP),
    STRING_ENUM_PAIR(Flags,BUMP_MAP),
    STRING_ENUM_PAIR(Flags,CUBE_MAP),
    STRING_ENUM_PAIR(Flags,VOLUME_TEXTURE),
    STRING_ENUM_PAIR(Flags,HAS_ALPHA),
    STRING_ENUM_PAIR(Flags,VERTICAL_FLIP)
  };
  const std::uint32_t kPixelTypeMask = 0xff;
  // The old PVR header identifier is the characters 'PVR!', V2 only. Usually ignored...
  const std::uint32_t kIdentifierV2 = 0x21525650;
  const std::uint32_t kHeaderSizeV1 = 44;
  const std::uint32_t kHeaderSizeV2 = 52;
  
  std::vector<std::string> column_names {
  "Height",
  "Width",
  "Number of MIP maps",
  "Pixel format flags",
  "Data size",
  "Number of bits",
  "Red mask?",
  "Green mask?",
  "Blue mask?",
  "Alpha mask?",
  "Magic number",
  "Number of surfaces"
  };
}
// PVR Version 1 & 2
class PvrLegacyHeader: public IHeader {
public:
  virtual bool LoadHeader(std::ifstream& file, std::string& error_string) {
    HEADER_PRE_LOAD(file)
    std::uint32_t pvr_version;
    file.read(reinterpret_cast<char*>(&pvr_version), sizeof pvr_version);
    if(pvr_version != PvrLegacyProps::kHeaderSizeV1 && pvr_version != PvrLegacyProps::kHeaderSizeV2) {
      error_string = "Not a valid legacy (v1 or v2) PVR file";
      return false;
    }
    file.read(reinterpret_cast<char*>(&this->impl_v1_), sizeof(this->impl_v1_));
    if(pvr_version == PvrLegacyProps::kHeaderSizeV2) {
      file.read(reinterpret_cast<char*>(&this->impl_v2_), sizeof(this->impl_v2_));
    }
    HEADER_POST_LOAD(file)
  }
  virtual std::string ToString(){
    std::string out_string("");
    const auto& impl_v1(this->impl_v1_);
    const auto& impl_v2(this->impl_v2_);
    APPEND_FORMATTED_ROW(out_string,PvrLegacyProps::column_names[0],impl_v1.height)
    APPEND_FORMATTED_ROW(out_string,PvrLegacyProps::column_names[1],impl_v1.width)
    APPEND_FORMATTED_ROW(out_string,PvrLegacyProps::column_names[2],impl_v1.mip_map_count+1)
    APPEND_FORMATTED_ROW(out_string,PvrLegacyProps::column_names[3],impl_v1.pixel_format_flags)
    APPEND_FORMATTED_ROW(out_string,PvrLegacyProps::column_names[4],impl_v1.data_size)
    APPEND_FORMATTED_ROW(out_string,PvrLegacyProps::column_names[5],impl_v1.bit_count)
    APPEND_FORMATTED_ROW(out_string,PvrLegacyProps::column_names[6],impl_v1.red_mask)
    APPEND_FORMATTED_ROW(out_string,PvrLegacyProps::column_names[7],impl_v1.green_mask)
    APPEND_FORMATTED_ROW(out_string,PvrLegacyProps::column_names[8],impl_v1.blue_mask)
    APPEND_FORMATTED_ROW(out_string,PvrLegacyProps::column_names[9],impl_v1.alpha_mask)
    std::string magic_number_string(impl_v2.magic_number == 0?std::string("-"):std::to_string(impl_v2.magic_number));
    APPEND_FORMATTED_ROW_RAW(out_string,PvrLegacyProps::column_names[10],magic_number_string)
    std::string num_surfaces_string(impl_v2.num_surfaces == 0?std::string("-"):std::to_string(impl_v2.num_surfaces));
    APPEND_FORMATTED_ROW_RAW(out_string,PvrLegacyProps::column_names[11],num_surfaces_string)
    return out_string;
  }
  virtual std::string ToCsvString(){assert(0);}
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

namespace PvrV3Props {
  enum CompressedFormat {
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
  std::multimap<unsigned int,std::string> compressed_format_names {
  STRING_ENUM_PAIR(CompressedFormat,PVRTCI_2bpp_RGB),
  STRING_ENUM_PAIR(CompressedFormat,PVRTCI_2bpp_RGBA),
  STRING_ENUM_PAIR(CompressedFormat,PVRTCI_4bpp_RGB),
  STRING_ENUM_PAIR(CompressedFormat,PVRTCI_4bpp_RGBA),
  STRING_ENUM_PAIR(CompressedFormat,PVRTCII_2bpp),
  STRING_ENUM_PAIR(CompressedFormat,PVRTCII_4bpp),
  STRING_ENUM_PAIR(CompressedFormat,ETC1),
  STRING_ENUM_PAIR(CompressedFormat,DXT1),
  STRING_ENUM_PAIR(CompressedFormat,DXT2),
  STRING_ENUM_PAIR(CompressedFormat,DXT3),
  STRING_ENUM_PAIR(CompressedFormat,DXT4),
  STRING_ENUM_PAIR(CompressedFormat,DXT5),
  STRING_ENUM_PAIR(CompressedFormat,BC1),
  STRING_ENUM_PAIR(CompressedFormat,BC2),
  STRING_ENUM_PAIR(CompressedFormat,BC3),
  STRING_ENUM_PAIR(CompressedFormat,BC4),
  STRING_ENUM_PAIR(CompressedFormat,BC5),
  STRING_ENUM_PAIR(CompressedFormat,BC6),
  STRING_ENUM_PAIR(CompressedFormat,BC7),
  STRING_ENUM_PAIR(CompressedFormat,UYVY),
  STRING_ENUM_PAIR(CompressedFormat,YUY2),
  STRING_ENUM_PAIR(CompressedFormat,BW1bpp),
  STRING_ENUM_PAIR(CompressedFormat,SharedExponentR9G9B9E5),
  STRING_ENUM_PAIR(CompressedFormat,RGBG8888),
  STRING_ENUM_PAIR(CompressedFormat,GRGB8888),
  STRING_ENUM_PAIR(CompressedFormat,ETC2_RGB),
  STRING_ENUM_PAIR(CompressedFormat,ETC2_RGBA),
  STRING_ENUM_PAIR(CompressedFormat,ETC2_RGB_A1),
  STRING_ENUM_PAIR(CompressedFormat,EAC_R11),
  STRING_ENUM_PAIR(CompressedFormat,EAC_RG11)
  };
  std::vector<std::string> column_names {
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
};
class PvrV3Header: public IHeader {
public:
  virtual bool LoadHeader(std::ifstream& file, std::string& error_string) {
    HEADER_PRE_LOAD(file)
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
    HEADER_POST_LOAD(file)
  };
  virtual std::string ToString() {
    std::string out_string("");
    const auto& impl(this->impl_);
    APPEND_FORMATTED_ROW(out_string,PvrV3Props::column_names[0],impl.flags)
    if(impl.pixel_format.u32[1] == 0) {
      APPEND_FORMATTED_ROW_RAW(out_string,PvrV3Props::column_names[1],
        PvrV3Props::compressed_format_names.find(impl.pixel_format.u8[0])->second)
      for(unsigned int index = 0; index < 8; index++)
        APPEND_FORMATTED_ROW_RAW(out_string,PvrV3Props::column_names[2+index],c_empty_string)
      }
    else {
      APPEND_FORMATTED_ROW_RAW(out_string,PvrV3Props::column_names[1],c_empty_string)
      // Channel names
      for(unsigned int index = 0; index < 4; index++) {
        // Convert unsigned int pulled from the header into a literal character
        char value(c_empty_string[0]);
        std::sscanf((char*)&impl.pixel_format.u8[index],"%c",&value);
        APPEND_FORMATTED_ROW_RAW(out_string,PvrV3Props::column_names[2+index],value)
      }
      // Bits per-pixel
      for(unsigned int index = 4; index < 8; index++)
        if(impl.pixel_format.u8[index]==0)
          APPEND_FORMATTED_ROW_RAW(out_string,PvrV3Props::column_names[2+index],c_empty_string)
        else
          APPEND_FORMATTED_ROW(out_string,PvrV3Props::column_names[2+index],impl.pixel_format.u8[index])
    }
    APPEND_FORMATTED_ROW_RAW(out_string,PvrV3Props::column_names[10],
      ColorSpaceNames.find(impl.color_space)->second)
    APPEND_FORMATTED_ROW_RAW(out_string,PvrV3Props::column_names[11],
      VariableTypeNames.find(impl.channel_type)->second)
    APPEND_FORMATTED_ROW(out_string,PvrV3Props::column_names[12],impl.height)
    APPEND_FORMATTED_ROW(out_string,PvrV3Props::column_names[13],impl.width)
    APPEND_FORMATTED_ROW(out_string,PvrV3Props::column_names[14],impl.depth)
    APPEND_FORMATTED_ROW(out_string,PvrV3Props::column_names[15],impl.num_surfaces)
    APPEND_FORMATTED_ROW(out_string,PvrV3Props::column_names[16],impl.num_faces)
    APPEND_FORMATTED_ROW(out_string,PvrV3Props::column_names[17],impl.mip_map_count)
    APPEND_FORMATTED_ROW(out_string,PvrV3Props::column_names[18],impl.meta_data_size)
    return out_string;
  };
  virtual std::string ToCsvString() {
    std::string csv_string("");
    const auto& impl(this->impl_);
    AppendCsvifiedRow(csv_string,std::to_string(impl.flags));
    if(impl.pixel_format.u32[1] == 0) {
      AppendCsvifiedRow(csv_string,
        PvrV3Props::compressed_format_names.find(impl.pixel_format.u8[0])->second);
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
