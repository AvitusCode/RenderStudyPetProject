#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>

class Skybox : public Mesh
{
public:
	Skybox(unsigned int shaderProgramID, const Cubemap& cubemap);

	void Core::drawObject() const override;
	void Core::setupObject() override;

	const Cubemap& getCubemap() const;
	Cubemap& getCubemap();

private:
	Cubemap cubemap;
	static glm::vec3 boxVertices[];
};