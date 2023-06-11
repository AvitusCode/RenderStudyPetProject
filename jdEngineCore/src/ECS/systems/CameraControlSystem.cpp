#include "ECS/systems/CameraControlSystem.h"
#include "ECS/components/Transform.h"
#include "ECS/components/Camera.h"
#include "ECS/jdEngine.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Display.h"

std::shared_ptr<CameraControlSystem> CameraControlSystem::getCameraControlSystem()
{
	auto& jd_engine = jd::Engine::getEngine();
	static bool sys_on = false;

	if (!sys_on)
	{
		auto cameraSys = jd_engine.RegisterSystem<CameraControlSystem>();
		Signature signature;
		signature.set(jd_engine.GetComponentType<Transform>());
		signature.set(jd_engine.GetComponentType<Camera>());
		jd_engine.SetSystemSignature<CameraControlSystem>(signature);
		sys_on = true;
		return cameraSys;
	}

	return jd_engine.GetSystem<CameraControlSystem>();
}

void CameraControlSystem::OnInit()
{
	auto& jd_engine = jd::Engine::getEngine();
	jd_engine.AddEventListener(METHOD_LISTENER(Events::Window::INPUT, CameraControlSystem::InputListener));
	jd_engine.AddEventListener(METHOD_LISTENER(Events::Window::MOUSE, CameraControlSystem::MouseListener));
	jd_engine.AddEventListener(METHOD_LISTENER(Events::Window::SCROLL, CameraControlSystem::ScrollListener));

	for (auto& entity : mEntities)
	{
		auto& camera = jd_engine.GetComponent<Camera>(entity);
		updateCameraVectors(camera.orientation, camera.RightAngle, camera.UpAngle);
	}
}

void CameraControlSystem::OnUpdate(float dt)
{
	auto& jd_engine  = jd::Engine::getEngine();
	auto& jd_display = Display::getDisplay();
	auto& display_configs = jd_display.getConfigs();

	if (display_configs.showCursor) {
		return;
	}

	// Пока что поддерживаем только одну камеру
	for (auto& entity : mEntities)
	{
		auto& transform = jd_engine.GetComponent<Transform>(entity);
		auto& camera = jd_engine.GetComponent<Camera>(entity);

		// Keyboard
		const float velocity = camera.MovementSpeed * dt;
		const jd::Quaternion qF = (camera.orientation * jd::Quaternion::identity(0.0f, 0.0f, -1.0f, 0.0f)) * camera.orientation.conjugate();
		const glm::vec3 Front = qF;
		const glm::vec3 Right = glm::normalize(glm::cross(Front, glm::vec3{ 0.0f, 1.0f, 0.0f }));

		if (key_button == GLFW_KEY_W && key_action == GLFW_PRESS) {
			transform.position += Front * velocity;
		}
		else if (key_button == GLFW_KEY_S && key_action == GLFW_PRESS) {
			transform.position -= Front * velocity;
		}

		if (key_button == GLFW_KEY_A && key_action == GLFW_PRESS) {
			transform.position -= Right * velocity;
		}
		else if (key_button == GLFW_KEY_D && key_action == GLFW_PRESS)
		{
			transform.position += Right * velocity;
		}
	}
}

void CameraControlSystem::OnDispose()
{

}

void CameraControlSystem::updateCameraVectors(jd::Quaternion& orientation, float RightAngle, float UpAngle)
{
	jd::Quaternion aroundY = jd::Quaternion::angleAxis(glm::radians(-RightAngle), glm::vec3{0.0f, 1.0f, 0.0f});
	jd::Quaternion aroundX = jd::Quaternion::angleAxis(glm::radians(UpAngle), glm::vec3{ 1.0f, 0.0f, 0.0f });
	orientation = aroundY * aroundX;
}

void CameraControlSystem::InputListener(EventComponent* event) 
{
	KeyPressedEvent* kpe = static_cast<KeyPressedEvent*>(event);

	key_button = kpe->getKey();
	key_action = kpe->getAction();
}

void CameraControlSystem::MouseListener(EventComponent* event)
{
	auto& jd_engine = jd::Engine::getEngine();
	auto& jd_display = Display::getDisplay();
	auto& display_configs = jd_display.getConfigs();

	if (display_configs.showCursor) {
		return;
	}

	for (auto& entity : mEntities)
	{
		auto& camera = jd_engine.GetComponent<Camera>(entity);
		auto& transform = jd_engine.GetComponent<Transform>(entity);

		if (display_configs.firstMouse)
		{
			last_xoffset = display_configs.mousePosX;
		    last_yoffset = display_configs.mousePosY;
			display_configs.firstMouse = false;
		}

		float n_xoffset = display_configs.mousePosX - last_xoffset;
		float n_yoffset = last_yoffset - display_configs.mousePosY;

		last_xoffset = display_configs.mousePosX;
		last_yoffset = display_configs.mousePosY;

		n_xoffset *= camera.MouseSensitivity;
		n_yoffset *= camera.MouseSensitivity;

		camera.RightAngle += n_xoffset;
		camera.UpAngle += n_yoffset;

		updateCameraVectors(camera.orientation, camera.RightAngle, camera.UpAngle);
	}
}

void CameraControlSystem::ScrollListener(EventComponent* event)
{
	MouseScrolledEvent* mse = static_cast<MouseScrolledEvent*>(event);

	scroll_offset = mse->getOffset();

	auto& jd_engine = jd::Engine::getEngine();
	for (auto& entity : mEntities)
	{
		auto& camera = jd_engine.GetComponent<Camera>(entity);

		if (camera.Zoom >= 1.0f && camera.Zoom <= 45.0f) {
			camera.Zoom -= scroll_offset;
		}
		if (camera.Zoom <= 1.0f) {
			camera.Zoom = 1.0f;
		}
		if (camera.Zoom >= 45.0f) {
			camera.Zoom = 45.0f;
		}
	}
}
