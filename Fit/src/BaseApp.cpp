#include "BaseApp.h"

#include <iostream>
#include <algorithm>

using namespace fit;
using namespace std;

BaseApp::BaseApp() {
	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");

	mainLoop = make_shared<SDLEventProc>(true);

  mainWindow = addWindow();
  setupMainWindowEvents();


	mainLoop->setIdleCallback(bind(&BaseApp::handleIdle, this));

	mainLoop->setEventHandler([this](SDL_Event const&e)->bool {
		handleEvent(e);
		return true;
	});

	
	//resources 
	resourceDir = string(RESOURCE_DIR);
}

BaseApp::~BaseApp() {
}

int BaseApp::run() {
	try {
		for (auto &c : initCallbacks)c.callback();
		(*mainLoop)();
		for (auto &c : cleanupCallbacks)c.callback();
	}
	catch (const exception &e) {
		cerr << e.what() << "\n";
		waitForPress();
	}
	catch (...) {
		cout << "Unknown exception\n";
	}

	return 0;
}

void BaseApp::waitForPress() {
#ifdef WIN32
	system("pause");
#else
	cout << "Press ENTER to continue...\n";
	cin.get();
#endif
}

void BaseApp::quit() {
	mainLoop->quit();
}

SDLWindowShared BaseApp::addWindow(SDLWindowShared &window) {
	windows.push_back(window);
	mainLoop->addWindow(to_string(window->getId()), window);
	int w = window->getWidth();
	int h = window->getHeight();
	window->setSize(w, h);

  SDLWindowWeak ptr(window);
  addEventCallback([=](SDL_Event &e)  {
    if(e.window.event==SDL_WINDOWEVENT_CLOSE) {
      removeWindow(ptr.lock());      
    }
  },SDL_WINDOWEVENT,window->getId());
	return window;
}

SDLWindowShared BaseApp::addWindow(int width, int height, bool resizable, bool fullscreen, int multisample) {
	return addWindow(make_shared<SDLWindow>(width, height, resizable, fullscreen, multisample));
}

SDLWindowShared BaseApp::getWindowByID(int id) {
  for(auto &w:windows) {
    if (w->getId() == id)return w;
  }
  return nullptr;
}

void BaseApp::removeWindow(SDLWindowShared & window) {  
  window->hide();
  eventCallbacks.erase(remove_if(eventCallbacks.begin(), eventCallbacks.end(), [&](EventCallbackFilterID c) {return c.window == window->getId(); }), eventCallbacks.end());
  drawCallbacks.erase(remove_if(drawCallbacks.begin(), drawCallbacks.end(), [&](DrawCallbackFilterID c) {return c.window == window->getId(); }), drawCallbacks.end());
  mainLoop->removeWindow(to_string(window->getId()));
  windows.erase(remove(windows.begin(), windows.end(), window), windows.end());
}

void BaseApp::handleEvent(SDL_Event const & e) {
	//for (auto &ec : eventCallbacks) {
  for (int i = 0; i < eventCallbacks.size(); i++) {
    auto &ec = eventCallbacks.at(i);
		if ((ec.type == ANY_EVENT || ec.type == e.type) &&
			(ec.window == ANY_WINDOW || ec.window == e.window.windowID)) {

      if((e.type == SDL_MOUSEMOTION ||
        e.type == SDL_MOUSEBUTTONDOWN ||
        e.type ==SDL_MOUSEBUTTONUP ||
        e.type == SDL_MOUSEWHEEL ) && ImGui::GetIO().WantCaptureMouse) {
        continue;
      }

      if ((e.type == SDL_KEYDOWN ||
        e.type == SDL_KEYUP) && ImGui::GetIO().WantCaptureKeyboard) {
        continue;
      }
			ec.callback(e);
		}
	}
}

void BaseApp::handleIdle() {
	dt = timer.elapsedFromLast();
	for (auto&c : updateCallbacks)c.callback(dt);
	for (auto &w : windows) {
		for (auto &dc : drawCallbacks) {
			if (dc.window == w->getId()) dc.callback();
		}
	}
}

int BaseApp::addInitCallback(function<void()> const & callback) {
  int id = idGen++;
  initCallbacks.emplace_back(CallbackID{ id,callback });
  return id;
}

int BaseApp::addCleanupCallback(function<void()> const & callback) {
  int id = idGen++;
  cleanupCallbacks.emplace_back(CallbackID{ id,callback });
  return id;
}

int BaseApp::addUpdateCallback(function<void(float)> const & callback) {
  int id = idGen++;
  updateCallbacks.emplace_back(UpdateCallbackID{ id,callback });
  return id;
}

