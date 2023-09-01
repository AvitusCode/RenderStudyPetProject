#pragma once
#include "../System.h"
#include <memory>

class Shader;

class SkyboxSystem : public System
{
public:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

	static std::shared_ptr<SkyboxSystem> getSystem();

private:
	std::unique_ptr<Shader> skyShader;
};