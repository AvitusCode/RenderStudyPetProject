#pragma once

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

struct GLFWwindow;

// Callback function to register mouse input
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// Callback function to register scrolling
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// Used to register only first key press of a given key
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// Used to process various key presses like movement keys
void processInput(GLFWwindow* window, float deltaTime);
// Callback function to handle window size change
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
