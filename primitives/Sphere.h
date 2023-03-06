#pragma once
#include "../render/Core.h"

class Sphere : public Core
{
public:
	Sphere() = default;

	void drawObject() const override;
	void setupObject() override;

	void setShaderID(GLuint shader);

	~Sphere();

private:
	GLuint sphereVAO;
	GLuint sphereVBO;
	GLuint sphereEBO;
	GLuint indexes;
};