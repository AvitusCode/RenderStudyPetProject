#include "../main.h"
#include "scene_3.h"
#include "../render/SkyBox.h"
#include "../render/Mesh.h"
#include "../render/Model.h"
#include "../render/Shader.h"
#include "../render/Texture.h"
#include "../Window.h"
#include "../objects/Camera.h"

#include <iostream>

namespace
{

    constexpr int SHADOW_WIDTH = 1024;
    constexpr int SHADOW_HEIGHT = 1024;

    bool shadows = true;
}

// Creating a neccessary new data
void scene_3::OnCreate()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
    Camera& camera = handle.getCamera();

    camera.getPosition() = glm::vec3(0.0f, 0.0f, 3.0f);
    camera.setMSpeed(15.0f);

    ShaderComponent vertexComponent;
    ShaderComponent fragmentComponent;
    ShaderComponent geometryComponent;

    vertexComponent.loadComponentFromFile("res/shaders_scene4/point_shadow.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/shaders_scene4/point_shadow.frag", GL_FRAGMENT_SHADER);
    geometryComponent.loadComponentFromFile("res/shaders_scene4/shadows_depth.geom", GL_GEOMETRY_SHADER);
    Shader* shader = new Shader(vertexComponent, fragmentComponent);

    vertexComponent.loadComponentFromFile("res/shaders_scene4/shadows_depth.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/shaders_scene4/shadows_depth.frag", GL_FRAGMENT_SHADER);
    Shader* depthShader = new Shader(vertexComponent, fragmentComponent, geometryComponent);

    vertexComponent.loadComponentFromFile("res/shaders_scene4/light.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/shaders_scene4/light.frag", GL_FRAGMENT_SHADER);
    Shader* lightShader = new Shader( vertexComponent, fragmentComponent );

    vertexComponent.loadComponentFromFile("res/silhouette/silhouette.vert", GL_VERTEX_SHADER);
    fragmentComponent.loadComponentFromFile("res/silhouette/silhouette.frag", GL_FRAGMENT_SHADER);
    geometryComponent.loadComponentFromFile("res/silhouette/silhouette.geom", GL_GEOMETRY_SHADER);
    Shader* silhouetteShader = new Shader( vertexComponent, fragmentComponent, geometryComponent );

    assets.storeShader(shader, "shader");
    assets.storeShader(lightShader, "light");
    assets.storeShader(depthShader, "depth");
    assets.storeShader(silhouetteShader, "silhouette");

    ub.initUniformBuffer();
    ub.bindingUniformBlock(lightShader->getId(), "Matrices");
    ub.bindingUniformBlock(shader->getId(), "Matrices");
    ub.bindingUniformBlock(silhouetteShader->getId(), "Matrices");
    
    Texture* diffuseMap = new Texture("res/img/wood.png", "wood");
    assets.storeTexture(diffuseMap, "wood");
   
    lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

    // Генерируем фреймбуффер и кубическую карту теней
    glGenFramebuffers(1, &cubeMapFBO);
    glGenTextures(1, &depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (uint16_t i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, cubeMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR:: framebuffer did not complete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader->use();
    shader->setInt("diffuseTexture", 0);
    shader->setInt("depthMap", 1);

    float vertices[] = {
       -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 0
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 1
        1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // 2   
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 1
       -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 0
       -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // 3

      -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 4
       1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 5
       1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 6
       1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 6
      -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 7
      -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 4

     -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 7
     -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 3
     -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 0
     -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 0
     -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 4
     -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 7

     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 6
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 2
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 1       
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 2
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 6
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 5  

    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // 0
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // 2
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // 5
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // 5
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // 4
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // 0

   -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // 3
    1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 6
    1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 1  
    1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 6
   -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // 3
   -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // 7  
    };

    float leastVertices[] = {
        -1.0f, -1.0f, -1.0f, // 0
         1.0f,  1.0f, -1.0f,  // 1
         1.0f, -1.0f, -1.0f,  // 2   
        -1.0f,  1.0f, -1.0f,  // 3
        -1.0f, -1.0f,  1.0f, // 4
         1.0f, -1.0f,  1.0f,  // 5
         1.0f,  1.0f,  1.0f,  // 6
        -1.0f,  1.0f,  1.0f  // 7
    };

    std::vector<GLuint> indexes = { 0, 1, 2, 1, 0, 3, 4, 5, 6, 6, 7, 4, 7, 3, 0, 0, 4, 7, 6, 2, 1, 2, 6, 5, 0, 2, 5, 5, 4, 0, 3, 6, 1, 6, 3, 7 };
    adj_indexes = makeAdjacencyIndexes(12, indexes);
    
    glGenVertexArrays(1, &adj_cubeVAO);
    glGenBuffers(1, &adj_cubeVBO);
    glGenBuffers(1, &adj_cubeEBO);

    glBindVertexArray(adj_cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, adj_cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(leastVertices), leastVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, adj_cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, adj_indexes.size() * sizeof(GLuint), &adj_indexes[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
   

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Связываем вершинные атрибуты
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // for light VAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void scene_3::render_scene(unsigned int cubeVAO, GLenum param)
{
    glBindVertexArray(cubeVAO);
    switch (param)
    {
    case GL_TRIANGLES:
        glDrawArrays(param, 0, 36);
        break;
    case GL_TRIANGLES_ADJACENCY:
        glDrawElements(param, (GLsizei)adj_indexes.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void scene_3::render(const Shader& shader, unsigned int cubeVAO, bool flag, GLenum param)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(5.0f));
    shader.setMat4("model", model);
    
    glDisable(GL_CULL_FACE);
    if (flag)
        shader.setInt("reverse_normals", 1);
    render_scene(cubeVAO, param);
    if (flag)
        shader.setInt("reverse_normals", 0);
    glEnable(GL_CULL_FACE);

    // Cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    render_scene(cubeVAO, param);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    render_scene(cubeVAO, param);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    render_scene(cubeVAO, param);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    render_scene(cubeVAO, param);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    render_scene(cubeVAO, param);
}

void scene_3::OnUpdate(float dt)
{
    auto& handle = *static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
    Camera& camera = handle.getCamera();

    Shader* shader      = assets.getShader("shader");
    Shader* depthShader = assets.getShader("depth");
    Shader* lightShader = assets.getShader("light");
    Shader* silhouetteShader = assets.getShader("silhouette");
    Texture* diffuseMap = assets.getTexture("wood");
  
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float near_plane = 1.0f;
    float far_plane = 25.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, cubeMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    depthShader->use();
    for (uint16_t i = 0; i < shadowTransforms.size(); i++) {
        depthShader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    depthShader->setFloat("far_plane", far_plane);
    depthShader->setVec3("lightPos", lightPos);
    render(*depthShader, cubeVAO, false, GL_TRIANGLES);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, handle.getWidth(), handle.getHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    projection = glm::perspective(glm::radians(camera.getZoom()), (float)handle.getWidth() / (float)handle.getHeight(), 0.1f, 100.0f);
    view = camera.GetViewMatrix();
    ub.updateUniformBuffer(view, projection);
    
    shader->use();
    shader->setVec3("lightPos", lightPos);
    shader->setVec3("viewPos", camera.getPosition());
    shader->setInt("shadows", shadows);
    shader->setFloat("far_plane", far_plane);
    glActiveTexture(GL_TEXTURE0);
    diffuseMap->bindTexture();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    render(*shader, cubeVAO, true, GL_TRIANGLES);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightShader->use();
    lightShader->setMat4("model", model);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


    // TEST SILHOUETTE
    glClear(GL_DEPTH_BUFFER_BIT);

    silhouetteShader->use();
    silhouetteShader->setVec3("colorSilhouette", glm::vec3(1.0, 0.0, 0.0));
    silhouetteShader->setVec3("lightPos", lightPos);
    render(*silhouetteShader, adj_cubeVAO, false, GL_TRIANGLES_ADJACENCY);
}

void scene_3::OnDispose()
{
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);

    glDeleteVertexArrays(1, &adj_cubeVAO);
    glDeleteBuffers(1, &adj_cubeVBO);
    glDeleteBuffers(1, &adj_cubeEBO);
}

const Assets& scene_3::getAssets() const {
    return assets;
}
