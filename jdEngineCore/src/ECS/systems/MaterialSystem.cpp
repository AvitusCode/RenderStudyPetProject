#include "ECS/systems/MaterialSystem.h"
#include "ECS/jdEngine.h"
#include "ECS/components/Renderable.h"
#include "ECS/components/Material.h"


void MaterialSystem::OnInit()
{

}

void MaterialSystem::OnUpdate(float dt)
{
	auto& jd_engine = jd::Engine::getEngine();

	for (const auto entity : mEntities)
	{
		auto& renderable = jd_engine.GetComponent<Renderable>(entity);
		const auto& material = jd_engine.GetComponent<Material>(entity);

		renderable.shader->use();
		renderable.shader->set("material.ambient", material.ambient);
		renderable.shader->set("material.diffuse", material.diffuse);
		renderable.shader->set("material.specular", material.specular);
		renderable.shader->setFloat("material.shininess", material.shininess);
	}
}

void MaterialSystem::OnDispose()
{

}