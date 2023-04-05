#include "Renderer.h"

Renderer::Renderer(){
	this->lightManager = {};
	draw_mode = GL_TRIANGLES;
}

Renderer::Renderer(unsigned int lightShaderProgramID){
	lightManager = LightManager(lightShaderProgramID);
	draw_mode = GL_TRIANGLES;
}

void Renderer::setDrawMode(GLenum m) {
	draw_mode = m;
}

GLenum Renderer::getDrawMode() const {
	return draw_mode;
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
		object->setMode(draw_mode);
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