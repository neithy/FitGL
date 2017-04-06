#include "OpenGLApp.h"

#include <geGL/StaticCalls.h>
#include <geGL/geGL.h>

#include <iostream>

using namespace fit;
using namespace std;
using namespace ge::gl;


OpenGLApp::OpenGLApp(Options const&options): BaseApp() {
  initGL(options); 
  //gui
  addInitCallback([&]() {
      ImGui_ImplSdlGL3_Init(mainWindow->getWindowHandle());
  });
  addCleanupCallback([]() {
      ImGui_ImplSdlGL3_Shutdown();
  });
}

OpenGLApp::~OpenGLApp() {
}

void OpenGLApp::initGL(Options const&options) {
	int version = 450;
	while (version >= options.minimumVersion && mainContext == nullptr) {
		mainContext = mainWindow->createContext("context", version, options.profile, options.flags, options.vsync);
		version -= 10;
	}
	if (mainContext == nullptr) {
		string msg = "OpenGL context version "
			+ to_string(options.minimumVersion / 100) + "."
			+ to_string((options.minimumVersion % 100) / 10)
			+ " not supported.\n";
		cerr << msg;
		initFailed = true;
		mainLoop->removeWindow(std::to_string(mainWindow->getId()));
		return;
	}

  ge::gl::init(SDL_GL_GetProcAddress);

	auto c = glGetString(GL_VERSION);
	std::cout << c << "\n";
	c = glGetString(GL_VENDOR);
	std::cout << c << "\n";
	c = glGetString(GL_RENDERER);
	std::cout << c << "\n";
	enableDebug();

}

void OpenGLApp::handleIdle(){
	dt = timer.elapsedFromLast();
	for (auto&c : updateCallbacks)c.callback(dt);

	for (auto &w : windows) {
		w->makeCurrent(mainContext);
    glViewport(0, 0, w->getWidth(), w->getHeight());
		ImGui_ImplSdlGL3_NewFrame(w->getWindowHandle());
		for (auto &dc : drawCallbacks) {
			if (dc.window == w->getId()) dc.callback();
		}
		ImGui::Render();
		w->swap();
	}
}

void OpenGLApp::handleEvent(SDL_Event const& e) {
  ImGui_ImplSdlGL3_ProcessEvent(const_cast<SDL_Event*>(&e));
  BaseApp::handleEvent(e);  
}

void OpenGLApp::enableDebug() {
  ge::gl::setHighDebugMessage();
}
