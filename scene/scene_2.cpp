#include "../main.h"
#include "scene_2.h"
#include "../render/SkyBox.h"
#include "../render/Mesh.h"
#include "../render/Model.h"
#include "../Window.h"
#include "../objects/Camera.h"

// Creating a neccessary new data
void scene_2::OnCreate()
{
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
    Camera& camera = handle.getCamera();

    camera.getPosition() = glm::vec3(0.0f, 0.0f, 3.0f);
    camera.setMSpeed(150.0f);

	Shader* asteroidShader = new Shader(load_shader("res/shaders_scene_3/asteroids.vert", "res/shaders_scene_3/asteroids.frag"));
	Shader* planetShader = new Shader(load_shader("res/shaders_scene_3/planet.vert", "res/shaders_scene_3/planet.frag"));
    Shader* skyboxShader = new Shader(load_shader("res/shaders/skybox.vert", "res/shaders/skybox.frag"));
	assets.storeShader(asteroidShader, "asteroid");
	assets.storeShader(planetShader, "planet");
    assets.storeShader(skyboxShader, "skybox");

	Model* rock = new Model("res/objects/rock/rock.obj", asteroidShader->getId());
	Model* planet = new Model("res/objects/planet/planet.obj", planetShader->getId());
    planet->translateModel(glm::vec3(0.0f, -3.0f, 0.0f));
    planet->scaleModel(glm::vec3(10.0f, 10.0f, 10.0f));

    Skybox* skybox = new Skybox(skyboxShader->getId(), Cubemap("res/img/skybox/space2/"));

    rock->setupObject();
    planet->setupObject();
    skybox->setupObject();
    _rock = rock;
    _planet = planet;
    _skybox = skybox;

    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.getZoom()), (float)handle.getWidth() / (float)handle.getHeight(), 0.1f, 1000.0f);

    uniformBuffer.initUniformBuffer();
    uniformBuffer.bindingUniformBlock(asteroidShader->getId(), "Matrices");
    uniformBuffer.bindingUniformBlock(planetShader->getId(), "Matrices");
    uniformBuffer.bindingUniformBlock(skyboxShader->getId(), "Matrices");
    uniformBuffer.updateUniformBuffer(view, projection);

    amount = 100000;
    std::vector<glm::mat4> modelMatrices(amount);
    srand(glfwGetTime());
    float radius = 150.0;
    float offset = 25.0f;

    auto rand_offset = [](float offset) {
        return (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    };

    for (size_t i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);

        float angle = (float)i / (float)amount * 360.0f;
        float displacement = rand_offset(offset);
        float x = sin(angle) * radius + displacement;
        displacement = rand_offset(offset);
        float y = displacement * 0.4f; // make our height a little less
        displacement = rand_offset(offset);
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. mag coeff from 0.05 to 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. A random rotation
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        modelMatrices[i] = model;
    }

    glGenBuffers(1, &instance_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (size_t i = 0; i < rock->getMeshes().size(); i++)
    {
        unsigned int VAO = rock->getMeshes()[i].getVAO();
        glBindVertexArray(VAO);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

void scene_2::OnUpdate()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader* asteroidShader = assets.getShader("asteroid");
    Shader* planetShader = assets.getShader("planet");
    Shader* skyboxShader = assets.getShader("skybox");

    auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
    Camera& camera = handle.getCamera();

    projection = glm::perspective(glm::radians(45.0f), (float)handle.getWidth() / (float)handle.getHeight(), 0.1f, 1000.0f);
    view = camera.GetViewMatrix();
    uniformBuffer.updateUniformBuffer(view, projection);

    skyboxShader->use();
    _skybox->drawObject();

    // draw planet
    planetShader->use();
    _planet->drawObject();

    // draw rocks
    asteroidShader->use();
    asteroidShader->setInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _rock->getLoadedTex()[0].getId());
    const std::vector<Mesh>& ms = _rock->getMeshes();
    for (size_t i = 0; i < ms.size(); i++)
    {
        glBindVertexArray(ms[i].getVAO());
        glDrawElementsInstanced(GL_TRIANGLES, ms[i].getIndices().size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
}

void scene_2::OnDispose() 
{
    delete _rock;
    delete _planet;
    delete _skybox;
    _rock = nullptr;
    _planet = nullptr;
    _skybox = nullptr;
    glDeleteFramebuffers(1, &instance_buffer);
}

const Assets& scene_2::getAssets() const {
	return assets;
}