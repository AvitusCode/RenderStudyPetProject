#include "scene_4.h"
#include "../main.h"
#include "../render/Shader.h"
#include "../render/Model.h"
#include "../stb_image.h"
#include "../Window.h"
#include "../objects/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <vector>
#include <iostream>

unsigned int loadTexture(const char* path);

void scene_4::OnCreate()
{
    auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
    Camera& camera = handle.getCamera();
    camera.getPosition() = glm::vec3(0.0f, 0.0f, 15.0f);

	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    ShaderComponent vertexComponent;
    ShaderComponent fragmentComponent;

    vertexComponent.loadComponentFromFile("res/pbr_shaders/pbr.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/pbr_shaders/pbr.frag", GL_FRAGMENT_SHADER);
    Shader* pbrShader = new Shader(vertexComponent, fragmentComponent);

    vertexComponent.loadComponentFromFile("res/pbr_shaders/cubemap.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/pbr_shaders/transform_to_cubemap.frag", GL_FRAGMENT_SHADER);
    Shader* transformToCubemapShader = new Shader( vertexComponent, fragmentComponent );

    fragmentComponent.loadComponentFromFile("res/pbr_shaders/irradiance.frag", GL_FRAGMENT_SHADER);
    Shader* irradianceShader = new Shader( vertexComponent, fragmentComponent );

    fragmentComponent.loadComponentFromFile("res/pbr_shaders/prefilter.frag", GL_FRAGMENT_SHADER);
    Shader* prefilterShader = new Shader( vertexComponent, fragmentComponent );

    vertexComponent.loadComponentFromFile("res/pbr_shaders/brdf.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/pbr_shaders/brdf.frag", GL_FRAGMENT_SHADER);
    Shader* brdfShader = new Shader( vertexComponent, fragmentComponent );

    vertexComponent.loadComponentFromFile("res/pbr_shaders/background.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/pbr_shaders/background.frag", GL_FRAGMENT_SHADER);
    Shader* backgroundShader = new Shader( vertexComponent, fragmentComponent );

    vertexComponent.loadComponentFromFile("res/pbr_shaders/blur.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/pbr_shaders/blur.frag", GL_FRAGMENT_SHADER);
    Shader* blurShader = new Shader( vertexComponent, fragmentComponent );

    fragmentComponent.loadComponentFromFile("res/pbr_shaders/bloom_final.frag", GL_FRAGMENT_SHADER);
    Shader* bloomShader = new Shader( vertexComponent, fragmentComponent );

    assets.storeShader(pbrShader, "pbr");
    assets.storeShader(transformToCubemapShader, "transformToCube");
    assets.storeShader(irradianceShader, "irradiance");
    assets.storeShader(prefilterShader, "prefilter");
    assets.storeShader(brdfShader, "brdf");
    assets.storeShader(backgroundShader, "background");
    assets.storeShader(blurShader, "blur");
    assets.storeShader(bloomShader, "bloom");

    blurShader->use();
    blurShader->setInt("image", 0);

    bloomShader->use();
    bloomShader->setInt("scene", 0);
    bloomShader->setInt("bloomBlur", 0);

    pbrShader->use();
    pbrShader->setInt("irradianceMap", 0);
    pbrShader->setInt("prefilterMap", 1);
    pbrShader->setInt("brdfLUT", 2);
    pbrShader->setInt("albedoMap", 3);
    pbrShader->setInt("normalMap", 4);
    pbrShader->setInt("metallicMap", 5);
    pbrShader->setInt("roughnessMap", 6);
    pbrShader->setInt("aoMap", 7);

    backgroundShader->use();
    backgroundShader->setInt("environmentMap", 0);

    // Ржавое железо
    materials[0].Albedo = loadTexture("res/pbr/rusted_iron/albedo.png");
    materials[0].Normal = loadTexture("res/pbr/rusted_iron/normal.png");
    materials[0].Mettalic = loadTexture("res/pbr/rusted_iron/metallic.png");
    materials[0].Roughness = loadTexture("res/pbr/rusted_iron/roughness.png");
    materials[0].AO = loadTexture("res/pbr/rusted_iron/ao.png");
    materials[0].position = glm::vec3(-5.0, 0.0, 2.0);
    // Золото
    materials[1].Albedo = loadTexture("res/pbr/gold/albedo.png");
    materials[1].Normal = loadTexture("res/pbr/gold/normal.png");
    materials[1].Mettalic = loadTexture("res/pbr/gold/metallic.png");
    materials[1].Roughness = loadTexture("res/pbr/gold/roughness.png");
    materials[1].AO = loadTexture("res/pbr/gold/ao.png");
    materials[1].position = glm::vec3(-3.0, 0.0, 2.0);

    // Трава
    materials[2].Albedo = loadTexture("res/pbr/grass/albedo.png");
    materials[2].Normal = loadTexture("res/pbr/grass/normal.png");
    materials[2].Mettalic = loadTexture("res/pbr/grass/metallic.png");
    materials[2].Roughness = loadTexture("res/pbr/grass/roughness.png");
    materials[2].AO = loadTexture("res/pbr/grass/ao.png");
    materials[2].position = glm::vec3(-1.0, 0.0, 2.0);

    // Пластик
    materials[3].Albedo = loadTexture("res/pbr/plastic/albedo.png");
    materials[3].Normal = loadTexture("res/pbr/plastic/normal.png");
    materials[3].Mettalic = loadTexture("res/pbr/plastic/metallic.png");
    materials[3].Roughness = loadTexture("res/pbr/plastic/roughness.png");
    materials[3].AO = loadTexture("res/pbr/plastic/ao.png");
    materials[3].position = glm::vec3(1.0, 0.0, 2.0);

    // Кирпичная стена
    materials[4].Albedo = loadTexture("res/pbr/wall/albedo.png");
    materials[4].Normal = loadTexture("res/pbr/wall/normal.png");
    materials[4].Mettalic = loadTexture("res/pbr/wall/metallic.png");
    materials[4].Roughness = loadTexture("res/pbr/wall/roughness.png");
    materials[4].AO = loadTexture("res/pbr/wall/ao.png");
    materials[4].position = glm::vec3(3.0, 0.0, 2.0);

    m_cube.setupObject();
    m_quad.setupObject();
    m_sphere.setupObject();

    // PBR: настройка фреймбуфера
    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // PBR: загрузка HDR-карты окружения
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float* data = stbi_loadf("res/pbr/powerplant.hdr", &width, &height, &nrComponents, 0);
    unsigned int hdrTexture = 0;
    if (data)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load HDR image." << std::endl;
    }

    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (uint16_t i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    transformToCubemapShader->use();
    transformToCubemapShader->setInt("equirectangularMap", 0);
    transformToCubemapShader->setMat4("projection", captureProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (uint16_t i = 0; i < 6; i++)
    {
        transformToCubemapShader->setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_cube.drawObject();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // PBR: создаем кубическую карту облученности, и приводим размеры захвата FBO к размерам карты облученности
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (uint16_t i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    // PBR: решаем диффузный интеграл, применяя операцию свертки для создания кубической карты облученности
    irradianceShader->use();
    irradianceShader->setInt("environmentMap", 0);
    irradianceShader->setMat4("projection", captureProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (uint16_t i = 0; i < 6; i++)
    {
        irradianceShader->setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_cube.drawObject();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // PBR: создаем префильтрованную кубическую карту, и приводим размеры захвата FBO к размерам префильтрованной карты
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (uint16_t i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // PBR: применяем симуляцию квази Монте-Карло для освещения окружающей среды, чтобы создать префильтрованную (кубическую)карту
    prefilterShader->use();
    prefilterShader->setInt("environmentMap", 0);
    prefilterShader->setMat4("projection", captureProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (uint16_t mip = 0; mip < maxMipLevels; mip++)
    {
        // Изменяем размеры фреймбуфера в соответствии с размерами мипмап-карты
        unsigned int mipWidth = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = static_cast<float>(mip) / static_cast<float>(maxMipLevels - 1);
        prefilterShader->setFloat("roughness", roughness);
        for (uint16_t i = 0; i < 6; i++)
        {
            prefilterShader->setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_cube.drawObject();
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // PBR: генерируем 2D LUT-текстуру при помощи используемых уравнений BRDF
    glGenTextures(1, &brdfLUTTexture);

    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

    glViewport(0, 0, 512, 512);
    brdfShader->use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_quad.drawObject();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteRenderbuffers(1, &captureRBO);
    glDeleteFramebuffers(1, &captureFBO);

    glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)handle.getWidth() / (float)handle.getHeight(), 0.1f, 100.0f);
    pbrShader->use();
    pbrShader->setMat4("projection", projection);
    backgroundShader->use();
    backgroundShader->setMat4("projection", projection);

    glViewport(0, 0, handle.getWidth(), handle.getHeight());

    // EXPERIMENT
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

    glGenTextures(2, colorBuffers);
    for (uint16_t i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, handle.getWidth(), handle.getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }

    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, handle.getWidth(), handle.getHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ping-pong-фреймбуфер для размытия
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (uint16_t i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, handle.getWidth(), handle.getHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer not complete!" << std::endl;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

template<typename T, size_t N>
size_t arraySize(const T(&)[N]) {
    return N;
}

void scene_4::OnUpdate(float dt)
{
    auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
    Camera& camera = handle.getCamera();

    // lightning
    glm::vec3 lightPositions[] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3(10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3(10.0f, -10.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };

    Shader* pbrShader        = assets.getShader("pbr");
    Shader* backgroundShader = assets.getShader("background");
    Shader* blurShader       = assets.getShader("blur");
    Shader* finalShader      = assets.getShader("bloom");

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Рендеринг сцены, передавая свертку карты облученности в завершающий шейдер
    pbrShader->use();
    pbrShader->setBool("isLight", false);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    pbrShader->setMat4("view", view);
    pbrShader->setVec3("camPos", camera.getPosition());

    // Связываем предварительно вычисленные IBL-данные
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

    for (const auto& material : materials)
    {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, material.Albedo);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, material.Normal);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, material.Mettalic);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, material.Roughness);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, material.AO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, material.position);
        pbrShader->setMat4("model", model);
        m_sphere.drawObject();
    }

    const size_t sz = arraySize(lightPositions);
    pbrShader->setBool("isLight", true);
    for (size_t i = 0; i < sz; i++)
    {
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin((float)glfwGetTime() * 5.0f) * 5.0f, 0.0f, 0.0f);
        newPos = lightPositions[i];
        pbrShader->setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
        pbrShader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
        pbrShader->setVec3("trueColor", lightColors[i]);

        model = glm::mat4(1.0f);
        model = glm::translate(model, newPos);
        model = glm::scale(model, glm::vec3(0.5f));
        pbrShader->setMat4("model", model);
        m_sphere.drawObject();
    }

    backgroundShader->use();
    backgroundShader->setMat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // отображаем карту облученности
    // glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // отображаем карту префильтра
    m_cube.drawObject();

    // EXPERIMENTAL
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;
    blurShader->use();
    for (uint16_t i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
        blurShader->setInt("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);
        m_quad.drawObject();
        horizontal = !horizontal;
        if (first_iteration) {
            first_iteration = false;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    bool bloom = true;
    float exposure = 1.0f;
    finalShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
    finalShader->setInt("bloom", bloom);
    finalShader->setFloat("exposure", exposure);
    m_quad.drawObject();
}

void scene_4::OnDispose()
{
    for (auto& material : materials) {
        glDeleteTextures(1, &material.Albedo);
        glDeleteTextures(1, &material.Normal);
        glDeleteTextures(1, &material.Mettalic);
        glDeleteTextures(1, &material.Roughness);
        glDeleteTextures(1, &material.AO);
    }
  
    
    glDeleteTextures(1, &envCubemap);
    glDeleteTextures(1, &irradianceMap);
    glDeleteTextures(1, &prefilterMap);
    glDeleteTextures(1, &brdfLUTTexture);

    glDeleteTextures(2, colorBuffers);
    glDeleteRenderbuffers(1, &rboDepth);
    glDeleteFramebuffers(1, &hdrFBO);
    
    glDeleteTextures(2, pingpongColorbuffers);
    glDeleteFramebuffers(2, pingpongFBO);
}

const Assets& scene_4::getAssets() const{
	return assets;
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1) {
            format = GL_RED;
        }
        else if (nrComponents == 3) {
            format = GL_RGB;
        }
        else if (nrComponents == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
