#pragma once

#include "cg\cg_app.hpp"

#include "cg\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <chrono>

class OglHelper;

struct Particle
{
	glm::vec3 position;
	float size;
	glm::vec3 color;
	glm::vec3 originalColor;
	bool isSphereSelected = false;

};

struct Ray {
	glm::vec3 origin;      // The starting point of the ray
	glm::vec3 direction;   // The direction in which the ray extends

	// Constructor to initialize the ray with an origin and direction
	Ray(const glm::vec3& _origin, const glm::vec3& _direction)
		: origin(_origin), direction(_direction) {}
};


class App : public cg::CgApp
{

public:

	static std::shared_ptr<App> create(const unsigned w, const unsigned h);

	App(const unsigned w, const unsigned h);

	void init();
	void render();
	void cleanup();
	void resize(const unsigned w, const unsigned h);

	void mousePosition(const double xpos, const double ypos);
	void mouseButton(const int button, const int action, const int mods);
	//glm::vec2  worldToScreen(const glm::vec3& worldCoordinates, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
	glm::vec2 worldToScreen(glm::vec3 worldCoords);
	//glm::vec3 screenToWorld(const glm::vec2& screenCoordinates, float depth, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
	glm::vec3 screenToWorld(glm::vec2 screenCoords);
	//bool intersects(const Ray& ray, const Particle& particle);
	void dragSphere(Particle& particle);
	glm::mat4 createViewportMatrix(const glm::vec2& viewSize, const glm::vec2& viewOffset);

	bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1);
	bool intersects(const Ray& ray, const glm::vec3& center, float& radius);


private:
	Particle* selectedParticle = nullptr;
	std::vector<Particle*> intersectedParticles;

	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;
	glm::mat4 mViewportMatrix;
	glm::vec3 mouseCoordinates;
	glm::vec3 particleOrigColor;
	glm::vec2 mCurrentMouse;
	glm::vec2 viewSize = glm::vec2(mWindowWidth, mWindowHeight);
	glm::vec2 viewOffset = glm::vec2(0.0f);

	std::vector<Particle> mParticles;

	// stores beginning of last frame
	std::chrono::steady_clock::time_point mLastFrameTime;
	// elapsed seconds since init was completed
	float mTotalElapsedSeconds = 0.f;

	std::shared_ptr<OglHelper> mScene = nullptr;

	unsigned mWindowWidth;
	unsigned mWindowHeight;
	float mWindowAspectRatio;
	bool mousePressed = false;
	bool beforeColorFrame = true;
};
