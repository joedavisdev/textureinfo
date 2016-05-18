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
        printf("ERROR: Unable to open %s\n", file_name.c_str());
        continue;
      }
      PvrV3Header pvr_header;
      if(pvr_header.LoadHeader(file, file_name) ) {
        printf("========================================\n");
        printf("%s\n",file_name.c_str());
        printf("========================================\n");
        printf("%s",pvr_header.ToString().c_str());
      }
    }
  }
}