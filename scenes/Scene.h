#pragma once

class SceneManager;

class Scene
{
public:
	virtual ~Scene() = default;

	inline SceneManager* getSceneManager() {
		return s_manager;
	}

	virtual void OnCreate() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnDispose() = 0;

private:
	friend class SceneManager;
	SceneManager* s_manager;
};