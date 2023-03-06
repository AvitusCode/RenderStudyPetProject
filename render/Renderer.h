#pragma once
#include <vector>
#include <map>

#include "Core.h"
#include "../light/LightManager.h"
#include "../light/Light.h"
#include "Mesh.h"

class Renderer
{
public:
	Renderer();
	Renderer(unsigned int lightShaderProgramID);

	// Adds an object (model, mesh, skybox...) to the renderer's object vector
	Renderer& addObject(Core* objectPtr);
	// Adds a light (dirlight, pointlight, spotlight ...) to the renderer's LightManager
	Renderer& addLight(Light* lightPtr);

	// Initializes the renderer data, done before the render loop
	void init();
	// Draws the renderer's content
	void render();
	// Utilize all data
	void end();

	const LightManager& getLightManager() const;
	LightManager& setLightManager();
	std::vector<Core*>& setObjects();

private:
	// references for all mesh objects
	std::vector<Core*> objects;
	LightManager lightManager;
};