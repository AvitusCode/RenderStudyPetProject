#include "Renderer.h"

Renderer::Renderer(){
	this->lightManager = {};
}

Renderer::Renderer(unsigned int lightShaderProgramID){
	lightManager = LightManager(lightShaderProgramID);
}

LightManager& Renderer::setLightManager() {
	return lightManager;
}

std::vector<Core*>& Renderer::setObjects() {
	return objects;
}

const LightManager& Renderer::getLightManager() const {
	return lightManager;
}

Renderer& Renderer::addObject(Core* objectPtr)
{
	objects.push_back(objectPtr);
	return *this;
}

Renderer& Renderer::addLight(Light* lightPtr)
{
	lightManager.addLight(lightPtr);
	return *this;
}

void Renderer::init()
{
	// Initializes the light manager
	if (lightManager.getShaderId() != 0){
		lightManager.init();
	}

	// shader ID using the shaderMap table
	for (Core* object : objects) {
		object->setupObject();
	}
}

void Renderer::render()
{
	for (const Core* const& object : objects)
	{
		glUseProgram(object->getShaderId());
		object->drawObject();
	}
}

void Renderer::end()
{
	for (Core* object : objects) {
		delete object;
	}
}