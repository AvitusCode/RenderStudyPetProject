#include "Render/ShaderManager.h"
#include <stdexcept>


Shader& ShaderManager::createShaderProgram(const std::string& key)
{
    if (containsShaderProgram(key))
    {
        std::string msg = "Shader program with key '" + key + "' already exists!";
        throw std::runtime_error(msg.c_str());
    }

    auto shaderProgram = std::make_unique<Shader>();
    shaderProgram->createProgram();
    shader_cashe[key] = std::move(shaderProgram);

    return getShaderProgram(key);
}

Shader& ShaderManager::getShaderProgram(const std::string& key) const
{
    if (!containsShaderProgram(key))
    {
        std::string msg = "Attempting to get non-existing shader program with key '" + key + "'!";
        throw std::runtime_error(msg.c_str());
    }

    return *shader_cashe.at(key);
}

bool ShaderManager::containsShaderProgram(const std::string& key) const
{
    return shader_cashe.count(key) > 0;
}