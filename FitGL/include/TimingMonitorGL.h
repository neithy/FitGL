#pragma once

#include "FitGLClasses.h"
#include <vector>

#include "geGLclasses.h"

class fit::TimingMonitorGL {
public:
  TimingMonitorGL(int intervals = 1, int historyLength = 200);
  ~TimingMonitorGL();

  void startFrame();
  void timestamp(std::string name="");
  void draw();
protected:  

  //std::vector<float> data;
  float* data;
  int dataLength;
  int offset = 0;
  std::vector<std::string> names;

  int historyLength;
  int intervals;
  int frameTimestamps=0;

  std::vector<GLuint> queries;


  ge::gl::BufferShared buffer;
  ge::gl::ProgramShared program;
  ge::gl::TextureShared texture;
};
