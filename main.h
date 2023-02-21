#pragma once
// This is the main file, responsible for importing other header files, setting up context, necessary objects, and executing the render loop

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "objects/Camera.h"
#include "scene/SceneManager.h"

extern int windowWidth;
extern int windowHeight;

extern Camera camera;
extern GLFWwindow* window;
extern SceneManager* ptr_sm;
