#pragma once
#include "FitClasses.h"

class fit::Image {
public:
	~Image() { if (data)delete[]data; }
	unsigned int* data = nullptr;
	int width = 0;
	int height = 0;
	//GLenum format = GL_RGBA;
	//GLenum type = GL_UNSIGNED_BYTE;
};
