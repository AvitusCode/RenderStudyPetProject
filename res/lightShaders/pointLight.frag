#version 440 core

#include_part

#include "material.frag"

struct PointLight
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

vec3 calcPointLight(const PointLight light, const vec3 normal, const vec3 FragPos, const vec3 viewDir, const Material material);

#definition_part

vec3 calcPointLight(const PointLight light, const vec3 normal, const vec3 FragPos, const vec3 viewDir, const Material material)
{
    vec3 lightDir = normalize(light.position - FragPos);

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3 bissH = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, bissH), 0.0), material.shininess);

	// Attenuation
	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return (light.ambientColor + light.diffuseColor * diff * material.diffuse + light.specularColor * spec * material.specular) * attenuation;
}