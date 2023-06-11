#include "ECS/systems/ModelSystem2D.h"
#include "ECS/jdEngine.h"
#include "ECS/components/Mesh2D.h"
#include "ECS/components/Renderable.h"

#include <glm/glm.hpp>

std::shared_ptr<ModelSystem2D> ModelSystem2D::getModelSystem()
{
	auto& jd_engine = jd::Engine::getEngine();
	static bool sys_on = false;

	if (!sys_on)
	{
		auto modelSys = jd_engine.RegisterSystem<ModelSystem2D>();
		Signature signature;
		signature.set(jd_engine.GetComponentType<Renderable>());
		signature.set(jd_engine.GetComponentType<Mesh2D>());
		jd_engine.SetSystemSignature<ModelSystem2D>(signature);
		sys_on = true;
		return modelSys;
	}

	return jd_engine.GetSystem<ModelSystem2D>();
}

void ModelSystem2D::OnInit()
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

void ModelSystem2D::OnUpdate(float dt)
{
	OnInit();
}

void ModelSystem2D::OnDispose()
{
	auto& jd_engine = jd::Engine::getEngine();
	for (Entity entity : mEntities)
	{
		auto& mesh = jd_engine.GetComponent<Mesh2D>(entity);
		delete mesh.shape;
	}
}