#version 440 core
layout (location = 0) out vec4 FragColor;

in VS_OUT {
    vec2 FragPos;
    vec2 TexCoords;
} gs_inputs;

uniform sampler2D texture_diffuse1;
uniform vec3 stdColor;

void main()
{
    FragColor = vec4(stdColor, 1.0);
}