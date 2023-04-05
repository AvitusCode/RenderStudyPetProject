#pragma once
#include "Scene.h"
#include "../Assets.h"
#include "../support/uniform_buffer.h"
#include <glm/glm.hpp>

class Model;
class Skybox;
class GLFWwindow;

class scene_3 : public Scene
{
public:
	scene_3(GLFWwindow* win) : window(win), ub(0) {};
	void OnCreate() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

	const Assets& getAssets() const;

private:
	GLFWwindow* window;
	UniformBuffer ub;

	Assets assets;

	glm::vec3 lightPos;

	unsigned int cubeMapFBO;
	unsigned int depthCubeMap;

	unsigned int lightVAO;
	unsigned int lightVBO;

	unsigned int cubeVAO;
	unsigned int cubeVBO;

	std::vector<GLuint> adj_indexes;
	unsigned int adj_cubeVAO;
	unsigned int adj_cubeVBO;
	unsigned int adj_cubeEBO;

	glm::mat4 view;
	glm::mat4 projection;

	void render(const Shader& shader, unsigned int cubeVAO, bool flag, GLenum param);
	void render_scene(unsigned int cubeVAO, GLenum param);
};