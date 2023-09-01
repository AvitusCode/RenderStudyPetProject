#pragma once
#include "Scene.h"
#include "ECS/System.h"
#include "Render/Sampler.h"
#include <memory>
#include <vector>

/// <summary>
/// @brief Main app scene, which will be a wrapper for a user works
/// </summary>
class AppScene : public Scene
{
public:
	void OnCreate() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;
private:
	std::vector<std::shared_ptr<System>> systems;
	Sampler* mainSampler;
};