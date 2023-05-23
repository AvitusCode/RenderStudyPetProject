#include "TextureManager.h"
#include <stdexcept>

void TextureManager::loadTexture2D(const std::string& key, const std::string& fileName, const std::string& type, bool generateMipmaps)
{
	if (containsTexture(key)) {
		return;
	}

	auto tex_ptr = std::make_unique<Texture>();
	if (!tex_ptr->loadTexture2D(fileName, generateMipmaps))
	{
		std::string msg = "ERROR: Could not load texture with key '" + key + "' from file '" + fileName + "'!";
		throw std::runtime_error(msg.c_str());
	}
	tex_ptr->setTextureType(type);
	_textureCache[key] = std::move(tex_ptr);
}

const Texture* TextureManager::getTexture(const std::string& key) const
{
	if (!containsTexture(key))
	{
		std::string msg = "ERROR: Attempting to get non-existing texture with key '" + key + "'!";
		throw std::runtime_error(msg.c_str());
	}

	return _textureCache.at(key).get();
}
bool TextureManager::containsTexture(const std::string& key) const
{
	return _textureCache.count(key) > 0;
}

std::string TextureManager::containsTextureWithPath(const std::string& filePath) const
{
	for (const auto& [key, texture] : _textureCache)
	{
		if (texture->getFilePath() == filePath) {
			return key;
		}
	}

	return "";
}

bool TextureManager::deleteTexture(const std::string& key)
{
	if (containsTexture(key))
	{
		_textureCache.erase(key);
		return true;
	}

	return false;
}

void TextureManager::clearTextureCache()
{
	_textureCache.clear();
}