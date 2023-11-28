#include "display_window.hpp"
#include "raytracer.hpp"
#include "scene.hpp"

#include <GLM\common.hpp>

#include <iostream>

void DisplayWindow::Deleter::operator()(DisplayWindow*& obj) const
{
	if (obj == nullptr)
		return;

	glDeleteProgram(obj->mDisplayShader);
	glDeleteTextures(1, &obj->mDisplayTexture);
	glDeleteVertexArrays(1, &obj->mDummyMesh);

	delete obj;
	obj = nullptr;
}

std::shared_ptr< DisplayWindow > DisplayWindow::create(const unsigned w, const unsigned h)
{
	// allocate ogl resources

	glm::vec4 black(0.f);

	GLuint textureHandle = 0;
	glGenTextures(1, &textureHandle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &black[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glBindTexture(GL_TEXTURE_2D, 0);

	// even the simplest opengl program needs a vertex/fragment shader pair
	// later, we will load these shaders from text files (with syntax highlighting)
	// but as of now, we'll just hardcode this stuff

	std::string vertexShaderSource =
		"#version 450 core\n"
		"const vec3 positions[4] = vec3[4](vec3(-1.0, 1.0, 0.0), vec3(-1.0, -1.0, 0.0), vec3(1.0, 1.0, 0.0), vec3(1.0, -1.0, 0.0));\n"
		"out vec2 uv;\n"
		"void main() {\n"
		"uv = 0.5 * positions[gl_VertexID].xy + vec2(0.5);\n"
		"gl_Position = vec4(positions[gl_VertexID], 1.0); }\n";

	std::string fragmentShaderSource =
		"#version 450 core\n"
		"layout (location=0) out vec4 fragmentColor;\n"
		"in vec2 uv;\n"
		"uniform sampler2D inputTexture;\n"
		"void main() {\n"
		"ivec2 size = textureSize(inputTexture, 0);\n"
		"ivec2 coords = clamp(ivec2(gl_FragCoord.xy), ivec2(0), size - ivec2(1));\n"
		"fragmentColor = texelFetch(inputTexture, coords, 0);\n"
		"if (gl_FragCoord.x < 0 || gl_FragCoord.x >= size.x || gl_FragCoord.y < 0 || gl_FragCoord.y >= size.y) discard; }\n";

	GLuint programHandle = glCreateProgram();
	GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertexSource = vertexShaderSource.c_str();
	glShaderSource(vertexShaderHandle, 1, &vertexSource, nullptr);
	glCompileShader(vertexShaderHandle);

	const char* fragmentSource = fragmentShaderSource.c_str();
	glShaderSource(fragmentShaderHandle, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShaderHandle);

	GLint compileStatus;
	glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &compileStatus);

	GLint logSize = 0;
	glGetShaderiv(vertexShaderHandle, GL_INFO_LOG_LENGTH, &logSize);
	if (logSize > 0)
	{
		char* buffer = new char[logSize];
		memset(buffer, 0, logSize);
		glGetShaderInfoLog(vertexShaderHandle, logSize, NULL, buffer);
		std::cout << "vertex shader log:" << buffer << std::endl;
		delete[] buffer;
	}

	glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(fragmentShaderHandle, GL_INFO_LOG_LENGTH, &logSize);
	if (logSize > 0)
	{
		char* buffer = new char[logSize];
		memset(buffer, 0, logSize);
		glGetShaderInfoLog(fragmentShaderHandle, logSize, NULL, buffer);
		std::cout << "fragment shader log:" << buffer << std::endl;
		delete[] buffer;
	}

	glAttachShader(programHandle, vertexShaderHandle);
	glAttachShader(programHandle, fragmentShaderHandle);

	glLinkProgram(programHandle);

	GLint linkStatus;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);

	glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logSize);
	if (logSize > 0)
	{
		char* buffer = new char[logSize];
		memset(buffer, 0, logSize);
		glGetProgramInfoLog(programHandle, logSize, NULL, buffer);
		std::cout << "program log:" << buffer << std::endl;
		delete[] buffer;
	}

	GLuint meshHandle = 0;
	glGenVertexArrays(1, &meshHandle);

	// create app once all resources are allocated
	// -> guarantees that we have all the necessary stuff
	// DisplayWindow::Deleter makes sure that everything is cleaned up

	std::shared_ptr<DisplayWindow> result(new DisplayWindow(w, h), DisplayWindow::Deleter());
	result->mDummyMesh = meshHandle;
	result->mDisplayTexture = textureHandle;
	result->mDisplayShader = programHandle;
	result->mDisplayTextureWidth = w;
	result->mDisplayTextureHeight = h;
	return result;
}

DisplayWindow::DisplayWindow(const unsigned w, const unsigned h) :
	CgApp(), mWindowWidth(w), mWindowHeight(h)
{
}

void DisplayWindow::resize(const unsigned w, const unsigned h)
{
	mWindowWidth = w;
	mWindowHeight = h;
}

void DisplayWindow::render()
{
	// A: framebuffer, which will store the raytraced image
	// framebuffer size must fit the size of the opengl texture resource used for displaying the results
	// (texture resource is created with a fixed size, for performance reasons)
	Framebuffer framebuffer;
	framebuffer.width = mDisplayTextureWidth;
	framebuffer.height = mDisplayTextureHeight;
	framebuffer.pixels.resize(framebuffer.width * framebuffer.height);

	// B: camera
	Camera camera;
	camera.fov = glm::radians(45.f);
	camera.aspectRatio = framebuffer.width / (float)framebuffer.height;	// camera aspect ratio should fit fbo aspect ratio!
	camera.position = glm::vec3(0.f, 0.f, 5.f);
	camera.forward = glm::vec3(0.f, 0.f, -1.f);
	camera.up = glm::vec3(0.f, 1.f, 0.f);

	// C: scene - stored as interleaved triangle soup
	std::vector<VertexData> scene;
	makeDemoScene(scene);

	PointLight pointi;
	pointi.position = { 5,5,5 };
	pointi.color = { 1,1,1 };
	std::vector<PointLight> lights;
	lights.push_back(pointi);
	// TODO: render the scene as viewed from the camera into the framebuffer
	Raytracer::renderScene(scene, lights, camera, framebuffer);
	
	// finally, display the raytracing results
	display(framebuffer);
}

void DisplayWindow::display(Framebuffer const& framebuffer)
{
	// upload the rendered scene to an opengl texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDisplayTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, framebuffer.width, framebuffer.height, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.pixels.data());
	glBindTexture(GL_TEXTURE_2D, 0);

	// display opengl texture in window
	glViewport(0, 0, mWindowWidth, mWindowHeight);
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(mDisplayShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDisplayTexture);
	glBindVertexArray(mDummyMesh);
	glUniform1i(glGetUniformLocation(mDisplayShader, "inputTexture"), 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}