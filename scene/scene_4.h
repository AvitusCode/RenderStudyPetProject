#pragma once
#include "Scene.h"
#include "../Assets.h"
#include <array>
#include <glm/glm.hpp>
#include "../primitives/Cube.h"
#include "../primitives/Quad.h"
#include "../primitives/Sphere.h"

class Model;
class GLFWwindow;

class scene_4 : public Scene
{
public:
    scene_4(GLFWwindow* win) : window(win) {}
	void OnCreate() override;
	void OnUpdate(float dt) override;
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

    GLFWwindow* window;

	Assets assets;
    std::array<MaterialPBRTexture, 5> materials;
    Cube m_cube;
    Quad m_quad;
    Sphere m_sphere;

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