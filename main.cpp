#include <string>
#include <iostream>
#include <fstream>
#include <tuple>

#include <cstring>
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

static const
std::vector<std::tuple<std::string, std::string>> ParameterInfo {
{"--csv","Write header information to a CSV file (calling directory)"}
};

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
static void PrintHeaderInfo(const std::string& file_name, const std::string& header_info) {
  PrintDivider();
  std::cout << file_name << std::endl;
  PrintDivider();
  std::cout << header_info << std::endl;
}
static void PrintHelp() {
  std::cout << "usage: textureinfo ";
  for(const auto&parameter:ParameterInfo)
    std::cout << "[" << std::get<0>(parameter) << "] ";
  std::cout << "..." << std::endl << std::endl;
  std::cout << "descriptions:" << std::endl;
  for(const auto&parameter:ParameterInfo)
    std::cout << "\t" << std::get<0>(parameter) << ": "
      << std::get<1>(parameter) << std::endl;
}

//*-------------------------------
// main
//-------------------------------*/
int main (int argc, char *argv[]) {
  std::vector<std::string> pvr_files;
  // Loop through args
  for(unsigned int index = 1; index < argc; index++) {
    // Flags
    if(strcmp(argv[index], "/?") == 0 ||
    strcmp(argv[index], "-h") == 0 ||
    strcmp(argv[index], "--help") == 0) {
      PrintHelp();
    }
    if(argv[index] == std::get<0>(ParameterInfo[0])) {
      s_parameters.print_csv = true;
      continue;
    }
    // Files
    std::string file_name(argv[index]);
    std::string ext_name(GetFilenameExt(file_name.c_str()));
    if(ext_name == "pvr") {
      pvr_files.push_back(std::move(file_name));
    }
  }
  // CSV setup
  std::ofstream csv_pvr_output;
  if(s_parameters.print_csv) {
    csv_pvr_output.open(s_defaults.csv_pvr_name);
    if(!csv_pvr_output.is_open()) {
      printf("ERROR: Unable to open %s",s_defaults.csv_pvr_name.c_str());
    }
    for(auto& variable_name : PvrV3Props::var_names)
      csv_pvr_output << variable_name << ',';
    csv_pvr_output << std::endl;
  }
  
  std::string error_string("");
  // Process PVR files
  for(const auto& file_name: pvr_files) {
    std::ifstream file(file_name, std::ifstream::binary);
    if(!file.is_open()) {
      printf("ERROR: Unable to open %s\n", file_name.c_str());
      continue;
    }
    PvrV3Header pvr_header;
    if(!pvr_header.LoadHeader(file, error_string)) {
      printf("ERROR: %s - %s\n",file_name.c_str(), error_string.c_str());
      continue;
    }
    if(s_parameters.print_csv) {
      csv_pvr_output << pvr_header.ToCsvString().c_str() << std::endl;
    }
    else {
      PrintHeaderInfo(file_name,pvr_header.ToString());
    }
  }
  
  // Shutdown
  csv_pvr_output.close();
}