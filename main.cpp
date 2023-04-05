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
#include "scene/scene_tesselation.h"
#include "Window.h"

float deltaTime = 0.0f;

// TODO: tesselation fix

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

int main(void)
{
	WindowManager wm;

	if (wm.init("OpenGl Pet-Project", 1920, 1080)){
		return -1;
	}

	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, 0);

	// Here we can set all neccessary scenes
	SceneManager sm{};
	sm.Add<scene_1>("scene_1", wm.getGLFWwindow());
	sm.Add<scene_2>("scene_2", wm.getGLFWwindow());
	sm.Add<scene_3>("scene_3", wm.getGLFWwindow());
	sm.Add<scene_4>("scene_4", wm.getGLFWwindow());
	sm.Add<scene_buffers>("scene_buffers", wm.getGLFWwindow());
	sm.Add<scene_tesselation>("scene_tesselation", wm.getGLFWwindow()); // i = 5
	sm.SetScene(1);

	float lastFrame = 0.0f;
	// Render loop
	while (!wm.shouldClose())
	{
		// Calculates elapsed time since last frame for time-based calculations
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Processes any mouse or keyboard input for camera movement
		wm.processInput(deltaTime);

		sm.OnUpdate(currentFrame);

		int glErrorCurrent = glGetError();
		if (glErrorCurrent != 0) { 
			std::cerr << "OpenGL error: " << glErrorCurrent << std::endl;
		}

		wm.swapBuffers();
		wm.pollEvents();
	}

	// Terminate process (fix problem)
	sm.OnDispose();
	wm.shutdown();

	return 0;
}