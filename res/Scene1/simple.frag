#version 440 core
layout (location = 0) out vec4 FragColor;

in VS_OUT
{
    vec3 fragPos;
    vec3 Normal;
    smooth vec2 TexCoords;
} vs_in;

uniform sampler2D texture_diffuse1;

const vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
const vec3 lightPos = vec3(0.0f, 10.0f, 0.0f);

void main()
{
    float ambient = 0.2;
    vec3 ambientLight = ambient * lightColor;

    vec3 lightDir = normalize(lightPos - vs_in.fragPos);
    float diff = max(dot(lightDir, vs_in.Normal), 0.0);
    vec3 diffuseLight = diff * lightColor;

    //vec3 result = (ambientLight + diffuseLight) * texture(texture_diffuse1, vs_in.TexCoords).rgb;
    vec3 result = texture(texture_diffuse1, vs_in.TexCoords).rgb;

    FragColor = vec4(result, 1.0);
}
