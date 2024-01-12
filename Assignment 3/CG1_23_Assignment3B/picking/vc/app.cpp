#include "app.hpp"
#include "oglhelper.hpp"

#include <GLFW\glfw3.h>

#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>


App::App(const unsigned w, const unsigned h) : CgApp(),
	mWindowWidth(w),
	mWindowHeight(h),
	mWindowAspectRatio(w / static_cast<float>(h))
{
}

std::shared_ptr<App> App::create(const unsigned w, const unsigned h)
{
	std::shared_ptr<App> result(new App(w, h));
	result->init();
	return result;
}

void App::init()
{
	mScene = OglHelper::create();

	// storing view & projection matrices in member variables, since you'll need them
	// you're also going to need the viewport matrix, so I suggest creating it here

	mViewMatrix = glm::lookAt(
		glm::vec3(10.f, 10.f, 20.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f));
	mScene->setViewMatrix(mViewMatrix);

	mProjectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.f);
	mScene->setProjectionMatrix(mProjectionMatrix);

	mViewportMatrix = glm::mat4(
		glm::vec4(mWindowWidth / 2, 0, 0, 0),
		glm::vec4(0, mWindowHeight / 2, 0, 0),
		glm::vec4(0, 0, 0.5f, 0),
		glm::vec4(mWindowWidth / 2, mWindowHeight / 2, 0.5f, 1));
	// generate random spheres

	std::random_device randomDevice;
	std::mt19937 mt(randomDevice());
	std::uniform_real_distribution<float> posDistribution(-5.f, +5.f);
	std::uniform_real_distribution<float> sizeDistribution(0.25f, 0.75f);
	std::uniform_real_distribution<float> hueDistribution(0.f, 1.f);

	mParticles.reserve(50);
	for (unsigned i = 0; i < 50; ++i)
	{
		float x = posDistribution(mt);
		float y = posDistribution(mt);
		float z = posDistribution(mt);

		float s = sizeDistribution(mt);

		// http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
		float hue = hueDistribution(mt);
		glm::vec3 p = glm::abs(glm::fract(glm::vec3(hue, hue, hue) + glm::vec3(1, 2 / 3.f, 1 / 3.f)) * 6.f - glm::vec3(3));
		glm::vec3 color = glm::mix(glm::vec3(1), glm::clamp(p - glm::vec3(1), 0.f, 1.f), 0.5f);

		mParticles.push_back( { glm::vec3(x, y, z), s, color });

	}
	mLastFrameTime = std::chrono::high_resolution_clock::now();
}

void App::render()
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	float elapsedSeconds = std::chrono::duration<float>(now - mLastFrameTime).count();
	mTotalElapsedSeconds += elapsedSeconds;
	mLastFrameTime = now;

	mScene->startCurrentFrame(mWindowWidth, mWindowHeight);

	glm::vec4 cameraPosition = glm::inverse(mViewMatrix) * glm::vec4(0.f, 0.f, 0.f, 1.f); // Adjust the camera position as needed
	glm::vec3 camera = glm::vec3(cameraPosition) / cameraPosition.w;

	glm::vec3 rayDirection = glm::normalize(mouseCoordinates - camera);
	Ray ray(camera, rayDirection);
	/*std::cout << "Raydirection " << rayDirection << std::endl;
	std::cout << "RayOrigin " << rayOrigin << std::endl;*/

	for (auto& particle : mParticles)
	{
		mScene->drawSphere(glm::translate(glm::mat4(1.f), particle.position)
			* glm::scale(glm::mat4(1.f), glm::vec3(particle.size)), particle.color);

		if (!selectedParticle && intersects(ray, particle.position, particle.size) && mousePressed)
		{
			intersectedParticles.push_back(&particle);
		}
	}

	if (!selectedParticle && !intersectedParticles.empty())
	{
		Particle* temp = intersectedParticles[0];

		for (auto* const intersectedParticle : intersectedParticles)
		{
			if (intersectedParticle->position.z > temp->position.z)
			{
				temp = intersectedParticle;
			}
		}
		selectedParticle = temp;
	}

	if (selectedParticle)
	{
		if (beforeColorFrame)
		{
			particleOrigColor = selectedParticle->color;
		}

		selectedParticle->color = glm::vec3(1.f, 0.f, 0.f);
		dragSphere(*selectedParticle);
		beforeColorFrame = false;
	}

	if (selectedParticle && !mousePressed)

	{
		selectedParticle->color = particleOrigColor;
		selectedParticle = nullptr;
		beforeColorFrame = true;
	}
	
	mScene->endCurrentFrame();
}

void App::cleanup()
{
}

void App::resize(const unsigned w, const unsigned h)
{
	mWindowWidth = w;
	mWindowHeight = h;
	mWindowAspectRatio = mWindowWidth / static_cast<float>(h);
}

