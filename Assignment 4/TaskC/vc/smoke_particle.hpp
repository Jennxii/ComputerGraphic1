#pragma once

#include "cg\glm.hpp"

struct SmokeParticle
{
	float lifetime = -1.f;		// negative lifetime --> particle has never been spawned, after first spawn lifetime will always be >= 0.f
	glm::vec3 spawnPosition;
	glm::vec3 moveDirection;
};
