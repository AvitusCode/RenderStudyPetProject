#include "Cubemap.h"

#include <iostream>
#include "../stb_image.h"

Cubemap::Cubemap() : texID(0){

}

Cubemap::Cubemap(std::string facesPath) : texID(0)
{
	faces.push_back(facesPath + "right.png");
	faces.push_back(facesPath + "left.png");
	faces.push_back(facesPath + "top.png");
	faces.push_back(facesPath + "bottom.png");
	faces.push_back(facesPath + "front.png");
	faces.push_back(facesPath + "back.png");
}

Cubemap::Cubemap(const std::vector<std::string>& faces) : texID(0){
	this->faces = faces;
}

Cubemap& Cubemap::operator=(const Cubemap& cm)
{
	if (this != &cm) {
		if (texID) {
			glDeleteTextures(1, &texID);
		}
		texID = cm.texID;
		faces = cm.faces;
	}
	
	return *this;
}

Cubemap& Cubemap::operator=(Cubemap&& cm) noexcept
{
	if (this != &cm)
	{
		if (texID) {
			glDeleteTextures(1, &texID);
		}
		texID = cm.texID;
		cm.texID = 0;
		faces = std::move(cm.faces);
	}

	return *this;
}

void Cubemap::setupObject()
{
	if (texID){
		glDeleteTextures(1, &texID);
	}

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	int width, height, nrChannels;
	
	for (int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format = GL_RGB;
			if (nrChannels == 1) {
				format = GL_RED;
			}
			if (nrChannels == 3) {
				format = GL_RGB;
			}
			if (nrChannels == 4) {
				format = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cerr << "ERROR: Failed to load cubemap texture at path: " << faces[i] << std::endl;
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &texID);
			texID = 0;
			return;
		}

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

unsigned int Cubemap::getId() const {
	return texID;
}
const std::vector<std::string>& Cubemap::getFaces() const {
	return faces;
}