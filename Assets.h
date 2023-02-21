#pragma once
#include <string>
#include <unordered_map>

class Shader;
class Texture;
struct Material;

class Assets
{
public:
    Assets() = default;

    void storeShader(Shader* shader, const std::string& name);
    [[nodiscard]] Shader* getShader(const std::string& name) const;

    void storeTexture(Texture* texture, const std::string& name);
    [[nodiscard]] Texture* getTexture(const std::string& name) const;

    void storeMaterial(Material* material, const std::string& name);
    [[nodiscard]] Material* getMaterial(const std::string& name) const;

    ~Assets();

private:
    std::unordered_map<std::string, Shader*>  shader_map;
    std::unordered_map<std::string, Texture*> texture_map;
    std::unordered_map<std::string, Material*> material_map;
};