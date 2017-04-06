#pragma once

#include <FitGLClasses.h>

#include <functional>
#include <vector>
#include "BaseApp.h"


class fit::OpenGLApp : public fit::BaseApp {
public:
  struct Options {
    int minimumVersion = 430;
    bool vsync = true;
    int multisample = 1;
    SDLWindow::Profile profile = SDLWindow::CORE;
    SDLWindow::Flag flags = SDLWindow::Flag::DEBUG;
  };

public:
  OpenGLApp(Options const&options = Options());
	virtual ~OpenGLApp();
  
protected:
	SDL_GLContext mainContext = nullptr;
  void handleIdle() override;
  void handleEvent(SDL_Event const &e) override;
	void enableDebug();
	void initGL(Options const&options);
};
