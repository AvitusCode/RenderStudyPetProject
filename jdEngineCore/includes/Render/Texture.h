#pragma once
#include <string>
#include <glad/glad.h>

class Texture
{
public:
    Texture() = default;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;
    ~Texture() noexcept;

    /**
     * Creates texture from given raw data.
     *
     * @param data             Pointer to raw data of the texture
     * @param width            Width of the texture
     * @param height           Height of the texture
     * @param format           Format of the texture data (e.g. GL_RGB)
     * @param generateMipmaps  True, if mipmaps should be generated automatically
     *
     * @return True, if texture has been loaded correctly or false otherwise.
     */
    bool createFromData(const unsigned char* data, GLsizei width, GLsizei height, GLenum format, bool generateMipmaps = false);
    bool loadTexture2D(const std::string& filePath, bool generateMipmaps = true);
    void bind(GLenum textureUnit = 0) const;
    void deleteTexture() noexcept;
    GLuint getID() const;
    void setTextureType(const std::string& type);
    std::string getTextureType() const;
    std::string getFilePath() const;
    GLsizei getWidth() const;
    GLsizei getHeight() const;
    bool isLoaded() const;
    bool resize(GLsizei newWidth, GLsizei newHeight);

    /**
     * Gets number of available OpenGL texture image units of current hardware.
     */
    static int getNumTextureImageUnits();

private:
    GLuint m_textureID{ 0 }; // OpenGL-assigned texture ID
    GLsizei m_width{ 0 }; // Width of texture in pixels
    GLsizei m_height{ 0 }; // Height of texture in pixels
    GLenum m_format{ 0 }; // Format this texture is represented with

    std::string m_texture_type{ 0 }; // type of texture sample
    std::string m_filePath{ 0 }; // Path of file from which the texture has been loaded (might be empty, if texture was simply created from data)

    bool isLoadedCheck() const;
};