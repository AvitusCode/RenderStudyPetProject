#include <iostream>

#include "interface/imgui/imgui.h"
#include "interface/imgui/imgui_impl_glfw.h"
#include "interface/imgui/imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "render/Shader.h"
#include "render/Model.h"
#include "render/Mesh.h"
#include "render/SkyBox.h"
#include "render/Texture.h"
#include "objects/Camera.h"
#include "interface/interface.h"
#include "interface/input.h"
#include "render/Cubemap.h"
#include "render/Renderer.h"
#include "light/DirLight.h"
#include "light/PointLight.h"
#include "light/SpotLight.h"
#include "support/uniform_buffer.h"

#include "scene/SceneManager.h"
#include "scene/scene_1.h"
#include "scene/scene_2.h"
#include "scene/scene_3.h"
#include "scene/scene_4.h"
#include "scene/scene_buffers.h"
#include "Window.h"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

int windowWidth = WINDOW_WIDTH;
int windowHeight = WINDOW_HEIGHT;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLFWwindow* window;
SceneManager* ptr_sm;

int setupGlfwContext(WindowManager& wm)
{
	int res = wm.init("OpenGl Pet-Project", WINDOW_WIDTH, WINDOW_HEIGHT);
	window = wm.getWindow();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	return res;
}

int main(void)
{
	WindowManager wm;

	if (setupGlfwContext(wm) != 0){
		return -1;
	}

	// Here we can set all neccessary scenes
	SceneManager sm{};
	ptr_sm = &sm;
	sm.Add<scene_1>("scene_1");
	sm.Add<scene_2>("scene_2");
	sm.Add<scene_3>("scene_3");
	sm.Add<scene_4>("scene_4");
	sm.Add<scene_buffers>("scene_buffers");
	sm.SetScene(0);

	// Render loop
	while (!wm.shouldClose())
	{
		// Calculates elapsed time since last frame for time-based calculations
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Processes any mouse or keyboard input for camera movement
		processInput(window, deltaTime);

		sm.OnUpdate();

		int glErrorCurrent = glGetError();
		if (glErrorCurrent != 0) { 
			std::cerr << "OpenGL error: " << glErrorCurrent << std::endl;
		}

		wm.swapBuffers();
		wm.pollEvents();
	}

	// Terminate process
	sm.OnDispose();
	wm.shutdown();

	return 0;
}