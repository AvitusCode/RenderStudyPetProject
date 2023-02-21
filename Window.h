#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <bitset>


class WindowManager
{
public:
	int init(
		std::string const& windowTitle, unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);

	void update();
	bool shouldClose();
	void swapBuffers();
	void pollEvents();
	void setWindowShouldClose();
	void shutdown();
	GLFWwindow* getWindow();

private:
	GLFWwindow* mWindow;
	std::bitset<8> mButtons;
};