#include "Render/VertexBuffer.h"
#include "Utils/logger.h"
#include "Utils/jd_string.h"

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
{
	*this = std::move(other);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	if (this != &other)
	{
		deleteVBO();
		m_bufferID = std::exchange(other.m_bufferID, 0);
		m_bufferType = std::exchange(other.m_bufferType, 0);
		m_rawData = std::move(other.m_rawData);
		m_bytesAdded = std::exchange(other.m_bytesAdded, 0);
		m_uploadedDataSize = std::exchange(other.m_uploadedDataSize, 0);
	}

	return *this;
}

VertexBuffer::~VertexBuffer() noexcept {
	deleteVBO();
}

void VertexBuffer::makeVBO(size_t reserveSizeBytes)
{
	if (isBufferCreated()) {
		deleteVBO();
	}

	glGenBuffers(1, &m_bufferID);
	m_rawData.reserve(reserveSizeBytes > 0 ? reserveSizeBytes : 1024);
	DLOG(INFO) << "creating VertexBuffer with ID=" << m_bufferID << "; Type=" << m_bufferType << "; size=" << m_rawData.capacity();
}

void VertexBuffer::bindVBO(GLenum bufferType)
{
	if (!isBufferCreated()) {
		LOG(FATAL) << "buffer is not created";
	}

	m_bufferType = bufferType;
	glBindBuffer(bufferType, m_bufferID);
}

void VertexBuffer::addRawData(const void* ptrData, size_t dataSizeBytes, size_t repeat)
{
	const size_t allBytes = dataSizeBytes * repeat;
	const size_t capacity = m_bytesAdded + allBytes;

	if (capacity > m_rawData.capacity())
	{
		size_t newCapacity = m_rawData.capacity() << 1;
		while (newCapacity < capacity) {
			newCapacity <<= 1;
		}

		std::vector<uint8_t> newRawData;
		newRawData.reserve(newCapacity);
		memcpy(newRawData.data(), m_rawData.data(), m_bytesAdded);
		m_rawData = std::move(newRawData);
	}

	for (size_t i = 0; i < repeat; i++)
	{
		memcpy(m_rawData.data() + m_bytesAdded, ptrData, dataSizeBytes);
		m_bytesAdded += dataSizeBytes;
	}

	DLOG(INFO) << jd::fmt::print("Add raw data to buffer_id=%ul, size=%ull", m_bufferID, allBytes);
}


void* VertexBuffer::getRawDataPointer() {
	return m_rawData.data();
}


void VertexBuffer::uploadDataToGPU(GLenum usageHint, bool clear)
{
	if (!isBufferCreated()) {
		LOG(FATAL) << "buffer is not created";
	}

	glBufferData(m_bufferType, m_bytesAdded, m_rawData.data(), usageHint);
	m_uploadedDataSize = m_bytesAdded;
	m_bytesAdded = 0;

	DLOG(INFO) << jd::fmt::print("Load raw data to GPU from buffer_id=%ul, size=%ull, delete on RAM %s", 
		m_bufferID, m_rawData.size(), clear ? "true" : "false");

	if (clear) {
		m_rawData.clear();
		m_rawData.reserve(0);
	}
}


void* VertexBuffer::mapBufferToMemory(GLenum usageHint) const {
	return isDataUploaded() ? glMapBuffer(m_bufferType, usageHint) : nullptr;
}


void* VertexBuffer::mapSubBufferToMemory(GLenum usageHint, size_t offset, size_t length) const {
	return isDataUploaded() ? glMapBufferRange(m_bufferType, offset, length, usageHint) : nullptr;
}

void VertexBuffer::unmapBuffer() const {
	glUnmapBuffer(m_bufferType);
}

GLuint VertexBuffer::getBufferID() const {
	return m_bufferID;
}

size_t VertexBuffer::getBufferSize() {
	return isDataUploaded() ? m_uploadedDataSize : m_bytesAdded;
}

void VertexBuffer::deleteVBO() noexcept
{
	if (!isBufferCreated()) {
		DLOG(WARNING) << "VBO has already deleted!";
		return;
	}

	DLOG(INFO) << "Delete VBO with ID=" << m_bufferID << "; Type=" << m_bufferType << std::endl;

	glDeleteBuffers(1, &m_bufferID);
	m_rawData.clear();
	m_bytesAdded = m_uploadedDataSize = m_bufferID = 0;
}

bool VertexBuffer::isBufferCreated() const {
	return m_bufferID != 0;
}

bool VertexBuffer::isDataUploaded() const {
	return m_uploadedDataSize > 0;
}