#include "scene_tesselation.h"
#include "../main.h"
#include "../render/Shader.h"
#include "../render/Model.h"
#include "../Window.h"
#include "../objects/Camera.h"
#include "../light/DirLight.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "../GL_ERROR.h"
float tesselationLevel = 3.0f;

void scene_tesselation::OnCreate()
{
	auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	Camera& camera = handle.getCamera();
	camera.getPosition() = glm::vec3(0.0f, 0.0f, 15.0f);

	glEnable(GL_DEPTH_TEST);
	GLint maxPatchesVerts = 0;
	glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchesVerts);
	std::cout << "Max patches verts = " << maxPatchesVerts << std::endl;

	ShaderComponent vertexShader;
	ShaderComponent TCShader;
	ShaderComponent TEShader;
	ShaderComponent fragmentShader;

	vertexShader.loadComponentFromFile("res/tesselation/tess.vert", GL_VERTEX_SHADER);
	TCShader.loadComponentFromFile("res/tesselation/tess.tesc", GL_TESS_CONTROL_SHADER);
	TEShader.loadComponentFromFile("res/tesselation/tess.tese", GL_TESS_EVALUATION_SHADER);
	fragmentShader.loadComponentFromFile("res/tesselation/tess.frag", GL_FRAGMENT_SHADER);

	Shader* shaderMain = new Shader(vertexShader, TCShader, TEShader, fragmentShader);
	assets.storeShader(shaderMain, "shaderMain");

	shaderMain->use();
	shaderMain->setInt("texture_diffuse1", 0);
	shaderMain->setInt("texture_specular1", 1);

	Model* model = new Model("res/objects/backpack/backpack.obj", shaderMain->getId());
	model->scaleModel(glm::vec3(0.5f));

	render.addObject(model);
	render.setLightManager().setShaderId(shaderMain->getId());

	DirectionalLight* dirLight = new DirectionalLight(
		glm::vec3(0.4f),
		glm::vec3(0.7f),
		glm::vec3(1.0f),
		glm::vec3(-0.2f, -1.0f, -0.3f));

	render.addLight(dirLight);
	render.setDrawMode(GL_PATCHES);
	render.init();
}

void scene_tesselation::OnUpdate(float dt)
{
	auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
	Camera& camera = handle.getCamera();
	Shader* shaderMain = assets.getShader("shaderMain");
	// Clears the buffers and last frame before rendering the next one
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)handle.getWidth() / (float)handle.getHeight(), 0.01f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 VP = projection * view;

	shaderMain->use();
	shaderMain->setFloat("gTessellationLevel", tesselationLevel);
	shaderMain->setMat4("VP", VP);
	shaderMain->setVec3("viewPos", camera.getPosition());
	
	render.setLightManager().init();
	render.render();
	glCheckError();
}

void scene_tesselation::OnDispose()
{
	render.end();
}

const Assets& scene_tesselation::getAssets() const {
	return assets;
}