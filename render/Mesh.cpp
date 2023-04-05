#include "Mesh.h"
#include <iostream>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

bool operator==(const Edge& lhs, const Edge& rhs) {
	return lhs.getFrom() == rhs.getFrom() && lhs.getTo() == rhs.getTo();
}

bool operator<(const Edge& lhs, const Edge& rhs)
{
	if (lhs.getFrom() < rhs.getFrom()) {
		return true;
	}

	if (lhs.getFrom() > rhs.getFrom()) {
		return false;
	}

	return lhs.getTo() < rhs.getTo();
}

struct adjHash {
	size_t operator()(const Edge& edge) const noexcept
	{
		const size_t x = 2'946'901;
		const auto uiHash = std::hash<GLuint>{};
		return uiHash(edge.getFrom()) * x + uiHash(edge.getTo());
	}
};

[[nodiscard]] std::vector<GLuint> makeAdjacencyIndexes(GLuint numTris, const std::vector<GLuint>& srcIndexes)
{
	assert(3 * numTris == srcIndexes.size());

	const GLuint n = 3 * numTris;
	std::vector<GLuint> result(2 * n);
	std::unordered_map<Edge, GLuint, adjHash> adj;

	for (GLuint i = 0; i < numTris; i++)
	{
		const GLuint j = 3 * i;
		adj[Edge{ srcIndexes[j], srcIndexes[j + 1] }] = srcIndexes[j + 2];
		adj[Edge{ srcIndexes[j + 1], srcIndexes[j + 2] }] = srcIndexes[j];
		adj[Edge{ srcIndexes[j + 2], srcIndexes[j] }] = srcIndexes[j + 1];
	}

	for (GLuint i = 0; i < numTris; i++)
	{
		const GLuint j = 6 * i;
		const GLuint k = 3 * i;

		result[j] = srcIndexes[k];
		result[j + 2] = srcIndexes[k + 1];
		result[j + 4] = srcIndexes[k + 2];

		result[j + 1] = adj[Edge{ srcIndexes[k + 1], srcIndexes[k] }];
		result[j + 3] = adj[Edge{ srcIndexes[k + 2], srcIndexes[k + 1] }];
		result[j + 5] = adj[Edge{ srcIndexes[k], srcIndexes[k + 2] }];
	}

	return result;
}

Mesh::Mesh()
{
	this->shaderProgramID = 0;
	this->VAO = 0;
	this->VBO = 0;
	this->EBO = 0;
	this->modelMatrix = glm::mat4(1.0f);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, unsigned int shaderId)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->shaderProgramID = shaderId;
	this->modelMatrix = glm::mat4(1.0f);
	this->VAO = 0;
	this->VBO = 0;
	this->EBO = 0;
	//setupObject();
}

void Mesh::setConfig(int param){
	config ^= param;
}

GLuint Mesh::getVAO() const {
	return VAO;
}
const std::vector<unsigned int>& Mesh::getIndices() const{
	return indices;
}

void Mesh::drawObject() const
{
	glBindVertexArray(VAO);

	if (textures.size() != 0)
	{
		size_t diffuseNr = 1;
		size_t specularNr = 1;
		size_t normalNr = 1;
		size_t heightNr = 1;
		size_t roughnessNr = 1;
		size_t aoNr = 1;

		size_t textureNr = 1;

		for (size_t i = 0; i < textures.size(); i++)
		{
			std::string number;
			std::string name = textures[i].getType();

			// TODO: fix the problem with tex activation

			if (name == "texture_diffuse"){
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular"){
				number = std::to_string(specularNr++);
			}
			else if (name == "texture_normal"){
				number = std::to_string(normalNr++);
			}
			else if (name == "texture_height"){
				number = std::to_string(heightNr++);
			}
			else if (name == "texture") {
				number = std::to_string(textureNr++);
			}
			else if (name == "texture_roughness") {
				number = std::to_string(roughnessNr++);
			}
			else if (name == "texture_ao") {
				number = std::to_string(aoNr++);
			}
			else {
				std::cout << "No such texture: " << name << std::endl;
				continue;
			}

			int locationId = glGetUniformLocation(shaderProgramID, (name + number).c_str());
			if (locationId == -1) {
				continue;
			}

			glUniform1i(locationId, i);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i].getId());
		}
	}

	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, &modelMatrix[0][0]);

	if (m_mode == GL_PATCHES) {
		glPatchParameteri(GL_PATCH_VERTICES, 1);
	}

	if (indices.size() == 0){
		glDrawArrays(m_mode, 0, (GLsizei)vertices.size() * sizeof(float));
	}
	else{
		glDrawElements(m_mode, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	}
}

void Mesh::setupObject()
{
	if (VAO || VBO || EBO)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		VAO = VBO = EBO = 0;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	if (indices.size() > 0) 
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}

	if (!config[0]) {
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	}

	if (!config[1]) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	}

	if (!config[2]) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	}

	if (!config[3]) {
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	}

	if (!config[4]) {
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	}

	glBindVertexArray(0);
}

Mesh& Mesh::rotateMesh(float degrees, const glm::vec3& rotationPoint)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), rotationPoint);
	return *this;
}

Mesh& Mesh::rotateMesh(float degrees, float x, float y, float z)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), glm::vec3(x, y, z));
	return *this;
}

Mesh& Mesh::translateMesh(const glm::vec3& translation)
{
	modelMatrix = glm::translate(modelMatrix, translation);
	return *this;
}

Mesh& Mesh::translateMesh(float x, float y, float z)
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
	return *this;
}

Mesh& Mesh::scaleMesh(const glm::vec3& scaleVec)
{
	modelMatrix = glm::scale(modelMatrix, scaleVec);
	return *this;
}

Mesh& Mesh::scaleMesh(float scaleX, float scaleY, float scaleZ)
{
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, scaleZ));
	return *this;
}