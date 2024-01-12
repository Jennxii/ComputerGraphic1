#pragma once

#include "ogl.hpp"

#include <iostream>

namespace cg
{
	class OglDebugOutput
	{

	public:

		static void _stdcall debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* msg, void const* data)
		{
			// remove some notifications, to keep the console clean
			if (severity == GL_DEBUG_SEVERITY_NOTIFICATION || severity == GL_DEBUG_SEVERITY_LOW)
				return;

			std::cout << "\n------------OGL DEBUG------------" << std::endl;
			std::cout << "source:     " << parseSource(source) << std::endl;
			std::cout << "type:       " << parseType(type) << std::endl;
			std::cout << "severity:   " << parseSeverity(severity) << std::endl;
			std::cout << "debug call: " << msg << std::endl;
			std::cout << "\n---------------------------------" << std::endl;
		}

	private:

		static std::string parseType(GLenum type)
		{
			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:
				return "error";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				return "deprecated behavior";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				return "undefined behavior";
			case GL_DEBUG_TYPE_PORTABILITY:
				return "portability issue";
			case GL_DEBUG_TYPE_PERFORMANCE:
				return "performance issue";
			case GL_DEBUG_TYPE_MARKER:
				return "stream annotation";
			case GL_DEBUG_TYPE_OTHER:
				return "other";
			default:
				return "";
			}
		}

		static std::string parseSource(GLenum source)
		{
			switch (source)
			{
			case GL_DEBUG_SOURCE_API:
				return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				return "window system";
			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				return "shader compiler";
			case GL_DEBUG_SOURCE_THIRD_PARTY:
				return "third party";
			case GL_DEBUG_SOURCE_APPLICATION:
				return "application";
			case GL_DEBUG_SOURCE_OTHER:
				return "other";
			default:
				return "";
			}
		}

		static std::string parseSeverity(GLenum severity)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				return "high";
			case GL_DEBUG_SEVERITY_MEDIUM:
				return "medium";
			case GL_DEBUG_SEVERITY_LOW:
				return "low";
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				return "notification";
			default:
				return("");
			}
		}
	};
}