#include <glad/glad.h>
#include "Scenes/Demo2.h"
#include "ECS/jdEngine.h"

#include "ECS/components/Camera.h"
#include "ECS/components/Mesh3D.h"
#include "ECS/components/Renderable.h"
#include "ECS/components/Transform.h"
#include "ECS/components/AssimpModel.h"
#include "ECS/components/SkyboxComponent.h"

#include "ECS/systems/RenderSystem.h"
#include "ECS/systems/CameraControlSystem.h"
#include "ECS/systems/AssimpModelSystem.h"
#include "ECS/systems/SkyboxSystem.h"

#include "Models/Model.h"
#include "Render/UniformBuffer.h"
#include "Utils/ThreadPool.h"
#include <random>

// TODO: fix dinamic position change of the asteroids

void Demo2::OnCreate()
{
	jd::Engine& engine = jd::Engine::getEngine();
	auto& modelManager = engine.getModelManager();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	auto renderSystem = SYSTEM(Render);
	systems.push_back(renderSystem);
	auto cameraControlSystem = SYSTEM(CameraControl);
	systems.push_back(cameraControlSystem);
	auto assimpMS = SYSTEM(AssimpModel);
	systems.push_back(assimpMS);
	auto skyboxSystem = SYSTEM(Skybox);
	systems.push_back(skyboxSystem);

	auto& shaderManager = engine.getShaderManager();
	{
		auto& shader = shaderManager.createShaderProgram("asteroid");
		ShaderComponent vertComponent, fragComponent;
		vertComponent.loadComponentFromFile(ROOT_DIR "res/Scene2/asteroids.vert", GL_VERTEX_SHADER);
		fragComponent.loadComponentFromFile(ROOT_DIR "res/Scene2/asteroids.frag", GL_FRAGMENT_SHADER);
		shader.addComponentToProgram(vertComponent);
		shader.addComponentToProgram(fragComponent);
		shader.linkProgram();
		shader.use();
		shader.setInt("texture_diffuse1", 0);
		UniformBuffer::bindShaderUniformBlock(shader.getId(), UniformBlockBindingPoints::MATRICES, "Matrices");
	}

	{
		auto& shader = shaderManager.createShaderProgram("planet");
		ShaderComponent vertComponent, fragComponent;
		vertComponent.loadComponentFromFile(ROOT_DIR "res/Scene2/planet.vert", GL_VERTEX_SHADER);
		fragComponent.loadComponentFromFile(ROOT_DIR "res/Scene2/planet.frag", GL_FRAGMENT_SHADER);
		shader.addComponentToProgram(vertComponent);
		shader.addComponentToProgram(fragComponent);
		shader.linkProgram();
		shader.use();
		shader.setInt("texture_diffuse1", 0);
		UniformBuffer::bindShaderUniformBlock(shader.getId(), UniformBlockBindingPoints::MATRICES, "Matrices");
	}

	// Skybox
	{
		Entity entity = engine.CreateEntity();
		SkyboxComponent sc = loadSkybox(ROOT_DIR "res/skybox/space/");
		sc.is_fog = false;
		engine.AddComponent<SkyboxComponent>(entity, std::move(sc));
	}

	// Planet
	{
		Entity entity = engine.CreateEntity();
		Renderable renderComponent;
		renderComponent.shader = std::addressof(shaderManager.getShaderProgram("planet"));
		modelManager.cachedModelName("planet");
		modelManager.loadModelObj(ROOT_DIR "res/objects/planet/planet.obj", "planet", aiProcess_Triangulate | aiProcess_FlipUVs);
		JdModel amodel{modelManager.getModel("planet")};
		engine.AddComponent<Renderable>(entity, std::move(renderComponent));
		engine.AddComponent<JdModel>(entity, std::move(amodel));
		engine.AddComponent<Transform>(entity,
			Transform{ glm::vec3(0.0f, -3.0f, 0.0f),
					   glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3(10.0f, 10.0f, 10.0f) });

	}

	// asteroid
	Entity asteroid{};
	const size_t amount = 100'000;
	{
		Entity entity = engine.CreateEntity();
		Renderable renderComponent;
		renderComponent.shader = std::addressof(shaderManager.getShaderProgram("asteroid"));
		modelManager.cachedModelName("asteroid");
		modelManager.loadModelObj(ROOT_DIR "res/objects/rock/rock.obj", "asteroid", aiProcess_Triangulate | aiProcess_FlipUVs);
		JdModel amodel{ modelManager.getModel("asteroid") };
		engine.AddComponent<Renderable>(entity, std::move(renderComponent));
		engine.AddComponent<JdModel>(entity, std::move(amodel));
		engine.AddComponent<Transform>(entity,
			Transform{ glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3(0.0f, 0.0f, 0.0f) });
		asteroid = entity;
	}

	// Set sampler and init systems
	auto& samplerManager = engine.getSamplerManager();
	mainSampler = samplerManager.createSampler("Demo2", MagFilter::MAG_FILTER_BILINEAR, MinFilter::MIN_FILTER_TRILINEAR, 2u, 0u);

	for (auto&& sys : systems) {
		sys->OnInit();
	}

	// Preparations to instanced draw
	modelMatrices.resize(amount);
	rocksPos.resize(amount);

	std::mt19937 gen(std::random_device{}());
	const float radius = 150.0;
	const float offset = 25.0f;

	auto rand_offset = [&gen](float offset) {
		return (static_cast<int>(gen()) % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
	};

	for (size_t i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);

		float angle = (float)i / (float)amount * 360.0f;
		float displacement = rand_offset(offset);
		float x = sin(angle) * radius + displacement;
		displacement = rand_offset(offset);
		float y = displacement * 0.4f; // make our height a little less
		displacement = rand_offset(offset);
		float z = cos(angle) * radius + displacement;

		glm::vec3 positions = glm::vec3(x, y, z);
		model = glm::translate(model, positions);
		rocksPos[i] = std::move(positions);

		// 2. mag coeff from 0.05 to 0.25f
		float scale = (static_cast<int>(gen()) % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. A random rotation
		float rotAngle = (static_cast<int>(gen()) % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		modelMatrices[i] = model;
	}

	// make instanced VBO
	instanceBuffer.makeVBO(amount * sizeof(glm::mat4));
	instanceBuffer.bindVBO(GL_ARRAY_BUFFER);
	instanceBuffer.addRawData(modelMatrices.data(), amount * sizeof(glm::mat4));
	instanceBuffer.uploadDataToGPU(GL_DYNAMIC_DRAW, true);

	auto& ar = engine.GetComponent<Renderable>(asteroid);
	ar.instansedCount = amount;
	for (auto& component : ar.rdata)
	{
		component.drawArray = GL_INSTANCED_DRAW;
		glBindVertexArray(component.VAO);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}

void updateMultiThread(size_t a, size_t b, glm::mat3 rotatingY, glm::mat4* modelMatrices, glm::vec3* rocksPos)
{
	for (size_t i = a; i < b; i++)
	{
		*(rocksPos + i) = rotatingY * (*(rocksPos + i));
		modelMatrices[i][3][0] = rocksPos[i].x;
		modelMatrices[i][3][1] = rocksPos[i].y;
		modelMatrices[i][3][2] = rocksPos[i].z;
	}
}

void Demo2::OnUpdate(float dt)
{
	mainSampler->bind(0u);

	for (auto&& system : systems) {
		system->OnUpdate(dt);
	}

	// rotate around the planet
	const glm::vec3 rot_axis = glm::vec3(0.0f, 0.0f, 1.0f);
	const float rot_angle = glm::radians(0.20f / dt);
	glm::mat3 rotatingY = glm::mat3(1.0f);
	rotatingY[0][0] = cos(rot_angle);
	rotatingY[2][0] = sin(rot_angle);
	rotatingY[0][2] = -sin(rot_angle);
	rotatingY[2][2] = cos(rot_angle);

	const size_t page_size = 10'000;
	std::vector<std::future<void>> futures;
	auto& threadPool = jd::utils::ThreadPool::Instance();
	for (size_t i = 0; i < modelMatrices.size(); i += page_size) {
		futures.push_back(threadPool.submit(updateMultiThread, i, i + page_size, rotatingY, modelMatrices.data(), rocksPos.data()));
	}
	for (auto&& f : futures) {
		f.wait();
	}

	instanceBuffer.bindVBO();
	void* ptr_data = instanceBuffer.mapBufferToMemory(GL_WRITE_ONLY);
	memcpy(ptr_data, modelMatrices.data(), modelMatrices.size() * sizeof(glm::mat4));
	instanceBuffer.unmapBuffer();
}

void Demo2::OnDispose()
{

}