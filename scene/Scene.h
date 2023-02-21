#pragma once

class SceneManager;

class Scene
{
public:
	SceneManager* GetSceneManager() const {
		return sceneManager;
	};
	virtual void OnCreate() {};
	virtual void OnUpdate() {};
	virtual void OnDispose() {};
	virtual ~Scene() = default;
private:
	friend class SceneManager;
	SceneManager* sceneManager;
};