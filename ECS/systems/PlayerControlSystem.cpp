#include "PlayerControlSystem.h"

#include "../components/Player.h"
#include "../components/Thrust.h"
#include "../components/Transform.h"
#include "../jdEngine.h"

void PlayerControlSystem::OnInit()
{
	auto& jd_engine = jd::Engine::getEngine();
	jd_engine.AddEventListener(METHOD_LISTENER(Events::Window::INPUT, PlayerControlSystem::InputListener));
}

void PlayerControlSystem::OnUpdate(float dt)
{
}

void PlayerControlSystem::OnDispose()
{

}

void PlayerControlSystem::InputListener(EventComponent* event) {
}
