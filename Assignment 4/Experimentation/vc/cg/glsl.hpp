#pragma once

#include "glm.hpp"
#include "ogl.hpp"

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <filesystem>

namespace cg
{

	/*
	*	loads shader a shader source from the given filepath
	*	if this fails, the returned string will be empty & a message will be printed to std::cout
	*/
	std::string loadShaderSource(const std::filesystem::path& filepath);

	enum class ShaderType
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		GEOMETRY_SHADER,
		UNDEF
	};

	struct ShaderInfo
	{
		ShaderInfo(const ShaderType t, std::string const& src) : type(t), source(src) {}

		ShaderType		type = ShaderType::UNDEF;
		std::string		source = "";
	};

	class GlslProgram : public std::enable_shared_from_this < GlslProgram >
	{

	public:

		static std::shared_ptr< GlslProgram > create(const std::vector<ShaderInfo>& shaders, const bool printLog = false);

		/*
		*	makes the GlslProgram the active program
		*	meaning: it will be used for subsequent rendering operations
		*	nullptr -> no program
		*/
		static void useProgram(std::shared_ptr< GlslProgram > prog);

		/*
		*	various functions for setting uniforms
		*/
		void setUniformTexVal(const std::string& mName, const unsigned int v);
		void setUniformVal(const std::string& mName, const float v);
		void setUniformIVal(const std::string& mName, const int v);
		void setUniformUVal(const std::string& mName, const unsigned int v);
		void setUniformBVal(const std::string& mName, const bool v);
		void setUniformVec2(const std::string& mName, const glm::vec2& v);
		void setUniformVec3(const std::string& mName, const glm::vec3& v);
		void setUniformVec4(const std::string& mName, const glm::vec4& v);
		void setUniformIVec2(const std::string& mName, const glm::ivec2& v);
		void setUniformIVec3(const std::string& mName, const glm::ivec3& v);
		void setUniformIVec4(const std::string& mName, const glm::ivec4& v);
		void setUniformUVec2(const std::string& mName, const glm::uvec2& v);
		void setUniformUVec3(const std::string& mName, const glm::uvec3& v);
		void setUniformUVec4(const std::string& mName, const glm::uvec4& v);
		void setUniformVec2(const std::string& mName, const float x, const float y);
		void setUniformVec3(const std::string& mName, const float x, const float y, const float z);
		void setUniformVec4(const std::string& mName, const float x, const float y, const float z, const float w);
		void setUniformIVec2(const std::string& mName, const int x, const int y);
		void setUniformIVec3(const std::string& mName, const int x, const int y, const int z);
		void setUniformIVec4(const std::string& mName, const int x, const int y, const int z, const int w);
		void setUniformUVec2(const std::string& mName, const unsigned int x, const unsigned int y);
		void setUniformUVec3(const std::string& mName, const unsigned int x, const unsigned int y, const unsigned int z);
		void setUniformUVec4(const std::string& mName, const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int w);
		void setUniformMat2(const std::string& mName, const glm::mat2& mat);
		void setUniformMat3(const std::string& mName, const glm::mat3& mat);
		void setUniformMat4(const std::string& mName, const glm::mat4& mat);

		GLint getFragDataLocation(const std::string& mName) const;
		GLint getAttributeLocation(const std::string& mName) const;
		GLint getUniformLocation(const std::string& mName) const;

		/*
		*	does exactly what the name says
		*	'active' means: the uniform / attribute was not optimized away by the compiler
		*/
		void printActiveUniforms() const;
		void printActiveAttributes() const;

		std::string getProgramLog() const;

		/*
		*	can be used to determine if the GlslProgram can safely be used, given the current state
		*	of the opengl pipeline
		*/
		bool isValid() const;

		// no copies & stuff
		GlslProgram(GlslProgram const&) = delete;
		GlslProgram& operator=(GlslProgram const&) = delete;
		GlslProgram(GlslProgram&&) = delete;
		GlslProgram& operator=(GlslProgram&&) = delete;

	private:

		~GlslProgram() = default;
		GlslProgram() = default;

		bool addShader(const ShaderType shaderType, const std::string& shaderSource);
		bool link();

		struct Deleter
		{
			void operator()(GlslProgram*& p) const;
		};

		static std::weak_ptr< GlslProgram > sCurrentProgram;

		void queryActiveUniforms();
		void queryActiveAttributes();
		GLint checkUniformInternal(const std::string& mName, const GLenum type);
		GLint checkUniformInternal(const std::string& mName, const std::vector< GLenum >& type);
		bool getWarned(const std::string& mName) const;
		void setWarned(const std::string& mName);
		GLenum getUniformType(const std::string& mName) const;

		struct VariableInfo
		{
			GLint	location;
			GLenum	type;
			GLint	size;
		};
		typedef std::map< std::string, VariableInfo >	LookupTable;

		std::map< std::string, bool > mWarnings;

		GLuint					mHandle = 0;
		std::string				mProgramLog;
		LookupTable				mUniforms;
		LookupTable				mAttributes;
	};

}