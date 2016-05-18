#include <string>
#include <fstream>
#include <stdint.h>

#include "textureinfo.h"

//*-------------------------------
// Global variables
//-------------------------------*/
static const struct Defaults {
  std::string csv_pvr_name;
  Defaults():csv_pvr_name("pvr_info.csv"){}
}s_defaults;
static struct Parameters {
  bool print_csv;
  Parameters():print_csv(false){}
}s_parameters;

//*-------------------------------
// Global functions
//-------------------------------*/
static std::string GetFilenameExt(std::string filename) {
  std::string ext_name("");
  std::string::size_type index = filename.rfind('.');
  if(index != std::string::npos)
    ext_name = filename.substr(index+1);
  return ext_name;
}
static void PrintDivider() {
  printf("========================================\n");
}

//*-------------------------------
// main
//-------------------------------*/
int main (int argc, char *argv[]) {
  std::ofstream csv_pvr_output;
  if(s_parameters.print_csv) {
    csv_pvr_output.open(s_defaults.csv_pvr_name);
    if(!csv_pvr_output.is_open()) {
      printf("ERROR: Unable to open %s",s_defaults.csv_pvr_name.c_str());
    }
    for(auto& variable_name : PvrV3HeaderVarNames)
      csv_pvr_output << variable_name << ',';
    csv_pvr_output << std::endl;
  }
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
        if(s_parameters.print_csv) {
          csv_pvr_output << pvr_header.ToCsvString().c_str() << std::endl;
        }
        else {
          PrintDivider();
          printf("%s\n",file_name.c_str());
          PrintDivider();
          printf("%s\n",pvr_header.ToString().c_str());
        }
      }
    }
  }
  csv_pvr_output.close();
}