#pragma once
#include "../System.h"
#include <vector>

class EventComponent;

class PlayerControlSystem : public System
{
public:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

private:

	void InputListener(EventComponent* event);
};
