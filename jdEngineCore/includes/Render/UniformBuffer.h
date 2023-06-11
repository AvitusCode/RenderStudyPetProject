#pragma once
#include <glad/glad.h>

class UniformBuffer final
{
public:
    UniformBuffer() = default;
    UniformBuffer(const UniformBuffer&) = delete;
    UniformBuffer& operator=(const UniformBuffer&) = delete;
    UniformBuffer(UniformBuffer&&) noexcept;
    UniformBuffer& operator=(UniformBuffer&&) noexcept;
    ~UniformBuffer() noexcept;
    void createUBO(const size_t byteSize, GLenum usageHint = GL_STREAM_DRAW);
    void bindUBO() const;

    /**
     * Update buffer data with specific size at the given byte offset.
     *
     * @param offset    Byte offset to set data at
     * @param ptrData   Pointer to data to be set
     * @param dataSize  Size of data to be set
     */
    void updateBufferData(const size_t offset, const void* ptrData, const size_t dataSize);
    void bindBufferBaseToBindingPoint(const GLuint bindingPoint) const;
    GLuint getBufferID() const;
    void deleteUBO() noexcept;

    static void bindShaderUniformBlock(const GLuint shaderId, const GLuint bindingPoint, const GLchar* blockName);

private:
    GLuint m_bufferID{ 0 }; // OpenGL assigned buffer ID
    size_t m_byteSize; // Holds buffer size in bytes

    bool m_isBufferCreated = false;
};

/**
 * Stores most common binding blocks used throughout the tutorials.
 */
class UniformBlockBindingPoints
{
public:
    static const int MATRICES{ 0 };
    static const int POINT_LIGHTS{ 1 };
};
