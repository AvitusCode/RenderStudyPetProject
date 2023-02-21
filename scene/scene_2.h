#pragma once
#include "Scene.h"
#include "../render/Renderer.h"
#include "../Assets.h"
#include "../support/uniform_buffer.h"
#include <glm/glm.hpp>

class Model;
class Skybox;

class scene_2 : public Scene
{
public:
	scene_2() : uniformBuffer(UniformBuffer(0)) {}
	void OnCreate() override;
	void OnUpdate() override;
	void OnDispose() override;

	const Assets& getAssets() const;

private:
	UniformBuffer uniformBuffer;
	Assets assets;

	unsigned int amount;
	Model* _rock = nullptr;
	Model* _planet = nullptr;
	Skybox* _skybox = nullptr;

	unsigned int instance_buffer;

	glm::mat4 view;
	glm::mat4 projection;
};