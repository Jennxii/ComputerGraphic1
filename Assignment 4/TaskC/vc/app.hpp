#pragma once

#include "cg\cg_app.hpp"
#include "cg\glm.hpp"
#include "cg\ogl.hpp"


#include "smoke_particle.hpp"
#include "camera.hpp"

#include <memory>
#include <chrono>

// fwd declarations
namespace cg
{
	class GlslProgram;
	struct TriangleMesh;
}

class App : public cg::CgApp
{

public:

	static std::shared_ptr<App> create(const unsigned w, const unsigned h);

	App(const unsigned w, const unsigned h);

	void init();
	void render();
	void cleanup();
	void resize(const unsigned w, const unsigned h);
	void keyInput(const int key, const int scancode, const int action, const int mods);
	void mouseButton(const int button, const int action, const int mods);
	void mousePosition(const double xpos, const double ypos);

private:

	std::chrono::steady_clock::time_point mLastFrameTime;
	float mTotalElapsedSeconds = 0.f;

	unsigned mWindowWidth;
	unsigned mWindowHeight;
	float mWindowAspectRatio;

	std::shared_ptr<cg::GlslProgram> mProgram;
	std::shared_ptr<cg::GlslProgram> mSpotLight;
	std::shared_ptr<cg::GlslProgram> mTexture;

	GLuint mTextureHandleA = 0;
	GLuint mTextureHandleB = 0;
	GLuint mTextureHandleC = 0;


	void drawShaded(std::shared_ptr<cg::TriangleMesh> mesh,
		glm::mat4 const& modelMatrix,
		glm::vec3 const& color);

	void drawTexture(std::shared_ptr<cg::TriangleMesh> mesh,
		glm::mat4 const& modelMatrix,
		glm::vec3 const& color);

	// smoke particle system
	unsigned mNumParticles;
	std::vector<SmokeParticle> mParticles;

	bool mStopTime = false;
	glm::uvec2 mMousePosition;
	Camera mCamera;

	// the meshes... formerly part of the oglhelper class; one of each geometry
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
