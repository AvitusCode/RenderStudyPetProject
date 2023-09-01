#pragma once
#include "Scene.h"
#include "ECS/System.h"
#include "Render/Sampler.h"
#include "Render/VertexBuffer.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Demo2 : public Scene
{
public:
	void OnCreate() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

private:
	std::vector<std::shared_ptr<System>> systems;
	Sampler* mainSampler;

	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::vec3> rocksPos;
	VertexBuffer instanceBuffer;
};