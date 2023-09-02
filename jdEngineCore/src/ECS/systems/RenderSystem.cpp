#include <glad/glad.h>
#include "ECS/systems/RenderSystem.h"
#include "ECS/components/Camera.h"
#include "ECS/components/Renderable.h"
#include "ECS/components/Transform.h"
#include "ECS/jdEngine.h"
#include "Display.h"
#include "Render/Shader.h"
#include "Render/Primitives.h"

#include "Events/WindowEvent.h"

#include "Utils/logger.h"

std::shared_ptr<RenderSystem> RenderSystem::getSystem()
{
	auto& jd_engine = jd::Engine::getEngine();
	static bool sys_on = false;

	if (!sys_on)
	{
		jd_engine.RegisterComponent<Renderable>();
		jd_engine.RegisterComponent<Transform>();
		jd_engine.RegisterComponent<Camera>();
		auto rendSys = jd_engine.RegisterSystem<RenderSystem>();
		jd_engine.RegisterComponent<Renderable>();
		Signature signature;
		signature.set(jd_engine.GetComponentType<Renderable>());
		jd_engine.SetSystemSignature<RenderSystem>(signature);
		sys_on = true;
		return rendSys;
	}

	return jd_engine.GetSystem<RenderSystem>();
}

// TODO: Add grid system

void RenderSystem::OnInit()
{
	auto& jd_engine = jd::Engine::getEngine();
	auto& jd_display = Display::getDisplay();

	jd_engine.AddEventListener(METHOD_LISTENER(Events::Window::RESIZED, RenderSystem::resizeListener));

	mCamera = jd_engine.CreateEntity();
	Transform transformStruct{ glm::vec3(0.0f, 20.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f) , glm::vec3(0.0f, 0.0f, 0.0f) };
	jd_engine.AddComponent(mCamera, transformStruct);

	Camera cameraStruct;
	cameraStruct.projectionTransform = Camera::GetProjectionTransform(45.0f, 0.1f, 1000.0f, jd_display.getWidth(), jd_display.getHeight());
	cameraStruct.MovementSpeed = 15.0f;
	jd_engine.AddComponent(mCamera, cameraStruct);

	frustum = Primitives::createFrustumFromCamera(mCamera, 
		static_cast<float>(jd_display.getWidth()) / static_cast<float>(jd_display.getHeight()),
		cameraStruct.Zoom, 0.1f, 1000.0f);

	UBO.createUBO(2 * sizeof(glm::mat4));
	UBO.bindUBO();
	UBO.bindBufferBaseToBindingPoint(UniformBlockBindingPoints::MATRICES);

	grid = new Grid{};
}

void RenderSystem::OnUpdate(float dt)
{
	auto& jd_engine = jd::Engine::getEngine();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto& cameraTransform = jd_engine.GetComponent<Transform>(mCamera);
	auto& camera = jd_engine.GetComponent<Camera>(mCamera);

	glm::mat4 view = Camera::GetViewMatrix(cameraTransform.position, camera.orientation);
	glm::mat4 projection = camera.projectionTransform;

	UBO.bindUBO();
	UBO.updateBufferData(0, &view[0][0], sizeof(glm::mat4));
	UBO.updateBufferData(sizeof(glm::mat4), &projection[0][0], sizeof(glm::mat4));

	grid->Draw();

	for (auto const& entity : mEntities)
	{
		const auto& renderable = jd_engine.GetComponent<Renderable>(entity);
		renderable.shader->use();
		renderable.shader->set("viewPos", cameraTransform.position);

		for (const auto& component : renderable.rdata)
		{
			glBindVertexArray(component.VAO);
			setTextures(renderable.shader->getId(), component.textures);

			renderable.shader->set("Model", component.Model);
			renderable.shader->set("iModel", component.inverseModel);

			switch (component.drawArray)
			{
			case GL_ARRAY_BUFFER:
				glDrawArrays(component.drawModeType, 0, component.sizeOfDraw * sizeof(float));
				break;
			case GL_ELEMENT_ARRAY_BUFFER:
				glDrawElements(component.drawModeType, component.sizeOfDraw, GL_UNSIGNED_INT, nullptr);
				break;
			case GL_PRIMITIVE_RESTART:
				glEnable(GL_PRIMITIVE_RESTART);
				glPrimitiveRestartIndex(renderable.primitiveRestartIndx);
				glDrawElements(component.drawModeType, component.sizeOfDraw, GL_UNSIGNED_INT, nullptr);
				glDisable(GL_PRIMITIVE_RESTART);
				break;
			case GL_INSTANCED_DRAW:
				glDrawElementsInstanced(component.drawModeType, component.sizeOfDraw, GL_UNSIGNED_INT, nullptr, renderable.instansedCount);
				break;
			}
		}
	}

	glBindVertexArray(0);
}

void RenderSystem::OnDispose()
{
	delete grid;
}

void RenderSystem::setTextures(const GLuint shaderId, const std::vector<const Texture*>& textures)
{
	size_t diffuseNr = 1;
	size_t specularNr = 1;
	size_t normalNr = 1;
	size_t heightNr = 1;
	size_t roughnessNr = 1;
	size_t aoNr = 1;
	size_t textureNr = 1;

	for (size_t i = 0; i < textures.size(); i++)
	{
		std::string number;
		std::string name = textures[i]->getTextureType();

		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		}
		else if (name == "texture_normal") {
			number = std::to_string(normalNr++);
		}
		else if (name == "texture_height") {
			number = std::to_string(heightNr++);
		}
		else if (name == "texture") {
			number = std::to_string(textureNr++);
		}
		else if (name == "texture_roughness") {
			number = std::to_string(roughnessNr++);
		}
		else if (name == "texture_ao") {
			number = std::to_string(aoNr++);
		}
		else {
			LOG(WARNING) << "No such texture: " << name;
			continue;
		}

		int locationId = glGetUniformLocation(shaderId, (name + number).c_str());
		if (locationId == -1) {
			continue;
		}

		glUniform1i(locationId, static_cast<GLuint>(i));
		textures[i]->bind(static_cast<GLenum>(i));
	}
}

void RenderSystem::resizeListener(EventComponent* event)
{
	auto& jd_engine = jd::Engine::getEngine();
	auto& camera = jd_engine.GetComponent<Camera>(mCamera);

	WindowResizedEvent* wre = static_cast<WindowResizedEvent*>(event);
	camera.projectionTransform = Camera::GetProjectionTransform(camera.Zoom, 0.1f, 1000.0f, wre->getWidth(), wre->getHeight());
	frustum = Primitives::createFrustumFromCamera(mCamera,
		static_cast<float>(wre->getWidth()) / static_cast<float>(wre->getHeight()),
		camera.Zoom, 0.1f, 1000.0f);
}

