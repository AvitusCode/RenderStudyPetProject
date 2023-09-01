#pragma once
#include "ECS/components/AssimpModel.h"
#include "ECS/components/Renderable.h"
#include "ECS/components/SkyboxComponent.h"
#include <assimp/postprocess.h>
#include <memory>

/*
* Load model to AssimpModel component
* @param path: path with directories to the source file
* @param flag: set configurations to calc normals, tangentSpace and etc;
* @return std::optional<AssimpModel>: AssimpModel if loading process has been successed
*/

std::unique_ptr<AssimpModel> loadModel(const std::string& path, unsigned int flag = 
	aiProcess_Triangulate |
	aiProcess_FlipUVs |
	aiProcess_CalcTangentSpace |
	aiProcess_GenSmoothNormals);

/*
* Setup static mesh
* @param mesh: static mesh
* @return bool: is mesh correctly uploaded
*/
bool setUpStaticMesh(Mesh3D& mesh, Renderable& renderable);

/*
* 
* Load skybox component
* @param path: Path to the source file
* @return SkyboxComponent: component of the skybox
*/
SkyboxComponent loadSkybox(const std::string& path);

