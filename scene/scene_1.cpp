#include "../main.h"
#include "scene_1.h"
#include "../render/SkyBox.h"
#include "../render/Mesh.h"
#include "../render/Model.h"
#include "../interface/interface.h"

// Creating a neccessary new data
void scene_1::OnCreate()
{
	// Depth test for depth buffering, blending for transparency
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.getPosition() = glm::vec3(-5.0f, 3.0f, 15.0f);

	Shader* phongShader = new Shader(load_shader("res/shaders/phong.vert", "res/shaders/phong.frag"/*, "res/shaders/phong.geom"*/));
	Shader* skyboxShader = new Shader(load_shader("res/shaders/skybox.vert", "res/shaders/skybox.frag"));
	Shader* gridShader = new Shader(load_shader("res/shaders/grid.vert", "res/shaders/grid.frag"));
	assets.storeShader(phongShader, "phongShader");
	assets.storeShader(skyboxShader, "skyboxShader");
	assets.storeShader(gridShader, "gridShader");

	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(camera.getZoom()), (float)windowWidth / (float)windowHeight, 0.01f, 100.0f);

	// Sets up global uniforms for each shader used
	uniformBuffer.initUniformBuffer();
	uniformBuffer.bindingUniformBlock(phongShader->getId(), "Matrices");
	uniformBuffer.bindingUniformBlock(skyboxShader->getId(), "Matrices");
	uniformBuffer.updateUniformBuffer(view, projection);

	Texture* crate = new Texture("res/img/container2.png", "texture_diffuse", false);
	Material* boxTex = new Material(*crate);
	assets.storeTexture(crate, "crate");
	assets.storeMaterial(boxTex, "boxTex");

	// Sets up variables for the phong lighting shader
	phongShader->use();

	phongShader->setInt("texture1", 0);
	phongShader->setVec3("viewPos", camera.getPosition());
	phongShader->setVec3("material.ambient", boxTex->ambient);
	phongShader->setVec3("material.diffuse", boxTex->diffuse);
	phongShader->setVec3("material.specular", boxTex->specular);
	phongShader->setFloat("material.shininess", boxTex->shininess);

	// Creates a renderer for drawing objects
	renderer.setLightManager().setShaderId(phongShader->getId());

	// Sets up lighting for the renderer's LightManager
	PointLight* pointLight = new PointLight(
		glm::vec3(0.0f, 0.2f, 0.0f), 
		glm::vec3(0.0f, 0.5f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec3(15.0f, 5.0f, 5.0f), 
		1.0f, 0.045f, 0.0075f);

	PointLight* pointLight2 = new PointLight(
		glm::vec3(0.2f, 0.0f, 0.0f), 
		glm::vec3(0.5f, 0.0f, 0.0f), 
		glm::vec3(1.0f, 0.0f, 0.0f), 
		glm::vec3(-5.0f, 5.0f, -5.0f), 
		1.0f, 0.045f, 0.0075f);

	DirectionalLight* dirLight = new DirectionalLight(
		glm::vec3(0.4f), 
		glm::vec3(0.7f), 
		glm::vec3(1.0f), 
		glm::vec3(-0.2f, -1.0f, -0.3f));

	SpotLight* spotLight = new SpotLight(
		glm::vec3(0.6f), 
		glm::vec3(0.8f), 
		glm::vec3(1.0f), 
		camera.getPosition(), 
		1.0f, 0.09f, 0.032f, 
		camera.getFront(), 
		glm::cos(glm::radians(12.5f)), 
		glm::cos(glm::radians(15.0f)));

	SpotLight* sl2 = new SpotLight(
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3(-10.0f, 15.0f, 0.0f), 
		0.0f, 0.0f, 0.001f, 
		glm::vec3(0.0f, -1.0f, 0.0f), 
		glm::cos(glm::radians(12.5f)), 
		glm::cos(glm::radians(12.5f)));

	playerLight = spotLight;

	renderer.addLight(pointLight)
		.addLight(pointLight2)
		.addLight(dirLight)
		.addLight(spotLight)
		.addLight(sl2);

	Skybox* skybox = new Skybox(skyboxShader->getId(), Cubemap("res/img/skybox/sky/"));
	ptr_skybox = skybox;
	Model* model = new Model("res/models/sea_keep/scene.gltf", phongShader->getId());
	model->scaleModel(0.05f, 0.05f, 0.05f);
	model->rotateModel(-90.0f, 1.0f, 0.0f, 0.0f);

	Model* model2 = new Model("res/models/boat/scene.gltf", phongShader->getId());
	model2->translateModel(10.0f, -8.5f, -0.5f);
	model2->rotateModel(-90.0f, 1.0f, 0.0f, 0.0f);
	model2->scaleModel(glm::vec3(0.01));

	std::vector<Vertex> vert(2);
	vert[0].Position = glm::vec3(0.0f, 0.0f, 0.0f);
	vert[1].Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Mesh* grid = new Mesh(vert, {}, {}, gridShader->getId());

	renderer.addObject(skybox)
		.addObject(model)
		.addObject(model2)
		.addObject(grid);

	// Initializes the Render and ImGui system
	renderer.init();
	RenderGui::ImGuiInit(window, renderer);
}

void scene_1::OnUpdate()
{
	Shader* phongShader = assets.getShader("phongShader");
	Shader* gridShader  = assets.getShader("gridShader");
	Material* boxTex = assets.getMaterial("boxTex");

	// Clears the buffers and last frame before rendering the next one
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
	projection = glm::perspective(glm::radians(camera.getZoom()), (float)windowWidth / (float)windowHeight, 0.01f, 100.0f);
	view = camera.GetViewMatrix();
	uniformBuffer.updateUniformBuffer(view, projection);

	// Models for phong lighting
	phongShader->use();
	phongShader->setFloat("time", (float)glfwGetTime());
	phongShader->setVec3("viewPos", camera.getPosition());
	phongShader->setVec3("material.ambient", boxTex->ambient);
	phongShader->setVec3("material.diffuse", boxTex->diffuse);
	phongShader->setVec3("material.specular", boxTex->specular);
	phongShader->setFloat("material.shininess", boxTex->shininess);

	playerLight->position = camera.getPosition();
	playerLight->direction = camera.getFront();

	renderer.setLightManager().init();
	renderer.render();
	RenderGui::ImGuiDrawWindows(camera, boxTex->ambient, boxTex->diffuse, boxTex->specular, boxTex->shininess, *ptr_skybox);
}

void scene_1::OnDispose(){
	renderer.end();
}

const Renderer& scene_1::getRenderer() const{
	return renderer;
}

const Assets& scene_1::getAssets() const{
	return assets;
}