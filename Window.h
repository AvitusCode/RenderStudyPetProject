#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include "objects/Camera.h"

class Event;

class WindowManager final
{
public:
	WindowManager() = default;
	WindowManager(const WindowManager&) = delete;
	WindowManager(WindowManager&&) = delete;
	WindowManager& operator=(const WindowManager&) = delete;
	WindowManager& operator=(WindowManager&&) = delete;

	int init(std::string const& windowTitle, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

	void update();
	bool shouldClose();
	void swapBuffers();
	void pollEvents();
	void setWindowShouldClose();
	void shutdown();
	void processInput(float deltaTime);
	void setCallback(const std::function<void(Event*)>& fn);

	__forceinline GLFWwindow* getGLFWwindow() const { return mWindow; }
	Camera& getCamera() { return camera; }
	int getWidth() const { return windowWidth; }
	int getHeight() const { return windowHeight; }

private:
	GLFWwindow* mWindow;
	int windowWidth;
	int windowHeight;

	Camera camera;
	std::function<void(Event*)> fnCallback;

	// support data
	float lastX;
	float lastY;

	bool showCursor = false;
	bool wireframeMode = false;
	bool firstMouse = true;
	bool toggleFullscreen = false;

	// When toggling fullscreen on, save the resolution of window
	// If fullscreen is then toggled off, set window size to these values
	int lastWidth;
	int lastHeight;
	// Used to save the last coordinates of the windowed mode window
	int lastXPos = 50;
	int lastYPos = 50;

	// Callback function to register mouse input
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	// Callback function to register scrolling
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	// Used to register only first key press of a given key
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	// Callback function to handle window size change
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	// Callback function to register window`s close signal
	static void close_callback(GLFWwindow* window);
};