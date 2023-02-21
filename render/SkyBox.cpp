#include "SkyBox.h"

#include <glad/glad.h>

// A list of vertices that represent a box used to draw any skybox
glm::vec3 Skybox::boxVertices[] = {
	glm::vec3( -1.0f,  1.0f, -1.0f),
    glm::vec3( -1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f,  1.0f, -1.0f),
    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),
    glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(-1.0f, 1.0f,  1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f,  1.0f),
    glm::vec3(1.0f,  1.0f,  1.0f),
    glm::vec3(1.0f,  1.0f,  1.0f),
    glm::vec3(1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),
    glm::vec3(-1.0f,  1.0f,  1.0f),
    glm::vec3(1.0f,  1.0f,  1.0f),
    glm::vec3(1.0f, 1.0f,  1.0f),
    glm::vec3(1.0f, -1.0f,  1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),
    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f,  1.0f, -1.0f),
    glm::vec3(1.0f,  1.0f, 1.0f),
    glm::vec3(1.0f,  1.0f,  1.0f),
    glm::vec3(-1.0f,  1.0f,  1.0f),
    glm::vec3(-1.0f,  1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f,-1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, -1.0f,  1.0f),
    glm::vec3(1.0f, -1.0f,  1.0f) };

// Setup default mesh variables with parent constructor, then set class specific variables
Skybox::Skybox(unsigned int shaderProgramID, const Cubemap& cubemap)
	: Mesh()
{
    vertices.resize(36);

    for (size_t i = 0; i < vertices.size(); i++){
        vertices[i].Position = boxVertices[i];
    }
    
    this->shaderProgramID = shaderProgramID;
	this->cubemap = cubemap;
}

const Cubemap& Skybox::getCubemap() const{
    return cubemap;
}

Cubemap& Skybox::getCubemap(){
    return cubemap;
}

// Draw the skybox
void Skybox::drawObject() const
{
	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(getVAO());
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.getId());
	glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDepthFunc(GL_LESS);
}

void Skybox::setupObject()
{
	Mesh::setupObject();
    cubemap.setupObject();
}