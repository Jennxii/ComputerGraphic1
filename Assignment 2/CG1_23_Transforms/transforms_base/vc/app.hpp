#pragma once

#include "..\..\cg\cg_app.hpp"

#include "..\..\cg\glm.hpp"

#include <memory>
#include <chrono>

class OglHelper;

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

private:

	// helper function for drawing a coordinate system - coords = model matrix
	void drawCoordinateSystem(glm::mat4 const& coords, const float length, const float thickness);

	// stores beginning of last frame
	std::chrono::steady_clock::time_point mLastFrameTime;
	// elapsed seconds since init was completed
	float mTotalElapsedSeconds = 0.f;

	std::shared_ptr<OglHelper> mScene = nullptr;

	unsigned mWindowWidth;
	unsigned mWindowHeight;
	float mWindowAspectRatio;
	glm::vec3 cameraMoveVector = { 0.f, 3.f, 5.f };
	float smokeSize1 = 0.1f;
	float smokeSize2 = 0.1f;
	float smokeSize3 = 0.1f;
	float smokeSize4 = 0.1f;

	float smokePosX1 = -0.55f;
	float smokePosX2 = -0.55f;
	float smokePosX3 = -0.55f;
	float smokePosX4 = -0.55f;

	float smokePosX1R = -0.35f;
	float smokePosX2R = -0.35f;
	float smokePosX3R = -0.35f;
	float smokePosX4R = -0.35f;

	float smokePosY1 = 0.5f;
	float smokePosY2 = 0.5f;
	float smokePosY3 = 0.5f;
	float smokePosY4 = 0.5f;

	float smokePosZ1 = 0.f;
	float smokePosZ2 = 0.f;
	float smokePosZ3 = 0.f;
	float smokePosZ4 = 0.f;

	float lifeTime1 = 4.f;
	float lifeTime2 = 4.f;
	float lifeTime3 = 4.f;
	float lifeTime4 = 4.f;

	float maxLifeTime = 4.f;

	struct SmokeParticle
	{
		float smokeSize;
		float smokePosY;
		float lifeTime1;
		float lifeTime2;
		float maxLifeTime;
		float smokePosX;
		float smokePosZ;
		bool active;  // Flag to indicate whether the particle is active or not
	};
	std::vector<SmokeParticle> smokeParticles;
	int maxParticles = 5;
};
