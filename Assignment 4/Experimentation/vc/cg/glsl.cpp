#include "glsl.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include "windows.h"

namespace cg
{
	std::string toGLSLType(const GLenum val)
	{
		switch (val)
		{
			case GL_FLOAT : return "float";
			case GL_FLOAT_VEC2 : return "vec2";
			case GL_FLOAT_VEC3 : return "vec3";
			case GL_FLOAT_VEC4: return "vec4";
			case GL_INT : return "int";
			case GL_INT_VEC2 : return "ivec2";
			case GL_INT_VEC3 : return "ivec3";
			case GL_INT_VEC4 : return "ivec4";
			case GL_UNSIGNED_INT : return "unsigned int";
			case GL_UNSIGNED_INT_VEC2 : return "uvec2";
			case GL_UNSIGNED_INT_VEC3 : return "uvec3";
			case GL_UNSIGNED_INT_VEC4 : return "uvec4";
			case GL_BOOL : return "bool";
			case GL_BOOL_VEC2 : return "bvec2";
			case GL_BOOL_VEC3 : return "bvec3";
			case GL_BOOL_VEC4 : return "bvec4";
			case GL_FLOAT_MAT2 : return "mat2";
			case GL_FLOAT_MAT3 : return "mat3";
			case GL_FLOAT_MAT4 : return "mat4";
			case GL_SAMPLER_1D : return "sampler1D";
			case GL_SAMPLER_2D : return "sampler2D";
			case GL_SAMPLER_3D : return "sampler3D";
			case GL_SAMPLER_CUBE : return "samplerCube";
			default: return "UNKNOWN / UNSUPPORTED TYPE";
		}
	}

	std::string loadShaderSource(const std::filesystem::path& filepath)
	{
		std::string source;

		std::ifstream file( filepath.c_str() );
		if (!file.is_open())
		{
			std::ostringstream msg;
			std::cout << "could not open shader source file: " << filepath << ", are you sure the file exists?" << std::endl;
			return source;
		}

		while (!file.eof())
		{
			std::string line;
			getline( file, line );
			source.append( line );
			source.append( "\n" );
		}

		file.close();

		return source;
	}

	struct TmpProgramBinding
	{
		TmpProgramBinding(std::shared_ptr< GlslProgram > t, std::shared_ptr< GlslProgram > o) : tmpProg(t), oldProg(o)
		{
			if (oldProg != tmpProg)
				GlslProgram::useProgram(tmpProg);
		}

		~TmpProgramBinding()
		{
			if (oldProg != tmpProg)
				GlslProgram::useProgram(oldProg);
		}

		std::shared_ptr< GlslProgram > tmpProg;
		std::shared_ptr< GlslProgram > oldProg;
	};

	std::weak_ptr< GlslProgram > GlslProgram::sCurrentProgram = std::weak_ptr< GlslProgram >();

	void GlslProgram::useProgram( std::shared_ptr< GlslProgram > p )
	{
		sCurrentProgram = p;
		if ( p.get() )	glUseProgram( p->mHandle );
		else			glUseProgram( 0 );
	}

	std::shared_ptr< GlslProgram > GlslProgram::create(const std::vector<ShaderInfo>& shaders, const bool printLog)
	{
		std::shared_ptr< GlslProgram > result;

		std::shared_ptr< GlslProgram > p( new GlslProgram, GlslProgram::Deleter() );
		p->mHandle = glCreateProgram();

		bool ok = true;
		for ( ShaderInfo const& s : shaders )
		{
			ok &= p->addShader( s.type, s.source );
		}
		ok &= p->link();

		if ( !ok )
		{
			std::cout << "--- program error log ---" << std::endl;
			std::cout << p->getProgramLog() << std::endl;
			std::string output = "One of your GLSL Programs could not be created, please check the console for an error log.";
			std::wstring stemp = std::wstring(output.begin(), output.end());
			LPCWSTR sw = stemp.c_str();
			int msgboxID = MessageBox(
				NULL,
				sw,
				(LPCWSTR)L"Yarr, your GLSL program could not be created",
				MB_ICONWARNING | MB_OK
			);
		}
		else
		{
			if ( printLog )
			{
				std::cout << "--- program attrib & uniform info ---" << std::endl;
				p->printActiveUniforms();
				p->printActiveAttributes();
			}
			result = p;
		}

		return result;
	}

