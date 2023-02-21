#pragma once
#include "Light.h"

// A directional light is a type of light that lights up every object in the world from a given direction

struct DirectionalLight : public Light
{
public:
	DirectionalLight(const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor, const glm::vec3& direction);
	void Light::sendToShader(unsigned int shaderProgramID, unsigned int index) override;
	LightType Light::getLightType() const override;

public:
	glm::vec3 direction;
};