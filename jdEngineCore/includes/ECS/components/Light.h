#pragma once
#include <glm/glm.hpp>
#include <variant>

struct DirLight
{
	glm::vec3 direction;
};

struct PointLight
{
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	glm::vec3 direction;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

// Struct for light with ambient, diffuse and specular components
struct Light
{
	// The colors of each of the light's components
	std::variant<std::monostate, DirLight, PointLight, SpotLight> lightType;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};