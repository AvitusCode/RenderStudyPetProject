#include "Scenes/AppScene.h"
#include "ECS/jdEngine.h"

#include "ECS/components/Camera.h"
#include "ECS/components/Renderable.h"
#include "ECS/components/Transform.h"
#include "ECS/components/AssimpModel.h"
#include "ECS/components/SkyboxComponent.h"
#include "ECS/components/Material.h"
#include "ECS/components/Light.h"
#include "ECS/components/FogComponent.h"

#include "ECS/systems/CameraControlSystem.h"
#include "ECS/systems/AssimpModelSystem.h"
#include "ECS/systems/RenderSystem.h"
#include "ECS/systems/SkyboxSystem.h"
#include "ECS/systems/LightSystem.h"
#include "ECS/systems/MaterialSystem.h"
#include "ECS/systems/FogSystem.h"

#include "Models/Model.h"
#include "Models/Terrain.h"
#include "Render/UniformBuffer.h"
#include "Utils/ThreadPool.h"

#include <string>

void AppScene::OnCreate()
{
	auto renderSystem = SYSTEM(Render);
	auto cameraControlSystem = SYSTEM(CameraControl);
	auto assimpMS = SYSTEM(AssimpModel);
	auto materialSys = SYSTEM(Material);
	auto lightSys = SYSTEM(Light);
	auto fogSys = SYSTEM(Fog);
	auto skySys = SYSTEM(Skybox);
	systems.push_back(renderSystem);
	systems.push_back(cameraControlSystem);
	systems.push_back(assimpMS);
	systems.push_back(materialSys);
	systems.push_back(lightSys);
	systems.push_back(fogSys);
	systems.push_back(skySys);

	for (auto&& sys : systems) {
		sys->OnInit();
	}

	jd::utils::ThreadPool::Instance();
}

void AppScene::OnUpdate(float dt)
{
	for (auto&& sys : systems) {
		sys->OnUpdate(dt);
	}
}

void AppScene::OnDispose()
{

}