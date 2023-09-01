#include "Display.h"
#include <iostream>

#include "ECS/jdEngine.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

int Display::init(std::string const& windowTitle, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT)
{
	if (mWindow) {
		std::cerr << "ERROR" << std::endl; // log it
		return -1;
	}
	windowWidth = WINDOW_WIDTH;
	windowHeight = WINDOW_HEIGHT;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowTitle.c_str(), nullptr, nullptr);
	if (mWindow == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
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
	glfwSetMouseButtonCallback(mWindow, mouse_button_callback);

	displayConfigs.mousePosX = (float)windowWidth / 2;
	displayConfigs.mousePosY = (float)windowHeight / 2;
	displayConfigs.firstMouse = true;
	displayConfigs.lastWidth = windowWidth;
	displayConfigs.lastHeight = windowHeight;

	auto& jd_engine = jd::Engine::getEngine();
	jd_engine.AddEventListener(METHOD_LISTENER(Events::Window::INPUT, Display::processInput));
	jd_engine.AddEventListener(METHOD_LISTENER(Events::Window::QUIT, Display::setWindowShouldClose));
	jd_engine.AddEventListener(METHOD_LISTENER(Events::Window::MOUSE, Display::mouseWindow));

	return 0;
}

int Display::updateFPS(double curTime)
{
	deltaTime = curTime - prevTime;
	prevTime = curTime;
	m_nextFPS++;

	if (prevTime - prevTimeFPS > 1.0)
	{
		prevTimeFPS = curTime;
		m_FPS = m_nextFPS;
		m_nextFPS = 0;
	}

	std::string str = "jdEngine, FPS: " + std::to_string(m_FPS);
	glfwSetWindowTitle(mWindow, str.c_str());

	return m_FPS;
}

void Display::update() {
	glfwGetFramebufferSize(mWindow, &windowWidth, &windowHeight);
	glfwSwapBuffers(mWindow);
}

