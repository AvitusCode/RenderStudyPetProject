#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>

#include "GL_ERROR.h"

#include "ECS/configurations.h"
#include "ECS/jdEngine.h"
#include "Display.h"
#include "Gui/Interface.h"
#include "Scenes/Demo1.h"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

// TODO: Сделать интерфейс с возможностями
/*
1) Менять параметры тумана
2) Добавлять объекты
3) Открывать по ним файлы
4) Frustum culing
5) Quad tree
6*) shadow mapping
*/

int main()
{
    auto& jd_engine = jd::Engine::getEngine();
	Display& display = Display::getDisplay();
	display.init("jdEngine", WINDOW_WIDTH, WINDOW_HEIGHT);
	display.setVerticalSinhronization(true);

	SceneManager& sm = jd_engine.getSceneManager();
	sm.emplaceScene<Demo1>("BackPack");
	sm.SetScene(0);

	jd_engine.GuiInit();

	float dt = 0.0f;
	while (!display.shouldClose())
	{
		const auto start_time = std::chrono::steady_clock::now();

		sm.OnUpdate(dt);

		jd_engine.GuiBegin();
		jd::Interface::ProgMenu();
		jd_engine.GuiEnd();

		glCheckError();

		display.updateFPS(glfwGetTime());
		display.processRealTime();

		display.update();
		display.pollEvents();

		const auto end_time = std::chrono::steady_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(end_time - start_time).count();
	}

	jd_engine.GuiDispose();
	display.shutdown();
	jd_engine.Destroy();

	return 0;
}