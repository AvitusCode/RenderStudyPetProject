#version 330 core

struct DirectionalLight
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	vec3 direction;
};

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

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

vec3 phongResult(vec3 ambient, vec3 diffuse, vec3 specular, float dif, float spec, float attenuation, float intensity);
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir);

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
	vec3 Normal;
} gs_inputs;

// DEFINING OUTPUT VALUES
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

uniform Material material;

#define NR_DIR_LIGHTS 32
#define NR_POINT_LIGHTS 32
#define NR_SPOT_LIGHTS 32
uniform DirectionalLight dirLights[NR_DIR_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform int nrDirLights;
uniform int nrPointLights;
uniform int nrSpotLights;

vec3 phongResult(vec3 ambient, vec3 diffuse, vec3 specular, float dif, float spec, float attenuation, float intensity)
{
    vec3 a = ambient;
	vec3 d = diffuse * dif * material.diffuse;
	vec3 s = specular * spec * material.specular;

	return (a + d + s) * attenuation * intensity;
}

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	//vec3 reflectDir = reflect(-lightDir, normal);
	vec3 bissH = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, bissH), 0.0), 4.0 * material.shininess);

	return phongResult(light.ambientColor, light.diffuseColor, light.specularColor, diff, spec, 1.0, 1.0);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - gs_inputs.FragPos);

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	// vec3 reflectDir = reflect(-lightDir, normal);
	vec3 bissH = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, bissH), 0.0), 4.0 * material.shininess);

	// Attenuation
	float distance = length(light.position - gs_inputs.FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return phongResult(light.ambientColor, light.diffuseColor, light.specularColor, diff, spec, attenuation, 1.0);;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - gs_inputs.FragPos);
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	// vec3 reflectDir = reflect(-lightDir, normal);
	vec3 bissH = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, bissH), 0.0), 4.0 * material.shininess);

	// Attenuation
	float distance = length(light.position - gs_inputs.FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return phongResult(light.ambientColor, light.diffuseColor, light.specularColor, diff, spec, attenuation, intensity);
}

void main()
{
	vec3 viewDir = normalize(viewPos - gs_inputs.FragPos);
	vec3 result = vec3(0.0);

	for (int i = 0; i < nrDirLights; i++){
		result += calcDirLight(dirLights[i], gs_inputs.Normal, viewDir);
	}

    for (int i = 0; i < nrPointLights; i++){
		result += calcPointLight(pointLights[i], gs_inputs.Normal, gs_inputs.FragPos, viewDir);
	}

	for (int i = 0; i < nrSpotLights; i++){
		result += calcSpotLight(spotLights[i], gs_inputs.Normal, gs_inputs.FragPos, viewDir);
	}

	FragColor = vec4(result, 1.0);

	// If texture coordinates are supplied, apply the texture
	if (gs_inputs.TexCoords != 0){
		FragColor *= texture(texture_diffuse1, gs_inputs.TexCoords);
	}
}