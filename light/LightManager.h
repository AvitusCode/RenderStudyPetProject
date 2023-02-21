#pragma once

#include <vector>

#include "Light.h"
#include "DirLight.h"
#include "PointLight.h"
#include "SpotLight.h"

// A lightmanager serves to manages multiple lights. It is used in a renderer, and serves to easily add
// different lights to a scene and seamlessly handles sending all needed data to the shaders
class LightManager
{
public:
	LightManager();
	LightManager(unsigned int spID);
	~LightManager();

	void addLight(Light* light);
	template<typename LIGHT, typename... Args>
	void addLight(Args &&... args){
		lights.push_back(std::forward<Args>(args)...);
	}
	void init();

	std::vector<DirectionalLight*> getDirLights() const;
	std::vector<PointLight*> getPointLights() const;
	std::vector<SpotLight*> getSpotLights() const;
	unsigned int getShaderId() const;
	void setShaderId(unsigned int id);

private:
	unsigned int shaderProgramID;

	unsigned int nrDirLights;
	unsigned int nrPointLights;
	unsigned int nrSpotLights;
	std::vector<Light*> lights;
};