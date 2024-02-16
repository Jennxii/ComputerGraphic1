#pragma once

#define GLM_FORCE_RADIANS

#include <GLM\glm.hpp>
#include <GLM\common.hpp>
#include <GLM\gtc\quaternion.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtx\euler_angles.hpp>

#include <ostream>

/*
*	output operators (for debug purposes)
*	glm::vec4, glm::vec3, gl::vec2,
*	glm::ivec4, glm::ivec3, glm::ivec2
* 	glm::uvec4, glm::uvec3, glm::uvec2
*	glm::mat4, glm::mat3, glm::mat2
*/

template<typename T>
std::ostream& operator<<(std::ostream& out, glm::vec<4, T> const& v)
{
	out << "( " << v.x << " | " << v.y << " | " << v.z << " | " << v.w << " )";
	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, glm::vec<3, T> const& v)
{
	out << "( " << v.x << " | " << v.y << " | " << v.z << " )";
	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, glm::vec<2, T> const& v)
{
	out << "( " << v.x << " | " << v.y << " )";
	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, glm::mat<4, 4, T> const& m)
{
	for (unsigned row = 0; row < 4; ++row)
	{
		out << "| ";
		for (unsigned col = 0; col < 4; ++col)
		{
			out << m[col][row];
			out << " ";
		}
		out << "|";
		if (row < 3) out << "\n";
	}
	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, glm::mat<3, 3, T> const& m)
{
	for (unsigned row = 0; row < 3; ++row)
	{
		out << "| ";
		for (unsigned col = 0; col < 3; ++col)
		{
			out << m[col][row];
			out << " ";
		}
		out << "|";
		if (row < 2) out << "\n";
	}
	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, glm::mat<2, 2, T> const& m)
{
	for (unsigned row = 0; row < 2; ++row)
	{
		out << "| ";
		for (unsigned col = 0; col < 2; ++col)
		{
			out << m[col][row];
			out << " ";
		}
		out << "|";
		if (row < 1) out << "\n";
	}
	return out;
}
