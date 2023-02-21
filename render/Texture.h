#pragma once

#include <string>

// A helper class to easily load and use 2D textures
class Texture
{
public:
	Texture();
	Texture(const std::string& filename, std::string type, bool stbiFlipOnLoad = false);

	void bindTexture();
	void unbindTexture();

	void setTexture(const std::string& filename, std::string type, bool stbiFlipOnLoad);

	unsigned int getId() const;
	const std::string& getType() const;
	const std::string& getPath() const;
	void setPath(const char* _path);

	Texture& operator=(const Texture& tex);

private:
	unsigned int texID;
	std::string type;
	std::string path;
};