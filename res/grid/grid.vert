#version 440 core
// @Author page from http://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/

out vec3 nearPoint;
out vec3 farPoint;

layout (std140) uniform Matrices
{
	mat4 View;
	mat4 Projection;
};

const vec3 gridPlane[6] = vec3[]
(
	vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 UnprojectPoint(float x, float y, float z, mat4 vi, mat4 pr)
{
	mat4 viewInv = inverse(vi);
	mat4 projInv = inverse(pr);
	vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);

	return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main()
{
	vec3 p = gridPlane[gl_VertexID].xyz;
	nearPoint = UnprojectPoint(p.x, p.y, 0.0, View, Projection);
	farPoint = UnprojectPoint(p.x, p.y, 1.0, View, Projection);

	gl_Position = vec4(p, 1.0);
}