#pragma once

#include "..\..\cg\glsl.hpp"

#include <chrono>

namespace cg
{
	class GlslProgram;
	struct TriangleMesh;
}

class OglHelper
{

public:

	static std::shared_ptr<OglHelper> create();

	void setViewMatrix(glm::mat4 const& view);

	void startCurrentFrame(const unsigned w, const unsigned h);
	void endCurrentFrame();

	void drawSphere(glm::mat4 const& model, glm::vec3 const& color, const bool usePattern = true, glm::vec2 const& patternSize = glm::vec2(10.f));
	void drawCylinder(glm::mat4 const& model, glm::vec3 const& color, const bool usePattern = true, glm::vec2 const& patternSize = glm::vec2(10.f));
	void drawCube(glm::mat4 const& model, glm::vec3 const& color, const bool usePattern = true, glm::vec2 const& patternSize = glm::vec2(10.f));
	void drawRing(glm::mat4 const& model, glm::vec3 const& color, const bool usePattern = true, glm::vec2 const& patternSize = glm::vec2(10.f));
	void drawDisk(glm::mat4 const& model, glm::vec3 const& color, const bool usePattern = true, glm::vec2 const& patternSize = glm::vec2(10.f));
	void drawCone(glm::mat4 const& model, glm::vec3 const& color, const bool usePattern = true, glm::vec2 const& patternSize = glm::vec2(10.f));
	void drawPyramid(glm::mat4 const& model, glm::vec3 const& color, const bool usePattern = true, glm::vec2 const& patternSize = glm::vec2(10.f));
	void drawPlane(glm::mat4 const& model, glm::vec3 const& color, const bool usePattern = true, glm::vec2 const& patternSize = glm::vec2(10.f));
	void drawQuad(glm::mat4 const& model, glm::vec3 const& color, const bool usePattern = true, glm::vec2 const& patternSize = glm::vec2(10.f));

private:

	void init();

	std::shared_ptr<cg::GlslProgram> mShadingProgram;

	glm::vec3 mAmbientColor;
	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;

	std::shared_ptr<cg::TriangleMesh> mCube;
	std::shared_ptr<cg::TriangleMesh> mSphere;
	std::shared_ptr<cg::TriangleMesh> mCylinder;
	std::shared_ptr<cg::TriangleMesh> mDisk;
	std::shared_ptr<cg::TriangleMesh> mRing;
	std::shared_ptr<cg::TriangleMesh> mPyramid;
	std::shared_ptr<cg::TriangleMesh> mCone;
	std::shared_ptr<cg::TriangleMesh> mPlane;
	std::shared_ptr<cg::TriangleMesh> mQuad;

};
