#pragma once
#include "Scene.h"
#include "../render/Renderer.h"
#include "../Assets.h"
#include "../support/uniform_buffer.h"
#include <glm/glm.hpp>

class Model;
class Skybox;
class GLFWwindow;

class scene_2 : public Scene
{
public:
	scene_2(GLFWwindow* win) : window(win), uniformBuffer(UniformBuffer(0)) {}
	void OnCreate() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

	const Assets& getAssets() const;
private:
	void updateMultiThread(size_t a, size_t b, glm::mat3 rotatingY);
private:
	GLFWwindow* window;
	UniformBuffer uniformBuffer;
	Assets assets;

	glm::vec3 planetPos;
	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::vec3> rocks_pos;

	unsigned int amount;
	Model* _rock = nullptr;
	Model* _planet = nullptr;
	Skybox* _skybox = nullptr;

	unsigned int instance_buffer;

	glm::mat4 view;
	glm::mat4 projection;
};