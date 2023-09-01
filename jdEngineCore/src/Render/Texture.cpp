#include "Render/Texture.h"
#include "Utils/logger.h"
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

// @fixme
// A very unpleasant violation of the semantics of constancy
void Texture::complete() const
{
    if (!isLoaded()) {
        tex_completer();
    }
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
    if (m_textureID == 0) {
        LOG(ERROR) << "glGenTextures has failed!";
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);

    if (generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    LOG(INFO) << "Creating texture with id=" << m_textureID;
    return true;
}

bool Texture::loadTexture2D(const std::string& filePath, bool generateMipmaps)
{
    stbi_set_flip_vertically_on_load(true);
    int bytesPerPixel;
    auto imageData = stbi_load(filePath.c_str(), &m_width, &m_height, &bytesPerPixel, 0);
    
    if (imageData == nullptr)
    {
        LOG(ERROR) << "Failed to load image " << filePath;
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

    tex_completer = [this, imageData, format, generateMipmaps]() mutable {
        bool res{ false };
        if (!imageData) {
            return res;
        }
        res = createFromData(imageData, m_width, m_height, format, generateMipmaps);
        stbi_image_free(imageData);
        imageData = nullptr;
        return res;
    };
    m_filePath = filePath;

    return true;
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

    if (tex_completer) {
        tex_completer();
    }
    LOG(INFO) << "Delete texture with id=" << m_textureID;

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
        LOG(ERROR) << "not loaded texture!";
        return false;
    }

    return true;
}
