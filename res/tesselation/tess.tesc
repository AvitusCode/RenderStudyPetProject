#version 440 core
layout (vertices = 1) out;

uniform float gTessellationLevel;

in VS_OUT
{
    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_in[];

// all coords in world space
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

// Искользуем слово patch, чтобы подсказать программе,
// что функция переменная вычисляется для патча, а не для cp
patch out OutputPatch out_Patch;

vec3 projectToPlane(vec3 ppoint, vec3 pplane, vec3 pnormal)
{
   vec3 v = ppoint - pplane;
   float len = dot(v, pnormal);
   vec3 d = len * pnormal;

   return (ppoint - d);
}

void calcPoss()
{
    out_Patch.B030 = vs_in[0].WorldPos;
    out_Patch.B003 = vs_in[1].WorldPos;
    out_Patch.B300 = vs_in[2].WorldPos;

    vec3 EdgeB300 = out_Patch.B003 - out_Patch.B030;
    vec3 EdgeB030 = out_Patch.B300 - out_Patch.B003;
    vec3 EdgeB003 = out_Patch.B030 - out_Patch.B300;

    out_Patch.B021 = out_Patch.B030 + EdgeB300 / 3.0;
    out_Patch.B012 = out_Patch.B030 + EdgeB300 * 2.0 / 3.0;
    out_Patch.B102 = out_Patch.B003 + EdgeB030 / 3.0;
    out_Patch.B201 = out_Patch.B003 + EdgeB030 * 2.0 / 3.0;
    out_Patch.B210 = out_Patch.B300 + EdgeB003 / 3.0;
    out_Patch.B120 = out_Patch.B300 + EdgeB003 * 2.0 / 3.0;

    // Проецируем каждую точку посередине на плоскость, определяемую ближайшей вершиной и ее нормалью
    out_Patch.B021 = projectToPlane(out_Patch.B021, out_Patch.B030, vs_in[0].Normal);
    out_Patch.B012 = projectToPlane(out_Patch.B012, out_Patch.B003, vs_in[1].Normal);
    out_Patch.B102 = projectToPlane(out_Patch.B102, out_Patch.B003, vs_in[1].Normal);
    out_Patch.B201 = projectToPlane(out_Patch.B201, out_Patch.B300, vs_in[2].Normal);
    out_Patch.B210 = projectToPlane(out_Patch.B210, out_Patch.B300, vs_in[2].Normal);
    out_Patch.B120 = projectToPlane(out_Patch.B120, out_Patch.B030, vs_in[0].Normal);

    vec3 center = (out_Patch.B003 + out_Patch.B030 + out_Patch.B300) / 3.0;
    out_Patch.B111 = (out_Patch.B021 + out_Patch.B012 + out_Patch.B102 + 
                      out_Patch.B201 + out_Patch.B210 + out_Patch.B120) / 6.0;
    out_Patch.B111 += (out_Patch.B111 - center) / 2.0;
}

float housholdOp(vec3 Ni, vec3 Nj, vec3 Pi, vec3 Pj)
{
   float top    = dot((Pj - Pi), (Nj + Ni));
   float bottom = dot((Pj - Pi), (Pj - Pi));

   return 2.0 * top / bottom;
}

void calcNormals()
{
    out_Patch.N020 = vs_in[0].Normal; // 1
    out_Patch.N002 = vs_in[1].Normal; // 2
    out_Patch.N200 = vs_in[2].Normal; // 3

    float v12 = housholdOp(out_Patch.N020, out_Patch.N002, out_Patch.B030, out_Patch.B003);
    float v31 = housholdOp(out_Patch.N200, out_Patch.N020, out_Patch.B300, out_Patch.B030);
    float v23 = housholdOp(out_Patch.N002, out_Patch.N200, out_Patch.B003, out_Patch.B300);

    out_Patch.N110 = normalize(out_Patch.N020 + out_Patch.N002 - v12 * (out_Patch.B003 - out_Patch.B030));
    out_Patch.N101 = normalize(out_Patch.N020 + out_Patch.N200 - v31 * (out_Patch.B030 - out_Patch.B300));
    out_Patch.N011 = normalize(out_Patch.N002 + out_Patch.N200 - v23 * (out_Patch.B300 - out_Patch.B003));
}

void main()
{
    for (int i = 0; i < 3; i++) {
        out_Patch.TexCoord[i] = vs_in[i].TexCoords;
    }

    calcPoss();
    calcNormals();

    gl_TessLevelOuter[0] = gTessellationLevel;
    gl_TessLevelOuter[1] = gTessellationLevel;
    gl_TessLevelOuter[2] = gTessellationLevel;
    gl_TessLevelInner[0] = gTessellationLevel;
}
