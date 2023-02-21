#include "Light.h"

Light::Light() : ambientColor(glm::vec3(1.0f)), diffuseColor(glm::vec3(1.0f)), specularColor(glm::vec3(1.0f))
{
}

Light::Light(const glm::vec3& aC, const glm::vec3& dC, const glm::vec3& sC) : 
	ambientColor(aC), diffuseColor(dC), specularColor(sC)
{
}