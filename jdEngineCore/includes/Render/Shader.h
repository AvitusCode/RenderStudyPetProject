#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

#include "ShaderComponent.h"

class Shader
{
public:
    Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&&) noexcept;
    Shader& operator=(Shader&&) noexcept;

    template<typename... Components>
    Shader(Components&... scomponents);

    ~Shader() noexcept;
    // Активация шейдера
    void use() const;

    // new architecture
    template<typename T>
    void set(const std::string& name, const T& value) const
    {
        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location == -1) {
            return;
        }

        if constexpr (std::is_same_v<T, glm::vec2>) {
            glUniform2fv(location, 1, &value[0]);
        }
        else if constexpr (std::is_same_v<T, glm::vec3>) {
            glUniform3fv(location, 1, &value[0]);
        }
        else if constexpr (std::is_same_v<T, glm::vec4>) {
            glUniform4fv(location, 1, &value[0]);
        }
        else if constexpr (std::is_same_v<T, glm::mat2>) {
            glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
        }
        else if constexpr (std::is_same_v<T, glm::mat3>) {
            glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
        }
        else if constexpr (std::is_same_v<T, glm::mat4>) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
        }
    }

    // Полезные uniform-функции
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    // Adding neccessary shader components
    bool addComponentToProgram(const ShaderComponent& scomponent) const;
    /**
     * Links the program. If the function succeeds, shader program is ready to use.
     * @return True, if the shader has been linked or false otherwise.
     */
    bool linkProgram();
    void deleteProgram() noexcept;
    void createProgram();

    GLuint getId() const;
private:
    GLuint ID = 0;
    bool is_linked = false;
};

template<typename... Components>
Shader::Shader(Components&... scomponents) : ID(0), is_linked(false)
{
    if (sizeof...(Components) < 2) {
        return;
    }

    createProgram();

    (addComponentToProgram(scomponents), ...);

    linkProgram();
}