#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(lightPos - FragPos.xyz);
    lightDistance /= far_plane;
    gl_FragDepth = lightDistance;
}