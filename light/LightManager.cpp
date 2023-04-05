#include <iostream>
#include <glad/glad.h>
#include "LightManager.h"

LightManager::LightManager() : shaderProgramID(0), nrDirLights(0), nrPointLights(0), nrSpotLights(0)
{
}

LightManager::LightManager(unsigned int spID) : shaderProgramID(spID), nrDirLights(0), nrPointLights(0), nrSpotLights(0)
{
}

unsigned int LightManager::getShaderId() const {
	return shaderProgramID;
}

void LightManager::setShaderId(unsigned int id){
	shaderProgramID = id;
}

void LightManager::addLight(Light* lightPtr){
	lights.push_back(lightPtr);
}

void LightManager::init()
{
	// Make sure the count of each light is set to 0 in case the LightManager gets initialized again
	this->nrDirLights = 0;
	this->nrPointLights = 0;
	this->nrSpotLights = 0;

	for (Light* light : lights)
	{
		switch (light->getLightType())
		{
		case LightType::LIGHT_DIRLIGHT:
			light->sendToShader(shaderProgramID, nrDirLights++);
			break;

		case LightType::LIGHT_POINTLIGHT:
			light->sendToShader(shaderProgramID, nrPointLights++);
			break;

		case LightType::LIGHT_SPOTLIGHT:
			light->sendToShader(shaderProgramID, nrSpotLights++);
			break;
		}
	}

	int checkId = glGetUniformLocation(shaderProgramID, "nrDirLights");
	if (checkId != -1) {
	    glUniform1i(checkId, nrDirLights);
    }

	checkId = glGetUniformLocation(shaderProgramID, "nrPointLights");
	if (checkId != -1) {
		glUniform1i(checkId, nrPointLights);
	}

	checkId = glGetUniformLocation(shaderProgramID, "nrSpotLights");
	if (checkId != -1) {
		glUniform1i(checkId, nrSpotLights);
	}
}

// TODO: 

std::vector<DirectionalLight*> LightManager::getDirLights() const
{
	std::vector<DirectionalLight*> dirLights;

	for (Light* light : lights)
	{
		if (light->getLightType() == LightType::LIGHT_DIRLIGHT)
		{
			DirectionalLight* lightPtr = dynamic_cast<DirectionalLight*>(light);

			if (lightPtr == NULL){
				std::cerr << "ERROR::LightManager::getDirLights() - heve nullptr when attempting to cast Light object to DirectionalLight" << std::endl;
			}
			else{
				dirLights.push_back(lightPtr);
			}
		}
	}

	return dirLights;
}

std::vector<PointLight*> LightManager::getPointLights() const
{
	std::vector<PointLight*> pointLights;

	for (Light* light : lights)
	{
		if (light->getLightType() == LightType::LIGHT_POINTLIGHT)
		{
			PointLight* lightPtr = dynamic_cast<PointLight*>(light);
			if (lightPtr == NULL){
				std::cerr << "ERROR::LightManager::getPointLights() - Got nullptr when attempting to cast Light object to PointLight" << std::endl;
			}
			else{
				pointLights.push_back(lightPtr);
			}
		}
	}

	return pointLights;
}

std::vector<SpotLight*> LightManager::getSpotLights() const
{
	std::vector<SpotLight*> spotLights;

	for (Light* light : lights)
	{
		if (light->getLightType() == LightType::LIGHT_SPOTLIGHT)
		{
			SpotLight* lightPtr = dynamic_cast<SpotLight*>(light);
			if (lightPtr == NULL){
				std::cout << "ERROR::LightManager::getSpotLights() - Got nullptr when attempting to cast Light object to SpotLight" << std::endl;
			}
			else{
				spotLights.push_back(lightPtr);
			}
		}
	}

	return spotLights;
}

LightManager::~LightManager() = default;