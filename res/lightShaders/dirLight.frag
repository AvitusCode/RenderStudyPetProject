#version 440 core

#include_part

#include "material.frag"

struct DirectionalLight
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	vec3 direction;
};

vec3 calcDirLight(const DirectionalLight light, const vec3 normal, const vec3 viewDir, const Material material);

#definition_part

vec3 calcDirLight(const DirectionalLight light, const vec3 normal, const vec3 viewDir, const Material material)
{
    vec3 lightDir = normalize(-light.direction);

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3 bissH = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, bissH), 0.0), material.shininess);

	return (light.ambientColor + light.diffuseColor * diff * material.diffuse + light.specularColor * spec * material.specular);
}