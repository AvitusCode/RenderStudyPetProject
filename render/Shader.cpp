#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>

Shader::Shader() : ID(0), is_linked(false) {}

Shader::Shader(Shader&& other) noexcept{
    *this = std::move(other);
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        deleteProgram();

        is_linked = std::exchange(other.is_linked, false);
        ID = std::exchange(other.ID, 0);
    }

    return *this;
}

Shader::~Shader() noexcept {
    deleteProgram();
}

unsigned int Shader::getId() const {
    return ID;
}

void Shader::use() const {
    if (is_linked) {
        glUseProgram(ID);
    }
}

void Shader::createProgram() {
    if (ID != 0) {
        return;
    }

    ID = glCreateProgram();
}

void Shader::deleteProgram() noexcept
{
    if (ID == 0) {
        return;
    }

    // TODO: log
    std::cout << "Deleting shaderProgram with ID " << ID << std::endl;

    glDeleteProgram(ID);
    is_linked = false;
}

    // Полезные uniform-функции
    // ------------------------------------------------------------------------
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
    // ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
    // ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
    // ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const glm::vec2& value) const{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
    // ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
    // ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
    // ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
    // ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
    // ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

bool Shader::addComponentToProgram(const ShaderComponent& scomponent) const
{
    if (!scomponent.isCompiled()) {
        return false;
    }

    glAttachShader(ID, scomponent.getID());
    return true;
}

bool Shader::linkProgram()
{
    if (is_linked) {
        return true;
    }

    glLinkProgram(ID);
    GLint linkStatus = 0;
    glGetProgramiv(ID, GL_LINK_STATUS, &linkStatus);
    is_linked = linkStatus == GL_TRUE;

    if (!is_linked)
    {
        std::cerr << "ERROR: link program failuare with ID = " << ID << std::endl;

        GLint logStatus = 0;
        glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logStatus);

        if (logStatus > 0)
        {
            GLchar* logMessage = new GLchar[logStatus];
            glGetProgramInfoLog(ID, logStatus, nullptr, logMessage);
            std::cerr << "Log message: \n\n" << logMessage << std::endl;
            delete[] logMessage;
        }

        return false;
    }

    return true;
}
