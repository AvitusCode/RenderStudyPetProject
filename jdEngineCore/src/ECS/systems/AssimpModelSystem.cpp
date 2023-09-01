#include "ECS/systems/AssimpModelSystem.h"
#include "ECS/jdEngine.h"

#include "Models/Model.h"
#include "ECS/components/AssimpModel.h"
#include "ECS/components/Transform.h"
#include "ECS/components/Renderable.h"
#include "Events/EntityCreationEvent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::shared_ptr<AssimpModelSystem> AssimpModelSystem::getSystem()
{
	auto& jd_engine = jd::Engine::getEngine();
	static bool sys_on = false;

	if (!sys_on)
	{
		jd_engine.RegisterComponent<JdModel>();
		jd_engine.RegisterComponent<Renderable>();
		jd_engine.RegisterComponent<Transform>();
		jd_engine.RegisterSystem<AssimpModelSystem>();
		Signature signature;
		signature.set(jd_engine.GetComponentType<JdModel>());
		signature.set(jd_engine.GetComponentType<Renderable>());
		signature.set(jd_engine.GetComponentType<Transform>());
		jd_engine.SetSystemSignature<AssimpModelSystem>(signature);
		sys_on = true;
	}

	return jd_engine.GetSystem<AssimpModelSystem>();
}

void AssimpModelSystem::OnInit()
{
	auto& jd_engine = jd::Engine::getEngine();
	jd_engine.AddEventListener(METHOD_LISTENER(Events::Creation::ENTITY, AssimpModelSystem::ModelAddListener));

	for (Entity entity : mEntities)
	{
		auto& jdmodel = jd_engine.GetComponent<JdModel>(entity);
		auto& assimpModel = *jdmodel.model;
	
		updateEntityFirst(entity, assimpModel);
	}
}

void AssimpModelSystem::OnUpdate(float dt)
{
	auto& jd_engine = jd::Engine::getEngine();

	for (Entity entity : mEntities)
	{
		auto& jdmodel = jd_engine.GetComponent<JdModel>(entity);
		auto& assimpModel = *jdmodel.model;
		// Optimize process if Model is static
		if (assimpModel.is_static) {
			return;
		}

		updateEntity(entity, assimpModel);
	}
}

void AssimpModelSystem::updateEntity(Entity entity, AssimpModel& assimpModel)
{
	auto& jd_engine = jd::Engine::getEngine();
	auto& transform = jd_engine.GetComponent<Transform>(entity);
	auto& renderable = jd_engine.GetComponent<Renderable>(entity);

	for (auto& mesh : assimpModel.meshes)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, transform.position);
		model = glm::scale(model, transform.scale);

		renderable.rdata.back().Model = model;
		renderable.rdata.back().inverseModel = glm::transpose(glm::inverse(model));
	}
}

void AssimpModelSystem::updateEntityFirst(Entity entity, AssimpModel& assimpModel)
{
	auto& jd_engine = jd::Engine::getEngine();
	auto& transform = jd_engine.GetComponent<Transform>(entity);
	auto& renderable = jd_engine.GetComponent<Renderable>(entity);

	for (auto& mesh : assimpModel.meshes)
	{
		setUpStaticMesh(mesh, renderable);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, transform.position);
		model = glm::scale(model, transform.scale);

		renderable.rdata.back().Model = model;
		renderable.rdata.back().inverseModel = glm::transpose(glm::inverse(model));
	}
}

void AssimpModelSystem::ModelAddListener(EventComponent* event)
{
	EntityCreationEvent* creation_event = static_cast<EntityCreationEvent*>(event);
	auto entity = creation_event->getEntity();
	auto& jd_engine = jd::Engine::getEngine();
	auto& jdmodel = jd_engine.GetComponent<JdModel>(entity);
	auto& assimpModel = *jdmodel.model;
	updateEntityFirst(entity, assimpModel);
}

void AssimpModelSystem::OnDispose()
{

}