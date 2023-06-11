#include "ECS/systems/PlayerControlSystem.h"

#include "ECS/components/Player.h"
#include "ECS/components/Thrust.h"
#include "ECS/components/Transform.h"
#include "ECS/jdEngine.h"

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
