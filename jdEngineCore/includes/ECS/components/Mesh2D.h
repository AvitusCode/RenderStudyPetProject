#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Render/Texture.h"
#include "Render/Primitives.h"
#include <memory>

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
	std::unique_ptr<Primitives::Shape2D> shape = nullptr;
	TYPE type = TYPE::UNKNOWN;
};