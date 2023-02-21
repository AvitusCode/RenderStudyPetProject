#include "Scene.h"
#include "SceneManager.h"

void SceneManager::OnUpdate()
{
    if (scenes.size() > current_scene)
    {
        if (first_start)
        {
            first_start = false;
            scenes.at(current_scene)->OnCreate();
            prev_scene = current_scene;
        }
        else if (prev_scene != current_scene)
        {
            scenes.at(prev_scene)->OnDispose();
            prev_scene = current_scene;
            scenes.at(current_scene)->OnCreate();
        }
        scenes.at(current_scene)->OnUpdate();
    }
    else{
        exit(EXIT_FAILURE);
    }
}

void SceneManager::OnDispose()
{
    for (auto& scene : scenes) {
        scene->OnDispose();
    }

    scenes.clear();
}

void SceneManager::SetScene(size_t id)
{
    if (id < scenes.size())
    {
        prev_scene = current_scene;
        current_scene = id;
    }
}

void SceneManager::SetScene(const std::string& name)
{
    if (named_scenes.count(name) == 0)
    {
        SetScene(0);
        return;
    }

    auto id = named_scenes.at(name);
    SetScene(id);
}

void SceneManager::NextScene()
{
    current_scene++;
    if (current_scene >= scenes.size()){
        current_scene = 0;
    }
}