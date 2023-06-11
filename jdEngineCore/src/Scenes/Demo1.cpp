#include "Scenes/Demo1.h"

#include "ECS/jdEngine.h"

#include "ECS/components/Camera.h"
#include "ECS/components/Renderable.h"
#include "ECS/components/Transform.h"
#include "ECS/components/AssimpModel.h"
#include "ECS/components/SkyboxComponent.h"

#include "ECS/components/Material.h"
#include "ECS/components/Light.h"
#include "ECS/components/DirLight.h"
#include "ECS/components/PointLight.h"

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

#include <string>
#include <stdexcept>

void Demo1::OnCreate()
{
	jd::Engine& engine = jd::Engine::getEngine();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	engine.RegisterComponent<Camera>();
	engine.RegisterComponent<Mesh3D>();
	engine.RegisterComponent<Renderable>();
	engine.RegisterComponent<Transform>();
	engine.RegisterComponent<AssimpModel>();
	engine.RegisterComponent<SkyboxComponent>();
	
	engine.RegisterComponent<Material>();
	engine.RegisterComponent<Light>();
	engine.RegisterComponent<DirLight>();
	engine.RegisterComponent<PointLight>();
	engine.RegisterComponent<FogComponent>();

	auto renderSystem = RenderSystem::getRenderSystem();
	systems.push_back(renderSystem);

	auto cameraControlSystem = CameraControlSystem::getCameraControlSystem();
	systems.push_back(cameraControlSystem);

	auto assimpMS = AssimpModelSystem::getAssimpModelSystem();
	systems.push_back(assimpMS);

	auto materialSys = engine.RegisterSystem<MaterialSystem>();
	{
		Signature signature;
		signature.set(engine.GetComponentType<Renderable>());
		signature.set(engine.GetComponentType<Material>());
		engine.SetSystemSignature<MaterialSystem>(signature);
	}
	systems.push_back(materialSys);

	auto lightSys = engine.RegisterSystem<LightSystem>();
	{
		Signature signature;
		signature.set(engine.GetComponentType<Light>());
		signature.set(engine.GetComponentType<Renderable>());
		engine.SetSystemSignature<LightSystem>(signature);
	}
	systems.push_back(lightSys);

	auto fogSys = engine.RegisterSystem<FogSystem>();
	{
		Signature signature;
		signature.set(engine.GetComponentType<Renderable>());
		signature.set(engine.GetComponentType<FogComponent>());
		engine.SetSystemSignature<FogSystem>(signature);
	}
	systems.push_back(fogSys);

	auto skySys = engine.RegisterSystem<SkyboxSystem>();
	{
		Signature signature;
		signature.set(engine.GetComponentType<SkyboxComponent>());
		engine.SetSystemSignature<SkyboxSystem>(signature);
	}
	systems.push_back(skySys);

	// ENTITIES
	ShaderComponent vertexComponent, fragmentComponent, materialComp, DirComp, PointComp, SpotComp, FogComp;
	
	vertexComponent.loadComponentFromFile(ROOT_DIR "res/phongShader/phong.vert", GL_VERTEX_SHADER);
	fragmentComponent.loadComponentFromFile(ROOT_DIR "res/phongShader/phong.frag", GL_FRAGMENT_SHADER);
	materialComp.loadComponentFromFile(ROOT_DIR "res/lightShaders/material.frag", GL_FRAGMENT_SHADER);
	DirComp.loadComponentFromFile(ROOT_DIR "res/lightShaders/dirLight.frag", GL_FRAGMENT_SHADER);
	PointComp.loadComponentFromFile(ROOT_DIR "res/lightShaders/pointLight.frag", GL_FRAGMENT_SHADER);
	SpotComp.loadComponentFromFile(ROOT_DIR "res/lightShaders/spotLight.frag", GL_FRAGMENT_SHADER);
	FogComp.loadComponentFromFile(ROOT_DIR "res/fogShader/fog.frag", GL_FRAGMENT_SHADER);
	
	Shader shader{ vertexComponent, fragmentComponent, materialComp, DirComp, PointComp, SpotComp, FogComp };

	UniformBuffer::bindShaderUniformBlock(shader.getId(), UniformBlockBindingPoints::MATRICES, "Matrices");

	shader.use();
	shader.set("texture_diffuse1", 0);

	// Creaate Assimp model Entity
	Entity entity = engine.CreateEntity();
	Renderable renderComponent;
	demoShader = std::move(shader);
	renderComponent.shader = &demoShader;
	AssimpModel amodel = loadModel(ROOT_DIR "res/objects/house/medieval_house.obj");

	engine.AddComponent<Renderable>(entity, std::move(renderComponent));
	engine.AddComponent<AssimpModel>(entity, std::move(amodel));
	engine.AddComponent<Transform>(entity,
		Transform{ glm::vec3(0.0f, -2.0f, -10.0f),
				   glm::vec3(0.0f, 0.0f, 0.0f),
				   glm::vec3(0.01f, 0.01f, 0.01f) });
	engine.AddComponent<Material>(entity, Material{ glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f });

	// Create Terrain Entity
	entity = engine.CreateEntity();
	Renderable terrainRend;
	Mesh3D terrainMesh;
	auto& textureManager = engine.getTextureManager();
	textureManager.loadTexture2D("terrain", ROOT_DIR "res/Scene1/sand.png", "texture_diffuse");
	makeTerrain(ROOT_DIR "res/Scene1/heightMap.png", terrainMesh, terrainRend);
	terrainRend.rdata[0].Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f));
	terrainRend.rdata[0].Model = glm::scale(terrainRend.rdata[0].Model, glm::vec3(200.0f, 20.0f, 200.0f));
	terrainRend.rdata[0].inverseModel = glm::transpose(glm::inverse(terrainRend.rdata[0].Model));
	terrainRend.rdata[0].textures.push_back(textureManager.getTexture("terrain"));
	terrainRend.shader = &demoShader;
	
	engine.AddComponent<Mesh3D>(entity, std::move(terrainMesh));
	engine.AddComponent<Renderable>(entity, std::move(terrainRend));
	engine.AddComponent<Material>(entity, Material{ glm::vec3(0.2f), glm::vec3(0.7f), glm::vec3(0.2f), 8.0f });

	// Sampler configuration
	auto& samplerManager = engine.getSamplerManager();
	mainSampler = samplerManager.createSampler("main", MagFilter::MAG_FILTER_BILINEAR, MinFilter::MIN_FILTER_TRILINEAR, 2u, 0u);

	// Skybox
	entity = engine.CreateEntity();
	SkyboxComponent skyComponent = loadSkybox(ROOT_DIR "res/skybox/desert/");
	skyComponent.is_fog = true;
	engine.AddComponent<SkyboxComponent>(entity, std::move(skyComponent));

	// light
	entity = engine.CreateEntity();
	engine.AddComponent<Light>(entity, Light{ LightType::LIGHT_DIRLIGHT, glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.2f) });
	engine.AddComponent<DirLight>(entity, DirLight{glm::vec3(0.0f, -10.0f, 0.0f)});
	engine.AddComponent<Renderable>(entity, Renderable{ {}, 0, &demoShader });

	Light pointLight{
		LightType::LIGHT_POINTLIGHT,
		glm::vec3(0.0f, 0.2f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	Light pointLight2{
		LightType::LIGHT_POINTLIGHT,
		glm::vec3(0.2f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f) 
	};

	entity = engine.CreateEntity();
	engine.AddComponent<Light>(entity, std::move(pointLight));
	engine.AddComponent<PointLight>(entity, PointLight{ 1.0f, 0.045f, 0.0075f });
	engine.AddComponent<Transform>(entity, Transform{ glm::vec3(-10.0f, 5.0f, -10.0f) });
	engine.AddComponent<Renderable>(entity, Renderable{ {}, 0, &demoShader });

	entity = engine.CreateEntity();
	engine.AddComponent<Light>(entity, std::move(pointLight2));
	engine.AddComponent<PointLight>(entity, PointLight{ 1.0f, 0.045f, 0.0075f });
	engine.AddComponent<Transform>(entity, Transform{ glm::vec3(10.0f, 5.0f, -15.0f) });
	engine.AddComponent<Renderable>(entity, Renderable{ {}, 0, &demoShader });

	// Fog Entity
	entity = engine.CreateEntity();
	engine.AddComponent<FogComponent>(entity, FogComponent{ glm::vec3(0.7f, 0.7f, 0.7f), 0.01f });
	engine.AddComponent<Renderable>(entity, Renderable{ {}, 0, &demoShader });

	// Activation al systems
	for (auto&& system : systems) {
		system->OnInit();
	}
}

void Demo1::OnUpdate(float dt)
{
	mainSampler->bind(0u);

	for (auto&& system : systems) {
		system->OnUpdate(dt);
	}
}

void Demo1::OnDispose()
{
	for (auto&& system : systems) {
		system->OnDispose();
	}
}