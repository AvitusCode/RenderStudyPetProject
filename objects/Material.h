#pragma once
#include "../render/Texture.h"
#include <glm/glm.hpp>


struct Material
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	Texture texture;

	Material();
	Material(const Texture& texture);
	Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, const Texture& texture);
};