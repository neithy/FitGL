#include <FitGL.h>

#include <bunny.h>

using namespace glm;

int main(int /*argc*/, char ** /*argv*/) {
	OpenGLApp app;	
  std::shared_ptr<Program> program;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	mat4 p;
	int z = 4;
	auto window0 = app.getMainWindow();
	window0->setSize(200, 200);
	window0->move(100, 100);
	auto window1 = app.addWindow(200, 200);
	window1->move(100, 300);
	auto window2 = app.addWindow(200, 200);
	window2->move(300, 100);
	auto window3 = app.addWindow(200, 200);
	window3->move(300, 300);

  std::string prefix = APP_RESOURCE_DIR;

	app.addInitCallback([&]() {
    auto vs = std::make_shared<Shader>(GL_VERTEX_SHADER, Loader::text(prefix + "lambert.vert"));
    auto fs = std::make_shared<Shader>(GL_FRAGMENT_SHADER, Loader::text(prefix + "lambert.frag"));
    program = std::make_shared<Program>(vs, fs);

		bunnyInit(vao, vbo, ebo);
	});

	app.addDrawCallback([&]() {
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
    program->use();

		int w = window0->getWidth();
		int h = window0->getHeight();
		glViewport(0, 0, w, h);
		p = perspective(radians(45.0f),float(w)/float(h), 0.1f, 1000.0f);
    program->setMatrix4fv("p", value_ptr(p));
		int z = 4;
		mat4 v = lookAt(vec3(z,z,z), vec3(0, 0, 0), vec3(0, 1, 0));
    program->setMatrix4fv("v", value_ptr(v));

		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
		
		bunnyDraw(vao);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	});

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int w = window1->getWidth();
		int h = window1->getHeight();
		glViewport(0, 0, w, h);
		mat4 v = lookAt(vec3(z, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0));
    program->setMatrix4fv("v", value_ptr(v));
		bunnyDraw(vao);
	}, window1);
	
	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int w = window2->getWidth();
		int h = window2->getHeight();
		glViewport(0, 0, w, h);
		mat4 v = lookAt(vec3(0, z, 0), vec3(0, 0, 0), vec3(1, 0, 0));
    program->setMatrix4fv("v", value_ptr(v));
		bunnyDraw(vao);
	}, window2);

	app.addDrawCallback([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int w = window3->getWidth();
		int h = window3->getHeight();
		glViewport(0, 0, w, h);
		mat4 v = lookAt(vec3(0, 0, z), vec3(0, 0, 0), vec3(0, 1, 0));
    program->setMatrix4fv("v", value_ptr(v));
		bunnyDraw(vao);
	}, window3);
	return app.run();
}
