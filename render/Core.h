#pragma once
#include <glad/glad.h>


class Core
{
public:
	Core() : shaderProgramID(0), m_mode(GL_TRIANGLES) {};

	unsigned int getShaderId() const {
		return shaderProgramID;
	};
	void setMode(GLenum m) {
		m_mode = m;
	}
	GLenum getMode() const {
		return m_mode;
	}

	virtual void drawObject() const = 0;
	virtual void setupObject() = 0;

	virtual ~Core() = default;

protected:
	GLuint shaderProgramID;
	GLenum m_mode;
};