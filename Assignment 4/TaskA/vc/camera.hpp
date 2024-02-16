#pragma once

#include "cg\glm.hpp"

#include <array>

class Camera
{
public:

	Camera();
	Camera(glm::vec3 const& pos);

	void setAspectRatio(const float a);
	glm::mat4 viewMatrix() const;
	glm::mat4 projectionMatrix() const;
	glm::vec3 position() const;

	void startRotation(glm::uvec2 const& mouse);
	void updateRotation(glm::uvec2 const& mouse);
	void endRotation();

	enum class Direction : unsigned { FWD = 0, BWD = 1, RIGHT = 2, LEFT = 3, UP = 4, DOWN = 5 };
	void startMoving(const Direction dir);
	void stopMoving(const Direction dir);

	void update();

private:

	std::array<bool, 6> mMovementStatus;
	float mMovementSpeed;

	glm::vec3 mPosition;
	glm::vec3 mForward;
	glm::vec3 mUp;
	glm::vec3 mRight;

	float mFoV;
	float mAspect;
	float mZNear;
	float mZFar;

	bool mIsRotating;
	glm::vec2 mMousePosition;

	float mYaw;
	float mPitch;
};
