#include <FitGL.h>
#include "bunny.h"

using namespace std;

int main(int /*argc*/, char ** /*argv*/) {
	OpenGLApp app;
	ProgramShared program;

	auto mainWindow = app.getMainWindow();
	
	std::string prefix = APP_RESOURCE_DIR;

  PerspectiveCameraShared camera = make_shared<PerspectiveCamera>();
  OrbitManipulatorShared manipulator = make_shared<OrbitManipulator>(camera);
	manipulator->setupCallbacks(app);	

  GLuint vao, vbo, ebo;

	app.addInitCallback([&]() {
		auto vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "lambert.vert"));
		auto fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "lambert.frag"));
		program = createProgram(vs, fs);

    bunnyInit(vao, vbo, ebo);
	});

	app.addResizeCallback([&](int w, int h) {
		glViewport(0, 0, w, h);
		camera->setAspect(float(w) / float(h));
	});
	
	app.addDrawCallback([&]() {
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//bunny

    program->use();
		program->setMatrix4fv("p", value_ptr(camera->getProjection()));
		program->setMatrix4fv("v", value_ptr(camera->getView()));

    bunnyDraw(vao);

		fpsLabel();
		label("Orbit manipulator:\nWSAD - Move center\nEQ - Up/Down\nRMB/LMB drag - rotate\nMMB drag - move center\nWheel - zoom",0,20,200,200);
	});
	return app.run();
}
