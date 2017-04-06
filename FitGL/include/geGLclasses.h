#pragma once

#include <memory>

#define geGL_class(x) class x; typedef std::shared_ptr<x> x##Shared; typedef std::weak_ptr<x> x##Weak;

#include <geGL/geGL.h>
namespace ge{
  namespace gl{
    geGL_class(AsynchronousQuery);
    geGL_class(Buffer);
    geGL_class(Framebuffer);
    geGL_class(Shader);
    geGL_class(Program);
    geGL_class(VertexArray);
    geGL_class(ProgramPipeline);
    geGL_class(Texture);
    geGL_class(Sampler);
    geGL_class(Renderbuffer);
    geGL_class(DebugMessage);
    geGL_class(FunctionLoaderInterface);
    geGL_class(DefaultLoader);
  }
};
