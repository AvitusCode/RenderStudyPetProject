#pragma once
#include <glm/glm.hpp>

enum class LightType : uint8_t
{
	LIGHT_NOTHING,
	LIGHT_DIRLIGHT,
	LIGHT_POINTLIGHT,
	LIGHT_SPOTLIGHT
};

// Struct for light with ambient, diffuse and specular components
struct Light
{
	// The colors of each of the light's components
	LightType m_lightType = LightType::LIGHT_NOTHING;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};