#include "ECS/jdEngine.h"
#include "Gui/JdGui.h"

namespace jd
{
	Engine::Engine() {
		Init();
	}

	void Engine::Init()
	{
		if (is_init) {
			return;
		}

		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mEventManager = std::make_unique<EventManager>();
		mSystemManager = std::make_unique<SystemManager>();

		mTextureManager = std::make_unique<TextureManager>();
		mSceneManager = std::make_unique<SceneManager>();
		mShaderManager = std::make_unique<ShaderManager>();
		mSamplerManager = std::make_unique<SamplerManager>();

		is_init = true;
	}

	void Engine::Destroy()
	{
		mTextureManager->clearTextureCache();
		mSamplerManager->clearSamplerCache();
	}

	SceneManager& Engine::getSceneManager() {
		return *mSceneManager.get();
	}

	TextureManager& Engine::getTextureManager() {
		return *mTextureManager.get();
	}

	ShaderManager& Engine::getShaderManager() {
		return *mShaderManager.get();
	}

	SamplerManager& Engine::getSamplerManager() {
		return *mSamplerManager.get();
	}

	// Entity methods
	Entity Engine::CreateEntity() {
		return mEntityManager->CreateEntity();
	}

	void Engine::DestroyEntity(Entity entity)
	{
		mEntityManager->DestroyEntity(entity);
		mComponentManager->EntityDestroyed(entity);
		mSystemManager->EntityDestroyed(entity);
	}

	// Event methods
	void Engine::AddEventListener(EventId eventId, std::function<void(EventComponent*)> const& listener) {
		mEventManager->AddListener(eventId, listener);
	}

	void Engine::SendEvent(EventComponent* event) {
		mEventManager->SendEvent(event);
	}

	// Gui methods
	void Engine::GuiInit() {
		imGui::instance().OnInit();
	}
	void Engine::GuiDispose() {
		imGui::instance().OnDispouse();
	}

	void Engine::GuiBegin() 
	{
		if (CONSOLE_DEBUG_MODE) {
			imGui::instance().GuiBegin();
		}
	}

	void Engine::GuiEnd() 
	{
		if (CONSOLE_DEBUG_MODE) {
			imGui::instance().GuiEnd();
		}
	}

	void Engine::ConsoleDebug() {
		CONSOLE_DEBUG_MODE = !CONSOLE_DEBUG_MODE;
	}

	bool Engine::IsDebugMode() {
		return CONSOLE_DEBUG_MODE;
	}
}