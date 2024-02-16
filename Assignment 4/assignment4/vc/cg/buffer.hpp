#pragma once

#include "glm.hpp"
#include "ogl.hpp"

#include <vector>
#include <memory>

namespace cg
{
	// an opengl buffer, currently used for vertex buffers (attributes & indices)

	struct Buffer
	{
		GLuint resourceHandle = 0;		// opengl vertex buffer handle
		size_t byteSize = 0;
	};

	// creates a completely uninitialized buffer object
	std::shared_ptr<Buffer> createEmptyBuffer();
}