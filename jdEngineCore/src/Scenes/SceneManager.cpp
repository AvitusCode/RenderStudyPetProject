#include "Scenes/SceneManager.h"
#include <iostream>

void SceneManager::OnUpdate(float dt)
{
	if (scenes.size() > current_scene)
	{
		if (first_scene) 
		{
			first_scene = false;
			scenes[current_scene]->OnCreate();
			prev_scene = current_scene;
		}
		else if (prev_scene != current_scene)
		{
			scenes[prev_scene]->OnDispose();
			scenes[current_scene]->OnCreate();

			prev_scene = current_scene;
		}

		scenes[current_scene]->OnUpdate(dt);
	}
	else {
		std::cerr << "ERROR: there are not current scene: " << current_scene << std::endl;
	}
}

void SceneManager::OnDispose()
{
	for (auto&& scene : scenes) {
		scene->OnDispose();
	}

	scenes.clear();
}

void SceneManager::SetScene(size_t id)
{
	prev_scene = current_scene;
	current_scene = id;
}

void SceneManager::SetScene(const std::string& name)
{
	auto iter = named_scenes.find(name);

	if (iter == named_scenes.end()) 
	{
		std::cerr << "ERROR: scene with name " << name << "does not exsist" << std::endl;
		return;
	}

	SetScene(iter->second);
}

void SceneManager::NextScene()
{
	prev_scene = current_scene;
	current_scene++;
}