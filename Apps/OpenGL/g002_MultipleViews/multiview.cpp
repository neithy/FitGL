#include <FitGL.h>

#include <bunny.h>

using namespace glm;

int main(int /*argc*/, char ** /*argv*/) {
	OpenGLApp app;	
  std::shared_ptr<Program> program;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	std::string prefix = APP_RESOURCE_DIR;
	auto mainWindow = app.getMainWindow();

	app.addInitCallback([&]() {
		auto vs = std::make_shared<Shader>(GL_VERTEX_SHADER, Loader::text(prefix+"lambert.vert"));
		auto fs = std::make_shared<Shader>(GL_FRAGMENT_SHADER, Loader::text(prefix + "lambert.frag"));
    program = std::make_shared<Program>(vs, fs);

		bunnyInit(vao,vbo,ebo);
	});

	app.addDrawCallback([&]() {
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
    program->use();

		int w = mainWindow->getWidth();
		int h = mainWindow->getHeight();

		mat4 p = perspective(radians(45.0f),float(w)/float(h), 0.1f, 1000.0f);
    program->setMatrix4fv("p", value_ptr(p));
		int z = 4;
		mat4 v = lookAt(vec3(z,z,z), vec3(0, 0, 0), vec3(0, 1, 0));
    program->setMatrix4fv("v", value_ptr(v));

		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		
		
		glViewport(w / 2, h / 2, w / 2, h / 2);
		bunnyDraw(vao);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		v = lookAt(vec3(z, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0));
    program->setMatrix4fv("v", value_ptr(v));
		glViewport(0, 0, w / 2, h / 2);
		bunnyDraw(vao);


		v = lookAt(vec3(0, z, 0), vec3(0, 0, 0), vec3(1, 0, 0));
    program->setMatrix4fv("v", value_ptr(v));
		glViewport(0, h / 2, h / 2, h / 2);
		bunnyDraw(vao);


		v = lookAt(vec3(0, 0, z), vec3(0, 0, 0), vec3(0, 1, 0));
    program->setMatrix4fv("v", value_ptr(v));
		glViewport(w / 2, 0, w / 2, h / 2);
		bunnyDraw(vao);
	});
	return app.run();
}
