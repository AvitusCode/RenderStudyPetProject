#version 440 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

layout (std140) uniform Matrices
{
    mat4 View;
    mat4 Projection;
};

uniform mat4 Model;

out VS_OUT {
    vec2 FragPos;
    vec2 TexCoords;
} gs_outputs;

void main()
{
    gs_outputs.TexCoords = aTexCoords;
    gs_outputs.FragPos = vec2(Model * vec4(aPos, 0.0, 1.0));
    gl_Position = Projection * View * Model * vec4(aPos, 0.0, 1.0);
}