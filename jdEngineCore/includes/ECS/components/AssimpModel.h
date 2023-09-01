#pragma once
#include "Mesh3D.h"
#include "Render/Texture.h"
#include <string>

struct AssimpModel
{
	std::string directory{"error"};
	std::vector<Mesh3D> meshes;
	bool is_static = true;
};

struct JdModel
{
	AssimpModel* model;
};

using Model = AssimpModel;