	void GlslProgram::Deleter::operator()(GlslProgram *& p) const
	{
		if ( p == nullptr )
			return;

		glDeleteProgram( p->mHandle );

		delete p;
		p = nullptr;
	}

	bool GlslProgram::addShader(const ShaderType shaderType, const std::string& shaderSource)
	{
		GLenum type;
		switch ( shaderType )
		{
		case ShaderType::VERTEX_SHADER:
			type = GL_VERTEX_SHADER;
			break;
		case ShaderType::FRAGMENT_SHADER:
			type = GL_FRAGMENT_SHADER;
			break;
		case ShaderType::GEOMETRY_SHADER:
			type = GL_GEOMETRY_SHADER;
			break;
		default:
			std::cout << "unknown shader type, was ignored " << std::endl;
			return false;
		}

		GLuint handle = glCreateShader( type );

		const char* source = shaderSource.c_str();
		glShaderSource( handle, 1, &source, nullptr );
		glCompileShader( handle );

		GLint compileStatus;
		glGetShaderiv( handle, GL_COMPILE_STATUS, &compileStatus );

		GLint sz = 0;
		glGetShaderiv( handle, GL_INFO_LOG_LENGTH, &sz );
		if (sz > 0)
		{
			char *buffer = new char[ sz ];
			memset( buffer, 0, sz );
			glGetShaderInfoLog( handle, sz, NULL, buffer );
			mProgramLog.append( buffer );
			delete[] buffer;
		}

		if (!compileStatus)
			return false;

		glAttachShader( mHandle, handle );
		return true;
	}

	bool GlslProgram::link()
	{
		glLinkProgram( mHandle );

		GLint linkStatus;
		glGetProgramiv( mHandle, GL_LINK_STATUS, &linkStatus );

		GLint sz = 0;
		glGetProgramiv( mHandle, GL_INFO_LOG_LENGTH, &sz );
		if (sz > 0)
		{
			char *buffer = new char[ sz ];
			memset( buffer, 0, sz );
			glGetProgramInfoLog( mHandle, sz, NULL, buffer );
			mProgramLog.append( buffer );
			delete[] buffer;
		}

		if (!linkStatus)
			return false;

		queryActiveAttributes();
		queryActiveUniforms();
		return true;
	}

	bool GlslProgram::isValid() const
	{
		glValidateProgram( mHandle );
		GLint validateStatus;
		glGetProgramiv( mHandle, GL_VALIDATE_STATUS, &validateStatus );
		return (validateStatus ? true : false);
	}

	std::string GlslProgram::getProgramLog() const
	{
		return mProgramLog;
	}

	void GlslProgram::queryActiveUniforms()
	{
		GLint count, maxLength;
		glGetProgramiv( mHandle, GL_ACTIVE_UNIFORMS, &count );
		glGetProgramiv( mHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength );

		char* buffer = new char[ maxLength ];
		memset( buffer, 0, maxLength );

		GLenum type;
		GLint size, location;
		std::string mName;

		for (int i = 0; i < count; ++i)
		{
			glGetActiveUniform( mHandle, i, maxLength, nullptr, &size, &type, buffer );
			mName = buffer;

			location = glGetUniformLocation( mHandle, buffer );
			VariableInfo info;
			info.location = location;
			info.size = size;
			info.type = type;
			mUniforms[ mName ] = info;
		}

		delete[] buffer;
	}

	void GlslProgram::queryActiveAttributes()
	{
		GLint count, maxLength;
		glGetProgramiv( mHandle, GL_ACTIVE_ATTRIBUTES, &count );
		glGetProgramiv( mHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength );

		char* buffer = new char[ maxLength ];
		memset( buffer, 0, maxLength );

		GLint size, location;
		GLenum type;
		std::string mName;

		for (int i = 0; i < count; ++i)
		{
			glGetActiveAttrib( mHandle, i, maxLength, nullptr, &size, &type, buffer );
			mName = buffer;
			location = glGetAttribLocation( mHandle, buffer );
			VariableInfo info;
			info.location = location;
			info.size = size;
			info.type = type;
			mAttributes[ mName ] = info;
		}

		delete[] buffer;
	}

