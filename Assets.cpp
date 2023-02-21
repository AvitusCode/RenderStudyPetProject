#include <iostream>
#include "Assets.h"
#include "render/Shader.h"
#include "render/Texture.h"
#include "objects/Material.h"

void Assets::storeShader(Shader* shader, const std::string& name){
	shader_map[name] = shader;
}

Shader* Assets::getShader(const std::string& name) const
{
	try {
		return shader_map.at(name);
	}
	catch (...) {
		std::cerr << "ERROR: there is no some shader" << std::endl;
	}

	return nullptr;
}

void Assets::storeTexture(Texture* texture, const std::string& name){
	texture_map[name] = texture;
}

Texture* Assets::getTexture(const std::string& name) const
{
	try {
		return texture_map.at(name);
	}
	catch (...) {
		std::cerr << "ERROR: there is no some texture" << std::endl;
	}

	return nullptr;
}

void Assets::storeMaterial(Material* material, const std::string& name) {
	material_map[name] = material;
}

Material* Assets::getMaterial(const std::string& name) const {
	try {
		return material_map.at(name);
	}
	catch (...) {
		std::cerr << "ERROR: there is no some Material" << std::endl;
	}

	return nullptr;
}

Assets::~Assets()
{
	for (auto& [name, shader] : shader_map) {
		delete shader;
	}

	for (auto& [name, texture] : texture_map) {
		delete texture;
	}

	for (auto& [name, material] : material_map) {
		delete material;
	}
}