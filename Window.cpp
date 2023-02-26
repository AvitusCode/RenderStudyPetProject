#include "Window.h"
#include <glad/glad.h>
#include <iostream>

#include "main.h"

#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/WindowEvent.h"

int WindowManager::init(std::string const& windowTitle, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT)
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

	glfwSetWindowUserPointer(mWindow, this);
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	glfwSetCursorPosCallback(mWindow, mouse_callback);
	glfwSetScrollCallback(mWindow, scroll_callback);
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetWindowCloseCallback(mWindow, close_callback);

	lastX = (float)windowWidth / 2;
	lastY = (float)windowHeight / 2;
	lastWidth = windowWidth;
	lastHeight = windowHeight;

	// Default event dispatcher
	// In the correct version, this function should be 
	// supplied to the WindowManager::init(...) input so that each WindowManager class 
	// can have its own unique dispatcher. 
	// This implementation is presented as an example.
	setCallback([this](Event* event) {
		switch (event->getType())
		{
		case Event::EventType::MOUSE_MOVE:
		{
			auto data = static_cast<MouseMovedEvent*>(event);
			if (showCursor) {
				return;
			}

			if (firstMouse)
			{
				lastX = static_cast<float>(data->getX());
				lastY = static_cast<float>(data->getY());
				firstMouse = false;
			}

			float xoffset = static_cast<float>(data->getX() - lastX);
			float yoffset = static_cast<float>(lastY - data->getY());

			lastX = static_cast<float>(data->getX());
			lastY = static_cast<float>(data->getY());

			camera.ProcessMouseMovement(xoffset, yoffset);
		}
		    break;
		case Event::EventType::MOUSE_SCROLL:
		{
			// If cursor is shown (interacting with UI), don't change FOV
			if (showCursor) {
				return;
			}

			auto data = static_cast<MouseScrolledEvent*>(event);
			camera.ProcessMouseScroll(data->getOffset());
		}
		    break;

		case Event::EventType::KEY_PRESSED:
		{
			auto data = static_cast<KeyPressedEvent*>(event);
			int key = data->getKey();
			int action = data->getAction();

			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(mWindow, GL_TRUE);
			}

			// Toggle to show/hide mouse cursor
			if (key == GLFW_KEY_Q && action == GLFW_PRESS)
			{
				showCursor = !showCursor;
				if (showCursor)
				{
					glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					firstMouse = true;
				}
				else {
					glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
			}

			// Toggle to set next scene
			if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
				
			}

			// Toggle to enable/disable wireframe drawing
			if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
			{
				wireframeMode = !wireframeMode;
				if (wireframeMode) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
			}

			// Toggle to go in or out of fullscreen mode
			if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
			{
				GLFWmonitor* monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);

				toggleFullscreen = !toggleFullscreen;
				if (toggleFullscreen)
				{
					// Saves windowed mode resolution
					lastWidth = windowWidth;
					lastHeight = windowHeight;
					// Saves windowed mode coordinates
					glfwGetWindowPos(mWindow, &lastXPos, &lastYPos);

					// Sets window to fullscreen mode and updates viewport
					glfwSetWindowMonitor(mWindow, monitor, 50, 50, mode->width, mode->height, mode->refreshRate);
					glViewport(0, 0, mode->width, mode->height);

					// Sets window resolution values to monitor resolution
					windowWidth = mode->width;
					windowHeight = mode->height;
				}
				else
				{
					// Sets window to windowed mode and updates viewport
					glfwSetWindowMonitor(mWindow, nullptr, lastXPos, lastYPos, lastWidth, lastHeight, mode->refreshRate);
					glViewport(0, 0, lastWidth, lastHeight);

					// Sets window resolution values to last saved windowed values
					windowWidth = lastWidth;
					windowHeight = lastHeight;
				}
			}
		}
		break;

		case Event::EventType::WINDOW_RESIZE:
		{
			auto data = static_cast<WindowResizedEvent*>(event);
			windowWidth = data->getWidth();
			windowHeight = data->getHeight();
		}
		    break;

		case Event::EventType::WINDOW_CLOSE:
			glfwSetWindowShouldClose(mWindow, GL_TRUE);
			break;
		}
		});

	return 0;
}

void WindowManager::setCallback(const std::function<void(Event*)>& fn) { 
	fnCallback = fn; 
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

void WindowManager::processInput(float deltaTime)
{
	// If cursor is shown (interacting with UI), don't handle movement
	if (showCursor) {
		return;
	}

	// Forward movement
	if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	// Backward movement
	if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	// Left movement
	if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	// Right movement
	if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
}

void WindowManager::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	MouseMovedEvent event(xpos, ypos);
	handle.fnCallback(&event);
}

void WindowManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	MouseScrolledEvent event(static_cast<float>(yoffset));
	handle.fnCallback(&event);
}

void WindowManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	KeyPressedEvent event(key, action);
	handle.fnCallback(&event);
}

void WindowManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	WindowResizedEvent event(width, height);
	handle.fnCallback(&event);

	glViewport(0, 0, width, height);
}

void WindowManager::close_callback(GLFWwindow* window) {
	auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	WindowClosedEvent event;
	handle.fnCallback(&event);
}