	void GlslProgram::printActiveUniforms() const
	{
		for (auto it = mUniforms.begin(); it != mUniforms.end(); ++it)
			std::cout << "active uniform\t\t" << it->first << " at ( uniform ) location " << it->second.location << " (" << toGLSLType(it->second.type) << ")" << std::endl;
	}

	void GlslProgram::printActiveAttributes() const
	{
		for (auto it = mAttributes.begin(); it != mAttributes.end(); ++it)
			std::cout << "active attribute\t" << it->first << " at ( attribute ) location " << it->second.location << " (" << toGLSLType(it->second.type) << ")" << std::endl;
	}

	void GlslProgram::setUniformTexVal(const std::string& mName, const unsigned int texUnit)
	{
		GLint location = checkUniformInternal(mName, { GL_SAMPLER_1D, GL_SAMPLER_2D, GL_SAMPLER_3D, GL_SAMPLER_CUBE } );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniform1i( location, texUnit );
	}

	void GlslProgram::setUniformVal(const std::string& mName, const float v)
	{
		GLint location = checkUniformInternal( mName, GL_FLOAT );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniform1f( location, v );
	}

	void GlslProgram::setUniformBVal(const std::string& mName, const bool v)
	{
		GLint location = checkUniformInternal(mName, GL_BOOL);
		TmpProgramBinding tmp(shared_from_this(), sCurrentProgram.lock());
		glUniform1i(location, v);
	}

	void GlslProgram::setUniformIVal(const std::string& mName, const int v )
	{
		GLint location = checkUniformInternal( mName, GL_INT );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniform1i( location, v );
	}

	void GlslProgram::setUniformUVal(const std::string& mName, const unsigned int v)
	{
		GLint location = checkUniformInternal( mName, GL_UNSIGNED_INT );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniform1ui( location, v );
	}

	void GlslProgram::setUniformVec2(const std::string& mName, const glm::vec2& v)
	{
		GLint location = checkUniformInternal(mName, GL_FLOAT_VEC2 );
		TmpProgramBinding tmp(shared_from_this(), sCurrentProgram.lock());
		glUniform2f(location, v.x, v.y);
	}

	void GlslProgram::setUniformVec3(const std::string& mName, const glm::vec3& v)
	{
		GLint location = checkUniformInternal(mName, GL_FLOAT_VEC3);
		TmpProgramBinding tmp(shared_from_this(), sCurrentProgram.lock());
		glUniform3f(location, v.x, v.y, v.z);
	}

	void GlslProgram::setUniformVec4(const std::string& mName, const glm::vec4& v)
	{
		GLint location = checkUniformInternal(mName, GL_FLOAT_VEC4);
		TmpProgramBinding tmp(shared_from_this(), sCurrentProgram.lock());
		glUniform4f(location, v.x, v.y, v.z, v.w);
	}

