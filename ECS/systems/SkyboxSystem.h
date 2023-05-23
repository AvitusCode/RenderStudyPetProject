#pragma once
#include "../System.h"

class Shader;

class SkyboxSystem : public System
{
public:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

private:
	Shader* skyShader;
};