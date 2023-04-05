#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;

out VS_OUT
{
    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
    vs_out.WorldPos = (model * vec4(aPos, 1.0)).xyz;
    vs_out.TexCoords = aTexCoords;
    vs_out.Normal = normalize(transpose(inverse(mat3(model))) * aNormal);
}