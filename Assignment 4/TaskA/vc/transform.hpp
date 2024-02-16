#pragma once

#include "cg\glm.hpp"

struct Transform
{
	glm::vec3 scale;
	glm::vec3 translation;
	glm::fquat orientation;

	Transform()
		: scale(1.f), translation(0.f), orientation(1.f, 0.f, 0.f, 0.f)
	{}

	Transform(glm::vec3 const& s, glm::vec3 const& t, glm::fquat const& o)
		: scale(s), translation(t), orientation(o)
	{}

	glm::mat4 modelMatrix() const
	{
		return glm::translate(glm::mat4(1.f), glm::vec3(translation))
			* glm::mat4_cast(orientation)
			* glm::scale(glm::mat4(1.f), scale);
	}

};
