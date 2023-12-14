#include "oglhelper.hpp"

#include "..\..\cg\glm.hpp"
#include "..\..\cg\glsl.hpp"
#include "..\..\cg\triangle_mesh.hpp"
#include "..\..\cg\primitives.hpp"

#include <iostream>

std::shared_ptr< OglHelper > OglHelper::create()
{
	std::shared_ptr< OglHelper > result(new OglHelper);
	result->init();
	return result;
}

void OglHelper::init()
{
	mShadingProgram = cg::GlslProgram::create({
		{ cg::ShaderType::VERTEX_SHADER, cg::loadShaderSource("..\\shaders\\shading.vert") },
		{ cg::ShaderType::FRAGMENT_SHADER, cg::loadShaderSource("..\\shaders\\shading.frag") }
		});

	mProjectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.f);
	mViewMatrix = glm::mat4(1.f);
	mAmbientColor = glm::vec3(0.8f, 0.8f, 1.f);

	cg::BaseMeshData tmp;
	cg::BasePrimitives::generateSphere(16, 8, tmp);
	mSphere = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateCylinder(30, 2, tmp);
	mCylinder = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateCube(tmp);
	mCube = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateDisk(30, 2, tmp);
	mDisk = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateRing(30, 1.0f, 0.8f, tmp);
	mRing = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateCone(30, 2, tmp);
	mCone = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateQuad(tmp);
	mQuad = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generatePyramid(tmp);
	mPyramid = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generatePlane(100, 100, tmp);
	mPlane = cg::buildGPUMesh(tmp);
}

void OglHelper::setViewMatrix(glm::mat4 const& view)
{
	mViewMatrix = view;
}

void OglHelper::startCurrentFrame(const unsigned int w, const unsigned int h)
{
	glClearDepth(1.f);
	glClearColor(mAmbientColor.x, mAmbientColor.y, mAmbientColor.z, 1.f);
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);	// face culling = disabled, since not every primitive has backfaces
	//glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	cg::GlslProgram::useProgram(mShadingProgram);

	glm::vec4 cameraPosition = glm::inverse(mViewMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	mShadingProgram->setUniformMat4("observer.world2eye", mViewMatrix);
	mShadingProgram->setUniformMat4("observer.eye2clip", mProjectionMatrix);
}

void OglHelper::drawCube(glm::mat4 const& modelMatrix, glm::vec3 const& color, const bool usePattern, glm::vec2 const& patternSize)
{
	mShadingProgram->setUniformMat4("object2world", modelMatrix);
	mShadingProgram->setUniformVec3("diffuseColor", color);
	mShadingProgram->setUniformBVal("usePattern", usePattern);
	mShadingProgram->setUniformVec2("patternSize", patternSize);

	glBindVertexArray(mCube->resourceHandle);
	glDrawElements(GL_TRIANGLES, mCube->indexCount, GL_UNSIGNED_INT, nullptr);
}

void OglHelper::drawCylinder(glm::mat4 const& modelMatrix, glm::vec3 const& color, const bool usePattern, glm::vec2 const& patternSize)
{
	mShadingProgram->setUniformMat4("object2world", modelMatrix);
	mShadingProgram->setUniformVec3("diffuseColor", color);
	mShadingProgram->setUniformBVal("usePattern", usePattern);
	mShadingProgram->setUniformVec2("patternSize", patternSize);

	glBindVertexArray(mCylinder->resourceHandle);
	glDrawElements(GL_TRIANGLES, mCylinder->indexCount, GL_UNSIGNED_INT, nullptr);
}

void OglHelper::drawSphere(glm::mat4 const& modelMatrix, glm::vec3 const& color, const bool usePattern, glm::vec2 const& patternSize)
{
	mShadingProgram->setUniformMat4("object2world", modelMatrix);
	mShadingProgram->setUniformVec3("diffuseColor", color);
	mShadingProgram->setUniformBVal("usePattern", usePattern);
	mShadingProgram->setUniformVec2("patternSize", patternSize);

	glBindVertexArray(mSphere->resourceHandle);
	glDrawElements(GL_TRIANGLES, mSphere->indexCount, GL_UNSIGNED_INT, nullptr);
}

