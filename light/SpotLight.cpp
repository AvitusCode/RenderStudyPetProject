#include <string>
#include <glad/glad.h>
#include "SpotLight.h"

SpotLight::SpotLight(const glm::vec3& ambientColor, const glm::vec3& diffuseColor, const glm::vec3& specularColor, const glm::vec3& position,
	float constant, float linear, float quadratic, 
	const glm::vec3& direction, 
	float cutOff, float outerCutOff) : Light(ambientColor, diffuseColor, specularColor)
{
	this->position = position;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;

	this->direction = direction;
	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;
}

void SpotLight::sendToShader(unsigned int shaderProgramID, unsigned int index)
{
	std::string lightLocation = "spotLights[" + std::to_string(index) + "]";

	std::string ambientLoc = lightLocation + ".ambientColor";
	std::string diffuseLoc = lightLocation + ".diffuseColor";
	std::string specularLoc = lightLocation + ".specularColor";
	std::string positionLoc = lightLocation + ".position";
	std::string directionLoc = lightLocation + ".direction";
	std::string constantLoc = lightLocation + ".constant";
	std::string linearLoc = lightLocation + ".linear";
	std::string quadraticLoc = lightLocation + ".quadratic";
	std::string cutOffLoc = lightLocation + ".cutOff";
	std::string outerCutOffLoc = lightLocation + ".outerCutOff";

	glUniform3fv(glGetUniformLocation(shaderProgramID, ambientLoc.c_str()), 1, &ambientColor[0]);
	glUniform3fv(glGetUniformLocation(shaderProgramID, diffuseLoc.c_str()), 1, &diffuseColor[0]);
	glUniform3fv(glGetUniformLocation(shaderProgramID, specularLoc.c_str()), 1, &specularColor[0]);
	glUniform3fv(glGetUniformLocation(shaderProgramID, positionLoc.c_str()), 1, &position[0]);
	glUniform3fv(glGetUniformLocation(shaderProgramID, directionLoc.c_str()), 1, &direction[0]);
	glUniform1f(glGetUniformLocation(shaderProgramID, constantLoc.c_str()), constant);
	glUniform1f(glGetUniformLocation(shaderProgramID, linearLoc.c_str()), linear);
	glUniform1f(glGetUniformLocation(shaderProgramID, quadraticLoc.c_str()), quadratic);
	glUniform1f(glGetUniformLocation(shaderProgramID, cutOffLoc.c_str()), cutOff);
	glUniform1f(glGetUniformLocation(shaderProgramID, outerCutOffLoc.c_str()), outerCutOff);
}

LightType SpotLight::getLightType() const {
	return LightType::LIGHT_SPOTLIGHT;
}