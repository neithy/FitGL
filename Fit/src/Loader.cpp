#include "Loader.h"

#include <fstream>

using namespace fit;

std::vector<unsigned char> Loader::binary(std::string const & fileName){
  std::ifstream f(fileName.c_str(), std::ios::binary);
  if (!f.is_open()) {
    std::string er = "File not found:" + fileName;
    throw std::runtime_error(er.c_str());
  }
  std::vector<unsigned char> data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
  f.close();
  return data;
}

std::string Loader::text(std::string const & fileName) {
	std::ifstream f(fileName.c_str());
	if (!f.is_open()) {
		std::string er = "File not found:" + fileName;
		throw std::runtime_error(er.c_str());
	}
	std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	f.close();
	return str;
}
