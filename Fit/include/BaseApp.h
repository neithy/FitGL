#pragma once
#include "FitClasses.h"

#include "SDLWindow.h"
#include "SDLEventProc.h"

#include <functional>
#include <vector>

// 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Gui.h"

#include "Timer.h"

#define ANY_EVENT -1
#define ANY_WINDOW -1

class fit::BaseApp {
public:
  BaseApp();
  virtual ~BaseApp();

  int run();
  void quit();
  void waitForPress();

  // windows
  SDLWindowShared addWindow(SDLWindowShared &window);
  SDLWindowShared addWindow(int width = 1024, int height = 768, bool resizable = true, bool fullscreen = false, int multisample = 1);
  SDLWindowShared getWindow(int i) { return windows[i]; }
  SDLWindowShared getWindowByID(int id);
  SDLWindowShared getMainWindow() { return mainWindow; }
  void removeWindow(SDLWindowShared &window);

  // resources
  std::string getResourceDir() { return resourceDir; }
  void setResourceDir(std::string const&s) { resourceDir = s; }

  //
  float getTimeFromStart() { return timer.elapsedFromStart(); }
  float getDt() { return dt; }

  // callbacks
  int addEventCallback(std::function<void(SDL_Event) > callback, int type = ANY_EVENT, int windowID = ANY_WINDOW);
  int addDrawCallback(std::function<void()> const&callback, SDLWindowShared window = nullptr);
  int addInitCallback(std::function<void()> const&callback);
  int addCleanupCallback(std::function<void()> const&callback);
  int addUpdateCallback(std::function<void(float /*dt*/)> const&callback);

  // specific event callbacks
  int addResizeCallback(std::function<void(int /*w*/, int /*h*/) >f, int window = ANY_WINDOW);
  int addMouseMoveCallback(std::function<void(int /*dx*/, int /*dy*/, int /*x*/, int /*y*/) >f, int window = ANY_WINDOW);
  int addMousePressCallback(std::function<void(Uint8 /*button*/, int /*x*/, int /*y*/) >f, int window = ANY_WINDOW);
  int addMouseReleaseCallback(std::function<void(Uint8 /*button*/, int /*x*/, int /*y*/) >f, int window = ANY_WINDOW);
  int addMouseWheelCallback(std::function<void(int /*delta*/) >f, int window = ANY_WINDOW);
  int addKeyPressCallback(std::function<void(SDL_Keycode /*key*/, Uint16 /*mod*/) > f, int window = ANY_WINDOW);
  int addKeyReleaseCallback(std::function<void(SDL_Keycode /*key*/, Uint16 /*mod*/) > f, int window = ANY_WINDOW);
  void removeCallback(int id);

  // events for main window
  void setupMainWindowEvents();
  virtual void init() {}
  virtual void update(float dt) {}
  virtual void draw() {}
  virtual void onResize(int /*w*/, int /*h*/) {}
  virtual void onMouseMove(int /*dx*/, int /*dy*/, int /*x*/, int /*y*/) {}
  virtual void onMousePress(Uint8 /*button*/, int /*x*/, int /*y*/) {}
  virtual void onMouseRelease(Uint8 /*button*/, int /*x*/, int /*y*/) {}
  virtual void onMouseWheel(int /*delta*/) {}
  virtual void onKeyPress(SDL_Keycode /*key*/, Uint16 /*mod*/) {}
  virtual void onKeyRelease(SDL_Keycode /*key*/, Uint16 /*mod*/) {}

protected:
  int idGen = 0;
  using Callback = std::function<void()>;
  using UpdateCallback = std::function<void(float)>;
  using EventCallback = std::function<void(SDL_Event)>;

  struct EventCallbackFilterID {
    int id;
    EventCallback callback;
    int type;
    int window;
  };

  struct DrawCallbackFilterID {
    int id;
    Callback callback;
    int window;
  };

  struct CallbackID {
    int id; 
    Callback callback;    
  };

  struct UpdateCallbackID {
    int id;
    UpdateCallback callback;
  };
  

  SDLWindowShared mainWindow;
	std::vector<SDLWindowShared> windows;
	SDLEventProcShared mainLoop;

	std::vector<EventCallbackFilterID> eventCallbacks;
	std::vector<DrawCallbackFilterID> drawCallbacks;
	std::vector<CallbackID> initCallbacks;
	std::vector<CallbackID> cleanupCallbacks;
	std::vector<UpdateCallbackID> updateCallbacks;


	virtual void handleEvent(SDL_Event const &e);
	virtual void handleIdle();

	Timer<float> timer;
	float dt;

	std::string resourceDir;
	bool initFailed = false;
};