void App::mousePosition(const double xpos, const double ypos)
{
	mCurrentMouse = glm::vec2(xpos, mWindowHeight - ypos);
	std::cout << "mouse moved" << std::endl;
	std::cout << "at location " << mCurrentMouse << std::endl;

}

void App::mouseButton(const int button, const int action, const int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mouseCoordinates = screenToWorld(mCurrentMouse);

		std::cout << "left mouse pressed" << std::endl;
		std::cout << "at location " << mCurrentMouse << std::endl;
		mousePressed = true;
		std::cout << "mousePressed "<< mousePressed << std::endl;

	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mousePressed = false;

		std::cout << "left mouse released" << mousePressed << std::endl;
		std::cout << "at location " << mCurrentMouse << std::endl;
	}
}


glm::vec2 App::worldToScreen(glm::vec3 worldCoords)
{
	glm::mat4 mvpMatrix = mViewportMatrix * mProjectionMatrix * mViewMatrix;

	// Transform the world coordinates to clip space
	glm::vec4 pixelCoordinates = mvpMatrix * glm::vec4(worldCoords, 1.0);
	pixelCoordinates /= pixelCoordinates.w;
	// Normalize to obtain normalized device coordinates (NDC)
	//glm::vec3 ndcSpacePos = glm::vec3(clipSpacePos) / clipSpacePos.w;

	//// Convert to screen coordinates
	//glm::vec2 screenCoordinates = ((glm::vec2(ndcSpacePos.x, ndcSpacePos.y) + 1.0f) / 2.0f) * viewSize + viewOffset;

	return pixelCoordinates;
}

glm::vec3 App::screenToWorld(glm::vec2 screenCoords)
{
	// Combine the transformations (view, projection, viewport)
	//glm::mat4 invMvpMatrix = glm::inverse(viewportMatrix * projectionMatrix * viewMatrix);
	glm::mat4 inverseViewportMatrix = glm::inverse(mViewportMatrix);
	glm::mat4 inverseProjectionMatrix = glm::inverse(mProjectionMatrix);
	glm::mat4 inverseViewMatrix = glm::inverse(mViewMatrix);

	//// Normalize the screen coordinates
	//glm::vec4 normalizedCoordinates;
	//normalizedCoordinates.x = 2.0f * (screenCoordinates.x - viewOffset.x) / viewSize.x - 1.0f;
	//normalizedCoordinates.y = 1.0f - 2.0f * (screenCoordinates.y - viewOffset.y) / viewSize.y;
	//normalizedCoordinates.z = 2.0f * depth - 1.0f;
	//normalizedCoordinates.w = 1.0f;

	// Transform the normalized coordinates to world coordinates
	glm::vec4 pixel_screen(screenCoords.x, screenCoords.y, 0.f, 1.f);
	glm::vec4 worldCoordinates = inverseViewMatrix * inverseProjectionMatrix * inverseViewportMatrix * pixel_screen;

	// Divide by the homogeneous coordinate to get the final world coordinates
	worldCoordinates /= worldCoordinates.w;

	return glm::vec3(worldCoordinates);
}
glm::mat4 App::createViewportMatrix(const glm::vec2& viewSize, const glm::vec2& viewOffset)
{
	// Your implementation here
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(viewOffset.x, viewOffset.y, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(viewSize.x / 2.0f, viewSize.y / 2.0f, 1.0f));
	glm::mat4 viewportMatrix = translate * scale;
	return viewportMatrix;
}



bool App::solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}

bool App::intersects(const Ray& ray, const glm::vec3& center, float& radius)
{
	glm::vec3 L = ray.origin - center;
	float a = glm::dot(ray.direction, ray.direction);
	float b = 2 * glm::dot(ray.direction, L);
	float c = glm::dot(L, L) - radius * radius;

	float t0, t1;
	if (!solveQuadratic(a, b, c, t0, t1)) return false;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1;
		if (t0 < 0) return false;
	}

	return true;
}	//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html

void App::dragSphere(Particle& particle)
{
	glm::vec4 particleCoordinates = glm::vec4(particle.position.x, particle.position.y, particle.position.z, 1.0f);

	glm::mat4 inverseViewportMatrix = glm::inverse(mViewportMatrix);
	glm::mat4 inverseProjectionMatrix = glm::inverse(mProjectionMatrix);
	glm::mat4 inverseViewMatrix = glm::inverse(mViewMatrix);

	particleCoordinates = mViewportMatrix * mProjectionMatrix * mViewMatrix * particleCoordinates;
	particleCoordinates /= particleCoordinates.w;

	glm::vec4 newParticleCoordinates = glm::vec4(mCurrentMouse.x, mCurrentMouse.y, particleCoordinates.z, 1.f);

	newParticleCoordinates = inverseViewMatrix * inverseProjectionMatrix * inverseViewportMatrix * newParticleCoordinates;
	newParticleCoordinates /= newParticleCoordinates.w;

	particle.position = glm::vec3(newParticleCoordinates);
}