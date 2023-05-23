#include "LightSystem.h"
#include "../jdEngine.h"
#include "../components/Light.h"
#include "../components/DirLight.h"
#include "../components/SpotLight.h"
#include "../components/PointLight.h"
#include "../components/Transform.h"
#include "../components/Renderable.h"
#include <unordered_map>

void LightSystem::OnInit()
{
    
}

void LightSystem::OnUpdate(float dt)
{
    auto& jd_engine = jd::Engine::getEngine(); 
    
    struct LightsComponents
    {
        GLuint nrDirs{};
        GLuint nrPoints{};
        GLuint nrSpots{};
    };
    std::unordered_map<GLuint, LightsComponents> shader_lights;

    for (const auto entity : mEntities)
    {
        const auto& light = jd_engine.GetComponent<Light>(entity);
        const auto& renderable = jd_engine.GetComponent<Renderable>(entity);

        std::string lightLocation = "";
        renderable.shader->use();

        switch (light.m_lightType)
        {
        case LightType::LIGHT_DIRLIGHT:
        {
            const auto& dirLight = jd_engine.GetComponent<DirLight>(entity);

            lightLocation = "dirLights[" + std::to_string(shader_lights[renderable.shader->getId()].nrDirs) + "]";
            renderable.shader->set(lightLocation + ".direction", dirLight.direction);
            shader_lights[renderable.shader->getId()].nrDirs++;
        }
            break;
        case LightType::LIGHT_POINTLIGHT:
        {
            const auto& pointLight = jd_engine.GetComponent<PointLight>(entity);
            const auto& transform = jd_engine.GetComponent<Transform>(entity);

            lightLocation = "pointLights[" + std::to_string(shader_lights[renderable.shader->getId()].nrPoints) + "]";
            renderable.shader->set(lightLocation + ".position", transform.position);
            renderable.shader->setFloat(lightLocation + ".constant", pointLight.constant);
            renderable.shader->setFloat(lightLocation + ".linear", pointLight.linear);
            renderable.shader->setFloat(lightLocation + ".quadratic", pointLight.quadratic);
            shader_lights[renderable.shader->getId()].nrPoints++;
        }
            break;
        case LightType::LIGHT_SPOTLIGHT:
        {
            const auto& spotLight = jd_engine.GetComponent<SpotLight>(entity);
            const auto& transform = jd_engine.GetComponent<Transform>(entity);

            lightLocation = "spotLights[" + std::to_string(shader_lights[renderable.shader->getId()].nrSpots) + "]";
            renderable.shader->set(lightLocation + ".direction", spotLight.direction);
            renderable.shader->set(lightLocation + ".position", transform.position);
            renderable.shader->setFloat(lightLocation + ".constant", spotLight.constant);
            renderable.shader->setFloat(lightLocation + ".linear", spotLight.linear);
            renderable.shader->setFloat(lightLocation + ".quadratic", spotLight.quadratic);
            renderable.shader->setFloat(lightLocation + ".cutOff", spotLight.cutOff);
            renderable.shader->setFloat(lightLocation + ".outerCutOff", spotLight.outerCutOff);
            shader_lights[renderable.shader->getId()].nrSpots++;
        }
            break;
        }

        renderable.shader->set(lightLocation + ".ambientColor", light.ambientColor);
        renderable.shader->set(lightLocation + ".diffuseColor", light.diffuseColor);
        renderable.shader->set(lightLocation + ".specularColor", light.specularColor);
    }

    for (const auto& [shader, component] : shader_lights)
    {
        glUseProgram(shader);
        glUniform1i(glGetUniformLocation(shader, "nrDirLights"), component.nrDirs);
        glUniform1i(glGetUniformLocation(shader, "nrPointLights"), component.nrPoints);
        glUniform1i(glGetUniformLocation(shader, "nrSpotLights"), component.nrSpots);
    }
}

void LightSystem::OnDispose()
{

}