	void GlslProgram::setUniformVec2(const std::string& mName, const float x, const float y )
	{
		GLint location = checkUniformInternal( mName, GL_FLOAT_VEC2 );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock());
		glUniform2f( location, x, y );
	}

	void GlslProgram::setUniformVec3(const std::string& mName, const float x, const float y, const float z)
	{
		GLint location = checkUniformInternal( mName, GL_FLOAT_VEC3 );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock());
		glUniform3f( location, x, y, z );
	}

	void GlslProgram::setUniformVec4(const std::string& mName, const float x, const float y, const float z, const float w)
	{
		GLint location = checkUniformInternal( mName, GL_FLOAT_VEC4 );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock());
		glUniform4f( location, x, y, z, w );
	}

	void GlslProgram::setUniformIVec2(const std::string& mName, const glm::ivec2& v)
	{
		GLint location = checkUniformInternal(mName, GL_INT_VEC2 );
		TmpProgramBinding tmp(shared_from_this(), sCurrentProgram.lock());
		glUniform2i(location, v.x, v.y);
	}

	void GlslProgram::setUniformIVec3(const std::string& mName, const glm::ivec3& v)
	{
		GLint location = checkUniformInternal(mName, GL_INT_VEC3);
		TmpProgramBinding tmp(shared_from_this(), sCurrentProgram.lock());
		glUniform3i(location, v.x, v.y, v.z);
	}

	void GlslProgram::setUniformIVec4(const std::string& mName, const glm::ivec4& v)
	{
		GLint location = checkUniformInternal(mName, GL_INT_VEC4);
		TmpProgramBinding tmp(shared_from_this(), sCurrentProgram.lock());
		glUniform4i(location, v.x, v.y, v.z, v.w);
	}

	void GlslProgram::setUniformIVec2(const std::string& mName, const int x, const int y)
	{
		GLint location = checkUniformInternal( mName, GL_INT_VEC2 );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniform2i( location, x, y );
	}

	void GlslProgram::setUniformIVec3(const std::string& mName, const int x, const int y, const int z)
	{
		GLint location = checkUniformInternal( mName, GL_INT_VEC3 );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniform3i( location, x, y, z );
	}

	void GlslProgram::setUniformIVec4(const std::string& mName, const int x, const int y, const int z, const int w)
	{
		GLint location = checkUniformInternal( mName, GL_INT_VEC4 );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniform4i( location, x, y, z, w );
	}

	void GlslProgram::setUniformUVec2(const std::string& mName, const glm::uvec2& v)
	{
		GLint location = checkUniformInternal(mName, GL_UNSIGNED_INT_VEC2 );
		TmpProgramBinding tmp(shared_from_this(), sCurrentProgram.lock());
		glUniform2ui(location, v.x, v.y);
	}

	void GlslProgram::setUniformUVec3(const std::string& mName, const glm::uvec3& v)
	{
		GLint location = checkUniformInternal(mName, GL_UNSIGNED_INT_VEC3);
		TmpProgramBinding tmp(shared_from_this(), sCurrentProgram.lock());
		glUniform3ui(location, v.x, v.y, v.z);
	}

	void GlslProgram::setUniformUVec4(const std::string& mName, const glm::uvec4& v)
	{
		GLint location = checkUniformInternal(mName, GL_UNSIGNED_INT_VEC4);
		TmpProgramBinding tmp(shared_from_this(), sCurrentProgram.lock());
		glUniform4ui(location, v.x, v.y, v.z, v.w);
	}

	void GlslProgram::setUniformUVec2(const std::string& mName, const unsigned int x, const unsigned int y)
	{
		GLint location = checkUniformInternal( mName, GL_UNSIGNED_INT_VEC2);
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniform2ui( location, x, y );
	}

	void GlslProgram::setUniformUVec3(const std::string& mName, const unsigned int x, const unsigned int y, const unsigned int z)
	{
		GLint location = checkUniformInternal( mName, GL_UNSIGNED_INT_VEC3);
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniform3ui( location, x, y, z );
	}

	void GlslProgram::setUniformUVec4(const std::string& mName, const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int w)
	{
		GLint location = checkUniformInternal( mName, GL_UNSIGNED_INT_VEC4);
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniform4ui( location, x, y, z, w );
	}

	void GlslProgram::setUniformMat2(const std::string& mName, const glm::mat2& mat)
	{
		GLint location = checkUniformInternal( mName, GL_FLOAT_MAT2 );
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniformMatrix2fv( location, 1, false, std::addressof( mat[ 0 ][ 0 ] ) );
	}

	void GlslProgram::setUniformMat3(const std::string& mName, const glm::mat3& mat)
	{
		GLint location = checkUniformInternal( mName, GL_FLOAT_MAT3);
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniformMatrix3fv( location, 1, false, std::addressof( mat[ 0 ][ 0 ] ) );
	}

	void GlslProgram::setUniformMat4(const std::string& mName, const glm::mat4& mat)
	{
		GLint location = checkUniformInternal( mName , GL_FLOAT_MAT4);
		TmpProgramBinding tmp( shared_from_this(), sCurrentProgram.lock() );
		glUniformMatrix4fv( location, 1, false,  std::addressof( mat[ 0 ][ 0 ] ) );
	}

	GLint GlslProgram::checkUniformInternal(const std::string &mName, const GLenum type)
	{
		GLint location = getUniformLocation( mName );
		GLenum queriedType = getUniformType( mName );
		bool wasWarned = getWarned(mName);
		if (location == -1 && !wasWarned)
		{
			setWarned(mName);
			std::string output = "Uniform " + mName + " was not found...\nDoublecheck the spelling!\n(Spelled correctly but UNUSED by the shader will also trigger this message)";
			std::wstring stemp = std::wstring(output.begin(), output.end());
			LPCWSTR sw = stemp.c_str();
			int msgboxID = MessageBox(
				NULL,
				sw,
				(LPCWSTR)L"Yarr, you missspelled a uniform",
				MB_ICONWARNING | MB_OK
			);
		}
		else if (type != queriedType && !wasWarned)
		{
			setWarned(mName);
			std::string output = "Uniform " + mName + " has wrong datatype: " + toGLSLType(queriedType) + "...\n(You attempted to set " + toGLSLType(type) + ")";
			std::wstring stemp = std::wstring(output.begin(), output.end());
			LPCWSTR sw = stemp.c_str();
			int msgboxID = MessageBox(
				NULL,
				sw,
				(LPCWSTR)L"Yarr, you misstyped a uniform",
				MB_ICONWARNING | MB_OK
			);
		}
		return location;
	}

	GLint GlslProgram::checkUniformInternal(const std::string& mName, const std::vector< GLenum >& types)
	{
		GLint location = getUniformLocation(mName);
		GLenum queriedType = getUniformType(mName);
		bool containsType = false;
		for (auto t : types)
		{
			containsType |= (t == queriedType);
		}
		bool wasWarned = getWarned(mName);
		if (location == -1 && !wasWarned)
		{
			setWarned(mName);
			std::string output = "Uniform " + mName + " was not found...\nDoublecheck the spelling!\n(Spelled correctly but UNUSED by the shader will also trigger this message)";
			std::wstring stemp = std::wstring(output.begin(), output.end());
			LPCWSTR sw = stemp.c_str();
			int msgboxID = MessageBox(
				NULL,
				sw,
				(LPCWSTR)L"Yarr, you missspelled a uniform",
				MB_ICONWARNING | MB_OK
			);
		}
		else if (!containsType && !wasWarned)
		{
			setWarned(mName);
			std::string output = "Uniform " + mName + " has wrong datatype: " + toGLSLType(queriedType) + "...\n";
			std::wstring stemp = std::wstring(output.begin(), output.end());
			LPCWSTR sw = stemp.c_str();
			int msgboxID = MessageBox(
				NULL,
				sw,
				(LPCWSTR)L"Yarr, you misstyped a uniform",
				MB_ICONWARNING | MB_OK
			);
		}
		return location;
	}

	GLint GlslProgram::getUniformLocation(const std::string& mName) const
	{
		GLint location = -1;
		auto it = mUniforms.find( mName );
		if ( it != mUniforms.end() )
			location = it->second.location;

		return location;
	}


	GLenum GlslProgram::getUniformType(const std::string& mName) const
	{
		GLenum type = -1;
		auto it = mUniforms.find(mName);
		if (it != mUniforms.end())
			type = it->second.type;

		return type;
	}


	bool GlslProgram::getWarned(const std::string& mName) const
	{
		bool res = false;
		auto it = mWarnings.find(mName);
		if (it != mWarnings.end())
			res = it->second;

		return res;
	}

	void GlslProgram::setWarned(const std::string& mName)
	{
		mWarnings[mName] = true;
	}

	GLint GlslProgram::getFragDataLocation(const std::string& mName) const
	{
		return glGetFragDataLocation( mHandle, mName.c_str() );
	}

	GLint GlslProgram::getAttributeLocation(const std::string& mName) const
	{
		GLint location = -1;
		auto it = mAttributes.find( mName );
		if ( it != mAttributes.end() )
			location = it->second.location;

		return location;
	}
}