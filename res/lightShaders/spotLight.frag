#version 440 core
// need you include Material

#include_part

#include "material.frag"

struct SpotLight
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	vec3 position;
	float constant;
	float linear;
	float quadratic;

	vec3 direction;
	float cutOff;
	float outerCutOff;
};

vec3 calcSpotLight(const SpotLight light, const vec3 normal, const vec3 FragPos, const vec3 viewDir, const Material material);

#definition_part

vec3 calcSpotLight(const SpotLight light, const vec3 normal, const vec3 FragPos, const vec3 viewDir, const Material material)
{
    vec3 lightDir = normalize(light.position - FragPos);
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3 bissH = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, bissH), 0.0), material.shininess);

	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return (light.ambientColor + light.diffuseColor * diff * material.diffuse + light.specularColor * spec * material.specular) * attenuation * intensity;
}
