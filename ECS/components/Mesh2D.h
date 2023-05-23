#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "../../render/Texture.h"
#include "../../render/Primitives.h"

enum class TYPE : uint8_t
{
	UNKNOWN = 255,
	CIRCLE = 0,
	POLYGON,
	TYPE_SIZE
};

template<typename E>
constexpr auto getIndxType(E e) noexcept {
	return static_cast<std::underlying_type_t<E>>(e);
}

struct Mesh2D
{
	// configs
	inline static GLuint position_attrib = 0;
	inline static GLuint texture_attrib = 1;

	glm::mat4 Model;
	Primitives::Shape2D* shape = nullptr;
	TYPE type = TYPE::UNKNOWN;
};