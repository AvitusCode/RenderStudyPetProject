#pragma once
#include <glm/glm.hpp>

struct RigidBody
{
	glm::vec3 velocity;
	glm::vec3 acceleration;
};

struct RigidBody2D
{
	glm::vec2 velocity; // 2x float
	float angularVelocity;
	float torque;
	float orient;

	float momentI;
	float imomentI;
	float mass;
	float imass;

	float staticFriction;
	float dynamicFriction;
	float restitution;
};
