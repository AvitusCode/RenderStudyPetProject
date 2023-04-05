#version 440 core
layout (triangles, equal_spacing, ccw) in;

uniform mat4 VP;

struct OutputPatch
{
    vec3 B030;
    vec3 B021;
    vec3 B012;
    vec3 B003;
    vec3 B102;
    vec3 B201;
    vec3 B300;
    vec3 B210;
    vec3 B120;
    vec3 B111;
    
    vec3 N200;
    vec3 N020;
    vec3 N002;
    vec3 N110;
    vec3 N011;
    vec3 N101;

    vec2 TexCoord[3];
};

patch in OutputPatch out_Patch;

out VS_FRAG
{
    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_frag_out;

vec2 interpolated2D(vec2 v0, vec2 v1, vec2 v2) {
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolated3D(vec3 v0, vec3 v1, vec3 v2) {
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main()
{
    // Интерполируем атрибуты выходящей вершины через барецентрические координаты
    vs_frag_out.TexCoords = interpolated2D(out_Patch.TexCoord[0], out_Patch.TexCoord[1], out_Patch.TexCoord[2]);
    // vs_frag_out.Normal = interpolated3D(out_Patch.Normal[0], out_Patch.Normal[1], out_Patch.Normal[2]);

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;
 
    float uPow3 = pow(u, 3);
    float vPow3 = pow(v, 3);
    float wPow3 = pow(w, 3);
    float uPow2 = pow(u, 2);
    float vPow2 = pow(v, 2);
    float wPow2 = pow(w, 2);

    vs_frag_out.WorldPos = out_Patch.B300 * wPow3 +
        out_Patch.B030 * uPow3 +
        out_Patch.B003 * vPow3 +
        out_Patch.B210 * 3.0 * wPow2 * u +
        out_Patch.B120 * 3.0 * w * uPow2 +
        out_Patch.B201 * 3.0 * wPow2 * v +
        out_Patch.B021 * 3.0 * uPow2 * v +
        out_Patch.B102 * 3.0 * w * vPow2 +
        out_Patch.B012 * 3.0 * u * vPow2 +
        out_Patch.B111 * 6.0 * w * u * v;

    vs_frag_out.Normal = out_Patch.N200 * wPow2 +
        out_Patch.N020 * uPow2 +
        out_Patch.N002 * vPow2 +
        out_Patch.N110 * w * u +
        out_Patch.N011 * u * v +
        out_Patch.N101 * w * v;

    gl_Position = VP * vec4(vs_frag_out.WorldPos, 1.0);
}
