#include "../main.h"
#include "scene_2.h"
#include "../render/SkyBox.h"
#include "../render/Mesh.h"
#include "../render/Model.h"
#include "../Window.h"
#include "../objects/Camera.h"

#include <iostream>
#include <future>

template<typename Iterator>
class IteratorRange
{
public:
    IteratorRange(Iterator b, Iterator e) : _begin(b), _end(e) {
    };

    Iterator begin() const {
        return _begin;
    }

    Iterator end() const {
        return _end;
    }

    size_t size() const {
        return std::distance(_begin, _end);
    }

private:
    Iterator _begin;
    Iterator _end;
};


template<typename Iterator>
class Paginator
{
public:
    Paginator(Iterator b, Iterator e, size_t pages)
    {
        Iterator first = b;

        while (first != e)
        {
            size_t left = std::distance(first, e);
            Iterator second = std::next(first, std::min(left, pages));
            ranges.emplace_back(first, second);
            first = second;
        }
    }

    auto begin() const {
        return ranges.begin();
    }

    auto end() const {
        return ranges.end();
    }

    size_t size() const {
        return ranges.size();
    }

private:
    std::vector<IteratorRange<Iterator>> ranges;
};

template<typename Container>
auto Paginate(Container& c, size_t pages) {
    return Paginator<decltype(c.begin())>(c.begin(), c.end(), pages);
}

// Creating a neccessary new data
void scene_2::OnCreate()
{
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
    Camera& camera = handle.getCamera();

    camera.getPosition() = glm::vec3(0.0f, 0.0f, 300.0f);
    camera.setMSpeed(150.0f);

    ShaderComponent vertexComponent;
    ShaderComponent fragmentComponent;

    vertexComponent.loadComponentFromFile("res/shaders_scene_3/asteroids.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/shaders_scene_3/asteroids.frag", GL_FRAGMENT_SHADER);
	Shader* asteroidShader = new Shader(vertexComponent, fragmentComponent);

    vertexComponent.loadComponentFromFile("res/shaders_scene_3/planet.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/shaders_scene_3/planet.frag", GL_FRAGMENT_SHADER);
	Shader* planetShader = new Shader( vertexComponent, fragmentComponent );

    vertexComponent.loadComponentFromFile("res/shaders/skybox.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/shaders/skybox.frag", GL_FRAGMENT_SHADER);
    Shader* skyboxShader = new Shader( vertexComponent, fragmentComponent );

	assets.storeShader(asteroidShader, "asteroid");
	assets.storeShader(planetShader, "planet");
    assets.storeShader(skyboxShader, "skybox");

    asteroidShader->use();
    asteroidShader->setInt("texture_diffuse1", 0);

	Model* rock = new Model("res/objects/rock/rock.obj", asteroidShader->getId());
	Model* planet = new Model("res/objects/planet/planet.obj", planetShader->getId());
    planetPos = glm::vec3(0.0f, -3.0f, 0.0f);
    planet->translateModel(planetPos);
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

    amount = 100'000;
    modelMatrices.resize(amount);
    rocks_pos.resize(amount);

    srand(static_cast<unsigned int>(glfwGetTime()));
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

        glm::vec3 positions = glm::vec3(x, y, z);
        model = glm::translate(model, positions);
        rocks_pos[i] = std::move(positions);

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
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_DYNAMIC_DRAW);

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

void scene_2::OnUpdate(float dt)
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _rock->getLoadedTex()[0].getId());
    const std::vector<Mesh>& ms = _rock->getMeshes();
    for (size_t i = 0; i < ms.size(); i++)
    {
        glBindVertexArray(ms[i].getVAO());
        glDrawElementsInstanced(GL_TRIANGLES, ms[i].getIndices().size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }


    // rotate around the planet
    const glm::vec3 rot_axis = glm::vec3(0.0f, 0.0f, 1.0f);
    const float rot_angle = glm::radians(0.20f / dt);
    glm::mat3 rotatingY = glm::mat3(1.0f);
    rotatingY[0][0] = cos(rot_angle);
    rotatingY[2][0] = sin(rot_angle);
    rotatingY[0][2] = -sin(rot_angle);
    rotatingY[2][2] = cos(rot_angle);

    std::vector<std::future<void>> futures;
    const size_t page_size = 10'000;
    assert(modelMatrices.size() % page_size == 0 && page_size < modelMatrices.size());

    for (size_t i = 0; i < modelMatrices.size(); i += page_size) 
    {
        futures.push_back(std::async([this, i, page_size, rotatingY] {
            updateMultiThread(i, i + page_size, rotatingY);
            }));
        
    }

    glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);

    void* ptr_ib = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr_ib, &modelMatrices[0], modelMatrices.size() * sizeof(glm::mat4));
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void scene_2::updateMultiThread(size_t a, size_t b, glm::mat3 rotatingY)
{
    for (size_t i = a; i < b; i++)
    {
        rocks_pos[i] = rotatingY * rocks_pos[i];
        modelMatrices[i][3][0] = rocks_pos[i].x;
        modelMatrices[i][3][1] = rocks_pos[i].y;
        modelMatrices[i][3][2] = rocks_pos[i].z;
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
    glDeleteBuffers(1, &instance_buffer);
}

const Assets& scene_2::getAssets() const {
	return assets;
}