#include "Texture.h"

#include <iostream>

#include <glad/glad.h>
#include "../stb_image.h"

Texture::Texture() : texID(0), type(""), path("") {};

Texture::Texture(const std::string& filename, std::string type, bool stbiFlipOnLoad){
	setTexture(filename, std::move(type), stbiFlipOnLoad);
}

void Texture::setTexture(const std::string& filename, std::string type, bool stbiFlipOnLoad)
{
	this->type = std::move(type);
	// Create OpenGL texture
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(stbiFlipOnLoad);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrChannels == 1) {
			format = GL_RED;
		}
		else if (nrChannels == 3) {
			format = GL_RGB;
		}
		else if (nrChannels == 4) {
			format = GL_RGBA;
		}
		else {
			texID = 0;
			std::cerr << "ERROR::texture loading problem, name: " << this->type << ", File:" << filename << std::endl;
			stbi_image_free(data);
			return;
		}

		glGenTextures(1, &texID);

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else
	{
		texID = 0;
		std::cerr << "ERROR::texture loading problem, name: " << this->type << ", File:" << filename << std::endl;
	}

	stbi_image_free(data);
}

void Texture::setPath(const char* _path) {
	path = _path;
}

Texture& Texture::operator=(const Texture& tex)
{
	texID = tex.texID;
	type = tex.type;
	path = tex.path;

	return *this;
}

unsigned int Texture::getId() const {
	return texID;
}
const std::string& Texture::getType() const{
	return type;
}
const std::string& Texture::getPath() const {
	return path;
}

void Texture::bindTexture(){
	glBindTexture(GL_TEXTURE_2D, texID);
}

void Texture::unbindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}