#pragma once

#include "cg\glm.hpp"

/*
*	evaluates a unit circle at time t
*	and tells you position / tangent
*/
struct Circle
{
	static glm::vec3 position(const float t)
	{
		float x = glm::sin(t);
		float z = glm::cos(t);
		return glm::vec3(x, 0, z);		// curve in xz plane -> y is 0
	}

	static glm::vec3 tangent(const float t)
	{
		glm::vec3 p0 = position(t - 0.1f);
		glm::vec3 p1 = position(t + 0.1f);
		return glm::normalize(p1 - p0);
	}
};

/*
*	evaluates the leniscate of bernoulli at time t
*	(basically, an infinity-symbol)
*	and tells you position / tangent
*/
struct LemniscateOfBernoulli
{
	static glm::vec3 position(const float t)
	{
		float s = 6.f / (3 - cos(2 * t));
		float x = s * cos(t);
		float z = s * sin(2 * t) / 2;
		return glm::vec3(x, 0, z);	// curve in xz plane -> y is 0
	}

	static glm::vec3 tangent(const float t)
	{
		glm::vec3 p0 = position(t - 0.1f);
		glm::vec3 p1 = position(t + 0.1f);
		return glm::normalize(p1 - p0);
	}
};
