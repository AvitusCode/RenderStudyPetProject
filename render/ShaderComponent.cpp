#include "ShaderComponent.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "../utils/utils.h"

ShaderComponent::~ShaderComponent() noexcept {
	deleteComponent();
}

bool ShaderComponent::loadComponentFromFile(const std::string& fileName, GLenum shaderType)
{
    deleteComponent();

    std::string shaderCode;
    
    try {
        std::ifstream ifs;
        ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        ifs.open(fileName);
        std::stringstream ss;

        ss << ifs.rdbuf();
        ifs.close();

        shaderCode = ss.str();
    }
    catch (const std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return false;
    }

    const char* pure_shader_code = shaderCode.c_str();

	m_shaderID = glCreateShader(shaderType);
	glShaderSource(m_shaderID, 1, &pure_shader_code, nullptr);
	glCompileShader(m_shaderID);

	GLint compileStatus = 0;
	glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE)
	{
		std::cerr << "ERROR: compile fail in" << fileName << std::endl;

		GLint logStatus = 0;
		glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &logStatus);

		if (logStatus > 0)
		{
			GLchar* logMessage = new GLchar[logStatus];
			glGetShaderInfoLog(m_shaderID, logStatus, nullptr, logMessage);
			std::cerr << "Compile message :\n\n" << logMessage << std::endl;
			delete[] logMessage;
		}

		return false;
	}

	m_shaderType = shaderType;
	m_isCompiled = true;
	return true;
}

bool ShaderComponent::isCompiled() const  {
	return m_isCompiled;
}

void ShaderComponent::deleteComponent() noexcept
{
	if (m_shaderID == 0) {
		return;
	}

	std::cout << "Deleting shader with ID " << m_shaderID << std::endl;
	glDeleteShader(m_shaderID);
	m_isCompiled = false;
	m_shaderID = 0;
}

GLuint ShaderComponent::getID() const {
	return m_shaderID;
}

GLenum ShaderComponent::getShaderType() const {
	return m_shaderType;
}