#include <glad/glad.h>
#include "Models/Model.h"
#include "ECS/jdEngine.h"
#include "Utils/stb_image.h"
#include "Render/Primitives.h"

#include <iostream>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <array>

// Start Assimp functions
void processNode(aiNode* node, const aiScene* scene, AssimpModel& dst);
Mesh3D processMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);
std::vector<const Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string& directory);

void processNode(aiNode* node, const aiScene* scene, AssimpModel& dst)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		dst.meshes.push_back(processMesh(mesh, scene, dst.directory));
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, dst);
	}
}

Mesh3D processMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory)
{
	Mesh3D mesh3D;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
			mesh3D.is_normals = true;
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
			mesh3D.is_texture = true;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vertex.Tangent.x = mesh->mTangents[i].x;
			vertex.Tangent.y = mesh->mTangents[i].y;
			vertex.Tangent.z = mesh->mTangents[i].z;

			vertex.Bitangent.x = mesh->mBitangents[i].x;
			vertex.Bitangent.y = mesh->mBitangents[i].y;
			vertex.Bitangent.z = mesh->mBitangents[i].z;

			mesh3D.is_tangents = true;
		}

		mesh3D.vertices.push_back(vertex);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			mesh3D.indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		auto loadTexture = [&mesh3D, &directory](aiMaterial* material, aiTextureType type, std::string texType)
		{
			std::vector<const Texture*> texMap = loadMaterialTextures(material, type, texType, directory);
			mesh3D.textures.insert(mesh3D.textures.end(), texMap.begin(), texMap.end());
		};

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		loadTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		loadTexture(material, aiTextureType_SPECULAR, "texture_specular");
		loadTexture(material, aiTextureType_NORMALS, "texture_normal");
		loadTexture(material, aiTextureType_HEIGHT, "texture_height");
		loadTexture(material, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
		loadTexture(material, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");
	}

	return mesh3D;
}
	
std::vector<const Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::string& directory)
{
	auto& engine = jd::Engine::getEngine();
	auto& textureManager = engine.getTextureManager();

	std::vector<const Texture*> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		std::string path = directory + "/" + std::string(str.C_Str());
		std::string key = "assimp_" + path;

		textureManager.loadTexture2D(key, path, typeName);

		const Texture* texture = textureManager.getTexture(key);
		textures.push_back(texture);
	}

	return textures;
}


std::unique_ptr<AssimpModel> loadModel(const std::string& path, unsigned int flag)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, flag);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		std::string msg = "ERROR: ASSIMP problems: " + std::string(import.GetErrorString());
		throw std::runtime_error(msg.c_str());
	}

	std::unique_ptr<AssimpModel> resultModel = std::make_unique<AssimpModel>();
	resultModel->directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene, *resultModel);

	return resultModel;
}
// End Assimp Functions

// Start mesh Functions
bool setUpStaticMesh(Mesh3D& mesh, Renderable& renderable)
{
	RenderComponent rc;
	for (auto& tex : mesh.textures) {
		tex->complete();
		rc.textures.push_back(tex);
	}

	glGenVertexArrays(1, &rc.VAO);
	glBindVertexArray(rc.VAO);
	rc.drawModeType = GL_TRIANGLES;
	rc.drawArray = GL_ARRAY_BUFFER;
	rc.sizeOfDraw = static_cast<GLsizei>(mesh.vertices.size());

	mesh.VBO.makeVBO(mesh.vertices.size() * sizeof(Vertex));
	mesh.VBO.bindVBO(GL_ARRAY_BUFFER);
	mesh.VBO.addRawData(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));
	mesh.VBO.uploadDataToGPU(GL_STATIC_DRAW, true);

	if (!mesh.indices.empty())
	{
		rc.drawArray = GL_ELEMENT_ARRAY_BUFFER;
		rc.sizeOfDraw = static_cast<GLsizei>(mesh.indices.size());

		mesh.EBO.makeVBO(mesh.indices.size() * sizeof(GLuint));
		mesh.EBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
		mesh.EBO.addRawData(mesh.indices.data(), mesh.indices.size() * sizeof(GLuint));
		mesh.EBO.uploadDataToGPU(GL_STATIC_DRAW, true);
	}

	glEnableVertexAttribArray(Mesh3D::position_attrib);
	glVertexAttribPointer(Mesh3D::position_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	if (mesh.is_normals) {
		glEnableVertexAttribArray(Mesh3D::normal_attrib);
		glVertexAttribPointer(Mesh3D::normal_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	}

	if (mesh.is_texture) {
		glEnableVertexAttribArray(Mesh3D::texture_attrib);
		glVertexAttribPointer(Mesh3D::texture_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	}

	if (mesh.is_tangents) {
		glEnableVertexAttribArray(Mesh3D::tangent_attrib);
		glVertexAttribPointer(Mesh3D::tangent_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

		glEnableVertexAttribArray(Mesh3D::bitangent_attrib);
		glVertexAttribPointer(Mesh3D::bitangent_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	}

	glBindVertexArray(0);

	renderable.rdata.push_back(std::move(rc));
	return true;
}


void getTextureSkybox(const std::string& path, SkyboxComponent& skyComponent)
{
	std::array<std::string, 6> faces;

	faces[0] = path + "right.png";
	faces[1] = path + "left.png";
	faces[2] = path + "top.png";
	faces[3] = path + "bottom.png";
	faces[4] = path + "front.png";
	faces[5] = path + "back.png";

	glGenTextures(1, &skyComponent.texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyComponent.texId);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(false);
	for (size_t i = 0; i < faces.size(); i++)
	{
		uint8_t* image = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (image)
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

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i), 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

			stbi_image_free(image);
		}
		else
		{
			std::string msg = "Error with skybox loading";
			throw std::runtime_error(msg.c_str());
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

SkyboxComponent loadSkybox(const std::string& path)
{
	SkyboxComponent skyComponent;
	getTextureSkybox(path, skyComponent);

	glGenVertexArrays(1, &skyComponent.VAO);
	glBindVertexArray(skyComponent.VAO);

	skyComponent.vbo.makeVBO(sizeof(Primitives::cubeVertices));
	skyComponent.vbo.bindVBO(GL_ARRAY_BUFFER);
	skyComponent.vbo.addData(Primitives::cubeVertices);
	skyComponent.vbo.uploadDataToGPU(GL_STATIC_DRAW, true);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);

	return skyComponent;
}