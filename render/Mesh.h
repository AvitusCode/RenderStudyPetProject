#pragma once
#include "Shader.h"
#include <vector>
#include <map>
#include <bitset>

#include <glad/glad.h>

#include "Core.h"
#include "Texture.h"
#include "Cubemap.h"
#include "../objects/Material.h"

#define G_POSITION  1
#define G_NORMAL    (1 << 1)
#define G_TEXTURE   (1 << 2)
#define G_TANGENT   (1 << 3)
#define G_BITANGENT (1 << 4)

#pragma pack(push, 1)
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};
#pragma pack(pop)

class Edge
{
public:
	Edge(GLuint f, GLuint t) : from(f), to(t) {}

	void setFrom(GLuint f) {
		from = f;
	}

	void setTo(GLuint t) {
		to = t;
	}

	GLuint getFrom() const {
		return from;
	}

	GLuint getTo() const {
		return to;
	}

private:
	GLuint from;
	GLuint to;
};

bool operator==(const Edge& lhs, const Edge& rhs);
bool operator<(const Edge& lhs, const Edge& rhs);

// Making new adjacency topology
// @param numTris - count of triangles
// @param srsIndexes - indexes od source mesh
[[nodiscard]] std::vector<GLuint> makeAdjacencyIndexes(GLuint numTris, const std::vector<GLuint>& srcIndexes);

// A mesh is any 3D object that contains vertices data, and may contain normals, textures & materials, indices etc.
class Mesh : public Core
{
public:
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, unsigned int shaderId);

	// Set config if it neccessary
	void setConfig(int param);

	// Used in render loop to draw vertices arrays to screen
	void drawObject() const override;
	void setupObject() override;

	// Rotates the object's model matrix using a vec3 or xyz floats
	Mesh& rotateMesh(float degrees, const glm::vec3& rotationPoint);
	Mesh& rotateMesh(float degrees, float x, float y, float z);

	// Translate the object's model matrix using a vec3 or xyz floats
	Mesh& translateMesh(const glm::vec3& translation);
	Mesh& translateMesh(float x, float y, float z);

	// Scales the object's model matrix using a vec3 or xyz floats
	Mesh& scaleMesh(const glm::vec3& scaleVec);
	Mesh& scaleMesh(float scaleX, float scaleY, float scaleZ);

	GLuint getVAO() const;
	const std::vector<unsigned int>& getIndices() const;
protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	std::bitset<5> config{ 0 };

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::mat4 modelMatrix; // The object's model matrix (position in world)
};