int BaseApp::addEventCallback(function<void(SDL_Event) > callback, int type, int window) {
  int id = idGen++;
  eventCallbacks.emplace_back(EventCallbackFilterID{ id, callback, type, window });
  return id;
}

int BaseApp::addDrawCallback(function<void()> const & callback, SDLWindowShared window) {
	if (window == nullptr)window = mainWindow;
  int id = idGen++;
  drawCallbacks.emplace_back(DrawCallbackFilterID{ id, callback, (int)window->getId()});
  return id;
}

int BaseApp::addResizeCallback(function<void(int, int) >f, int window) {
	return addEventCallback([f](SDL_Event const&e) {
		if (e.window.event == /*SDL_WINDOWEVENT_RESIZED*/SDL_WINDOWEVENT_SIZE_CHANGED) {
			f(e.window.data1, e.window.data2);
		}
	}, SDL_WINDOWEVENT, window);
}

int BaseApp::addMouseMoveCallback(function<void(int, int, int, int) >f, int window) {
	return addEventCallback([f](SDL_Event const&e) {
		f(e.motion.xrel, e.motion.yrel, e.motion.x, e.motion.y);
	}, SDL_MOUSEMOTION, window);
}

int BaseApp::addMousePressCallback(function<void(Uint8, int, int) >f, int window) {
	return addEventCallback([f](SDL_Event const&e) {
		f(e.button.button, e.button.x, e.button.y);
	}, SDL_MOUSEBUTTONDOWN, window);
}

int BaseApp::addMouseReleaseCallback(function<void(Uint8, int, int) >f, int window) {
	return addEventCallback([f](SDL_Event const&e) {
		f(e.button.button, e.button.x, e.button.y);
	}, SDL_MOUSEBUTTONUP, window);
}

int BaseApp::addMouseWheelCallback(function<void(int) > f, int window) {
	return addEventCallback([f](SDL_Event const&e) {
		f(e.wheel.y);
	}, SDL_MOUSEWHEEL, window);
}

int BaseApp::addKeyPressCallback(function<void(SDL_Keycode, Uint16) > f, int window) {
	return addEventCallback([f](SDL_Event const&e) {
		f(e.key.keysym.sym, e.key.keysym.mod);
	}, SDL_KEYDOWN, window);
}

int BaseApp::addKeyReleaseCallback(function<void(SDL_Keycode, Uint16) > f, int window) {
	return addEventCallback([f](SDL_Event const&e) {
		f(e.key.keysym.sym, e.key.keysym.mod);
	}, SDL_KEYUP, window);
}



template<typename T>
void removeCallbackID(T &vector, int id) {
  vector.erase(remove_if(vector.begin(), vector.end(), [&](typename T::value_type c) {return c.id == id; }), vector.end());
}

void BaseApp::removeCallback(int id) {
  removeCallbackID(initCallbacks, id);
  removeCallbackID(cleanupCallbacks, id);
  removeCallbackID(drawCallbacks, id);
  removeCallbackID(eventCallbacks, id);
  removeCallbackID(updateCallbacks, id);
}

void BaseApp::setupMainWindowEvents() {
	using namespace placeholders;
  SDLWindowShared mw = mainWindow;
	// init
	addInitCallback(bind(&BaseApp::init, this));
	addUpdateCallback(bind(&BaseApp::update, this, _1));
	// virtual function callbacks
	addResizeCallback(bind(&BaseApp::onResize, this, _1, _2), mw->getId());
	addMouseMoveCallback(bind(&BaseApp::onMouseMove, this, _1, _2, _3, _4), mw->getId());
	addMousePressCallback(bind(&BaseApp::onMousePress, this, _1, _2, _3), mw->getId());
	addMouseReleaseCallback(bind(&BaseApp::onMouseRelease, this, _1, _2, _3), mw->getId());
	addMouseWheelCallback(bind(&BaseApp::onMouseWheel, this, _1), mw->getId());
	addKeyPressCallback(bind(&BaseApp::onKeyPress, this, _1, _2), mw->getId());
	addKeyReleaseCallback(bind(&BaseApp::onKeyRelease, this, _1, _2), mw->getId());
	// close on ESC
	addKeyPressCallback([this](int key, int) {
		if (key == SDLK_ESCAPE)quit();
	});
	// close on window X
	addEventCallback([this](SDL_Event const&e) {
		quit(); }, SDL_QUIT);
	// draw
	addDrawCallback(bind(&BaseApp::draw, this), mw);
}


