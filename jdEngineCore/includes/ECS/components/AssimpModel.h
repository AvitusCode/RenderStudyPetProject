#pragma once
#include "Mesh3D.h"
#include "Render/Texture.h"
#include <string>

struct AssimpModel
{
	std::string directory;
	std::vector<Mesh3D> meshes;
};

using Model = AssimpModel;