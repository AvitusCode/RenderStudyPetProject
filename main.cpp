#include "Application.hpp"
#include "Utils/logger.h"
#include <iostream>
#include <chrono>

#include "GL_ERROR.h"

#include "ECS/configurations.h"
#include "ECS/jdEngine.h"
#include "Display.h"
#include "Gui/Interface.h"
#include "Scenes/Demo1.h"
#include "Scenes/Demo2.h"

// TODO: Сделать интерфейс с возможностями
/*
3) Добавление источников света на сцену
4) Манипуляции с захваченным объектом
*/

class DemoApp : public Application
{
private:
	void OnInit() override
	{
		display_.setVerticalSinhronization(true);
		SceneManager& sm = engine_.getSceneManager();
		sm.emplaceScene<Demo1>("BackPack");
		sm.emplaceScene<Demo2>("InstancedDrawAndThreadPoolTest");
		sm.SetScene(0);

		engine_.GuiInit();
	}
};

int main()
{
	jd::logging::InternalLogger::InitLog(jd::logging::DefaultLogFile);

	LOG(INFO) << "jd render engine VERSION = 0.3b has started";
	/*DemoApp demoApp;
	demoApp.Run();*/
	Application mainApp;
	mainApp.Run();
	LOG(INFO) << "Successfully end programm!";

	return 0;
}