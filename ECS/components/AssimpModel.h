#pragma once
#include "Mesh3D.h"
#include "../../render/Texture.h"
#include <string>

struct AssimpModel
{
	std::string directory;
	std::vector<Mesh3D> meshes;
};

using Model = AssimpModel;