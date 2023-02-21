#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// If you wanna use geom shader, then you need set VS_GEOM_OUT and parametets with low register first simbol
out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
	vec3 Normal;
} vs_out;

uniform mat4 model;

layout (std140) uniform Matrices
{
	mat4 view;
	mat4 projection;
};

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoord;
	vs_out.Normal = aNormal;
	gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}