void OglHelper::drawCone(glm::mat4 const& modelMatrix, glm::vec3 const& color, const bool usePattern, glm::vec2 const& patternSize)
{
	mShadingProgram->setUniformMat4("object2world", modelMatrix);
	mShadingProgram->setUniformVec3("diffuseColor", color);
	mShadingProgram->setUniformBVal("usePattern", usePattern);
	mShadingProgram->setUniformVec2("patternSize", patternSize);

	glBindVertexArray(mCone->resourceHandle);
	glDrawElements(GL_TRIANGLES, mCone->indexCount, GL_UNSIGNED_INT, nullptr);
}

void OglHelper::drawPyramid(glm::mat4 const& modelMatrix, glm::vec3 const& color, const bool usePattern, glm::vec2 const& patternSize)
{
	mShadingProgram->setUniformMat4("object2world", modelMatrix);
	mShadingProgram->setUniformVec3("diffuseColor", color);
	mShadingProgram->setUniformBVal("usePattern", usePattern);
	mShadingProgram->setUniformVec2("patternSize", patternSize);

	glBindVertexArray(mPyramid->resourceHandle);
	glDrawElements(GL_TRIANGLES, mPyramid->indexCount, GL_UNSIGNED_INT, nullptr);
}

void OglHelper::drawRing(glm::mat4 const& modelMatrix, glm::vec3 const& color, const bool usePattern, glm::vec2 const& patternSize)
{
	mShadingProgram->setUniformMat4("object2world", modelMatrix);
	mShadingProgram->setUniformVec3("diffuseColor", color);
	mShadingProgram->setUniformBVal("usePattern", usePattern);
	mShadingProgram->setUniformVec2("patternSize", patternSize);

	glBindVertexArray(mRing->resourceHandle);
	glDrawElements(GL_TRIANGLES, mRing->indexCount, GL_UNSIGNED_INT, nullptr);
}

void OglHelper::drawQuad(glm::mat4 const& modelMatrix, glm::vec3 const& color, const bool usePattern, glm::vec2 const& patternSize)
{
	mShadingProgram->setUniformMat4("object2world", modelMatrix);
	mShadingProgram->setUniformVec3("diffuseColor", color);
	mShadingProgram->setUniformBVal("usePattern", usePattern);
	mShadingProgram->setUniformVec2("patternSize", patternSize);

	glBindVertexArray(mQuad->resourceHandle);
	glDrawElements(GL_TRIANGLES, mQuad->indexCount, GL_UNSIGNED_INT, nullptr);
}

void OglHelper::drawPlane(glm::mat4 const& modelMatrix, glm::vec3 const& color, const bool usePattern, glm::vec2 const& patternSize)
{
	mShadingProgram->setUniformMat4("object2world", modelMatrix);
	mShadingProgram->setUniformVec3("diffuseColor", color);
	mShadingProgram->setUniformBVal("usePattern", usePattern);
	mShadingProgram->setUniformVec2("patternSize", patternSize);

	glBindVertexArray(mPlane->resourceHandle);
	glDrawElements(GL_TRIANGLES, mPlane->indexCount, GL_UNSIGNED_INT, nullptr);
}

void OglHelper::drawDisk(glm::mat4 const& modelMatrix, glm::vec3 const& color, const bool usePattern, glm::vec2 const& patternSize)
{
	mShadingProgram->setUniformMat4("object2world", modelMatrix);
	mShadingProgram->setUniformVec3("diffuseColor", color);
	mShadingProgram->setUniformBVal("usePattern", usePattern);
	mShadingProgram->setUniformVec2("patternSize", patternSize);

	glBindVertexArray(mDisk->resourceHandle);
	glDrawElements(GL_TRIANGLES, mDisk->indexCount, GL_UNSIGNED_INT, nullptr);
}

void OglHelper::endCurrentFrame()
{
	glBindVertexArray(0);
	cg::GlslProgram::useProgram(nullptr);
	
	glDisable(GL_MULTISAMPLE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}