#pragma once
#include <glm/glm.hpp>
#include <string>

class UniformBuffer
{
public:
	UniformBuffer() = delete;
	UniformBuffer(unsigned int point);
	~UniformBuffer() = default;

	void initUniformBuffer();
	// Updates the uniform buffer's data
	void updateUniformBuffer(const glm::mat4& view, const glm::mat4& projection);
	// Binding shader uniform block with uniform buffer 
	void bindingUniformBlock(unsigned int shaderId, const std::string& shaderName);

	const unsigned int getBindingPoint() const;
	void setBindingPoint(unsigned int point);


private:
	unsigned int bindingPoint;
	unsigned int UBO;
};