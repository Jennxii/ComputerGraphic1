#include "buffer.hpp"

namespace cg
{
	void deleteBuffer(Buffer*& buffer)
	{
		if (buffer == nullptr)
			return;

		glDeleteBuffers(1, &buffer->resourceHandle);

		delete buffer;
		buffer = nullptr;
	}

	std::shared_ptr<Buffer> createEmptyBuffer()
	{
		std::shared_ptr<Buffer> result(new Buffer, deleteBuffer);
		glGenBuffers(1, &result->resourceHandle);
		return result;
	}
}