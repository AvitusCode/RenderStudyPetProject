#pragma once
#include "Scene.h"
#include "../ECS/System.h"
#include "../render/Sampler.h"
#include "../render/Shader.h"
#include <vector>
#include <memory>

#include "../Timer.h"

class CollisionsDemo : public Scene
{
public:
	void OnCreate() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

private:
	std::vector<std::shared_ptr<System>> systems;
	std::shared_ptr<System> physSys;

	Sampler* mainSampler;

	Timer timer{};
	float accumulator = 0.0f;
};
