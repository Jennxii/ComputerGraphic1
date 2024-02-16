#include "triangle_mesh.hpp"

namespace cg
{
	void deleteTriangleMesh(TriangleMesh*& mesh)
	{
		if (mesh == nullptr)
			return;

		glDeleteVertexArrays(1, &mesh->resourceHandle);
		mesh->attributeBuffer = nullptr;
		mesh->indexBuffer = nullptr;

		delete mesh;
		mesh = nullptr;
	}

	std::shared_ptr<TriangleMesh> buildGPUMesh(BaseMeshData const& data)
	{
		std::shared_ptr<TriangleMesh> result(new TriangleMesh, deleteTriangleMesh);

#pragma region VertexBufferObjects
		result->indexBuffer = cg::createEmptyBuffer();
		result->indexBuffer->byteSize = sizeof(unsigned) * data.indices.size();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->indexBuffer->resourceHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, result->indexBuffer->byteSize, (GLvoid const*)(data.indices.data()), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		result->attributeBuffer = cg::createEmptyBuffer();
		result->attributeBuffer->byteSize = sizeof(BaseVertex) * data.attributes.size();
		glBindBuffer(GL_ARRAY_BUFFER, result->attributeBuffer->resourceHandle);
		glBufferData(GL_ARRAY_BUFFER, result->attributeBuffer->byteSize, (GLvoid const*)(data.attributes.data()), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#pragma endregion

#pragma region VertexArrayObject
		glGenVertexArrays(1, &(result->resourceHandle));
		glBindVertexArray(result->resourceHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->indexBuffer->resourceHandle);
		glBindBuffer(GL_ARRAY_BUFFER, result->attributeBuffer->resourceHandle);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(cg::BaseVertex), (GLvoid const*)offsetof(cg::BaseVertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(cg::BaseVertex), (GLvoid const*)offsetof(cg::BaseVertex, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(4, 2, GL_FLOAT, false, sizeof(cg::BaseVertex), (GLvoid const*)offsetof(cg::BaseVertex, texcoord));
		glEnableVertexAttribArray(4);
		glBindVertexArray(0);
#pragma endregion

		result->indexCount = (unsigned)data.indices.size();
		result->vertexCount = (unsigned)data.attributes.size();
		return result;
	}
}