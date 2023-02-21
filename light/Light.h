#pragma once
#include <glm/glm.hpp>

enum class LightType
{
	LIGHT_DIRLIGHT,
	LIGHT_POINTLIGHT,
	LIGHT_SPOTLIGHT
};

// Struct for light with ambient, diffuse and specular components
struct Light
{
public:
	Light();
	Light(const glm::vec3& aC, const glm::vec3& dC, const glm::vec3& sC);
	
	void virtual sendToShader(unsigned int shaderProgramID, unsigned int index) = 0;
	LightType virtual getLightType() const = 0;

	virtual ~Light() = default;

public:
	// The colors of each of the light's components
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};