#include "app.hpp"
#include "oglhelper.hpp"

#include <GLFW\glfw3.h>

#include <iostream>



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

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.f, 4.f, 10.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f));
	mScene->setViewMatrix(view);

	mLastFrameTime = std::chrono::high_resolution_clock::now();
}

void App::drawCoordinateSystem(glm::mat4 const& coords, const float length, const float thickness)
{
	glm::mat4 cyl = glm::translate(glm::mat4(1.f), glm::vec3(0.f, length/2, 0.f)) * glm::scale(glm::mat4(1.f), glm::vec3(thickness, length, thickness));
	glm::mat4 cone = glm::translate(glm::mat4(1.f), glm::vec3(0.f, length+thickness, 0.f)) * glm::scale(glm::mat4(1.f), glm::vec3(1.5f*thickness, 2*thickness, 1.5f*thickness));

	glm::mat4 yAxis = glm::mat4(1.f);
	glm::mat4 xAxis = glm::mat4_cast(glm::fquat(glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 0.f, 0.f)));
	glm::mat4 zAxis = glm::mat4_cast(glm::fquat(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)));

	const glm::vec3 blue(0.33f, 0.33f, 0.99f);
	const glm::vec3 green(0.33f, 0.99f, 0.33f);
	const glm::vec3 red(0.99f, 0.33f, 0.33f);

	// we assume that startFrame has already been called

	mScene->drawCylinder(coords * xAxis * cyl, red, false);
	mScene->drawCone(coords * xAxis * cone, red, false);

	mScene->drawCylinder(coords * yAxis * cyl, green, false);
	mScene->drawCone(coords * yAxis * cone, green, false);

	mScene->drawCylinder(coords * zAxis * cyl, blue, false);
	mScene->drawCone(coords * zAxis * cone, blue, false);
}


