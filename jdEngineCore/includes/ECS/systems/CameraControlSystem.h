#pragma once
#include "../System.h"
#include <glm/glm.hpp>

#include <vector>
#include <bitset>
#include <memory>
#include "Events/EventComponent.h"
#include "Math/Quaternion.h"

class Event;

class CameraControlSystem : public System
{
public:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;
	static std::shared_ptr<CameraControlSystem> getCameraControlSystem();

private:
	float last_xoffset = 0;
	float last_yoffset = 0;

	float scroll_offset = 0;

	int key_button = 0;
	int key_action = 0;

	void InputListener(EventComponent* event);
	void MouseListener(EventComponent* event);
	void ScrollListener(EventComponent* event);

	void updateCameraVectors(jd::Quaternion& orientation, float RightAngle, float UpAngle);
};
