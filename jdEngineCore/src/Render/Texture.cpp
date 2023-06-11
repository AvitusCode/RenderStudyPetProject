#include "Render/Texture.h"
#include "Utils/stb_image.h"
#include <iostream>
#include <mutex>

Texture::Texture(Texture&& other) noexcept
{
    *this = std::move(other);
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        deleteTexture();
        m_textureID = std::exchange(other.m_textureID, 0);
        m_width = std::exchange(other.m_width, 0);
        m_height = std::exchange(other.m_height, 0);
        m_format = std::exchange(other.m_format, 0);
        m_texture_type = std::exchange(other.m_texture_type, "");
        m_filePath = std::exchange(other.m_filePath, "");
    }

    return *this;
}

Texture::~Texture() noexcept {
	deleteTexture();
}

bool Texture::createFromData(const unsigned char* data, GLsizei width, GLsizei height, GLenum format, bool generateMipmaps)
{
    if (isLoaded()) {
        return false;
    }

    m_width = width;
    m_height = height;
    m_format = format;

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);

    if (generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    return true;
}

bool Texture::loadTexture2D(const std::string& filePath, bool generateMipmaps)
{
    stbi_set_flip_vertically_on_load(true);
    int bytesPerPixel;
    const auto imageData = stbi_load(filePath.c_str(), &m_width, &m_height, &bytesPerPixel, 0);
    
    if (imageData == nullptr)
    {
        std::cerr << "ERROR: Failed to load image " << filePath << "!" << std::endl;
        return false;
    }

    GLenum format = 0;
    if (bytesPerPixel == 4) {
        format = GL_RGBA;
    }
    else if (bytesPerPixel == 3) {
        format = GL_RGB;
    }
    else if (bytesPerPixel == 1) {
        format = GL_DEPTH_COMPONENT;
    }

    const auto result = createFromData(imageData, m_width, m_height, format, generateMipmaps);
    stbi_image_free(imageData);
    m_filePath = filePath;

    return result;
}

void Texture::bind(GLenum textureUnit) const
{
    if (!isLoadedCheck()) {
        return;
    }

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::deleteTexture() noexcept
{
    if (!isLoaded()) {
        return;
    }

    // TODO: log
    std::cout << "Delete texture: " << m_textureID << std::endl;

    glDeleteTextures(1, &m_textureID);
    m_textureID = 0;
    m_width = m_height = 0;
    m_format = 0;
}

GLuint Texture::getID() const
{
    return m_textureID;
}

void Texture::setTextureType(const std::string& type) {
    m_texture_type = type;
}

std::string Texture::getTextureType() const
{
    return m_texture_type;
}

std::string Texture::getFilePath() const
{
    return m_filePath;
}

GLsizei Texture::getWidth() const
{
    return m_width;
}

GLsizei Texture::getHeight() const
{
    return m_height;
}

bool Texture::isLoaded() const
{
    return m_textureID != 0;
}

bool Texture::resize(GLsizei newWidth, GLsizei newHeight)
{
    if (!isLoadedCheck()) {
        return false;
    }

    const auto oldFormat = m_format;
    deleteTexture();

    return createFromData(nullptr, newWidth, newHeight, oldFormat, false);
}

int Texture::getNumTextureImageUnits()
{
    static std::once_flag flag;
    static int maxTextureUnits;
    std::call_once(flag, []() {glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits); });

    return maxTextureUnits;
}

bool Texture::isLoadedCheck() const
{
    if (!isLoaded())
    {
        std::cerr << "ERROR: not loaded texture!" << std::endl;
        return false;
    }

    return true;
}
