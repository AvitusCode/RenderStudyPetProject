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

uniform mat4 Model;
// uniform mat4 iModel;

out VS_OUT
{
    vec3 fragPos;
    vec3 Normal;
    smooth vec2 TexCoords;
} vs_out;


void main()
{
    vs_out.fragPos = (Model * vec4(aPos, 1.0)).xyz;
    vs_out.Normal = normalize(transpose(inverse(mat3(Model))) * aNormal);
    vs_out.TexCoords = aTexCoords;

    gl_Position = Projection * View * vec4(vs_out.fragPos, 1.0);
}