void App::render()
{
	glm::mat4 view = glm::lookAt(
		cameraMoveVector,
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f));
	mScene->setViewMatrix(view);

	struct Transform
	{
		glm::vec3 scale;
		glm::vec3 translation;
		glm::fquat orientation;

		glm::mat4 modelMatrix() const
		{
			return glm::translate(glm::mat4(1.f), glm::vec3(translation))
				* glm::mat4_cast(orientation)
				* glm::scale(glm::mat4(1.f), scale);
		}
		//glm::mat4 modelMatrix(const SmokeParticle& particle) const
		//{
		//	return glm::translate(glm::mat4(1.f), particle.position)
		//		* glm::scale(glm::mat4(1.f), particle.size * scale)
		//		* glm::mat4_cast(orientation)
		//		* glm::translate(glm::mat4(1.f), translation);
		//}
		//struct SmokeParticle
		//{
		//	float PosY;
		//	float PosX;
		//	float PosZ;
		//	float size;
		//	float lifetime;
		//	float maxLifetime;
		//};

		//std::vector<SmokeParticle> smokeParticles;

		//void initializeSmokeParticles()
		//{
		//	// Spawn particles above the ship's chimneys
		//	for (int i = 0; i < 50; ++i)
		//	{
		//		SmokeParticle particle;
		//		particle.PosX = -0.5f; 
		//		particle.PosY = 0.2f;
		//		particle.PosZ = 0.32f;
		//		particle.size = 0.1f; // Initial size
		//		particle.lifetime = 0.0f; // Initial lifetime
		//		particle.maxLifetime = 5.0f; // Maximum lifetime before resetting
		//		smokeParticles.push_back(particle);
		//	}
		//}

		//void updateSmokeParticles(float deltaTime)
		//{
		//	for (auto& particle : smokeParticles)
		//	{
		//		// Translate the particle upwards
		//		particle.PosY += 0.3f * deltaTime;
		//		particle.PosX -= 0.1f * deltaTime;
		//		particle.PosZ -= 0.1f * deltaTime;
		//		// Increase the size of the particle over time
		//		particle.size += 0.05f * deltaTime;

		//		// Check if the particle's lifetime exceeds the maximum lifetime
		//		if (particle.lifetime > particle.maxLifetime)
		//		{
		//			// Reset the particle above the ship's chimney
		//			particle.PosX = -0.5f;
		//			particle.PosY = 0.2f;
		//			particle.PosZ = 0.32f;
		//			particle.size = 0.1f;
		//			particle.lifetime = 0.0f;
		//		}

		//		// Increase the particle's lifetime
		//		particle.lifetime += deltaTime;
		//	}
		//}

		//void 
		//	renderSmokeParticles()
		//{
		//	for (const auto& particle : smokeParticles)
		//	{
		//		// Render a sphere at the particle's position with its size
		//		mScene->drawSphere(modelMatrix(particle), gray, false);
		//	}
		//}
	};

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	float elapsedSeconds = std::chrono::duration<float>(now - mLastFrameTime).count();
	mTotalElapsedSeconds += elapsedSeconds;
	mLastFrameTime = now;

	float vehicleSpeed1 = 1.5f;

	const glm::vec3 red(1.f, 0.f, 0.f);
	const glm::vec3 green(0.f, 1.f, 0.f);
	const glm::vec3 darkgreen(0.1f, 0.5f, 0.1f);
	const glm::vec3 blue(0.f, 0.f, 1.f);
	const glm::vec3 brown(0.5f, 0.3f, 0.f);
	const glm::vec3 lightbrown(0.5f, 0.25f, 0.f);
	const glm::vec3 darkbrown(0.2f, 0.1f, 0.f);
	const glm::vec3 white(1.f, 1.f, 1.f);
	const glm::vec3 gray(0.5f, 0.5f, 0.5f);
	const glm::vec3 lightgray(0.6f, 0.6f, 0.6f);


	mScene->startCurrentFrame(mWindowWidth, mWindowHeight);

	Transform identity = { glm::vec3(1.f), glm::vec3(0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };

	Transform shipOffset = { glm::vec3(1.f), glm::vec3(0.f, 0.f, 2.f),
		glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform shipOrbit = { glm::vec3(1.f), glm::vec3(0.f),
		glm::angleAxis(mTotalElapsedSeconds, glm::vec3(0.f, 1.f, 0.f)) };


	//Transform makePlaneBigger = { glm::vec3(100.f, 0.f, 100.f), glm::vec3(0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform plane = { glm::vec3(5.f, 0.01f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };

	Transform island = { glm::vec3(1.f, 0.01f, 1.f), glm::vec3(0.f, 0.2f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };

	Transform treeSmall = { glm::vec3(1.f), glm::vec3(-0.5f, 0.4f, 0.4f), glm::fquat(1.f, 0.05f, 0.2f, 0.1f) };
	Transform stump = { glm::vec3(0.05f, 0.2f, 0.05f), glm::vec3(0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform coneBottom = { glm::vec3(0.15f, 0.2f, 0.15f), glm::vec3(0.f, 0.1f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform coneMiddle = { glm::vec3(0.15f, 0.2f, 0.15f), glm::vec3(0.f, 0.18f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform coneTop = { glm::vec3(0.15f, 0.2f, 0.15f), glm::vec3(0.f, 0.26f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };

	Transform treeMedium = { glm::vec3(1.5f), glm::vec3(0.3f, 0.4f, 0.1f), glm::fquat(1.f, -0.05f, 0.2f, -0.07f) };
	Transform treeLarge = { glm::vec3(2.f), glm::vec3(0.f, 0.4f, -0.5f), glm::fquat(1.f, -0.1f, 0.2f, -0.05f) };

	Transform ship = { glm::vec3(1.f), glm::vec3(0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform shipBodydark = { glm::vec3(1.f, 0.2f, 0.2f), glm::vec3(0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform shipBodylight = { glm::vec3(1.f, 0.08f, 0.2f), glm::vec3(0.f, 0.13f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform shipFloor = { glm::vec3(1.f, 0.12f, 0.2f), glm::vec3(0.f, 0.13f, 0.f), glm::fquat(1.f, 0.8f, 0.f, 0.f) };
	Transform shipBody = { glm::vec3(0.6f, 0.3f, 0.15f), glm::vec3(-0.1f, 0.2f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform shipBodyFloor = {glm::vec3(0.6f, 0.1f, 0.15f), glm::vec3(-0.1f, 0.35f, 0.f), glm::fquat(1.f, 0.75f, 0.f, 0.f) };
	Transform shipBody2 = { glm::vec3(0.2f, 1.f, 0.1f), glm::vec3(0.1f, 0.f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform shipBodyFloor2 = { glm::vec3(0.19f, 0.1f, 0.09f), glm::vec3(0.1f, 0.51f, 0.f), glm::fquat(1.f, 0.7f, 0.f, 0.f) };

	//chimneys
	Transform chimney1 = { glm::vec3(0.1f, 0.6f, 0.1f), glm::vec3(-0.5f, 0.2f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.1f) };
	Transform chimney2 = { glm::vec3(0.1f, 0.6f, 0.1f), glm::vec3(-0.245f, 0.2f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.1f) };
	Transform chimneyred1 = { glm::vec3(0.11f), glm::vec3(-0.5555f, 0.5f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.1f) };
	Transform chimneyred2 = { glm::vec3(0.11f), glm::vec3(-0.31f, 0.5f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.1f) };


	//hab versucht den Smoke mittels Klasse und Liste zu machen, aber alles hat gefailed, leider...
	// 
	//for (auto& particle : smokeParticles)
	//{
	//	// Check if particle is active
	//	if (particle.active)
	//	{
	//		// Update particle properties
	//		particle.smokeSize += 0.025f * elapsedSeconds;
	//		particle.smokePosX -= 0.1f * elapsedSeconds;
	//		particle.smokePosY += 0.3f * elapsedSeconds;
	//		particle.smokePosZ -= 0.1f * elapsedSeconds;
	//		particle.lifeTime += elapsedSeconds;
	//		std::cout << particle.lifeTime << particle.maxLifeTime << std::endl;

	//		// Draw the particle
	//		Transform smoke1 = { glm::vec3(particle.smokeSize), glm::vec3(particle.smokePosX, particle.smokePosY, particle.smokePosZ), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	//		mScene->drawSphere(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * smoke1.modelMatrix(), white, false);

	//		// Reset particle properties if lifetime exceeds maximum lifetime
	//		if (particle.lifeTime > particle.maxLifeTime)
	//		{
	//			particle.active = false;  // Deactivate the particle
	//			maxParticles--;
	//		}
	//	}
	//}

	// Check if you can spawn a new particle
	//if (smokeParticles.size() < maxParticles)
	//{
	//	SmokeParticle newParticle;
	//	// Initialize new particle properties as needed
	//	newParticle.smokeSize = 0.1f;
	//	newParticle.smokePosY = 0.5f;
	//	newParticle.lifeTime = 0.f;
	//	newParticle.maxLifeTime = 4.f;
	//	newParticle.smokePosX = -0.55f;
	//	newParticle.smokePosZ = 0.f;
	//	newParticle.active = true;  // Activate the new particle
	//	smokeParticles.push_back(newParticle);
	//}
	//smoke1
	
	float multiplikator = 0.01f;

	//SmokeParticle particle;
	//particle.smokeSize = (0.1f);
	//particle.smokePosX = -0.55f;
	//particle.smokePosY = 0.5f;
	//particle.smokePosZ = 0.f;
	//particle.lifeTime = 0.0f;
	//particle.maxLifeTime = 5.0f;
	//smokeParticles.push_back(particle);

	if (mTotalElapsedSeconds > 0)
	{
		lifeTime1 -= (elapsedSeconds);// +i * multiplikator);

		smokeSize1 += 0.01f * elapsedSeconds;
		smokePosX1 -= 0.1f * elapsedSeconds;
		smokePosX1R -= 0.1f * elapsedSeconds;
		smokePosY1 += 0.2f * elapsedSeconds;
		smokePosZ1 -= 0.1f * elapsedSeconds;
		
		std::cout << lifeTime1 << maxLifeTime << std::endl;

		Transform smoke1 = { glm::vec3(smokeSize1), glm::vec3(smokePosX1, smokePosY1, smokePosZ1), glm::fquat(1.f, 0.f, 0.f, 0.f) };
		mScene->drawSphere(shipOrbit.modelMatrix()* shipOffset.modelMatrix()* ship.modelMatrix()* smoke1.modelMatrix(), white, false);
		Transform smoke2 = { glm::vec3(smokeSize1), glm::vec3(smokePosX1R, smokePosY1, smokePosZ1), glm::fquat(1.f, 0.f, 0.f, 0.f) };
		mScene->drawSphere(shipOrbit.modelMatrix()* shipOffset.modelMatrix()* ship.modelMatrix()* smoke2.modelMatrix(), white, false);

		if (lifeTime1 < 0)
		{
			smokePosY1 = 0.5f;
			lifeTime1 = 4.f;
			smokeSize1 = 0.1f;
			smokePosX1 = -0.55f;
			smokePosX1R = -0.35f;
			smokePosZ1 = 0.f;
		}
	}
	if (mTotalElapsedSeconds > 1)
	{
		lifeTime2 -= (elapsedSeconds);// +i * multiplikator);

		smokeSize2 += 0.01f * elapsedSeconds;
		smokePosX2 -= 0.1f * elapsedSeconds;
		smokePosX2R -= 0.1f * elapsedSeconds;
		smokePosY2 += 0.2f * elapsedSeconds;
		smokePosZ2 -= 0.1f * elapsedSeconds;

	//	std::cout << lifeTime1 << maxLifeTime << std::endl;

		Transform smoke1 = { glm::vec3(smokeSize2), glm::vec3(smokePosX2, smokePosY2, smokePosZ2), glm::fquat(1.f, 0.f, 0.f, 0.f) };
		mScene->drawSphere(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * smoke1.modelMatrix(), white, false);
		Transform smoke2 = { glm::vec3(smokeSize2), glm::vec3(smokePosX2R, smokePosY2, smokePosZ2), glm::fquat(1.f, 0.f, 0.f, 0.f) };
		mScene->drawSphere(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * smoke2.modelMatrix(), white, false);


		if (lifeTime2 < 0)
		{
			smokePosY2 = 0.5f;
			lifeTime2 = 4.f;
			smokeSize2 = 0.1f;
			smokePosX2 = -0.55f;
			smokePosX2R = -0.35f;
			smokePosZ2 = 0.f;
		}
	}
	if (mTotalElapsedSeconds > 2)
	{
		lifeTime3 -= (elapsedSeconds);// +i * multiplikator);

		smokeSize3 += 0.01f * elapsedSeconds;
		smokePosX3 -= 0.1f * elapsedSeconds;
		smokePosX3R -= 0.1f * elapsedSeconds;
		smokePosY3 += 0.2f * elapsedSeconds;
		smokePosZ3 -= 0.1f * elapsedSeconds;

		//	std::cout << lifeTime1 << maxLifeTime << std::endl;

		Transform smoke1 = { glm::vec3(smokeSize2), glm::vec3(smokePosX3, smokePosY3, smokePosZ3), glm::fquat(1.f, 0.f, 0.f, 0.f) };
		mScene->drawSphere(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * smoke1.modelMatrix(), white, false);
		Transform smoke2 = { glm::vec3(smokeSize2), glm::vec3(smokePosX3R, smokePosY3, smokePosZ3), glm::fquat(1.f, 0.f, 0.f, 0.f) };
		mScene->drawSphere(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * smoke2.modelMatrix(), white, false);


		if (lifeTime3 < 0)
		{
			smokePosY3 = 0.5f;
			lifeTime3 = 4.f;
			smokeSize3 = 0.1f;
			smokePosX3 = -0.55f;
			smokePosX3R = -0.35f;
			smokePosZ3 = 0.f;
		}
	}
	if (mTotalElapsedSeconds > 3)
	{
		lifeTime4 -= (elapsedSeconds);// +i * multiplikator);

		smokeSize4 += 0.01f * elapsedSeconds;
		smokePosX4 -= 0.1f * elapsedSeconds;
		smokePosX4R -= 0.1f * elapsedSeconds;
		smokePosY4 += 0.2f * elapsedSeconds;
		smokePosZ4 -= 0.1f * elapsedSeconds;

		//	std::cout << lifeTime1 << maxLifeTime << std::endl;

		Transform smoke1 = { glm::vec3(smokeSize2), glm::vec3(smokePosX4, smokePosY4, smokePosZ4), glm::fquat(1.f, 0.f, 0.f, 0.f) };
		mScene->drawSphere(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * smoke1.modelMatrix(), white, false);
		Transform smoke2 = { glm::vec3(smokeSize2), glm::vec3(smokePosX4R, smokePosY4, smokePosZ4), glm::fquat(1.f, 0.f, 0.f, 0.f) };
		mScene->drawSphere(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * smoke2.modelMatrix(), white, false);


		if (lifeTime4 < 0)
		{
			smokePosY4 = 0.5f;
			lifeTime4 = 4.f;
			smokeSize4 = 0.1f;
			smokePosX4 = -0.55f;
			smokePosX4R = -0.35f;
			smokePosZ4 = 0.f;
		}
	}
	//}
	//{//verschiedene lifetimes
	// //tutor meint, mit lifetime und nem multiplikator multiplizieren
	//	SmokeParticle& particle = smokeParticles[i];

		

	//	particle.smokeSize = (0.1f);
	//	particle.smokePosX = -0.55f;
	//	particle.smokePosY = 0.5f;
	//	particle.smokePosZ = 0.f;
	//	particle.lifeTime = 0.0f;
	//	particle.maxLifeTime = 5.0f;
	//	smokeParticles.push_back(particle);
	//}

	//for (int i = 0; i < 5; ++i) {
	//	particle.smokeSize = (0.1f);
	//	particle.smokePosX = -0.55f;
	//	particle.smokePosY = 0.5f;
	//	particle.smokePosZ = 0.f;
	//	particle.lifeTime = 0.0f;
	//	particle.maxLifeTime = 5.0f;
	//	smokeParticles.push_back(particle);
	//}
	//wheels
	float speedWheels = 25.f;
	for (float i = 0.f; i <= 157.5f; i += 22.5f)
	{
	 Transform speiche = { glm::vec3(0.02f, 0.25f, 0.02f), glm::vec3(0.f, 0.f, 0.24f), glm::rotate(glm::fquat(1.f, 0.f, 0.f, 0.f), glm::radians(i - (mTotalElapsedSeconds * speedWheels)), glm::vec3(0.0f, 0.0f, 1.0f))};
	 mScene->drawCube(shipOrbit.modelMatrix()* shipOffset.modelMatrix()* ship.modelMatrix()* speiche.modelMatrix(), lightbrown, false);

	}
	Transform wheelRingRight = { glm::vec3(0.3f), glm::vec3(0.f, 0.f, 0.22f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform wheelRingRight2 = { glm::vec3(0.3f), glm::vec3(0.f, 0.f, 0.263f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform wheelRingCylinderRight = { glm::vec3(0.3f, 0.045f, 0.3f), glm::vec3(0.f, 0.f, 0.24f),glm::rotate(glm::fquat(1.f, 0.f, 0.f, 0.f), glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f) )};

	//wheels2
	for (float i = 0.f; i <= 157.5f; i += 22.5f)
	{
		Transform speiche = { glm::vec3(0.02f, 0.25f, 0.02f), glm::vec3(0.f, 0.f, -0.24f), glm::rotate(glm::fquat(1.f, 0.f, 0.f, 0.f), glm::radians(i - (mTotalElapsedSeconds * speedWheels)), glm::vec3(0.0f, 0.0f, 1.0f)) };
		mScene->drawCube(shipOrbit.modelMatrix()* shipOffset.modelMatrix()* ship.modelMatrix() * speiche.modelMatrix(), lightbrown, false);

	}
	Transform wheelRingLeft = { glm::vec3(0.3f), glm::vec3(0.f, 0.f, -0.22f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform wheelRingLeft2 = { glm::vec3(0.3f), glm::vec3(0.f, 0.f, -0.263f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform wheelRingCylinderLeft = { glm::vec3(0.3f, 0.045f, 0.3f), glm::vec3(0.f, 0.f, -0.24f),glm::rotate(glm::fquat(1.f, 0.f, 0.f, 0.f), glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f)) };

	//flag
	Transform flagPole = { glm::vec3(0.005f, 2.f, 0.005f), glm::vec3(0.8f, 0.f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform flag = { glm::vec3(0.1f, 0.07f, 0.01f), glm::vec3(0.715f, 1.f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };

	Transform flagPole2 = { glm::vec3(0.005f, 1.5f, 0.005f), glm::vec3(-0.8f, 0.f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };
	Transform flag2 = { glm::vec3(0.1f, 0.07f, 0.01f), glm::vec3(-0.9f, 0.7f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f) };

	// let's render a coordinate system for fun
	// glm::mat4(1.f) creates the identity matrix (--> beware: the mat4 default ctor will initialize everything to 0)
	drawCoordinateSystem(glm::mat4(1.f), 2.f, 0.05f);
	// but why can we not see the coordinate system when starting the program?
	// --> where do you think the camera is located? ;-)
	// this is a typical error btw.

	//plane
	mScene->drawCube(/*makePlaneBigger.modelMatrix() * */ plane.modelMatrix(), blue);
	
	//island
	mScene->drawSphere(island.modelMatrix(), green, false);

	//treesmall
	mScene->drawCylinder(treeSmall.modelMatrix() * stump.modelMatrix(), brown);
	mScene->drawCone(treeSmall.modelMatrix()* coneBottom.modelMatrix(), darkgreen);
	mScene->drawCone(treeSmall.modelMatrix()* coneMiddle.modelMatrix(), green);
	mScene->drawCone(treeSmall.modelMatrix()* coneTop.modelMatrix(), darkgreen);

	//treemedium
	mScene->drawCylinder(treeMedium.modelMatrix()* stump.modelMatrix(), brown);
	mScene->drawCone(treeMedium.modelMatrix()* coneBottom.modelMatrix(), darkgreen);
	mScene->drawCone(treeMedium.modelMatrix()* coneMiddle.modelMatrix(), green);
	mScene->drawCone(treeMedium.modelMatrix()* coneTop.modelMatrix(), darkgreen);

	//treelarge
	mScene->drawCylinder(treeLarge.modelMatrix()* stump.modelMatrix(), brown);
	mScene->drawCone(treeLarge.modelMatrix()* coneBottom.modelMatrix(), darkgreen);
	mScene->drawCone(treeLarge.modelMatrix()* coneMiddle.modelMatrix(), green);
	mScene->drawCone(treeLarge.modelMatrix()* coneTop.modelMatrix(), darkgreen);

	//ship
	mScene->drawCylinder(shipOrbit.modelMatrix()* shipOffset.modelMatrix() * ship.modelMatrix()* shipBodydark.modelMatrix(), darkbrown, false);
	mScene->drawCylinder(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix()* shipBodylight.modelMatrix(), lightbrown, false);
	mScene->drawDisk(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix()* shipFloor.modelMatrix(), lightbrown, false);
	mScene->drawCylinder(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix()* shipBody.modelMatrix(), white);
	mScene->drawDisk(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * shipBodyFloor.modelMatrix(), white, false);
	mScene->drawCylinder(shipOrbit.modelMatrix()* shipOffset.modelMatrix()* ship.modelMatrix()* shipBody2.modelMatrix(), white);
	mScene->drawDisk(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix()* shipBodyFloor2.modelMatrix(), white, false);
	//chimneys
	mScene->drawCylinder(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix()* chimney1.modelMatrix(), gray);
	mScene->drawCylinder(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix()* chimney2.modelMatrix() , gray);
	mScene->drawCylinder(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix()* chimneyred1.modelMatrix(), red, false);
	mScene->drawCylinder(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix()* chimneyred2.modelMatrix(), red, false);
	//smoke

	//for (const auto& particle : smokeParticles)
	//{
	//	// Render a sphere at the particle's position with its size
	//	mScene->drawSphere(particle.transform.modelMatrix(), gray, false);
	//}
	//wheel
	mScene->drawRing(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * wheelRingRight.modelMatrix(), lightgray, false);
	mScene->drawRing(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * wheelRingRight2.modelMatrix(), lightgray, false);
	mScene->drawCylinder(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * wheelRingCylinderRight.modelMatrix(), lightgray, false);
	//wheel2
	mScene->drawRing(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * wheelRingLeft.modelMatrix() , lightgray, false);
	mScene->drawRing(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * wheelRingLeft2.modelMatrix(), lightgray, false);
	mScene->drawCylinder(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * wheelRingCylinderLeft.modelMatrix(), lightgray, false);

	mScene->drawCylinder(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * flagPole.modelMatrix(), darkbrown, false);
	mScene->drawCube(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * flag.modelMatrix(), blue, false);


	mScene->drawCylinder(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * flagPole2.modelMatrix(), darkbrown, false);
	mScene->drawCube(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * ship.modelMatrix() * flag2.modelMatrix(), blue, false);
	//wheels
	/*for (int i = 1; i < 7; i++)
	{
		mScene->drawCube(shipOffset.modelMatrix() * ship.modelMatrix() * speiche.modelMatrix(), red, false);
	}*/

	//mScene->drawCube(shipOffset.modelMatrix()* ship.modelMatrix()* speiche.modelMatrix(), red, false);



	mScene->endCurrentFrame();
}

void App::cleanup()
{
	// nothing to do
}

void App::resize(const unsigned w, const unsigned h)
{
	mWindowWidth = w;
	mWindowHeight = h;
	mWindowAspectRatio = mWindowWidth / static_cast<float>(h);
}

void App::keyInput(const int key, const int scancode, const int action, const int mods)
{
	// space button activates opengl wireframe mode -> we can see the triangles

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	const float stepSize = 1.f;
	const float rotationStep = 2.f;
	// Camera controls
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_W:
			cameraMoveVector.y += stepSize;
			break;
		case GLFW_KEY_S:
			cameraMoveVector.y -= stepSize;
			break;
		case GLFW_KEY_E:
			// Zoom out (increase distance)
			cameraMoveVector.z -= stepSize;
			break;
		case GLFW_KEY_Q:
			// Zoom in (decrease distance)
			cameraMoveVector.z += stepSize;
			break;
		case GLFW_KEY_A:
			// Rotate left (around Y-axis)
			cameraMoveVector = glm::rotate(glm::mat4(1.f), -glm::radians(rotationStep), glm::vec3(0, 1, 0)) * glm::vec4(cameraMoveVector, 1.0f);

			break;
		case GLFW_KEY_D:
			// Rotate right (around Y-axis)
			cameraMoveVector = glm::rotate(glm::mat4(1.f), glm::radians(rotationStep), glm::vec3(0, 1, 0)) * glm::vec4(cameraMoveVector, 1.0f);

			break;
		}
		// Update view matrix based on camera parameters
		glm::mat4 viewMatrix = glm::lookAt(cameraMoveVector, glm::vec3(0.f), glm::vec3(0, 1, 0));
		mScene->setViewMatrix(viewMatrix);
	}
}