#pragma once

#include <memory>

#define FitGL_class(x) class x; typedef std::shared_ptr<x> x##Shared; typedef std::weak_ptr<x> x##Weak;

namespace fit {
  FitGL_class(OpenGLApp);
};