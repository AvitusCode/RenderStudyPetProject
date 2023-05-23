#pragma once

struct SpotLight
{
	glm::vec3 direction;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};