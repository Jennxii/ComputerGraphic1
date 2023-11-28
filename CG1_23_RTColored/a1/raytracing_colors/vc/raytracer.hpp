#pragma once

#include <GLM\glm.hpp>

#include <vector>
struct PointLight
{
	glm::vec3 position;
	glm::vec3 color;
};
struct VertexData
{
	glm::vec3 position;			// position - xyz
	glm::vec3 color;			// color - the value for each channel is in [0.0, 1.0]
	glm::vec3 normal;			// normal direction - already has unit length
};

struct Camera
{
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;

	float fov;					// vertical fov
	float aspectRatio;			// horizontal fov = vertical fov * aspect ratio
	float zNear;
	float zFar;
};

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;		// unit length
};

typedef glm::vec<3, unsigned char> RGBval;

struct Framebuffer
{
	unsigned width;
	unsigned height;
	std::vector<RGBval> pixels;		// index of pixel (x, y) -> pixels[y * width + x]
};

class Raytracer
{

public:

	static void renderScene(
		std::vector<VertexData> const& scene,
		std::vector<PointLight> const& pointLights,
		Camera const& camera,
		Framebuffer &output);

	static Ray makeRay(
		Camera const& camera,							// the camera
		glm::uvec2 const& pixel,						// the pixel through which the ray should pass
		const unsigned width,							// the pixel width of the viewport
		const unsigned height);							// the pixel height of the viewport

	static bool intersects(
		glm::vec3 const& vertexA,						// triangle vertex a
		glm::vec3 const& vertexB,						// triangle vertex b
		glm::vec3 const& vertexC,						// triangle vertex c
		Ray const& ray,									// the ray
		float& t,										// distance to the camera
		float& u,
		float& v);

};

