#version 460 core
layout (triangles_adjacency) in;
layout (line_strip, max_vertices = 18) out;

in vec3 WorldPos[];

uniform vec3 lightPos;

void EmitLine(int p0, int p1)
{
    gl_Position = gl_in[p0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[p1].gl_Position;
    EmitVertex();

    EndPrimitive();
}

// triangles_adjacency
//
//   1-------2--e4---3
//    -     -  -   -
//   e3-   e1  e5 -
//       0 -e2-- 4
//        -     -
//      e6 -   -
//           5

void main()
{
    const float EPS = 0.00001;
    vec3 e1 = WorldPos[2] - WorldPos[0];
    vec3 e2 = WorldPos[4] - WorldPos[0];
    vec3 e3 = WorldPos[1] - WorldPos[0];
    vec3 e4 = WorldPos[3] - WorldPos[2];
    vec3 e5 = WorldPos[4] - WorldPos[2];
    vec3 e6 = WorldPos[5] - WorldPos[0];

    vec3 lightDir = normalize(lightPos - WorldPos[0]);
    vec3 Normal = cross(e1, e2);

    if (dot(Normal, lightDir) > EPS)
    {
        Normal = cross(e3, e1);
        if (dot(Normal, lightDir) <= 0) {
            EmitLine(0, 2);
        }

        Normal = cross(e4, e5);
        lightDir = normalize(lightPos - WorldPos[2]);
        if (dot(Normal, lightDir) <= 0) {
            EmitLine(2, 4);
        }

        Normal = cross(e2, e6);
        lightDir = normalize(lightPos - WorldPos[4]);
        if (dot(Normal, lightDir) <= 0) {
            EmitLine(4, 0);
        }
    }
}