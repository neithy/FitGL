#include <FitGL.h>
#include <bunny.h>

using namespace std;

int main(int /*argc*/, char ** /*argv*/) {
	OpenGLApp app;
	GLuint vao,vbo,ebo;
	ProgramShared program;
	
	auto mainWindow = app.getMainWindow();

	float lightDir = 0;
	float zoom = 4;
	float bunnyColor[3] = { 1.0,0.5,0.25};
	bool showDemo = false;
	ImVec2 optSize(200, 200);
	ImVec2 optPos(20, 40);

  PerspectiveCameraShared camera = make_shared<PerspectiveCamera>();
  OrbitManipulatorShared manipulator = make_shared<OrbitManipulator>(camera);
  manipulator->setupCallbacks(app);

	string prefix = APP_RESOURCE_DIR;
	app.addInitCallback([&]() {
		auto vs = compileShader(GL_VERTEX_SHADER, Loader::text(prefix + "lambert.vert"));
		auto fs = compileShader(GL_FRAGMENT_SHADER, Loader::text(prefix + "lambert.frag"));
		program = createProgram(vs, fs);

		bunnyInit(vao, vbo, ebo);
	});

	app.addDrawCallback([&]() {
		int w = mainWindow->getWidth();
		int h = mainWindow->getHeight();
		glViewport(0, 0, w, h); 
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		//bunny
		float lightDist = 50;
		vec3 lightPos(cos(lightDir), 1, sin(lightDir));
		lightPos = lightPos*lightDist;
		program->use();
		program->set3fv("color", bunnyColor);
		program->setMatrix4fv("p", value_ptr(camera->getProjection()));
		program->setMatrix4fv("v", value_ptr(camera->getView()));
		program->set3fv("lightPosition", value_ptr(lightPos));

		bunnyDraw(vao);

		// GUI
		using namespace ImGui;
		label("FPS: " + std::to_string(GetIO().Framerate));		

		Begin("Options",0,optSize);

		ImGui::SliderFloat("Light dir", &lightDir, 0.0f, 2*pi<float>());


		ImGui::ColorEdit3("Bunny color", bunnyColor);

		if (Button("show demo menu")) showDemo ^= 1;
		End();

		if (showDemo) {
			ImGui::ShowTestWindow(&showDemo);
		}
		

	});
	return app.run();
}
