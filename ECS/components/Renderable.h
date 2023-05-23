#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "../../render/Shader.h"
#include "../../render/Texture.h"

struct RenderComponent
{
	GLuint VAO;
	GLenum drawModeType;
	GLenum drawArray;
	GLsizei sizeOfDraw;
	glm::mat4 Model;
	glm::mat4 inverseModel;

	std::vector<const Texture*> textures;
};

struct Renderable
{
	std::vector<RenderComponent> rdata;
	GLuint primitiveRestartIndx = 0;
	const Shader* shader = nullptr;
};
