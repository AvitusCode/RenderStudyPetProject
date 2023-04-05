#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Model.h"

Model::Model(std::string path, GLuint shaderProgramID)
{
	this->shaderProgramID = shaderProgramID;
	loadModel(path);
}

const std::vector<Mesh>& Model::getMeshes() const {
	return meshes;
}

const std::vector<Texture>& Model::getLoadedTex() const {
	return textures_loaded;
}

void Model::drawObject() const
{
	for (int i = 0; i < meshes.size(); i++){
		meshes[i].drawObject();
	}
}

void Model::setupObject()
{
	for (int i = 0; i < meshes.size(); i++){
		meshes[i].setupObject();
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes (if any)
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; i++){
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; 

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}

		vertices.push_back(vertex);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++){
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		std::vector<Texture> roughnessMap = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
		textures.insert(textures.end(), roughnessMap.begin(), roughnessMap.end());

		std::vector<Texture> aoMap = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");
		textures.insert(textures.end(), aoMap.begin(), aoMap.end());
	}

	return Mesh(vertices, indices, textures, shaderProgramID);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (size_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (size_t j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].getPath().data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; 
				break;
			}
		}

		if (!skip)
		{
			std::string path = directory + '/' + std::string(str.C_Str());
			Texture texture = Texture(path, typeName, false);
			texture.setPath(str.C_Str());
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

Model& Model::rotateModel(float degrees, const glm::vec3& rotationPoint)
{
	for (size_t i = 0; i < meshes.size(); i++){
		meshes[i].rotateMesh(degrees, rotationPoint);
	}

	return *this;
}

Model& Model::rotateModel(float degrees, float x, float y, float z)
{
	for (size_t i = 0; i < meshes.size(); i++){
		meshes[i].rotateMesh(degrees, x, y, z);
	}

	return *this;
}

Model& Model::translateModel(const glm::vec3& translation)
{
	for (int i = 0; i < meshes.size(); i++){
		meshes[i].translateMesh(translation);
	}

	return *this;
}

Model& Model::translateModel(float x, float y, float z)
{
	for (int i = 0; i < meshes.size(); i++){
		meshes[i].translateMesh(x, y, z);
	}

	return *this;
}

Model& Model::scaleModel(const glm::vec3& scaleVec)
{
	for (int i = 0; i < meshes.size(); i++){
		meshes[i].scaleMesh(scaleVec);
	}

	return *this;
}

Model& Model::scaleModel(float scaleX, float scaleY, float scaleZ)
{
	for (int i = 0; i < meshes.size(); i++){
		meshes[i].scaleMesh(scaleX, scaleY, scaleZ);
	}

	return *this;
}
