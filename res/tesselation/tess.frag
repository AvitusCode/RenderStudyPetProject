#version 440 core
out vec4 FragColor;

in VS_FRAG
{
    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_frag_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct DirectionalLight
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	vec3 direction;
};

#define NR_DIR_LIGHTS 32
uniform DirectionalLight dirLights[NR_DIR_LIGHTS];
uniform int nrDirLights;

uniform vec3 viewPos;

void main()
{
    vec3 viewDir = normalize(viewPos - vs_frag_in.WorldPos);
	vec3 result = vec3(0.0);

	for (int i = 0; i < nrDirLights; i++) 
	{   
	    float ambient = 0.2;

	    vec3 lightDir = normalize(-dirLights[i].direction);
		float diff = max(dot(lightDir, vs_frag_in.Normal), 0.0);

		vec3 H = normalize(lightDir + viewDir);
		float spec = pow(max(dot(vs_frag_in.Normal, H), 0.0), 64);

		result += ambient * dirLights[i].ambientColor + 
		         diff * dirLights[i].diffuseColor * texture(texture_diffuse1, vs_frag_in.TexCoords).rgb +
				 spec * dirLights[i].specularColor * texture(texture_specular1, vs_frag_in.TexCoords).rgb;
	}

	FragColor = vec4(result, 1.0);
}
