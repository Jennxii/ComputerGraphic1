#include "app.hpp"
#include "parametric_curves.hpp"
#include "transform.hpp"

#include "cg\glm.hpp"
#include "cg\glsl.hpp"
#include "cg\triangle_mesh.hpp"
#include "cg\primitives.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <STBI\stb_image.h>

#include <GLFW\glfw3.h>
#include <cmath>
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

	mSpotLight = cg::GlslProgram::create({
		{ cg::ShaderType::VERTEX_SHADER, cg::loadShaderSource("..\\shaders\\spotLight.vert") },
		{ cg::ShaderType::FRAGMENT_SHADER, cg::loadShaderSource("..\\shaders\\spotLight.frag") }
		});

	mTexture = cg::GlslProgram::create({
	{ cg::ShaderType::VERTEX_SHADER, cg::loadShaderSource("..\\shaders\\texture.vert") },
	{ cg::ShaderType::FRAGMENT_SHADER, cg::loadShaderSource("..\\shaders\\texture.frag") }
		});

	mWaves = cg::GlslProgram::create({
	{ cg::ShaderType::VERTEX_SHADER, cg::loadShaderSource("..\\shaders\\waves.vert") },
	{ cg::ShaderType::FRAGMENT_SHADER, cg::loadShaderSource("..\\shaders\\waves.frag") }
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

	glm::vec4 black(0.f);
	stbi_set_flip_vertically_on_load(1);

	int w, h, c;
	unsigned char* data = nullptr;

	data = stbi_load("brot.jpg", &w, &h, &c, 0);
	std::cout << w << " " << h << " " << c << std::endl;
	glGenTextures(1, &mTextureHandleA);
	glBindTexture(GL_TEXTURE_2D, mTextureHandleA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &black[0]);
	glBindTexture(GL_TEXTURE_2D, 0);

	STBI_FREE(data);
	data = nullptr;

	data = stbi_load("meer.jpg", &w, &h, &c, 0);
	std::cout << w << " " << h << " " << c << std::endl;
	glGenTextures(1, &mTextureHandleB);
	glBindTexture(GL_TEXTURE_2D, mTextureHandleB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &black[0]);
	glBindTexture(GL_TEXTURE_2D, 0);


	STBI_FREE(data);
	data = nullptr;


	data = stbi_load("owls.png", &w, &h, &c, 0);
	std::cout << w << " " << h << " " << c << std::endl;
	glGenTextures(1, &mTextureHandleC);
	glBindTexture(GL_TEXTURE_2D, mTextureHandleC);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &black[0]);
	glBindTexture(GL_TEXTURE_2D, 0);


	STBI_FREE(data);
	data = nullptr;

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

	Transform shipOrbit(glm::vec3(3.f), glm::vec3(0.f), glm::angleAxis(0.3f  * mTotalElapsedSeconds, glm::vec3(0.f, 1.f, 0.f)));
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

	Transform leftWheel(glm::vec3(1.f), glm::vec3(0.f, 0.f, -0.35f), glm::angleAxis(mTotalElapsedSeconds, glm::vec3(0.f, 0.f, -1.f)));
	Transform rightWheel(glm::vec3(1.f), glm::vec3(0.f, 0.f, +0.35f), glm::angleAxis(mTotalElapsedSeconds, glm::vec3(0.f, 0.f, -1.f)));

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
	//here was directionalLightDir
	mProgram->setUniformVec3("directionalLightDirection", glm::normalize(glm::vec3(-1.f, -0.5f, -1.f)));
	// low intensity ambient
	mProgram->setUniformVec3("ambientLightColor", glm::vec3(0.05f, 0.075f, 0.1f));


	float degrees = 45.0f; // Angle in degrees
	const float spotLightAngle = degrees * 3.14159265 / 180.0f;

	float coneLengthScaler = 3.f;
	float coneRadiusScaler = glm::tan(spotLightAngle) * coneLengthScaler / 1;

	glm::vec3 spotLightColor = glm::vec3(1.0, 1.0, 0.00);
	glm::vec3 spotLightPosition = glm::vec3(1.65f, -0.22f, 0.f);
	glm::vec3 spotLightDirection = glm::vec3(1, -0.7, 0);

	Transform spotLightConePosition{ glm::vec3(1), spotLightPosition, glm::fquat(1, 0, 0, 0) };
	Transform spotLightConeRotator{ glm::vec3(1), glm::vec3(0), glm::fquat(glm::vec3(0, -1, 0), spotLightDirection) };

	Transform spotLightCone{ glm::vec3(coneRadiusScaler, coneLengthScaler, coneRadiusScaler), glm::vec3(0, 0 , 0), glm::fquat(1, 0, 0, 0) };
	glm::mat4 modelTf = shipOrbit.modelMatrix() * shipOffset.modelMatrix() * spotLightConePosition.modelMatrix() * spotLightConeRotator.modelMatrix() * spotLightCone.modelMatrix();

	//spotlight
	mProgram->setUniformVec3("spotlightColor", spotLightColor);
	mProgram->setUniformVec3("spotlightPosition", glm::vec3(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * spotLightConePosition.modelMatrix() * spotLightConeRotator.modelMatrix() * spotLightCone.modelMatrix() * glm::vec4(0.f, 0.5, 0.f, 1.f)));
	mProgram->setUniformVec3("spotlightDirection", glm::vec3(shipOrbit.modelMatrix() * glm::vec4(spotLightDirection + glm::vec3(2.0, -0.2, 0), 0)));
	mProgram->setUniformVal("spotlightAngle", atan(spotLightCone.scale.x/spotLightCone.scale.y));

	mProgram->setUniformMat4("modelMatrix", modelTf);
	


		
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
	

		// smoke particles
		for (auto const& smokeParticle : smokeParticles)
		{
			drawShaded(mSphere, smokeParticle.modelMatrix(), white);
		}

	glBindVertexArray(0);
	cg::GlslProgram::useProgram(nullptr);
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	cg::GlslProgram::useProgram(mSpotLight);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	mSpotLight->setUniformMat4("observer.viewMatrix", mCamera.viewMatrix());
	mSpotLight->setUniformMat4("observer.projectionMatrix", mCamera.projectionMatrix());
	mSpotLight->setUniformVec3("objectColor", glm::vec3(1.0, 1.0, 0));
	
	drawShaded(mCone, shipOrbit.modelMatrix()* shipOffset.modelMatrix()* spotLightConePosition.modelMatrix()* spotLightConeRotator.modelMatrix()* spotLightCone.modelMatrix(), spotLightColor);

	mSpotLight->setUniformMat4("modelMatrix", modelTf);


	glBindVertexArray(mCone->resourceHandle);

	glDrawElements(GL_TRIANGLES, mCone->indexCount, GL_UNSIGNED_INT, nullptr);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	cg::GlslProgram::useProgram(nullptr);
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	glActiveTexture(GL_TEXTURE0);	// tex unit 0 is the default, so as long as there si just a single texture, you can skip this
	glBindTexture(GL_TEXTURE_2D, mTextureHandleA);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTextureHandleC);

	cg::GlslProgram::useProgram(mTexture);
	mTexture->setUniformTexVal("textureA", 0);	// check the texture unit where texture A was bound -> shader wants that value
	mTexture->setUniformTexVal("textureB", 1);	// check the texture unit where texture B was bound -> shader wants that value

	mTexture->setUniformMat4("observer.viewMatrix", mCamera.viewMatrix());
	mTexture->setUniformMat4("observer.projectionMatrix", mCamera.projectionMatrix());
	mTexture->setUniformVec3("directionalLightColor", glm::vec3(0.4f));
	mTexture->setUniformVec3("directionalLightDirection", glm::normalize(glm::vec3(-1.f, -0.5f, -1.f)));
	mTexture->setUniformVec3("ambientLightColor", glm::vec3(0.05f, 0.075f, 0.1f));

	//spotlight
	mTexture->setUniformVec3("spotlightColor", spotLightColor);
	mTexture->setUniformVec3("spotlightPosition", glm::vec3(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * spotLightConePosition.modelMatrix() * spotLightConeRotator.modelMatrix() * spotLightCone.modelMatrix() * glm::vec4(0.f, 0.5, 0.f, 1.f)));
	mTexture->setUniformVec3("spotlightDirection", glm::vec3(shipOrbit.modelMatrix() * glm::vec4(spotLightDirection + glm::vec3(2.0, -0.2, 0), 0)));
	mTexture->setUniformVal("spotlightAngle", atan(spotLightCone.scale.x / spotLightCone.scale.y));
	mTexture->setUniformVal("phase", mTotalElapsedSeconds);

	// flag 1
	drawTexture(mPlane, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * backPole.modelMatrix() * flag.modelMatrix(), purple);
	// flag 2
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureHandleB);
	drawTexture(mPlane, shipOrbit.modelMatrix() * shipOffset.modelMatrix() * frontPole.modelMatrix() * flag.modelMatrix(), purple);

	//glm::mat4 flagModel = shipOrbit.modelMatrix() * shipOffset.modelMatrix() * frontPole.modelMatrix() * flag.modelMatrix();

	//mTexture->setUniformMat4("modelMatrix", flagModel);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	cg::GlslProgram::useProgram(nullptr);

	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	

	cg::GlslProgram::useProgram(mWaves);
	

	mWaves->setUniformMat4("observer.viewMatrix", mCamera.viewMatrix());
	mWaves->setUniformMat4("observer.projectionMatrix", mCamera.projectionMatrix());
	mWaves->setUniformVec3("directionalLightColor", glm::vec3(0.4f));
	mWaves->setUniformVec3("directionalLightDirection", glm::normalize(glm::vec3(-1.f, -0.5f, -1.f)));
	mWaves->setUniformVec3("ambientLightColor", glm::vec3(0.05f, 0.075f, 0.1f));

	//spotlight
	mWaves->setUniformVec3("spotlightColor", spotLightColor);
	mWaves->setUniformVec3("spotlightPosition", glm::vec3(shipOrbit.modelMatrix() * shipOffset.modelMatrix() * spotLightConePosition.modelMatrix() * spotLightConeRotator.modelMatrix() * spotLightCone.modelMatrix() * glm::vec4(0.f, 0.5, 0.f, 1.f)));
	mWaves->setUniformVec3("spotlightDirection", glm::vec3(shipOrbit.modelMatrix() * glm::vec4(spotLightDirection + glm::vec3(2.0, -0.2, 0), 0)));
	mWaves->setUniformVal("spotlightAngle", atan(spotLightCone.scale.x / spotLightCone.scale.y));
	mWaves->setUniformVal("phase", mTotalElapsedSeconds);

	// sea
	drawShaded(mDisk, seaLevel.modelMatrix(), blue);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_MULTISAMPLE);

	glBindVertexArray(0);
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
	//i think here was the Mat4 before
	mProgram->setUniformMat3("normalMatrix", normalTf);
	mProgram->setUniformVec3("kD", color);

	glBindVertexArray(mesh->resourceHandle);
	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, nullptr);

}

void App::drawTexture(std::shared_ptr<cg::TriangleMesh> mesh,
	glm::mat4 const& modelTf,
	glm::vec3 const& color)
{
	// we calculate the normal matrix on the cpu instead of in the shader.
	// also, I turn it into a mat3 & thus remove the translation part
	// so you don't have to worry about appending 0 or 1 to the vertex normal :-)
	glm::mat3 normalTf = glm::inverse(glm::transpose(glm::mat3(modelTf)));

	mTexture->setUniformMat4("modelMatrix", modelTf);
	//i think here was the Mat4 before
	mTexture->setUniformMat3("normalMatrix", normalTf);

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