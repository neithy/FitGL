#include "Loader.h"

#include <fstream>

std::vector<unsigned char> fit::Loader::binary(std::string const & fileName){
  std::ifstream f(fileName.c_str(), std::ios::binary);
  if (!f.is_open()) {
    std::string er = "File not found:" + fileName;
    throw std::runtime_error(er.c_str());
  }
  std::vector<unsigned char> data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
  f.close();
  return data;
}