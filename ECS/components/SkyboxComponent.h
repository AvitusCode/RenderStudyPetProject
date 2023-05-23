#pragma once
#include <glad/glad.h>
#include "../../render/VertexBuffer.h"

struct SkyboxComponent
{
	GLuint VAO;
	GLuint texId;
	VertexBuffer vbo;
	bool is_fog;
};