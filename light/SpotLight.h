#pragma once
#include "Light.h"

#include <glm/glm.hpp>

// A spot light is a type of light placed at a xyz point in space that emits a cone of light in a specific direction

struct SpotLight : public Light
{
public:
	SpotLight(const glm::vec3& ambientColor, 
		const glm::vec3& diffuseColor, 
		const glm::vec3& specularColor, 
		const glm::vec3& position,
		float constant, float linear, float quadratic, 
		const glm::vec3& direction, 
		float cutOff, float outerCutOff);

	void Light::sendToShader(unsigned int shaderProgramID, unsigned int index) override;
	LightType Light::getLightType() const override;

public:
	glm::vec3 position;
	glm::vec3 direction;

	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};