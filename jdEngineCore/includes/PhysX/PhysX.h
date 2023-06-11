#pragma once
#include <glm/glm.hpp>

namespace jd
{
	inline float cross(const glm::vec2& vec1, const glm::vec2& vec2) {
		return vec1.x * vec2.y - vec1.y * vec2.x;
	}

	inline float lenSqr(const glm::vec2& vec) {
		return vec.x * vec.x + vec.y * vec.y;
	}
}