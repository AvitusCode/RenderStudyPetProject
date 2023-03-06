#pragma once
#include "../render/Core.h"

class Quad : public Core
{
public:
	Quad() = default;

	void drawObject() const override;
	void setupObject() override;

	void setShaderID(GLuint shader);

	~Quad();

private:
	GLuint quadVAO;
	GLuint quadVBO;
};