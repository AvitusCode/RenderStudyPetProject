#pragma once
#include <glad/glad.h>

namespace gl_err
{
	GLenum glCheckError_(const char* file, int line);
}

#define glCheckError() gl_err::glCheckError_(__FILE__, __LINE__) 