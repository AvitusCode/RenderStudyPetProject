#pragma once
#include "Scene.h"
#include "../Assets.h"
#include <array>
#include <glm/glm.hpp>

class Model;

class scene_4 : public Scene
{
public:
	void OnCreate() override;
	void OnUpdate() override;
	void OnDispose() override;

	const Assets& getAssets() const;

private:
    struct MaterialPBRTexture
    {
        unsigned int Albedo;
        unsigned int Normal;
        unsigned int Mettalic;
        unsigned int Roughness;
        unsigned int AO;
        glm::vec3 position;
    };
	Assets assets;
    std::array<MaterialPBRTexture, 5> materials;

    unsigned int envCubemap;
    unsigned int irradianceMap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;

    // EXPERIMENTAL
    unsigned int hdrFBO;
    unsigned int rboDepth;
    unsigned int colorBuffers[2];
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];
};