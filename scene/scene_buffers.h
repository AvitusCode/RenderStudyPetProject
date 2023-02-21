#pragma once
#include "Scene.h"
#include "../Assets.h"
#include "../render/Renderer.h"

class scene_buffers : public Scene
{
public:
	scene_buffers() = default;
	void OnCreate() override;
	void OnUpdate() override;
	void OnDispose() override;

private:
	Assets assets;
	
	unsigned int cubeVAO;
	unsigned int cubeVBO;
	unsigned int planeVAO;
	unsigned int planeVBO;
	unsigned int quadVAO;
	unsigned int quadVBO;

	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int textureColorbufferMultisampler;
	unsigned int rbo;
	unsigned int intermediateFBO;
};