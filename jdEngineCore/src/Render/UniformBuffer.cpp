#include "Render/UniformBuffer.h"
#include "Utils/logger.h"

UniformBuffer::~UniformBuffer() noexcept {
	deleteUBO();
}

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept {
	*this = std::move(other);
}

UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
{
	if (this != &other)
	{
		m_bufferID = std::exchange(other.m_bufferID, 0);
		m_byteSize = std::exchange(other.m_byteSize, 0);
		m_isBufferCreated = other.m_isBufferCreated;
		other.m_isBufferCreated = false;
	}

	return *this;
}

void UniformBuffer::createUBO(const size_t byteSize, GLenum usageHint)
{
	if (m_isBufferCreated) {
		return;
	}

	glGenBuffers(1, &m_bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
	glBufferData(GL_UNIFORM_BUFFER, byteSize, nullptr, usageHint);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_byteSize = byteSize;
	m_isBufferCreated = true;
	LOG(INFO) << "creating UniformBuffer ID=" << m_bufferID;
}

void UniformBuffer::bindUBO() const
{
	if (!m_isBufferCreated) {
		LOG(ERROR) << "UBO does not exsist!";
		return;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
}

void UniformBuffer::updateBufferData(const size_t offset, const void* ptrData, const size_t dataSize)
{
	if (!m_isBufferCreated) {
		LOG(ERROR) << "UBO does not exsist!";
		return;
	}

	if (offset >= m_byteSize || offset + dataSize > m_byteSize) {
		LOG(ERROR) << "ERROR: to big data!";
		return;
	}

	glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, ptrData);
}

void UniformBuffer::bindBufferBaseToBindingPoint(const GLuint bindingPoint) const
{
	if (!m_isBufferCreated) {
		LOG(ERROR) << "UBO does not exsist!";
		return;
	}

	glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_bufferID, 0, m_byteSize);
}

void UniformBuffer::bindShaderUniformBlock(const GLuint shaderId, const GLuint bindingPoint, const GLchar* blockName) {
	glUniformBlockBinding(shaderId, glGetUniformBlockIndex(shaderId, blockName), bindingPoint);
}

GLuint UniformBuffer::getBufferID() const {
	return m_bufferID;
}

void UniformBuffer::deleteUBO() noexcept
{
	if (!m_isBufferCreated) {
		return;
	}

	LOG(INFO) << "Dell uniform buffer ID=" << m_bufferID;

	glDeleteBuffers(1, &m_bufferID);
	m_byteSize = m_bufferID = 0;
	m_isBufferCreated = false;
}