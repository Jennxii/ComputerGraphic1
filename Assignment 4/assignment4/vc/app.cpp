#include "app.hpp"
#include "parametric_curves.hpp"
#include "transform.hpp"

#include "cg\glm.hpp"
#include "cg\glsl.hpp"
#include "cg\triangle_mesh.hpp"
#include "cg\primitives.hpp"

#include <GLFW\glfw3.h>

#include <iostream>

App::App(const unsigned w, const unsigned h) : CgApp(),
	mWindowWidth(w),
	mWindowHeight(h),
	mWindowAspectRatio(w / static_cast<float>(h)),
	mCamera(glm::vec3(0.f, 2.f, 20.f))
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
	mProgram = cg::GlslProgram::create({
		{ cg::ShaderType::VERTEX_SHADER, cg::loadShaderSource("..\\shaders\\diffuse.vert") },
		{ cg::ShaderType::FRAGMENT_SHADER, cg::loadShaderSource("..\\shaders\\diffuse.frag") }
		});

	mNumParticles = 20;
	mParticles.resize(mNumParticles, { -1.f, glm::vec3(0.f), glm::vec3(0.f) });

	cg::BaseMeshData tmp;
	cg::BasePrimitives::generateSphere(30, 15, tmp);
	mSphere = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateCylinder(30, 2, tmp);
	mCylinder = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateCube(tmp);
	mCube = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateDisk(60, 10, tmp);
	mDisk = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateRing(60, 1.0f, 0.8f, tmp);
	mRing = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateCone(30, 1, tmp);
	mCone = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generateQuad(tmp);
	mQuad = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generatePyramid(tmp);
	mPyramid = cg::buildGPUMesh(tmp);
	cg::BasePrimitives::generatePlane(100, 100, tmp);
	mPlane = cg::buildGPUMesh(tmp);

	mLastFrameTime = std::chrono::high_resolution_clock::now();
}

