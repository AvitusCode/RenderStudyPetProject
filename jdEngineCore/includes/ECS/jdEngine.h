#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "SystemManager.h"
#include "Configurations.h"

#include "Render/TextureManager.h"
#include "Render/SamplerManager.h"
#include "Render/ShaderManager.h"
#include "Scenes/SceneManager.h"
#include "Models/ModelManager.h"

#include <memory>

namespace jd
{
	// This class need to be using as singlton
	class Engine
	{
	public:
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator=(const Engine&) = delete;
		Engine& operator=(Engine&&) = delete;

		static Engine& getEngine() {
			static Engine engine{};
			return engine;
		}

		void Init();
		void Destroy();

		// Entity methods
		Entity CreateEntity();

		void DestroyEntity(Entity entity);

		// Component methods
		template<typename T>
		ComponentType RegisterComponent();

		template<typename T>
		void AddComponent(Entity entity, T component);

		template<typename T>
		void RemoveComponent(Entity entity);

		template<typename T>
		T& GetComponent(Entity entity);

		template<typename T>
		ComponentType GetComponentType();

		// System methods
		template<typename T>
		std::shared_ptr<T> RegisterSystem();

		template<typename T>
		std::shared_ptr<T> GetSystem();

		template<typename T>
		void SetSystemSignature(Signature signature);

		// Event methods
		void AddEventListener(EventId eventId, std::function<void(EventComponent*)> const& listener);
		void SendEvent(EventComponent* event);

		SceneManager& getSceneManager();
		TextureManager& getTextureManager();
		ShaderManager& getShaderManager();
		SamplerManager& getSamplerManager();
		ModelManager& getModelManager();

		// Gui methods
		static void GuiInit();
		static void GuiDispose();
		static void GuiBegin();
		static void GuiEnd();
		static void ConsoleDebug();
		static bool IsDebugMode();
	private:
		Engine();

	private:
		// ECS managers
		std::unique_ptr<ComponentManager> mComponentManager;
		std::unique_ptr<EntityManager> mEntityManager;
		std::unique_ptr<EventManager> mEventManager;
		std::unique_ptr<SystemManager> mSystemManager;

		// UserManagers
		std::unique_ptr<TextureManager> mTextureManager;
		std::unique_ptr<SceneManager> mSceneManager;
		std::unique_ptr<ShaderManager> mShaderManager;
		std::unique_ptr<SamplerManager> mSamplerManager;
		std::unique_ptr<ModelManager> mModelManager;

		bool is_init = false;
		inline static bool CONSOLE_DEBUG_MODE = false;
	};

	// IMPLEMENTATION
	template<typename T>
	ComponentType Engine::RegisterComponent() {
		return mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void Engine::AddComponent(Entity entity, T component)
	{
		mComponentManager->AddComponent<T>(entity, std::move(component));

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void Engine::RemoveComponent(Entity entity)
	{
		mComponentManager->RemoveComponent<T>(entity);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& Engine::GetComponent(Entity entity) {
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType Engine::GetComponentType() {
		return mComponentManager->GetComponentType<T>();
	}

	// System methods
	template<typename T>
	std::shared_ptr<T> Engine::RegisterSystem() {
		return mSystemManager->RegisterSystem<T>();
	}

	template<typename T>
	std::shared_ptr<T> Engine::GetSystem() {
		return mSystemManager->GetSystem<T>();
	}

	template<typename T>
	void Engine::SetSystemSignature(Signature signature) {
		mSystemManager->SetSignature<T>(signature);
	}
}
