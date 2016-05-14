#include <string>
#include <fstream>
#include <stdint.h>

#include "textureinfo.h"

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
// main
//-------------------------------*/
int main (int argc, char *argv[]) {
  for(unsigned int index = 1; index < argc; index++) {
    std::string file_name(argv[index]);
    std::string ext_name(GetFilenameExt(file_name.c_str()));
    if(ext_name == "pvr") {
      std::ifstream file(file_name, std::ifstream::binary);
      if(!file.is_open()) {
        printf("WARNING: Unable to open %s\n", file_name.c_str());
        continue;
      }
      std::uint32_t pvr_version;
      file.read(reinterpret_cast<char*>(&pvr_version), sizeof pvr_version);
      if(pvr_version == 0x03525650)
        printf("DEBUG: %s endianess matches host\n",file_name.c_str());
      else if(pvr_version == 0x50565203)
        printf("DEBUG: %s endianess does not matches host\n",file_name.c_str());
      else
        printf("DEBUG: %s is not a valid PVR v3 file\n",file_name.c_str());
    }
  }
}