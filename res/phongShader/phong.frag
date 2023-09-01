#version 440 core
layout (location = 0) out vec4 FragColor;

#include "../lightShaders/material.frag"
#include "../lightShaders/dirLight.frag"
#include "../lightShaders/pointLight.frag"
#include "../lightShaders/spotLight.frag"
#include "../fogShader/fog.frag"

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
	vec3 Normal;
} gs_inputs;

smooth in vec4 eyeSpaceCoords;

uniform Fog fogStruct;

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

void main()
{
    vec3 viewDir = normalize(viewPos - gs_inputs.FragPos);
    vec3 result = vec3(0.0);

    for (int i = 0; i < nrDirLights; i++) {
        result += calcDirLight(dirLights[i], gs_inputs.Normal, viewDir, material);
    }

    for (int i = 0; i < nrPointLights; i++) {
        result += calcPointLight(pointLights[i], gs_inputs.Normal, gs_inputs.FragPos, viewDir, material);
    }

    for (int i = 0; i < nrSpotLights; i++) {
        result += calcSpotLight(spotLights[i], gs_inputs.Normal, gs_inputs.FragPos, viewDir, material);
    }

    FragColor = vec4(result, 1.0) * texture(texture_diffuse1, gs_inputs.TexCoords);

    // experimental Fog
    // float fogCoordinates = abs(eyeSpaceCoords.z / eyeSpaceCoords.w);
    // FragColor = mix(FragColor, vec4(fogStruct.color, 1.0), getFogFactorExp2(fogStruct, fogCoordinates));
}