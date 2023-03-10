#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(unsigned int index) : ID(index){}
Shader::~Shader() = default;

unsigned int Shader::getId() const {
    return ID;
}

    // ????????? ???????
void Shader::use() const
{
    glUseProgram(ID);
}

    // ???????? uniform-???????
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

    // ???????? ??????? ??? ???????? ?????? ??????????/?????????? ????????
bool Shader::checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }

    return true;
}

unsigned int load_shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    // 1. ????????? ????????? ???? ??????????/???????????? ???????
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    // ??????????, ??? ??????? ifstream ????? ????????? ??????????:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // ????????? ?????
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        // ?????? ?????????? ???????? ???????
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // ????????? ?????
        vShaderFile.close();
        fShaderFile.close();

        // ???????????? ? ????????? ?????????? ?????? ?? ??????
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // ???? ???? ? ??????????????? ??????? ????????????, ?? ????? ????????? ? ?????????????? ??????
        if (geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure& e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return 0;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. ??????????? ???????
    unsigned int vertex, fragment;

    // ????????? ??????
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    if (!Shader::checkCompileErrors(vertex, "VERTEX")) {
        return 0;
    }

    // ??????????? ??????
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    if (!Shader::checkCompileErrors(fragment, "FRAGMENT")){
        return 0;
    }

    // ???? ??? ??? ?????????????? ??????, ?? ??????????? ???
    unsigned int geometry;
    if (geometryPath != nullptr)
    {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        if (!Shader::checkCompileErrors(geometry, "GEOMETRY")) {
            return 0;
        }
    }

    // ????????? ?????????
    unsigned int index = glCreateProgram();
    glAttachShader(index, vertex);
    glAttachShader(index, fragment);
    if (geometryPath != nullptr) {
        glAttachShader(index, geometry);
    }
    glLinkProgram(index);

    if (!Shader::checkCompileErrors(index, "PROGRAM")) {
        return 0;
    }

    // ????? ????, ??? ?? ??????? ??????? ? ????? ??????????, ??????? ??, ?.?. ??? ??? ?????? ?? ?????
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr) {
        glDeleteShader(geometry);
    }

    return index;
}