void Display::shutdown() {
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

bool Display::shouldClose() {
	return glfwWindowShouldClose(mWindow);
}

void Display::swapBuffers() {
	glfwSwapBuffers(mWindow);
}

void Display::pollEvents() {
	glfwPollEvents();
}

void Display::setWindowShouldClose(EventComponent*) {
	glfwSetWindowShouldClose(mWindow, true);
}

void Display::resizeWindow(EventComponent* event)
{
	WindowResizedEvent* wre = static_cast<WindowResizedEvent*>(event);

	windowWidth = wre->getWidth();
	windowHeight = wre->getHeight();
}

void Display::mouseWindow(EventComponent* event)
{
	MouseMovedEvent* mme = static_cast<MouseMovedEvent*>(event);

	displayConfigs.mousePosX = mme->getX();
	displayConfigs.mousePosY = mme->getY();
}

void Display::processInput(EventComponent* event)
{
	KeyPressedEvent* data = static_cast<KeyPressedEvent*>(event);
	int key = data->getKey();
	int action = data->getAction();

	// Toggle to show/hide mouse cursor
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		displayConfigs.showCursor = !displayConfigs.showCursor;
		if (displayConfigs.showCursor)
		{
			glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			displayConfigs.firstMouse = true;
			jd::Engine::ConsoleDebug();
		}
		else 
		{
			glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			jd::Engine::ConsoleDebug();
		}
	}

	// Toggle to enable/disable wireframe drawing
	if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
	{
		displayConfigs.wireframeMode = !displayConfigs.wireframeMode;
		if (displayConfigs.wireframeMode) {
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

		displayConfigs.toggleFullscreen = !displayConfigs.toggleFullscreen;
		if (displayConfigs.toggleFullscreen)
		{
			// Saves windowed mode resolution
			displayConfigs.lastWidth = windowWidth;
			displayConfigs.lastHeight = windowHeight;
			// Saves windowed mode coordinates
			glfwGetWindowPos(mWindow, &displayConfigs.lastXPos, &displayConfigs.lastYPos);

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
			glfwSetWindowMonitor(
				mWindow,
				nullptr,
				displayConfigs.lastXPos,
				displayConfigs.lastYPos,
				displayConfigs.lastWidth,
				displayConfigs.lastHeight,
				mode->refreshRate);

			glViewport(0, 0, displayConfigs.lastWidth, displayConfigs.lastHeight);

			// Sets window resolution values to last saved windowed values
			windowWidth = displayConfigs.lastWidth;
			windowHeight = displayConfigs.lastHeight;
		}
	}

}

void Display::setVerticalSinhronization(bool enable) 
{
	glfwSwapInterval(enable ? 1 : 0);
	displayConfigs.m_isVerticalSynchronizationEnabled = enable;
}

void Display::processRealTime()
{
	static bool bW = false;
	static bool bA = false;
	static bool bS = false;
	static bool bD = false;

	if (displayConfigs.showCursor) {
		return;
	}

	auto& jd_engine = jd::Engine::getEngine();

	// Forward movement
	if (int what = glfwGetKey(mWindow, GLFW_KEY_W); what == GLFW_PRESS && !bW) {
		KeyPressedEvent event(GLFW_KEY_W, GLFW_PRESS);
		jd_engine.SendEvent(&event);
		bW = true;
	}
	else if (bW && what == GLFW_RELEASE) {
		KeyPressedEvent event(GLFW_KEY_W, GLFW_RELEASE);
		jd_engine.SendEvent(&event);
		bW = false;
	}
	// Backward movement
	if (int what = glfwGetKey(mWindow, GLFW_KEY_S); what == GLFW_PRESS && !bS) {
		KeyPressedEvent event(GLFW_KEY_S, GLFW_PRESS);
		jd_engine.SendEvent(&event);
		bS = true;
	}
	else if (bS && what == GLFW_RELEASE) {
		KeyPressedEvent event(GLFW_KEY_W, GLFW_RELEASE);
		jd_engine.SendEvent(&event);
		bS = false;
	}
	// Left movement
	if (int what = glfwGetKey(mWindow, GLFW_KEY_A); what == GLFW_PRESS && !bA) {
		KeyPressedEvent event(GLFW_KEY_A, GLFW_PRESS);
		jd_engine.SendEvent(&event);
		bA = true;
	}
	else if (bA && what == GLFW_RELEASE) {
		KeyPressedEvent event(GLFW_KEY_W, GLFW_RELEASE);
		jd_engine.SendEvent(&event);
		bA = false;
	}
	// Right movement
	if (int what = glfwGetKey(mWindow, GLFW_KEY_D); what == GLFW_PRESS && !bD) {
		KeyPressedEvent event(GLFW_KEY_D, GLFW_PRESS);
		jd_engine.SendEvent(&event);
		bD = true;
	}
	else if (bD && what == GLFW_RELEASE) {
		KeyPressedEvent event(GLFW_KEY_W, GLFW_RELEASE);
		jd_engine.SendEvent(&event);
		bD = false;
	}
}

void Display::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto& jd_engine = jd::Engine::getEngine();
	MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
	jd_engine.SendEvent(&event);
}

void Display::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto& jd_engine = jd::Engine::getEngine();
	MouseScrolledEvent event(static_cast<float>(yoffset));
	jd_engine.SendEvent(&event);
}

void Display::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		// empty
	}
}

void Display::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto& jd_engine = jd::Engine::getEngine();
	if (action == GLFW_PRESS) {
		KeyPressedEvent event(key, action);
		jd_engine.SendEvent(&event);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		WindowClosedEvent wc_event;
		jd_engine.SendEvent(&wc_event);
	}
}

void Display::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	auto& jd_engine = jd::Engine::getEngine();
	WindowResizedEvent event(width, height);
	
	jd_engine.SendEvent(&event);

	glViewport(0, 0, width, height);
}

void Display::close_callback(GLFWwindow* window) {
	auto& jd_engine = jd::Engine::getEngine();
	WindowClosedEvent event;
	jd_engine.SendEvent(&event);
}