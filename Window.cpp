#include "Window.h"
#include <glad/glad.h>
#include <iostream>

#include "main.h"

int WindowManager::init(
	std::string const& windowTitle, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT)
{
	windowWidth = WINDOW_WIDTH;
	windowHeight = WINDOW_HEIGHT;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowTitle.c_str(), NULL, NULL);
	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	return 0;
}

GLFWwindow* WindowManager::getWindow() {
	return mWindow;
}

void WindowManager::update(){
	glfwGetFramebufferSize(mWindow, &windowWidth, &windowHeight);
	glfwSwapBuffers(mWindow);
}

void WindowManager::setWindowShouldClose() {
	glfwSetWindowShouldClose(mWindow, true);
}

void WindowManager::shutdown(){
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

bool WindowManager::shouldClose() {
	return glfwWindowShouldClose(mWindow);
}

void WindowManager::swapBuffers() {
	glfwSwapBuffers(mWindow);
}

void WindowManager::pollEvents() {
	glfwPollEvents();
}