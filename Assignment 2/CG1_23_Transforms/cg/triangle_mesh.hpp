#pragma once

#include "glm.hpp"
#include "ogl.hpp"
#include "buffer.hpp"

#include <vector>
#include <memory>

namespace cg
{
	// a regular vertex
	// all our meshes use this vertex type, unless we want to use normal maps
	struct BaseVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
	};

	template<typename VertexType>
	struct TriangleMeshData
	{
		std::vector<VertexType>		attributes;
		std::vector<unsigned>		indices;
	};

	typedef TriangleMeshData<BaseVertex> BaseMeshData;

	struct TriangleMesh
	{
		GLuint resourceHandle = 0;					// opengl vertex array object handle
		std::shared_ptr<Buffer> attributeBuffer;
		std::shared_ptr<Buffer> indexBuffer;
		unsigned indexCount = 0;
		unsigned vertexCount = 0;
	};

	std::shared_ptr<TriangleMesh> buildGPUMesh(BaseMeshData const& data);
}