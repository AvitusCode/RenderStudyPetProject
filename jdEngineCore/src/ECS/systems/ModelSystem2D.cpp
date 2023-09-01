#include "ECS/systems/ModelSystem2D.h"
#include "ECS/jdEngine.h"
#include "ECS/components/Mesh2D.h"
#include "ECS/components/Renderable.h"

#include <glm/glm.hpp>

std::shared_ptr<Model2DSystem> Model2DSystem::getSystem()
{
	auto& jd_engine = jd::Engine::getEngine();
	static bool sys_on = false;

	if (!sys_on)
	{
		auto modelSys = jd_engine.RegisterSystem<Model2DSystem>();
		jd_engine.RegisterComponent<Renderable>();
		jd_engine.RegisterComponent<Mesh2D>();
		Signature signature;
		signature.set(jd_engine.GetComponentType<Renderable>());
		signature.set(jd_engine.GetComponentType<Mesh2D>());
		jd_engine.SetSystemSignature<Model2DSystem>(signature);
		sys_on = true;
		return modelSys;
	}

	return jd_engine.GetSystem<Model2DSystem>();
}

void Model2DSystem::OnInit()
{
	auto& jd_engine = jd::Engine::getEngine();

	for (Entity entity : mEntities)
	{
		auto& mesh = jd_engine.GetComponent<Mesh2D>(entity);
		auto& renderable = jd_engine.GetComponent<Renderable>(entity);

		auto& data = renderable.rdata.back();
		data.Model = mesh.Model;

		renderable.shader->use();
		renderable.shader->set("stdColor", glm::vec3(1.0f, 0.0f, 0.0f));
	}
}

void Model2DSystem::OnUpdate(float dt)
{
	OnInit();
}

void Model2DSystem::OnDispose()
{

}