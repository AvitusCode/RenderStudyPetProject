#pragma once
#include <glad/glad.h>
#include <system_error>
#include <string>

namespace gl_err
{
	GLenum glCheckError_(const char* file, int line);
}

namespace jd::err
{
	inline std::string makeError(int e) {
		return std::make_error_code(static_cast<std::errc>(e)).message();
	}
}

#define GL_ERROR() gl_err::glCheckError_(__FILE__, __LINE__) 