#pragma once
#include "Scene.h"
#include "ECS/System.h"
#include "Render/Sampler.h"
#include "Render/Shader.h"
#include <vector>
#include <memory>

class Demo1 : public Scene
{
public:
	void OnCreate() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

private:
	std::vector<std::shared_ptr<System>> systems;
	Shader demoShader;
	Sampler* mainSampler;
};