#pragma once
#include "Scene.h"
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <stdexcept>

class SceneManager
{
public:

	template<typename S, typename... Args>
	void emplaceScene(std::string name, Args&&... args)
	{
		void* raw_ptr = malloc(sizeof(S));
		if (!raw_ptr) {
			throw std::bad_alloc();
		}

		((S*)raw_ptr)->s_manager = this;
		S* scene = new (raw_ptr) S{ std::forward<Args>(args)... };
		scenes.emplace_back(scene);
		named_scenes[std::move(name)] = scenes.size() - 1;
	}

	void OnUpdate(float dt);
	void OnDispose();

	void SetScene(size_t id);
	void SetScene(const std::string& name);
	void NextScene();

private:
	std::map<std::string, size_t> named_scenes;
	std::vector<std::unique_ptr<Scene>> scenes;

	size_t current_scene = 0;
	size_t prev_scene = 0;
	bool first_scene = true;
};