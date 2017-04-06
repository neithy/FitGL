#include "Loader.h"

#include <fstream>

std::string fit::Loader::text(std::string const & fileName) {
	std::ifstream f(fileName.c_str());
	if (!f.is_open()) {
		std::string er = "File not found:" + fileName;
		throw std::runtime_error(er.c_str());
	}
	std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	f.close();
	return str;
}
