#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

#include <cstring>
#include <stdint.h>

#include "textureinfo.h"

//*-------------------------------
// Global variables
//-------------------------------*/
static const std::string s_version = "1.1.1";
namespace Defaults {
  enum Types {
    PVRV3,
    PVRLegacy,
    KTX
  };
  const std::vector<std::string> kTypeNames {
    "PVR (v3)",
    "PVR (legacy)",
    "KTX"
  };
  const std::vector<std::string> kCsvNames {
  "pvrV3.csv",
  "pvrLegacy.csv",
  "ktx.csv"
  };
};
static struct Parameters {
  bool print_csv;
  Parameters():print_csv(false){}
}s_parameters;

static const
std::vector<std::pair<std::string, std::string>> ParameterInfo {
{"--csv","Write header information to a CSV file (calling directory). Disables stdout print"}
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
static void PrintHeaderInfo(
  const std::string& file_name,
  const std::string& container_name,
  const std::string& header_info) {
  PrintDivider();
  std::cout << file_name << std::endl;
  PrintDivider();
  std::cout << "Container: " << container_name << std::endl;
  std::cout << header_info << std::endl;
}
static void PrintHelp() {
  const std::vector<std::string> supported_containers
  {"PVR (v1)","PVR (v2)","PVR (v3)","KTX"};
  std::cout << "version: " << s_version <<std::endl;
  std::cout << "usage: textureinfo ";
  for(const auto&parameter:ParameterInfo)
    std::cout << "[" << std::get<0>(parameter) << "] ";
  std::cout << "..." << std::endl << std::endl;
  std::cout << "Print a texture container's header information to stdout. Supported containers:" << std::endl;
  for(const auto& container:supported_containers)
      std::cout << "\t* " << container << std::endl;
  std::cout << std::endl;
  std::cout << "options:" << std::endl;
  for(const auto&parameter:ParameterInfo)
    std::cout << "\t" << std::get<0>(parameter) << ": "
      << std::get<1>(parameter) << std::endl;
}

//*-------------------------------
// main
//-------------------------------*/
int main (int argc, char *argv[]) {
  std::vector<std::string> pvr_files, ktx_files;
  //*-------------------------------
  // Loop through args
  //-------------------------------*/
  if(argc == 1) {
    PrintHelp();
    return 0;
  }
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
    else if(ext_name == "ktx") {
      ktx_files.push_back(std::move(file_name));
    }
  }
  //*-------------------------------
  // CSV setup
  //-------------------------------*/
  std::vector<std::ofstream> output_streams(Defaults::kCsvNames.size());
  if(s_parameters.print_csv) {
    for(unsigned int index = 0; index < output_streams.size(); ++index) {
      const std::string& output_name(Defaults::kCsvNames.at(index));
      std::ofstream& output(output_streams.at(index));
      output.open(output_name);
      if(!output.is_open()) {
        printf("ERROR: Unable to open %s", output_name.c_str());
      }
      std::vector<std::string> column_titles;
      switch (index) {
        case Defaults::Types::PVRV3:
          column_titles = PvrV3Info::column_names;
          break;
        case Defaults::Types::PVRLegacy:
          column_titles = PvrLegacyInfo::column_names;
          break;
        case Defaults::Types::KTX:
          column_titles = KTXInfo::column_names;
          break;
        default:
          assert(0);
          break;
      }
      output << "File name" << ',';
      for(auto& column_title : column_titles)
        output << column_title << ',';
      output << std::endl;
    }
  }
  //*-------------------------------
  // Load files & print
  //-------------------------------*/
  std::string error_string("");
  // PVR files
  for(const auto& file_name: pvr_files) {
    std::ifstream file(file_name, std::ifstream::binary);
    if(!file.is_open()) {
      printf("ERROR: Unable to open %s\n", file_name.c_str());
      continue;
    }
    unsigned int file_type(Defaults::Types::PVRV3);
    IHeader* header(nullptr);
    PvrV3Header pvr_header;
    PvrLegacyHeader pvr_legacy_header;
    // PVR v3
    if(pvr_header.LoadHeader(file, error_string)) header = &pvr_header;
    else file_type = Defaults::Types::PVRLegacy;
    // Legacy container
    if(header == nullptr) {
      if(pvr_legacy_header.LoadHeader(file, error_string)) {
        header = &pvr_legacy_header;
      }
      else {
        printf("ERROR: %s - %s\n",file_name.c_str(), error_string.c_str());
        continue;
      }
    }
    // Print
    if(s_parameters.print_csv)
      output_streams[file_type] << file_name << "," << header->ToCsvString() << std::endl;
    else
      PrintHeaderInfo(
        file_name,
        file_type == Defaults::Types::PVRV3?"PVR (v3)":"PVR (legacy)",
        header->ToString());
  }
  // KTX files
  for(const auto& file_name: ktx_files) {
    std::ifstream file(file_name, std::ifstream::binary);
    if(!file.is_open()) {
      printf("ERROR: Unable to open %s\n", file_name.c_str());
      continue;
    }
    KTXHeader ktx_header;
    if(!ktx_header.LoadHeader(file, error_string)) {
      printf("ERROR: %s - %s\n",file_name.c_str(), error_string.c_str());
        continue;
    }
    // Print
    if(s_parameters.print_csv)
      output_streams.at(Defaults::Types::KTX) << file_name << "," << ktx_header.ToCsvString() << std::endl;
    else
      PrintHeaderInfo(
        file_name,
        Defaults::kTypeNames.at(Defaults::Types::KTX),
        ktx_header.ToString());
  }
  
  // Shutdown
  for(auto& stream:output_streams)
    stream.close();
}
