#pragma once
#include <glm/glm.hpp>

struct Gravity
{
	glm::vec3 force{0.0f, -9.8f, 0.0f}; // standart Earth gravity constatn
};

struct Gravity2D
{
	glm::vec2 force{0.0f, -9.8f}; // standart Earth gravity constatn
};
