#pragma once
#include <glad/glad.h>
#include "Vertex.h"
#include <vector>
#include "Render/Texture.h"
#include "Render/VertexBuffer.h"

struct Mesh3D
{
	// configs
	inline static GLuint position_attrib = 0;
	inline static GLuint normal_attrib = 1;
	inline static GLuint texture_attrib = 2;
	inline static GLuint tangent_attrib = 3;
	inline static GLuint bitangent_attrib = 4;
	// end configs

	bool is_normals = false;
	bool is_texture = false;
	bool is_tangents = false;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<const Texture*> textures;

	VertexBuffer VBO;
	VertexBuffer EBO;

	glm::mat4 modelMatrix;
	glm::mat4 imodelMatrix;
};