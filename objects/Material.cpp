#include "Material.h"


Material::Material() : 
	ambient(glm::vec3(0.0f, 0.0f, 0.0f)), 
	diffuse(glm::vec3(0.0f, 0.0f, 0.0f)), 
	specular(glm::vec3(0.0f, 0.0f, 0.0f)), 
	shininess(0)
{

}

Material::Material(const Texture& texture)
{
	this->ambient = glm::vec3(1.0f);
	this->diffuse = glm::vec3(1.0f);
	this->specular = glm::vec3(1.0f);
	this->shininess = 1.0f;
	this->texture = texture;
}

Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, const Texture& texture)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
	this->texture = texture;
}