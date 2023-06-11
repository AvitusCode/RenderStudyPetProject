#pragma once
#include <glad/glad.h>
#include <set>
#include <vector>
#include <string>

class ShaderComponent
{
public:
    ShaderComponent() = default;
    ShaderComponent(const ShaderComponent&) = delete;
    ShaderComponent& operator=(const ShaderComponent&) = delete;
    ShaderComponent(ShaderComponent&&) noexcept;
    ShaderComponent& operator=(ShaderComponent&&) noexcept;
    ~ShaderComponent() noexcept;

    /**
     * Loads and compiles shader from a file.
     *
     * @param fileName    path to a file
     * @param shaderType  type of shader (vertex, fragment, geometry...)
     *
     * @return True, if the shader has been successfully loaded and compiled, false otherwise.
     */
    bool loadComponentFromFile(const std::string& fileName, GLenum shaderType);
    bool isCompiled() const;
    void deleteComponent() noexcept;
    GLuint getID() const;
    GLenum getShaderType() const;

private:
    GLuint m_shaderID = 0; // OpenGL-assigned shader ID
    GLenum m_shaderType = 0; // Type of shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...)
    bool m_isCompiled = false; // Flag telling, whether shader has been loaded and compiled successfully
};