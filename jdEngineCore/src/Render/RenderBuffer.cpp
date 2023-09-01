#include "Render/RenderBuffer.h"
#include "Utils/logger.h"

RenderBuffer::~RenderBuffer() noexcept {
	deleteRenderBuffer();
}

bool RenderBuffer::create(GLenum internalFormat, GLsizei width, GLsizei height)
{
	glGenRenderbuffers(1, &m_renderBufferID);
	if (!m_renderBufferID) {
		LOG(ERROR) << "creating render buffer " << internalFormat
			<< " and dimensions [" << width << ", " << height << "]!" << std::endl;
	}

    LOG(INFO) << "Created renderbuffer with ID: " << m_renderBufferID << std::endl;

    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);

    m_internalFormat = internalFormat;
    m_width = width;
    m_height = height;

    return true;
}

bool RenderBuffer::resize(GLsizei newWidth, GLsizei newHeight)
{
    if (!m_renderBufferID) {
        return false;
    }

    const auto internalFormat = m_internalFormat;
    deleteRenderBuffer();
    return create(internalFormat, newWidth, newHeight);
}

void RenderBuffer::deleteRenderBuffer() noexcept
{
    if (!m_renderBufferID) {
        return;
    }

    LOG(INFO) << "Deleting renderbuffer with ID: " << m_renderBufferID << std::endl;

    glDeleteRenderbuffers(1, &m_renderBufferID);
    m_width = m_height = m_renderBufferID = 0;
    m_stencilBits = m_depthBits = -1;
}

GLuint RenderBuffer::getID() const {
    return m_renderBufferID;
}

GLenum RenderBuffer::getInternalFormat() const {
    return m_internalFormat;
}

GLsizei RenderBuffer::getWidth() const {
    return m_width;
}

GLsizei RenderBuffer::getHeight() const {
    return m_height;
}

GLint RenderBuffer::getDepthBits()
{
    if (m_depthBits != -1 || !m_renderBufferID) {
        return m_depthBits;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferID);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE, &m_depthBits);
    return m_depthBits;
}

GLint RenderBuffer::getStencilBits()
{
    if (m_stencilBits != -1 || !m_renderBufferID) {
        return m_stencilBits;
    }

    // Query how many bits are used for stencil and cache the result
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferID);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_STENCIL_SIZE, &m_stencilBits);
    return m_stencilBits;
}