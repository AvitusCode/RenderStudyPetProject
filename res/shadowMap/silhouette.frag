#version 440 core
out vec4 FragColor;

uniform vec3 colorSilhouette;

void main()
{
    FragColor = vec4(colorSilhouette, 1.0);
}
