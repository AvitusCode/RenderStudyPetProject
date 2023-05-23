#include "FogSystem.h"
#include "../jdEngine.h"
#include "../components/FogComponent.h"
#include "../components/Renderable.h"

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