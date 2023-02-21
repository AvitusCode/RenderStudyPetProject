#pragma once
#include "Scene.h"
#include "../render/Renderer.h"
#include "../Assets.h"
#include "../support/uniform_buffer.h"
#include <glm/glm.hpp>

class Skybox;

class scene_1 : public Scene
{
public:
	scene_1() : uniformBuffer(UniformBuffer(0)) {}
	void OnCreate() override;
	void OnUpdate() override;
	void OnDispose() override;

	const Renderer& getRenderer() const;
	const Assets& getAssets() const;

private:
	UniformBuffer uniformBuffer;
	Assets assets;
	Renderer renderer;

	SpotLight* playerLight;
	Skybox* ptr_skybox;

	glm::mat4 view;
	glm::mat4 projection;
};