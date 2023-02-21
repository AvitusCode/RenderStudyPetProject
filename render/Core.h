#pragma once
#include <glad/glad.h>


class Core
{
public:
	Core() : shaderProgramID(0) {};

	unsigned int getShaderId() const {
		return shaderProgramID;
	};

	virtual void drawObject() const = 0;
	virtual void setupObject() = 0;

	virtual ~Core() = default;

protected:
	unsigned int shaderProgramID;
};