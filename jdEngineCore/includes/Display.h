#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

class EventComponent;

class Display final
{
public:
	Display(const Display&) = delete;
	Display(Display&&) = delete;
	Display& operator=(const Display&) = delete;
	Display& operator=(Display&&) = delete;

	/// <summary>
	/// Main initialisation func
	/// </summary>
	/// <param name="windowTitle"> Window Name </param>
	/// <param name="WINDOW_WIDTH"> Window Width </param>
	/// <param name="WINDOW_HEIGHT"> Window Height </param>
	/// <returns> 0 if all OK </returns>
	int init(std::string const& windowTitle, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

	static Display& getDisplay() {
		static Display display{};
		return display;
	}

	void update();
	bool shouldClose();
	void swapBuffers();
	void pollEvents();
	void shutdown();

	void setVerticalSinhronization(bool enable);

	// Events
	void processInput(EventComponent*);
	void setWindowShouldClose(EventComponent*);
	void resizeWindow(EventComponent*);
	void mouseWindow(EventComponent*);

	int updateFPS(double curTime);
	void processRealTime();

	inline GLFWwindow* getGLFWwindow() const { return mWindow; }
	inline int getWidth() const { return windowWidth; }
	inline int getHeight() const { return windowHeight; }
	
	struct DisplayConfigs
	{
		// support data
		float mousePosX = 0;
		float mousePosY = 0;

		bool showCursor = false;
		bool wireframeMode = false;
		bool firstMouse = true;
		bool toggleFullscreen = false;

		// When toggling fullscreen on, save the resolution of window
		// If fullscreen is then toggled off, set window size to these values
		int lastWidth = 0;
		int lastHeight = 0;
		// Used to save the last coordinates of the windowed mode window
		int lastXPos = 50;
		int lastYPos = 50;
		bool m_isVerticalSynchronizationEnabled = false;
	};

	inline DisplayConfigs& getConfigs() { return displayConfigs; }
private:
	Display() = default;
private:
	GLFWwindow* mWindow = nullptr;

	// Main metrics
	int windowWidth = 0;
	int windowHeight = 0;

	//Fps calcs
	double prevTime = 0.0;
	double prevTimeFPS = 0.0;
	double deltaTime = 0.0;
	int m_FPS = 0; 
	int m_nextFPS = 0; 

	DisplayConfigs displayConfigs;

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
	// Callback function to register mouse input
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};