#pragma once

#include "triangle_mesh.hpp"

namespace cg
{
	/*
	*	all of the following functions fill the provided BaseMeshData with vertex attribs & indices
	*	attributes: position, normal, uvs
	*	these can then be uploaded to the gpu
	*/
	namespace BasePrimitives
	{
		// generates a cube
		// axis aligned
		// lower left back corner: (-1, -1, -1)
		// upper left front corner: (1, 1, 1)
		void generateCube(BaseMeshData& meshData);

		// generates a quad
		// aligned with XY Plane
		// lower left vertex: (-1, -1, 0)
		// upper right vertex: (1, 1, 0)
		void generateQuad(BaseMeshData& meshData);

		// generates a plane
		// aligned with XY plane
		// left back vertex: (-1, -1, 0)
		// right front vertex: (1, 1, 0)
		// difference to quad: this geometry is tessellated, i.e. it can have way more vertices than just 4
		void generatePlane(const unsigned xSegs, const unsigned ySegs, BaseMeshData& meshData);

		// generates a 4-sided pyramid
		// aligned with Y axis
		// height: 1 ogl unit
		// 'radius': 1 ogl unit
		// bottom plane: y = -0.5
		void generatePyramid(BaseMeshData& meshData);

		// generates a sphere
		// center: origin
		// radius: ogl unit
		void generateSphere(const unsigned aSegs, const unsigned eSegs, BaseMeshData& meshData);

		// generates a cylinder mantle
		// center: origin
		// height: 1 ogl unit
		// radius: 1 ogl unit
		void generateCylinder(const unsigned aSegs, const unsigned hSegs, BaseMeshData& meshData);
		
		// generates a cone mantle
		// base is at -0.5, top is at +0.5
		// height: 1 ogl unit
		// radius: 1 ogl unit
		void generateCone(const unsigned aSegs, const unsigned hSegs, BaseMeshData& meshData);

		// generates a disk
		// aligned with XY Plane
		// center: origin
		// radius: 1 ogl unit
		void generateDisk(const unsigned aSegs, const unsigned rSegs, BaseMeshData& meshData);

		// generates a disk
		// aligned with XY Plane
		// center: origin
		// radius: 1 ogl unit
		void generateRing(const unsigned aSegs, const float rOuter, const float rInner, BaseMeshData& meshData);
	}
}