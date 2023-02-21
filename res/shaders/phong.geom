#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_GEOM_OUT{
    vec3 fragPos;
    vec2 texCoords;
	vec3 normal;
} gs_in[];

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
	vec3 Normal;
} gs_out;

out vec2 TexCoords;

uniform float time;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

   return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
    const float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1) / 2) * magnitude;

    return position + vec4(direction, 0.0);
}

void main()
{
    vec3 normal = GetNormal();

    for (int i = 0; i < 3; i++)
    {
       gl_Position = explode(gl_in[i].gl_Position, normal);
       gs_out.TexCoords = gs_in[i].texCoords;
       gs_out.FragPos = gs_in[i].fragPos;
       gs_out.Normal = gs_in[i].normal;
       EmitVertex();
    }

    EndPrimitive();
}