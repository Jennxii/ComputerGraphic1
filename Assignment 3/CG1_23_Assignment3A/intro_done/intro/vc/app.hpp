#pragma once

#include "cg/ogl.hpp"
#include "cg/cg_app.hpp"

#include <memory>
#include <chrono>

namespace cg
{
	class GlslProgram;
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

private:

	// stores beginning of last frame
	std::chrono::steady_clock::time_point mLastFrameTime;
	// elapsed seconds since init was completed
	float mTotalElapsedSeconds = 0.f;

	unsigned mWindowWidth;
	unsigned mWindowHeight;
	float mWindowAspectRatio;

	GLuint mVertexDataHandle = 0;
	GLuint mIndexDataHandle = 0;
	GLuint mVertexArrayHandle = 0;
	
	std::shared_ptr<cg::GlslProgram> mProgram = nullptr;
};