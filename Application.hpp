#pragma once
#include "ECS/jdEngine.h"
#include "Display.h"

class Application
{
public:
	Application();
	void Run();
protected:
	jd::Engine& engine_;
	Display& display_;
private:
	virtual void OnInit();
	virtual void Loop();
};