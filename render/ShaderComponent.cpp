#include "ShaderComponent.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include "../utils/jd_string.h"

bool getLinesFromShader(const std::string& fileName, std::vector<std::string>& result, std::set<std::string>& has_include, bool isReadingIncludedFile = false);

ShaderComponent::ShaderComponent(ShaderComponent&& other) noexcept
{
	*this = std::move(other);
}
ShaderComponent& ShaderComponent::operator=(ShaderComponent&& other) noexcept
{
	if (this != &other)
	{
		deleteComponent();
		m_shaderID = std::exchange(other.m_shaderID, 0);
		m_shaderType = std::exchange(other.m_shaderType, 0);
		m_isCompiled = std::exchange(other.m_isCompiled, false);
	}

	return *this;
}

ShaderComponent::~ShaderComponent() noexcept {
	deleteComponent();
}

bool ShaderComponent::loadComponentFromFile(const std::string& fileName, GLenum shaderType)
{
    deleteComponent();

	std::vector<std::string> fileLines;

	if (std::set<std::string> has_include; !getLinesFromShader(fileName, fileLines, has_include)) {
		return false;
	}

	std::vector<const char*> programSource;
	for (const auto& line : fileLines) {
		programSource.push_back(line.c_str());
	}

	m_shaderID = glCreateShader(shaderType);
	glShaderSource(m_shaderID, static_cast<GLsizei>(fileLines.size()), programSource.data(), nullptr);
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

	// TODO: log
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

bool getLinesFromShader(const std::string& fileName, std::vector<std::string>& result, std::set<std::string>& has_include, bool isReadingIncludedFile)
{
	std::ifstream file(fileName);

	if (!file.good() || !file.is_open()) 
	{
		std::cerr << "Open shader file error" << std::endl;
		return false;
	}

	const char slash = '/';
	std::string correctName = jd::strings::makeCorrectSlashes(fileName, slash);

	size_t slash_index = ~0;
	for (int i = static_cast<int>(fileName.size()) - 1; i >= 0; i--)
	{
		if (fileName[i] == slash)
		{
			slash_index = i;
			break;
		}
	}

	std::string startDir = fileName.substr(0, slash_index + 1);
    std::string line{};
	bool isInsideIncludePart = false;

	while (std::getline(file, line))
	{
		line += "\n"; // getline does not keep newline character
		std::istringstream ss(line);
		std::string firstToken;
		ss >> firstToken;

		if (firstToken == "#include")
		{
			std::string includeFileName;
			ss >> includeFileName;

			if (!includeFileName.empty() && includeFileName[0] == '\"' && includeFileName[includeFileName.size() - 1] == '\"')
			{
				includeFileName = jd::strings::makeCorrectSlashes(includeFileName.substr(1, includeFileName.size() - 2), slash);
				std::string directory = startDir;
				std::vector<std::string> subPaths = jd::strings::split(includeFileName, slash);
				std::string sFinalFileName = "";

				for (const std::string& subPath : subPaths)
				{
					if (subPath == "..") {
						directory = jd::strings::upDirectory(directory, slash);
					}
					else
					{
						if (!sFinalFileName.empty()) {
							sFinalFileName += slash;
						}
						sFinalFileName += subPath;
					}
				}

				const auto combinedIncludeFilePath = directory + sFinalFileName;
				if (has_include.find(combinedIncludeFilePath) == has_include.end())
				{
					has_include.insert(combinedIncludeFilePath);
					getLinesFromShader(directory + sFinalFileName, result, has_include, true);
				}
			}
			else
			{
				std::string msg = "incorrect #include format: " + includeFileName;
				throw std::runtime_error(msg.c_str());
			}
		}
		else if (firstToken == "#include_part") {
			isInsideIncludePart = true;
		}
		else if (firstToken == "#definition_part") {
			isInsideIncludePart = false;
		}
		else if (!isReadingIncludedFile || (isReadingIncludedFile && isInsideIncludePart)) {
			result.push_back(line);
		}
	}

	file.close();
	return true;
}