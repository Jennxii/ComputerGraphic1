#pragma once

#include "raytracer.hpp"

#include <GLM\glm.hpp>
#include <GLM\gtc\constants.hpp>

#include <vector>

static void makeTriangleScene(std::vector<VertexData>& data)
{
	data = {
		{ glm::vec3(-1.f, -1.f, -1.f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 1.f) },
		{ glm::vec3(+1.f, -1.f, -1.f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 0.f, 1.f) },
		{ glm::vec3(+0.f, +1.f, -1.f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.f, 0.f, 1.f) },
	};
}

static void makeDemoScene(std::vector<VertexData>& data)
{
	// 12 vertices of an icosahedron
	std::vector<glm::vec3> ico_vertices = {
		glm::vec3(+0.000000f, -0.525731f, +0.850651f),
		glm::vec3(+0.850651f, +0.000000f, +0.525731f),
		glm::vec3(+0.850651f, +0.000000f, -0.525731f),
		glm::vec3(-0.850651f, +0.000000f, -0.525731f),
		glm::vec3(-0.850651f, +0.000000f, +0.525731f),
		glm::vec3(-0.525731f, +0.850651f, +0.000000f),
		glm::vec3(+0.525731f, +0.850651f, +0.000000f),
		glm::vec3(+0.525731f, -0.850651f, +0.000000f),
		glm::vec3(-0.525731f, -0.850651f, +0.000000f),
		glm::vec3(+0.000000f, -0.525731f, -0.850651f),
		glm::vec3(+0.000000f, +0.525731f, -0.850651f),
		glm::vec3(+0.000000f, +0.525731f, +0.850651f)
	};

	// assigning colors to the icosahedron
	// basically, there's no "right" way of doing this, so I just assign colors (hues) based on the azimuth
	std::vector<glm::vec3> ico_colors;
	for (unsigned i = 0; i < 12; ++i)
	{
		float hue = glm::atan(ico_vertices[i].z, ico_vertices[i].x) / glm::pi<float>();
		glm::vec3 p = glm::abs(glm::fract(glm::vec3(hue, hue, hue) + glm::vec3(1, 2 / 3.f, 1 / 3.f)) * 6.f - glm::vec3(3));
		glm::vec3 color = 0.8f * glm::mix(glm::vec3(1), glm::clamp(p - glm::vec3(1), 0.f, 1.f), 0.66f);
		ico_colors.push_back(color);
	}

	// so we have 12 vertices, but 20 different normal vectors
	// this is because each face has its own "face" normal
	std::vector<glm::vec3> ico_normals = {
		glm::vec3(+0.934172f, +0.356822f, +0.000000f),
		glm::vec3(+0.934172f, -0.356822f, +0.000000f),
		glm::vec3(-0.934172f, +0.356822f, +0.000000f),
		glm::vec3(-0.934172f, -0.356822f, +0.000000f),
		glm::vec3(+0.000000f, +0.934172f, +0.356822f),
		glm::vec3(+0.000000f, +0.934172f, -0.356822f),
		glm::vec3(+0.356822f, +0.000000f, -0.934172f),
		glm::vec3(-0.356822f, +0.000000f, -0.934172f),
		glm::vec3(+0.000000f, -0.934172f, -0.356822f),
		glm::vec3(+0.000000f, -0.934172f, +0.356822f),
		glm::vec3(+0.356822f, +0.000000f, +0.934172f),
		glm::vec3(-0.356822f, +0.000000f, +0.934172f),
		glm::vec3(+0.577350f, +0.577350f, -0.577350f),
		glm::vec3(+0.577350f, +0.577350f, +0.577350f),
		glm::vec3(-0.577350f, +0.577350f, -0.577350f),
		glm::vec3(-0.577350f, +0.577350f, +0.577350f),
		glm::vec3(+0.577350f, -0.577350f, -0.577350f),
		glm::vec3(+0.577350f, -0.577350f, +0.577350f),
		glm::vec3(-0.577350f, -0.577350f, -0.577350f),
		glm::vec3(-0.577350f, -0.577350f, +0.577350f)
	};

	// so here we have 2 different objects (floor & icosahedron) shoved into the same data structure
	data = {
		// 2 triangles forming the "floor", color is light gray, normal points upwards
		{ glm::vec3(-10.f, -2.f, +10.f), glm::vec3(0.66f), glm::vec3(0.f, 1.f, 0.f) },
		{ glm::vec3(+10.f, -2.f, +10.f), glm::vec3(0.66f), glm::vec3(0.f, 1.f, 0.f) },
		{ glm::vec3(+10.f, -2.f, -10.f), glm::vec3(0.66f), glm::vec3(0.f, 1.f, 0.f) },
		{ glm::vec3(+10.f, -2.f, -10.f), glm::vec3(0.66f), glm::vec3(0.f, 1.f, 0.f) },
		{ glm::vec3(-10.f, -2.f, -10.f), glm::vec3(0.66f), glm::vec3(0.f, 1.f, 0.f) },
		{ glm::vec3(-10.f, -2.f, +10.f), glm::vec3(0.66f), glm::vec3(0.f, 1.f, 0.f) },
		// 20 triangles for the icosahedron
		{ ico_vertices[1], ico_colors[1], ico_normals[0] }, { ico_vertices[2], ico_colors[2], ico_normals[0] }, { ico_vertices[6], ico_colors[6], ico_normals[0] },
		{ ico_vertices[1], ico_colors[1], ico_normals[1] }, { ico_vertices[7], ico_colors[7], ico_normals[1] }, { ico_vertices[2], ico_colors[2], ico_normals[1] },
		{ ico_vertices[3], ico_colors[3], ico_normals[2] }, { ico_vertices[4], ico_colors[4], ico_normals[2] }, { ico_vertices[5], ico_colors[5], ico_normals[2] },
		{ ico_vertices[4], ico_colors[4], ico_normals[3] }, { ico_vertices[3], ico_colors[3], ico_normals[3] }, { ico_vertices[8], ico_colors[8], ico_normals[3] },
		{ ico_vertices[6], ico_colors[6], ico_normals[4] }, { ico_vertices[5], ico_colors[5], ico_normals[4] }, { ico_vertices[11], ico_colors[11], ico_normals[4] },
		{ ico_vertices[5], ico_colors[5], ico_normals[5] }, { ico_vertices[6], ico_colors[6], ico_normals[5] }, { ico_vertices[10], ico_colors[10], ico_normals[5] },
		{ ico_vertices[9], ico_colors[9], ico_normals[6] }, { ico_vertices[10], ico_colors[10], ico_normals[6] }, { ico_vertices[2], ico_colors[2], ico_normals[6] },
		{ ico_vertices[10], ico_colors[10], ico_normals[7] }, { ico_vertices[9], ico_colors[9], ico_normals[7] }, { ico_vertices[3], ico_colors[3], ico_normals[7] },
		{ ico_vertices[7], ico_colors[7], ico_normals[8] }, { ico_vertices[8], ico_colors[8], ico_normals[8] }, { ico_vertices[9], ico_colors[9], ico_normals[8] },
		{ ico_vertices[8], ico_colors[8], ico_normals[9] }, { ico_vertices[7], ico_colors[7], ico_normals[9] }, { ico_vertices[0], ico_colors[0], ico_normals[9] },
		{ ico_vertices[11], ico_colors[11], ico_normals[10] }, { ico_vertices[0], ico_colors[0], ico_normals[10] }, { ico_vertices[1], ico_colors[1], ico_normals[10] },
		{ ico_vertices[0], ico_colors[0], ico_normals[11] }, { ico_vertices[11], ico_colors[11], ico_normals[11] }, { ico_vertices[4], ico_colors[4], ico_normals[11] },
		{ ico_vertices[6], ico_colors[6], ico_normals[12] }, { ico_vertices[2], ico_colors[2], ico_normals[12] }, { ico_vertices[10], ico_colors[10], ico_normals[12] },
		{ ico_vertices[1], ico_colors[1], ico_normals[13] }, { ico_vertices[6], ico_colors[6], ico_normals[13] }, { ico_vertices[11], ico_colors[11], ico_normals[13] },
		{ ico_vertices[3], ico_colors[3], ico_normals[14] }, { ico_vertices[5], ico_colors[5], ico_normals[14] }, { ico_vertices[10], ico_colors[10], ico_normals[14] },
		{ ico_vertices[5], ico_colors[5], ico_normals[15] }, { ico_vertices[4], ico_colors[4], ico_normals[15] }, { ico_vertices[11], ico_colors[11], ico_normals[15] },
		{ ico_vertices[2], ico_colors[2], ico_normals[16] }, { ico_vertices[7], ico_colors[7], ico_normals[16] }, { ico_vertices[9], ico_colors[9], ico_normals[16] },
		{ ico_vertices[7], ico_colors[7], ico_normals[17] }, { ico_vertices[1], ico_colors[1], ico_normals[17] }, { ico_vertices[0], ico_colors[0], ico_normals[17] },
		{ ico_vertices[3], ico_colors[3], ico_normals[18] }, { ico_vertices[9], ico_colors[9], ico_normals[18] }, { ico_vertices[8], ico_colors[8], ico_normals[18] },
		{ ico_vertices[4], ico_colors[4], ico_normals[19] }, { ico_vertices[8], ico_colors[8], ico_normals[19] }, { ico_vertices[0], ico_colors[0], ico_normals[19] }
	};
}
