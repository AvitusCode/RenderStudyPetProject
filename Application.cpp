#include "Application.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "GL_ERROR.h"

#include "ECS/configurations.h"
#include "ECS/jdEngine.h"
#include "Display.h"
#include "Gui/Interface.h"
#include "Scenes/AppScene.h"
#include "Render/Grid.h"
#include "Render/UniformBuffer.h"
#include "ECS/components/Light.h"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

Application::Application() : engine_(jd::Engine::getEngine()), display_(Display::getDisplay())
{
	display_.init("jdEngine", WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Application::OnInit()
{
	/*glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	display_.setVerticalSinhronization(true);
	SceneManager& sm = engine_.getSceneManager();
	sm.emplaceScene<AppScene>("mainScene");
	sm.SetScene(0);
	ShaderManager& shaderManager = engine_.getShaderManager();
	SamplerManager& sampleManager = engine_.getSamplerManager();

	ShaderComponent vertexComponent, fragmentComponent, materialComp, DirComp, PointComp, SpotComp, FogComp;

	vertexComponent.loadComponentFromFile(ROOT_DIR "res/phongShader/phong.vert", GL_VERTEX_SHADER);
	fragmentComponent.loadComponentFromFile(ROOT_DIR "res/phongShader/phong.frag", GL_FRAGMENT_SHADER);
	materialComp.loadComponentFromFile(ROOT_DIR "res/lightShaders/material.frag", GL_FRAGMENT_SHADER);
	DirComp.loadComponentFromFile(ROOT_DIR "res/lightShaders/dirLight.frag", GL_FRAGMENT_SHADER);
	PointComp.loadComponentFromFile(ROOT_DIR "res/lightShaders/pointLight.frag", GL_FRAGMENT_SHADER);
	SpotComp.loadComponentFromFile(ROOT_DIR "res/lightShaders/spotLight.frag", GL_FRAGMENT_SHADER);
	FogComp.loadComponentFromFile(ROOT_DIR "res/fogShader/fog.frag", GL_FRAGMENT_SHADER);

	Shader& shader = shaderManager.createShaderProgram("shader_default");
	shader.addComponentToProgram(vertexComponent);
	shader.addComponentToProgram(fragmentComponent);
	shader.addComponentToProgram(materialComp);
	shader.addComponentToProgram(DirComp);
	shader.addComponentToProgram(PointComp);
	shader.addComponentToProgram(SpotComp);
	shader.addComponentToProgram(FogComp);
	shader.linkProgram();
	shader.use();
	shader.set("texture_diffuse1", 0);

	UniformBuffer::bindShaderUniformBlock(shader.getId(), UniformBlockBindingPoints::MATRICES, "Matrices");
	Sampler* sampler = sampleManager.createSampler("sampler_default", MagFilter::MAG_FILTER_BILINEAR, MinFilter::MIN_FILTER_TRILINEAR, 2u, 0u);

	engine_.GuiInit();
}

void Application::Loop()
{
	SceneManager& sm = engine_.getSceneManager();
	SamplerManager& sampleManager = engine_.getSamplerManager();
	Grid grid{};
	float dt = 0.0f;

	while (!display_.shouldClose())
	{
		const auto start_time = std::chrono::steady_clock::now();
		sampleManager.getSampler("sampler_default")->bind();
		sm.OnUpdate(dt);

		grid.Draw();

		engine_.GuiBegin();
		jd::Interface::ProgMenu();
		engine_.GuiEnd();

		GL_ERROR();

		display_.updateFPS(glfwGetTime());
		display_.processRealTime();

		display_.update();
		display_.pollEvents();

		const auto end_time = std::chrono::steady_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(end_time - start_time).count();
	}

	engine_.GuiDispose();
	display_.shutdown();
	engine_.Destroy();
}

void Application::Run()
{
	OnInit();
	Loop();
}