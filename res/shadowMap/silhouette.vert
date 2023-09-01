#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec3 WorldPos;

layout (std140) uniform Matrices
{
    mat4 View;
    mat4 Projection;
};

uniform mat4 Model;

void main()
{
    vec4 position = vec4(aPos, 1.0);
    WorldPos = (Model * position).xyz;
    gl_Position = Projection * View * Model * position;
}
