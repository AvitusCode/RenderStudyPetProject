#pragma once
#include "Texture.h"
#include <memory>
#include <unordered_map>

class TextureManager
{
public:
    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    void operator=(const TextureManager&) = delete;

    void loadTexture2D(const std::string& key, const std::string& fileName, const std::string& type = "", bool generateMipmaps = true);
    const Texture* getTexture(const std::string& key) const;
    bool containsTexture(const std::string& key) const;
    std::string containsTextureWithPath(const std::string& filePath) const;
    bool deleteTexture(const std::string& key);
    void clearTextureCache();

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> _textureCache; // Texture cache
};