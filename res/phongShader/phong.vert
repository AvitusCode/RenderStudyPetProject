#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangetn;

layout (std140) uniform Matrices
{
    mat4 View;
    mat4 Projection;
};

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
	vec3 Normal;
} gs_outputs;

smooth out vec4 eyeSpaceCoords;

uniform mat4 Model;

void main()
{
    gs_outputs.FragPos = vec3(Model * vec4(aPos, 1.0));
    gs_outputs.TexCoords = aTexCoords;
    gs_outputs.Normal = normalize(transpose(inverse(mat3(Model))) * aNormal);

    eyeSpaceCoords = View * Model * vec4(aPos, 1.0);

    gl_Position = Projection * View * Model * vec4(aPos, 1.0);
}