#pragma once
#include <vector>
#include <string>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Core.h"

class Model : public  Core
{
public:
	Model() = default;
	Model(std::string path, GLuint shaderProgramID);

	void drawObject() const override;
	void setupObject() override;

	// Rotates the object's model matrix using a vec3 or xyz floats
	Model& rotateModel(float degrees, const glm::vec3& rotationPoint);
	Model& rotateModel(float degrees, float x, float y, float z);

	// Translate the object's model matrix using a vec3 or xyz floats
	Model& translateModel(const glm::vec3& translation);
	Model& translateModel(float x, float y, float z);

	// Scales the object's model matrix using a vec3 or xyz floats
	Model& scaleModel(const glm::vec3& scaleVec);
	Model& scaleModel(float scaleX, float scaleY, float scaleZ);

	const std::vector<Mesh>& getMeshes() const;
	const std::vector<Texture>& getLoadedTex() const;

private:
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};