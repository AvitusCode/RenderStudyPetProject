#include "uniform_buffer.h"
#include <glad/glad.h>


UniformBuffer::UniformBuffer(unsigned int point) : bindingPoint(point), UBO(0)
{}

void UniformBuffer::initUniformBuffer()
{
	glGenBuffers(1, &UBO);

	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, UBO, 0, 2 * sizeof(glm::mat4));
}

// Updates the uniform buffer's data
void UniformBuffer::updateUniformBuffer(const glm::mat4& view, const glm::mat4& projection)
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &view[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projection[0][0]);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::bindingUniformBlock(unsigned int shaderId, const std::string& shaderName){
	glUniformBlockBinding(shaderId, glGetUniformBlockIndex(shaderId, shaderName.c_str()), bindingPoint);

}

const unsigned int UniformBuffer::getBindingPoint() const {
	return bindingPoint;
}

void UniformBuffer::setBindingPoint(unsigned int point)
{
	bindingPoint = point;
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, UBO, 0, 2 * sizeof(glm::mat4));
}