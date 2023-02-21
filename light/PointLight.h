#pragma once

#include <glm/glm.hpp>
#include "Light.h"

// A directional light is a type of light that shying from the point source that set in the world

struct PointLight : public Light
{
public:
	PointLight(const glm::vec3& ambientColor, 
		const glm::vec3& diffuseColor, 
		const glm::vec3& specularColor, 
		const glm::vec3& position, 
		float constant, float linear, float quadratic);
	void Light::sendToShader(unsigned int shaderProgramID, unsigned int index) override;
	LightType Light::getLightType() const override;

public:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;
};