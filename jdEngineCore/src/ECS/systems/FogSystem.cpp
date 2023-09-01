#include "ECS/systems/FogSystem.h"
#include "ECS/jdEngine.h"
#include "ECS/components/FogComponent.h"
#include "ECS/components/Renderable.h"

std::shared_ptr<FogSystem> FogSystem::getSystem()
{
	auto& jd_engine = jd::Engine::getEngine();
	static bool sys_on = false;

	if (!sys_on)
	{
		jd_engine.RegisterComponent<Renderable>();
		jd_engine.RegisterComponent<FogComponent>();
		auto fogSys = jd_engine.RegisterSystem<FogSystem>();
		Signature signature;
		signature.set(jd_engine.RegisterComponent<Renderable>());
		signature.set(jd_engine.RegisterComponent<FogComponent>());
		jd_engine.SetSystemSignature<FogSystem>(signature);
		sys_on = true;
		return fogSys;
	}

	return jd_engine.GetSystem<FogSystem>();
}

void FogSystem::OnInit()
{

}

void FogSystem::OnUpdate(float dt)
{
	auto& jd_engine = jd::Engine::getEngine();
	
	for (const auto entity : mEntities)
	{
		const auto& fog = jd_engine.GetComponent<FogComponent>(entity);
		const auto& renderable = jd_engine.GetComponent<Renderable>(entity);

		renderable.shader->use();
		renderable.shader->set("fogStruct.color", fog.color);
		renderable.shader->setFloat("fogStruct.density", fog.density);
	}
}

void FogSystem::OnDispose()
{

}