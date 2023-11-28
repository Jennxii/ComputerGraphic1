#include "raytracer.hpp"

#include <GLM\glm.hpp>
#include <GLM\common.hpp>
#include <GLM\gtc\quaternion.hpp>
#include <GLM\gtc\matrix_transform.hpp>

void Raytracer::renderScene(
	std::vector<VertexData> const& scene,
	std::vector<PointLight> const& pointLights,
	Camera const& camera,
	Framebuffer& output)
{
	size_t vertexCount = scene.size();
	assert(scene.size() % 3 == 0);		// sanity check

	const RGBval black(0, 0, 0);
	const RGBval white(255, 255, 255);

	std::fill(output.pixels.begin(), output.pixels.end(), black);

	for (unsigned x = 0; x < output.width; ++x)
	{
		for (unsigned y = 0; y < output.height; ++y)
		{
			unsigned index = y * output.width + x;
			glm::uvec2 pixel(x, y);

			Ray ray = makeRay(camera, pixel, output.width, output.height);

			// distance between ray origin and closest scene surface
			// initialized to maximum distance
			// -> basically means that this ray has not encountered any surface yet.
			float rayDistance = std::numeric_limits<float>::max();

			for (unsigned t = 0; t < scene.size(); t += 3)
			{
				VertexData const& a = scene[t];
				VertexData const& b = scene[t + 1];
				VertexData const& c = scene[t + 2];

				float distance;	// distance between ray origin and intersection
								// ray origin = camera position, so that's also the distance between camera and intersection
								// you can calc the intersection point as: ray.origin + distance * ray.direction
				float u, v;		// barycentric interpolation weights
				if (intersects(a.position, b.position, c.position, ray, distance, u, v))
				{
					if (distance < rayDistance)
					{
						rayDistance = distance;	// update min. distance along ray

						//glm::vec3 interpolated_color = u * a.color + v * b.color + (1.f - u - v) * c.color;
						glm::vec3 kD = u * a.color + v * b.color + (1.f - u - v) * c.color;
						glm::vec3 normal = u * a.normal + v * b.normal + (1.f - u - v) * c.normal;
						glm::vec3 N = normalize(normal);
						glm::vec3 intersection = ray.origin + distance * ray.direction; //Point P
						glm::vec3 sum(0.f);

						for (auto const& light : pointLights)
						{
							Ray shadowRay;
							
							shadowRay.direction= normalize(light.position - intersection);
							shadowRay.origin = intersection + 0.00001f * shadowRay.direction;

							bool isShadowed = false;
							for (unsigned t_sh = 0; t_sh < scene.size(); t_sh += 3)
							{
								VertexData const& a_sh = scene[t_sh];
								VertexData const& b_sh = scene[t_sh + 1];
								VertexData const& c_sh = scene[t_sh + 2];

								float distance_sh;
								float u_sh;
								float v_sh;
								if (intersects(a_sh.position, b_sh.position, c_sh.position, shadowRay, distance_sh, u_sh, v_sh))
								{
									isShadowed = true;
									break;
								}


							}
							if (!isShadowed)
							{
								glm::vec3 L = glm::normalize(light.position - intersection); //light direction L
								sum += light.color * kD * glm::max(glm::dot(N, L), 0.f);
								glm::vec3 color = glm::clamp(255.f * sum, glm::vec3(0.f), glm::vec3(255.f));
								output.pixels[index] = RGBval(color.r, color.g, color.b);
							}
							else
							{
								output.pixels[index] = glm::vec3(0, 0, 0);
							}
							
						}
						
					}
				}
			}
		}
	}
}

Ray Raytracer::makeRay(Camera const& camera, glm::uvec2 const& pixel, const unsigned width, const unsigned height)
{
	// tan(fov/2) = opp/adj
	// assume: adj = 1
	// tan(fov/2) = opp

	float plane_height = 2 * glm::tan(camera.fov / 2);
	float pixel_height = plane_height / height;

	float plane_width = plane_height * camera.aspectRatio;
	float pixel_width = plane_width / width;

	glm::vec3 side = glm::cross(camera.forward, camera.up);

	glm::vec3 lower_left = camera.position + 1.f * camera.forward
		- 0.5f * plane_height * camera.up
		- 0.5f * plane_width * side;

	glm::vec3 pixel_world = lower_left
		+ (pixel.y + 0.5f) * pixel_height * camera.up
		+ (pixel.x + 0.5f) * pixel_width * side;

	Ray r;
	r.direction = glm::normalize(pixel_world - camera.position);
	r.origin = camera.position;
	return r;
}

bool Raytracer::intersects(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c, Ray const& ray, float &t, float &u, float &v)
{
// compute intersection point, if it exists

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 N = glm::cross(ab, ac);

	float area = glm::length(N);		// 2 * area

	N = glm::normalize(N);

	// check if ray is parallel to triangle
	// both ray direction & N are normalized (unit length) - so dotND is the cosinus of the angle between the vectors
	// we want to figure out if the ray is parallel to the surface - in that case, it would be perpendicular to N (angle = 90°)
	// cos(90°) is 0, but instead of checking againt 0 directly, we use an epsilon threshold
	float dotND = glm::dot(N, ray.direction);
	if (glm::abs(dotND) < glm::epsilon<float>())
		return false;

	// find D
	float D = -1.f * (N.x * a.x + N.y * a.y + N.z * a.z);

	// check the lva slides to know where this formula comes from
	t = -1.f * (N.x * ray.origin.x + N.y * ray.origin.y + N.z * ray.origin.z + D) / dotND;

	// t is negative -> intersection is "behind" the ray origin
	if (t < 0.f)
		return false;

	// ray-plane intersection, but it might still be outside of the triangle!
	glm::vec3 P = ray.origin + t * ray.direction;

// inside out test

	glm::vec3 edge0 = b - a;
	glm::vec3 v0 = P - a;
	glm::vec3 c0 = glm::cross(edge0, v0);
	//float w = glm::length(c0) / area;
	if (glm::dot(N, c0) < 0.f)
		return false;

	glm::vec3 edge1 = c - b;
	glm::vec3 v1 = P - b;
	glm::vec3 c1 = glm::cross(edge1, v1);
	u = glm::length(c1) / area;
	if (glm::dot(N, c1) < 0.f)
		return false;

	glm::vec3 edge2 = a - c;
	glm::vec3 v2 = P - c;
	glm::vec3 c2 = glm::cross(edge2, v2);
	v = glm::length(c2) / area;
	if (glm::dot(N, c2) < 0.f)
		return false;

	return true;  //success, this ray hits the triangle
}