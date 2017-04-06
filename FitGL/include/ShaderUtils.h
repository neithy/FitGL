#pragma once

#include <geGLclasses.h>

template<typename...ARGS>
ge::gl::ShaderShared compileShader(GLenum type, ARGS... sources) {
  return std::make_shared<ge::gl::Shader>(type,sources...);
}

template<typename...ARGS>
ge::gl::ProgramShared createProgram(ARGS...args) {
  auto prog = std::make_shared<ge::gl::Program>(args...);
  if (!prog->getLinkStatus()) {
    throw std::runtime_error("Program link failed!");
  }
  return prog;
}