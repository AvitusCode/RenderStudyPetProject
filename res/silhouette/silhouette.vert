#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec3 WorldPos;

uniform mat4 MWP;
uniform mat4 model;

void main()
{
    vec4 position = vec4(aPos, 1.0);
    WorldPos = (model * position).xyz;
    gl_Position = MWP * position;
}