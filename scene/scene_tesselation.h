#pragma once
#include "Scene.h"
#include "../Assets.h"
#include "../render/Renderer.h"
#include <glm/glm.hpp>

class GLFWwindow;
class Model;

class scene_tesselation : public Scene
{
public:
	scene_tesselation(GLFWwindow* win) : window(win) {}
	void OnCreate() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

	const Assets& getAssets() const;

private:
	GLFWwindow* window;

	Renderer render;
	Assets assets;
};