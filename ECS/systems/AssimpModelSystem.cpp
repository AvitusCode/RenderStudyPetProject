#include "AssimpModelSystem.h"
#include "../jdEngine.h"

#include "../../Models/Model.h"
#include "../components/AssimpModel.h"
#include "../components/Transform.h"
#include "../components/Renderable.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::shared_ptr<AssimpModelSystem> AssimpModelSystem::getAssimpModelSystem()
{
	auto& jd_engine = jd::Engine::getEngine();
	static bool sys_on = false;

	if (!sys_on)
	{
		jd_engine.RegisterSystem<AssimpModelSystem>();
		Signature signature;
		signature.set(jd_engine.GetComponentType<AssimpModel>());
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

	for (Entity entity : mEntities)
	{
		auto& assimpModel = jd_engine.GetComponent<AssimpModel>(entity);
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
}

void AssimpModelSystem::OnUpdate(float dt)
{
	auto& jd_engine = jd::Engine::getEngine();

}

void AssimpModelSystem::OnDispose()
{

}