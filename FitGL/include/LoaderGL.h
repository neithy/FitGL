#pragma once

#include <FitGLClasses.h>
#include <geGL/geGL.h>
#include <string>
#include "ShaderUtils.h"

namespace fit { namespace Loader {
  ge::gl::TextureShared texture(std::string const &fileName, bool generateMipmap=true,
    GLenum filterMin= GL_LINEAR_MIPMAP_LINEAR, GLenum filterMag= GL_LINEAR,
    GLenum wrapR=GL_REPEAT, GLenum wrapS= GL_REPEAT);

  ge::gl::TextureShared cubemap(std::string const(&fileNames)[6], bool generateMipmap = true,
    GLenum filterMin = GL_LINEAR_MIPMAP_LINEAR, GLenum filterMag = GL_LINEAR,
    GLenum wrapR = GL_REPEAT, GLenum wrapS = GL_REPEAT);
}}
