#pragma once

#include <memory>

#define Fit_class(x) class x; typedef std::shared_ptr<x> x##Shared; typedef std::weak_ptr<x> x##Weak;


namespace fit {
    Fit_class(BaseApp);
    Fit_class(SDLWindow);
    Fit_class(SDLEventProc);
    Fit_class(Camera);
    Fit_class(PerspectiveCamera);
    Fit_class(Manipulator);
    Fit_class(OrbitManipulator);
    Fit_class(Image);
	Fit_class(FileWatcher);
};