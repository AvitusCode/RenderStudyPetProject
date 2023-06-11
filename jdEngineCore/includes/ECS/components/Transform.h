#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	mutable glm::mat4 m_modelMatrix;

	glm::mat4 computeModelMatrix() const
	{
		const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		// Y * X * Z
		const glm::mat4 rotationMatrix = transformY * transformX * transformZ;
		// translation * rotation * scale (also know as TRS matrix)
		m_modelMatrix = glm::translate(glm::mat4(1.0f), position) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
		return m_modelMatrix;
	}

	const glm::vec3& getGlobalPosition() const {
		return m_modelMatrix[3];
	}

	glm::vec3 getRight() const {
		return m_modelMatrix[0];
	}

	glm::vec3 getUp() const {
		return m_modelMatrix[1];
	}

	glm::vec3 getBackward() const {
		return m_modelMatrix[2];
	}

	glm::vec3 getForward() const {
		return -m_modelMatrix[2];
	}

	glm::vec3 getGlobalScale() const {
		return { glm::length(getRight()), glm::length(getUp()), glm::length(getBackward()) };
	}
};

struct Transform2D
{
	glm::vec2 position;
	glm::vec2 rotation;
	glm::vec2 scale;
};
