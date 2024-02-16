#pragma once

#include "glm.hpp"
#include "ogl.hpp"
#include "buffer.hpp"

#include <vector>
#include <memory>

namespace cg
{
	// a regular/default vertex
	// all our meshes use this vertex type
	struct BaseVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
	};

	// the data of a triangle mesh, stored as index triangle list
	// you could theoretically make a mesh out of any type of vertex
	// -> that's why it's a template
	template<typename VertexType>
	struct TriangleMeshData
	{
		std::vector<VertexType>		attributes;
		std::vector<unsigned>		indices;
	};

	typedef TriangleMeshData<BaseVertex> BaseMeshData;

	// what we need to render a triangle with the help of opengl:
	// 1 interleaved attribute buffer
	// 1 index buffer
	// 1 vertex array object
	// -> this is absolutely not the only way of representing a mesh
	// (i.e. we could use multiple, non-interleaved attribute buffers)
	// but we need some type of convention here.
	struct TriangleMesh
	{
		GLuint resourceHandle = 0;					// opengl vertex array object handle
		std::shared_ptr<Buffer> attributeBuffer;	// opengl buffer - stores the vertex data
		std::shared_ptr<Buffer> indexBuffer;		// opengl buffer - stores the index data
		unsigned indexCount = 0;					// number of indices (necessary if drawing with glDrawElements)
		unsigned vertexCount = 0;					// number of vertices (necessary if drawing with glDrawArrays)
	};

	// this function takes mesh data, uploads it to the GPU, and hands us a shared ptr to a TriangleMesh
	// (complete with a deleter, so we don't have to worry about freeing the resources)
	// the following attribute locations are hardcoded:
	// location 0 -> vec3 vertexPosition
	// location 1 -> vec3 vertexNormal
	// location 2 -> vec2 vertexUV
	std::shared_ptr<TriangleMesh> buildGPUMesh(BaseMeshData const& data);
}