void App::render()
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	float elapsedSeconds = std::chrono::duration<float>(now - mLastFrameTime).count();
	if (!mStopTime)
		mTotalElapsedSeconds += elapsedSeconds;
	mLastFrameTime = now;

	// update camera view & model matrices based on user keyboard / mouse input
	mCamera.update();

	const glm::vec3 blue(0.33f, 0.66f, 0.99f);
	const glm::vec3 purple(0.88f, 0.33f, 0.99f);
	const glm::vec3 green(0.33f, 0.99f, 0.33f);
	const glm::vec3 darkGreen(0.11f, 0.44f, 0.11f);
	const glm::vec3 brown(0.55f, 0.44f, 0.1f);
	const glm::vec3 red(0.99f, 0.33f, 0.33f);
	const glm::vec3 gray(0.5f);
	const glm::vec3 black(0.f);
	const glm::vec3 white(1.f);

	Transform seaLevel(glm::vec3(25.f), glm::vec3(0.f), glm::fquat(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f)));
	Transform island(glm::vec3(4.f, 0.25f, 4.f), glm::vec3(0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));

	Transform treeTrunk(glm::vec3(0.2f, 0.5f, 0.2f), glm::vec3(0.f, 0.2f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform tree1(glm::vec3(0.6f, 1.f, 0.6f), glm::vec3(0.f, 0.8f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform tree2(glm::vec3(0.575f, 0.9f, 0.575f), glm::vec3(0.f, 1.f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform tree3(glm::vec3(0.55f, 0.8f, 0.55f), glm::vec3(0.f, 1.2f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));

	Transform bigTree(glm::vec3(2.5f), glm::vec3(1.f, 0.f, -1.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform regularTree(glm::vec3(1.5f), glm::vec3(1.f, 0.f, 1.5f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform smallTree(glm::vec3(1.f), glm::vec3(-1.5f, 0.f, 0.5f), glm::fquat(1.f, 0.f, 0.f, 0.f));

	Transform shipOrbit(glm::vec3(3.f), glm::vec3(0.f), glm::angleAxis(0.3f * mTotalElapsedSeconds, glm::vec3(0.f, 1.f, 0.f)));
	Transform shipOffset(glm::vec3(1.f), glm::vec3(0.f, 0.f, 3.f), glm::fquat(1.f, 0.f, 0.f, 0.f));

	Transform shipMain1(glm::vec3(1.6f, 0.3f, 0.3f), glm::vec3(0.f, -0.05f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform shipMain2(glm::vec3(1.6f, 0.05f, 0.3f), glm::vec3(0.f, 0.125f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform shipMain2Top(glm::vec3(1.6f, 0.3f, 1.f), glm::vec3(0.f, 0.15f, 0.f), glm::fquat(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f)));

	Transform shipMid(glm::vec3(0.7f, 0.3f, 0.2f), glm::vec3(-0.1f, 0.25f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform shipMidTop(glm::vec3(0.7f, 0.2f, 1.f), glm::vec3(-0.1f, 0.4f, 0.f), glm::fquat(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f)));

	Transform shipRoof(glm::vec3(0.3f, 0.2f, 0.15f), glm::vec3(+0.2f, 0.45f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform shipRoofTop(glm::vec3(0.3f, 0.15f, 1.f), glm::vec3(+0.2f, 0.55f, 0.f), glm::fquat(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f)));

	Transform shipHeadlight(glm::vec3(0.05f, 0.2f, 0.05f), glm::vec3(+0.4f, 0.55f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform shipHeadlightTop(glm::vec3(0.05f, 0.05f, 1.f), glm::vec3(+0.4f, 0.65f, 0.f), glm::fquat(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f)));

	Transform chimneyAxis(glm::vec3(1.f), glm::vec3(0.f), glm::fquat(glm::vec3(0.f, 1.f, 0.f), glm::normalize(glm::vec3(-0.2f, 0.8f, 0.f))));
	Transform chimneyOffset1(glm::vec3(1.f), glm::vec3(-0.25f, 0.35f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform chimneyOffset2(glm::vec3(1.f), glm::vec3(-0.5f, 0.35f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));

	Transform chimney1(glm::vec3(0.12f, 0.6f, 0.1f), glm::vec3(0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform chimney2(glm::vec3(0.13f, 0.1f, 0.11f), glm::vec3(0.f, 0.3f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform chimneyTop(glm::vec3(0.12f, 0.1f, 1.f), glm::vec3(0.f, 0.35f, 0.f), glm::fquat(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f)));

	Transform leftWheel(glm::vec3(1.f), glm::vec3(0.f, 0.f, -0.35f), glm::angleAxis(mTotalElapsedSeconds, glm::vec3(0.f, 0.f, 1.f)));
	Transform rightWheel(glm::vec3(1.f), glm::vec3(0.f, 0.f, +0.35f), glm::angleAxis(mTotalElapsedSeconds, glm::vec3(0.f, 0.f, 1.f)));

	Transform wheelCylinderOuter(glm::vec3(0.35f, 0.1f, 0.35f), glm::vec3(0.f, 0.f, 0.f), glm::fquat(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)));
	Transform wheelCylinderInner(glm::vec3(0.28f, 0.1f, 0.28f), glm::vec3(0.f, 0.f, 0.f), glm::fquat(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)));
	Transform wheelCircleLeft(glm::vec3(0.35f, 0.35f, 1.f), glm::vec3(0.f, 0.f, 0.05f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform wheelCircleRight(glm::vec3(0.35f, 0.35f, 1.f), glm::vec3(0.f, 0.f, -0.05f), glm::fquat(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, -1.f)));

	Transform frontPole(glm::vec3(1.f), glm::vec3(+1.45f, 0.f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform backPole(glm::vec3(1.f), glm::vec3(-1.45f, 0.f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform flagpole(glm::vec3(0.0125f, 0.8f, 0.0125f), glm::vec3(0.f, 0.5f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));
	Transform flag(glm::vec3(0.36f, 0.18f, 1.f), glm::vec3(-0.36f, 0.7f, 0.f), glm::fquat(1.f, 0.f, 0.f, 0.f));

	std::vector<Transform> wheelSpokes(6);
	for (unsigned i = 0; i < 6; ++i)
	{
		wheelSpokes[i].orientation = angleAxis(glm::radians(i * 30.f), glm::vec3(0.f, 0.f, 1.f));
		wheelSpokes[i].scale = glm::vec3(0.32f, 0.02f, 0.02f);
	}

	std::vector<Transform> smokeParticles;
	for (unsigned particleId = 0; particleId < mNumParticles; ++particleId)
	{
		SmokeParticle& particle = mParticles[particleId];

		if (particle.lifetime > 2.f || particle.lifetime < 0.f)
		{
			if (particleId != 0 && particleId != mNumParticles/2 && mParticles[particleId - 1].lifetime < 0.2f)
				continue;

			particle.lifetime = 0.0f;

			if (particleId < mNumParticles/2)
			{
				glm::mat4 spawnTf = shipOrbit.modelMatrix() * shipOffset.modelMatrix() * chimneyOffset1.modelMatrix() * chimneyAxis.modelMatrix() * glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.35f, 0.f));
				particle.spawnPosition = spawnTf * glm::vec4(0.f, 0.f, 0.f, 1.f);
				particle.moveDirection = chimneyAxis.orientation * glm::vec3(0.f, 1.f, 0.f);
			}
			else
			{
				glm::mat4 spawnTf = shipOrbit.modelMatrix() * shipOffset.modelMatrix() * chimneyOffset2.modelMatrix() * chimneyAxis.modelMatrix() * glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.35f, 0.f));
				particle.spawnPosition = spawnTf * glm::vec4(0.f, 0.f, 0.f, 1.f);
				particle.moveDirection = chimneyAxis.orientation * glm::vec3(0.f, 1.f, 0.f);
			}
		}
		else
		{
			if (!mStopTime)
				particle.lifetime += elapsedSeconds;
		}

		Transform smokeParticle;
		smokeParticle.translation = particle.spawnPosition + 2.f * particle.lifetime * particle.moveDirection;
		smokeParticle.scale = glm::vec3(0.1f + 0.25f * particle.lifetime);
		smokeParticles.push_back(smokeParticle);
	}

	glClearDepth(1.f);
	glClearColor(0.075f, 0.075f, 0.1f, 1.f);
	glViewport(0, 0, mWindowWidth, mWindowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	cg::GlslProgram::useProgram(mProgram);
	mProgram->setUniformMat4("observer.viewMatrix", mCamera.viewMatrix());
	mProgram->setUniformMat4("observer.projectionMatrix", mCamera.projectionMatrix());
	// low intensity directional light (it's night)
	mProgram->setUniformVec3("directionalLightColor", glm::vec3(0.4f));
	mProgram->setUniformVec3("directionalLightDir", glm::normalize(glm::vec3(-1.f, -0.5f, -1.f)));
	// low intensity ambient
	mProgram->setUniformVec3("ambientLightColor", glm::vec3(0.05f, 0.075f, 0.1f));
		
		// sea
		drawShaded(mDisk, seaLevel.modelMatrix(), blue);
		// island
		drawShaded(mSphere, island.modelMatrix(), green);
		// tree 1
		drawShaded(mCylinder, bigTree.modelMatrix() * treeTrunk.modelMatrix(), brown);
		drawShaded(mCone, bigTree.modelMatrix() * tree1.modelMatrix(), darkGreen);
		drawShaded(mCone, bigTree.modelMatrix() * tree2.modelMatrix(), green);
		drawShaded(mCone, bigTree.modelMatrix() * tree3.modelMatrix(), darkGreen);
		// tree 2
		drawShaded(mCylinder, regularTree.modelMatrix() * treeTrunk.modelMatrix(), brown);
		drawShaded(mCone, regularTree.modelMatrix() * tree1.modelMatrix(), darkGreen);
		drawShaded(mCone, regularTree.modelMatrix() * tree2.modelMatrix(), green);
		drawShaded(mCone, regularTree.modelMatrix() * tree3.modelMatrix(), darkGreen);
		// tree 3
		drawShaded(mCylinder, smallTree.modelMatrix() * treeTrunk.modelMatrix(), brown);
		drawShaded(mCone, smallTree.modelMatrix() * tree1.modelMatrix(), darkGreen);
		drawShaded(mCone, smallTree.modelMatrix() * tree2.modelMatrix(), green);
		drawShaded(mCone, smallTree.modelMatrix() * tree3.modelMatrix(), darkGreen);
		// ship "body"
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * shipMain1.modelMatrix(), black);
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * shipMain2.modelMatrix(), brown);
		drawShaded(mDisk, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * shipMain2Top.modelMatrix(), brown);
		// cabin
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * shipMid.modelMatrix(), white);
		drawShaded(mDisk, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * shipMidTop.modelMatrix(), white);
		// roof
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * shipRoof.modelMatrix(), white);
		drawShaded(mDisk, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * shipRoofTop.modelMatrix(), white);
		// headlight
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * shipHeadlight.modelMatrix(), red);
		drawShaded(mDisk, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * shipHeadlightTop.modelMatrix(), red);
		// 1st chimney
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * chimneyOffset1.modelMatrix() * chimneyAxis.modelMatrix() * chimney1.modelMatrix(), gray);
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * chimneyOffset1.modelMatrix() * chimneyAxis.modelMatrix() * chimney2.modelMatrix(), red);	
		// 2nd chimney
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * chimneyOffset2.modelMatrix() * chimneyAxis.modelMatrix() * chimney1.modelMatrix(), gray);
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * chimneyOffset2.modelMatrix() * chimneyAxis.modelMatrix() * chimney2.modelMatrix(), red);
		// wheel 1
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * rightWheel.modelMatrix() * wheelCylinderOuter.modelMatrix(), gray);
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * rightWheel.modelMatrix() * wheelCylinderInner.modelMatrix(), gray);
		drawShaded(mRing, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * rightWheel.modelMatrix() * wheelCircleLeft.modelMatrix(), gray);
		drawShaded(mRing, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * rightWheel.modelMatrix() * wheelCircleRight.modelMatrix(), gray);
		for (auto& spoke : wheelSpokes)
			drawShaded(mCube, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * rightWheel.modelMatrix() * spoke.modelMatrix(), brown);
		// wheel 2
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * leftWheel.modelMatrix()* wheelCylinderOuter.modelMatrix(), gray);
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * leftWheel.modelMatrix()* wheelCylinderInner.modelMatrix(), gray);
		drawShaded(mRing, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * leftWheel.modelMatrix()* wheelCircleLeft.modelMatrix(), gray);
		drawShaded(mRing, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * leftWheel.modelMatrix()* wheelCircleRight.modelMatrix(), gray);
		for (auto& spoke : wheelSpokes)
			drawShaded(mCube, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * leftWheel.modelMatrix() * spoke.modelMatrix(), brown);
		// flagpole 1
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * backPole.modelMatrix() * flagpole.modelMatrix(), black);
		// flagpole 2
		drawShaded(mCylinder, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * frontPole.modelMatrix() * flagpole.modelMatrix(), black);
		// flag 1
		drawShaded(mPlane, shipOrbit.modelMatrix()* shipOffset.modelMatrix()* backPole.modelMatrix()* flag.modelMatrix(), purple);
		// flag 2
		drawShaded(mPlane, shipOrbit.modelMatrix()* shipOffset.modelMatrix()* frontPole.modelMatrix()* flag.modelMatrix(), purple);

		// smoke particles
		for (auto const& smokeParticle : smokeParticles)
		{
			drawShaded(mSphere, smokeParticle.modelMatrix(), white);
		}

	glBindVertexArray(0);
	cg::GlslProgram::useProgram(nullptr);

	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);
}

void App::drawShaded(std::shared_ptr<cg::TriangleMesh> mesh,
	glm::mat4 const& modelTf,
	glm::vec3 const& color)
{
	// we calculate the normal matrix on the cpu instead of in the shader.
	// also, I turn it into a mat3 & thus remove the translation part
	// so you don't have to worry about appending 0 or 1 to the vertex normal :-)
	glm::mat3 normalTf = glm::inverse(glm::transpose(glm::mat3(modelTf)));

	mProgram->setUniformMat4("modelMatrix", modelTf);
	mProgram->setUniformMat4("normalMatrix", normalTf);
	mProgram->setUniformVec3("kD", color);

	glBindVertexArray(mesh->resourceHandle);
	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, nullptr);
}

void App::cleanup()
{
}

void App::resize(const unsigned w, const unsigned h)
{
	mWindowWidth = w;
	mWindowHeight = h;
	mWindowAspectRatio = mWindowWidth / static_cast<float>(h);
	// update projection matrix
	mCamera.setAspectRatio(mWindowAspectRatio);
}

void App::mouseButton(const int button, const int action, const int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		mStopTime = !mStopTime;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mCamera.startRotation(mMousePosition);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mCamera.endRotation();
	}
}

void App::mousePosition(const double xpos, const double ypos)
{
	mMousePosition = glm::uvec2(xpos, mWindowHeight - ypos);
	if (xpos < 0 || xpos >= mWindowWidth || ypos < 0 || ypos >= mWindowHeight)
	{
		mCamera.endRotation();
		return;
	}
	else
	{
		mCamera.updateRotation(mMousePosition);
	}
}

void App::keyInput(const int key, const int scancode, const int action, const int mods)
{
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS)
			mCamera.startMoving(Camera::Direction::FWD);
		else if (action == GLFW_RELEASE)
			mCamera.stopMoving(Camera::Direction::FWD);
	}

	if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS)
			mCamera.startMoving(Camera::Direction::LEFT);
		else if (action == GLFW_RELEASE)
			mCamera.stopMoving(Camera::Direction::LEFT);
	}

	if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
			mCamera.startMoving(Camera::Direction::RIGHT);
		else if (action == GLFW_RELEASE)
			mCamera.stopMoving(Camera::Direction::RIGHT);
	}

	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS)
			mCamera.startMoving(Camera::Direction::BWD);
		else if (action == GLFW_RELEASE)
			mCamera.stopMoving(Camera::Direction::BWD);
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}