#pragma once
#include "../render/Core.h"

class Cube : public Core
{
public:
	Cube() = default;
	void drawObject() const override;
	void setupObject() override;

	void setShaderID(GLuint shader);
	~Cube();
private:
	GLuint cubeVAO;
	GLuint cubeVBO;
};