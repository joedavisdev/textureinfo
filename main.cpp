#include <string>
#include <cstdio>

std::string GetFilenameExt(std::string filename) {
  std::string ext_name("");
  std::string::size_type index = filename.rfind('.');
  if(index != std::string::npos)
    ext_name = filename.substr(index+1);
  return ext_name;
}
int main (int argc, char *argv[]) {
  for(unsigned int index = 1; index < argc; index++) {
    printf("DEBUG: File name: %s. ext name: %s\n",argv[index], GetFilenameExt(argv[index]).c_str());
  }
}