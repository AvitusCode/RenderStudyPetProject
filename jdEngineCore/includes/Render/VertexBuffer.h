#pragma once
#include <vector>
#include <cctype>
#include <glad/glad.h>


class VertexBuffer final
{
public:
    VertexBuffer() = default;

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&&) noexcept;
    VertexBuffer& operator=(VertexBuffer&&) noexcept;
    ~VertexBuffer() noexcept;

    void makeVBO(size_t reserveSizeBytes = 0);
    void bindVBO(GLenum bufferType = GL_ARRAY_BUFFER);

    /**
     * Adds raw data to the memory buffer, before they get uploaded.
     *
     * @param ptrData        Pointer to the raw data (arbitrary type)
     * @param dataSizeBytes  Size of the added data (in bytes)
     * @param repeat         How many times to repeat same data in the buffer (default is 1)
     */
    void addRawData(const void* ptrData, size_t dataSizeBytes, size_t repeat = 1);

    template<typename T>
    void addData(const T& ptrObj, size_t repeat = 1) {
        addRawData(std::addressof(ptrObj), static_cast<size_t>(sizeof(T)), repeat);
    }

    void* getRawDataPointer();

    /**
     * Uploads gathered data to the GPU memory. Now the VBO is ready to be used.
     * @param usageHint  Hint for OpenGL, how is the data intended to be used (GL_STATIC_DRAW, GL_DYNAMIC_DRAW)
     */
    void uploadDataToGPU(GLenum usageHint, bool clear = false);

    /**
     * Maps buffer data to a memory pointer.
     * @param usageHint  Hint for OpenGL, how is the data intended to be used (GL_STATIC_DRAW, GL_DYNAMIC_DRAW)
     * @return Pointer to the mapped data, or nullptr, if something fails.
     */
    void* mapBufferToMemory(GLenum usageHint) const;

    /**
     * Maps buffer sub-data to a memory pointer.
     * @param usageHint  Hint for OpenGL, how is the data intended to be used (GL_READ_ONLY, GL_WRITE_ONLY...`)
     * @param offset     Byte offset in buffer where to start
     * @param length     Byte length of the mapped 
     * @return Pointer to the mapped data, or nullptr, if something fails.
     */
    void* mapSubBufferToMemory(GLenum usageHint, size_t offset, size_t length) const;

    /**
     * Unmaps buffer from memory (must have been mapped previously).
     */
    void unmapBuffer() const;
    GLuint getBufferID() const;

    size_t getBufferSize();
    void deleteVBO() noexcept;

private:
    /**
     * Checks if the buffer has been created and has OpenGL-assigned ID.
     */
    bool isBufferCreated() const;

    /**
     * Checks if the the data has been uploaded to the buffer already.
     */
    bool isDataUploaded() const;
private:
	GLuint m_bufferID{};
	GLuint m_bufferType{};

	std::vector<uint8_t> m_rawData; // In-memory raw data buffer, used to gather the data for VBO
	size_t m_bytesAdded{ 0 }; // Number of bytes added to the buffer so far
	size_t m_uploadedDataSize{ 0 }; // Holds buffer data size after uploading to GPU (if it's not null, then data have been uploaded)
};