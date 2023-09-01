#include "ECS/systems/LightSystem.h"
#include "ECS/jdEngine.h"
#include "ECS/components/Light.h"
#include "ECS/components/Transform.h"
#include "ECS/components/Renderable.h"
#include <unordered_map>
#include <variant>
#include "Utils/logger.h"

std::shared_ptr<LightSystem> LightSystem::getSystem()
{
    auto& jd_engine = jd::Engine::getEngine();
    static bool sys_on = false;

    if (!sys_on)
    {
        jd_engine.RegisterComponent<Light>();
        jd_engine.RegisterComponent<Renderable>();
        jd_engine.RegisterComponent<Transform>();
        jd_engine.RegisterSystem<LightSystem>();
        Signature signature;
        signature.set(jd_engine.RegisterComponent<Light>());
        signature.set(jd_engine.RegisterComponent<Renderable>());
        jd_engine.SetSystemSignature<LightSystem>(signature);
        sys_on = true;
    }

    return jd_engine.GetSystem<LightSystem>();
}

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

        std::visit(overload{
            [&](const DirLight& dirLight) {
                lightLocation = "dirLights[" + std::to_string(shader_lights[renderable.shader->getId()].nrDirs) + "]";
                renderable.shader->set(lightLocation + ".direction", dirLight.direction);
                shader_lights[renderable.shader->getId()].nrDirs++;
            },
            [&](const PointLight& pointLight) {
                const auto& transform = jd_engine.GetComponent<Transform>(entity);

                lightLocation = "pointLights[" + std::to_string(shader_lights[renderable.shader->getId()].nrPoints) + "]";
                renderable.shader->set(lightLocation + ".position", transform.position);
                renderable.shader->setFloat(lightLocation + ".constant", pointLight.constant);
                renderable.shader->setFloat(lightLocation + ".linear", pointLight.linear);
                renderable.shader->setFloat(lightLocation + ".quadratic", pointLight.quadratic);
                shader_lights[renderable.shader->getId()].nrPoints++;
            },
            [&](const SpotLight& spotLight) {
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
            },
            [&](const auto& unknownLight) {
                LOG(ERROR) << "Promlem in Light System: unknown light!";
            }
            }, light.lightType);

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