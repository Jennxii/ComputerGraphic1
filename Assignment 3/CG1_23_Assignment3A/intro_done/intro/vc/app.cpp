#include "app.hpp"

#include "cg/glm.hpp"
#include "cg/glsl.hpp"

#include <iostream>

std::shared_ptr<App> App::create(const unsigned w, const unsigned h)
{
	std::shared_ptr<App> result(new App(w, h));
	result->init();
	return result;
}

App::App(const unsigned w, const unsigned h) :
	cg::CgApp(), mWindowWidth(w), mWindowHeight(h)
{
}

void App::init()
{
	struct VertexData
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uv;
	};

	// 5 or more vertices that make up the pyramid
	std::vector<VertexData> vertexData = {
		{ glm::vec3(0.f, +1.f,  0.f),	glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.125f, 1.f)},	// top front			0
		{ glm::vec3(0.f, +1.f,  0.f),	glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.375f, 1.f)},	// top right			1
		{ glm::vec3(0.f, +1.f,  0.f),	glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.625f, 1.f)},	// top back				2
		{ glm::vec3(0.f, +1.f,  0.f),	glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.875f, 1.f)},	// top left				3
		{ glm::vec3(-1.f, -1.f, +1.f),	glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f) },	// lower left front		4
		{ glm::vec3(+1.f, -1.f, +1.f),	glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.25f, 0.f) },	// lower right front	5
		{ glm::vec3(+1.f, -1.f, -1.f),	glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.5f, 0.f) },	// lower right back		6
		{ glm::vec3(-1.f, -1.f, -1.f),	glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.75f, 0.f) },	// lower left back		7
		{ glm::vec3(-1.f, -1.f, +1.f),	glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f) },	// lower left front 2	8

	};

	// 4 triangles -> 12 indices
	std::vector<unsigned> indexData = {
		4, 5, 0,	// front
		5, 6, 1,	// right
		6, 7, 2,	// back
		7, 8, 3		// left
	};

	// 1: vertex buffer object --> stores vertex attribute data

	glGenBuffers(1, &mVertexDataHandle);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexDataHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 2: index buffer object --> stores index list

	glGenBuffers(1, &mIndexDataHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexDataHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indexData.size(), indexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 3: load the shaders, compile and link into a glsl program
	
	mProgram = cg::GlslProgram::create({
		{ cg::ShaderType::VERTEX_SHADER, cg::loadShaderSource("..\\shaders\\shading.vert") },
		{ cg::ShaderType::FRAGMENT_SHADER, cg::loadShaderSource("..\\shaders\\shading.frag") }
	});

	// 4: vertex array object --> connects vertex buffer object, index buffer object and the shader

	glGenVertexArrays(1, &mVertexArrayHandle);
	glBindVertexArray(mVertexArrayHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexDataHandle);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexDataHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid const*)offsetof(VertexData, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid const*)offsetof(VertexData, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid const*)offsetof(VertexData, uv));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	mLastFrameTime = std::chrono::steady_clock::now();
	mTotalElapsedSeconds = 0.f;
}

void App::resize(const unsigned w, const unsigned h)
{
	mWindowWidth = w;
	mWindowHeight = h;
	mWindowAspectRatio = w / static_cast<float>(h);
}

void App::render()
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	float elapsedSeconds = std::chrono::duration<float>(now - mLastFrameTime).count();
	mTotalElapsedSeconds += elapsedSeconds;
	mLastFrameTime = now;

	glm::mat4 viewTf = glm::lookAt(glm::vec3(0.f, 0.f, 5.f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 projectionTf = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 20.f);
	glm::mat4 modelTf = glm::rotate(glm::mat4(1.f), mTotalElapsedSeconds, glm::vec3(0.f, 1.f, 0.f));

	// define viewport -> sets the viewport matrix
	glViewport(0, 0, mWindowWidth, mWindowHeight);

	// set clear color
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	// set clear depth (1 is default, so this line isn't really needed)
	glClearDepth(1.f);

	// clear color and depth buffer --> note that the entire buffer is cleared (not viewport-dependent)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// enable Z-testing
	glEnable(GL_DEPTH_TEST);

	// activate glsl program
	cg::GlslProgram::useProgram(mProgram);
	// set matrices
	mProgram->setUniformMat4("observer.world2eye", viewTf);
	mProgram->setUniformMat4("observer.eye2clip", projectionTf);
	mProgram->setUniformMat4("object2world", modelTf);

	// bind vertex array object for pyramid
	glBindVertexArray(mVertexArrayHandle);
	// draw
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
	
	// at the end of a frame, it's good style to unbind stuff, and also reset states
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(0);
	cg::GlslProgram::useProgram(nullptr);
}

void App::cleanup()
{
	glDeleteBuffers(1, &mVertexDataHandle);
	glDeleteBuffers(1, &mIndexDataHandle);
	glDeleteVertexArrays(1, &mVertexArrayHandle);
}