#pragma once
#include <map>
#include <string>
#include <memory>

#include "Shader.h"

class ShaderManager
{
public:
	ShaderManager() = default;
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
	
	/**
	 * Creates new shader program and stores it with specified key.
	 * @param key  Key to store shader program with
	 * @return Shader program instance with specified key.
	 */
	Shader& createShaderProgram(const std::string& key);

	/**
	 * Retrieves shader program with a specified key.
	 * @param key  Key to get shader program from
	 * @return Shader program instance from a specified key.
	 */
	Shader& getShaderProgram(const std::string& key) const;

	/**
	* Deleting shader program with a specific key
	* @param key Key to get shader program from
	*/
	void deleteShaderProgram(const std::string& key);
private:
	/**
	 * Checks, if shader program with specified key exists.
	 * @param  key  Shader program key to check existence of
	 * @return True if shader program exists, or false otherwise.
	 */
	bool containsShaderProgram(const std::string& key) const;

	std::map<std::string, std::unique_ptr<Shader>> shader_cashe;
};