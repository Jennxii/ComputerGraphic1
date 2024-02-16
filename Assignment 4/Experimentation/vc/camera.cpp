#include "camera.hpp"

Camera::Camera() : mFoV(45.f), mAspect(1.f), mZNear(0.1f), mZFar(100.f)
{
	mIsRotating = false;
	mMousePosition = glm::vec2(0.f);

	mPitch = 0.f;
	mYaw = 0.f;
	mPosition = glm::vec3(0.f, 0.f, 5.f);
	mForward = glm::normalize(glm::vec3(0.f, 0.f, -1.f));
	mUp = glm::vec3(0.f, 1.f, 0.f);
	mRight = glm::vec3(1.f, 0.f, 0.f);

	for (unsigned i = 0; i < 6; ++i)
		mMovementStatus[i] = false;

	mMovementSpeed = 0.05f;
}

Camera::Camera(glm::vec3 const& pos) : mFoV(45.f), mAspect(1.f), mZNear(0.1f), mZFar(100.f), mPosition(pos)
{
	mIsRotating = false;
	mMousePosition = glm::vec2(0.f);

	mPitch = 0.f;
	mYaw = 0.f;
	mForward = glm::normalize(glm::vec3(0.f, 0.f, -1.f));
	mUp = glm::vec3(0.f, 1.f, 0.f);
	mRight = glm::vec3(1.f, 0.f, 0.f);

	for (unsigned i = 0; i < 6; ++i)
		mMovementStatus[i] = false;

	mMovementSpeed = 0.05f;
}

void Camera::setAspectRatio(const float a)
{
	mAspect = a;
}

glm::mat4 Camera::projectionMatrix() const
{
	return glm::perspective(glm::radians(mFoV), mAspect, mZNear, mZFar);
}

glm::mat4 Camera::viewMatrix() const
{
	return glm::lookAt(mPosition, mPosition + mForward, mUp);
}

glm::vec3 Camera::position() const
{
	return mPosition;
}

void Camera::startRotation(glm::uvec2 const& mouse)
{
	mIsRotating = true;
	mMousePosition = mouse;
}

void Camera::updateRotation(glm::uvec2 const& mouse)
{
	if (!mIsRotating)
	{
		return;
	}

	float xoffset = (float)mouse.x - mMousePosition.x;
	float yoffset = (float)mouse.y - mMousePosition.y;

	mMousePosition = mouse;

	xoffset *= 0.1f;
	yoffset *= 0.1f;

	mYaw += xoffset;
	mPitch += yoffset;

	if (mPitch > 89.0f)
		mPitch = 89.0f;
	if (mPitch < -89.0f)
		mPitch = -89.0f;

	glm::vec3 dir;
	dir.x = cos(glm::radians(mYaw - 90.f)) * cos(glm::radians(mPitch));
	dir.y = sin(glm::radians(mPitch));
	dir.z = sin(glm::radians(mYaw - 90.f)) * cos(glm::radians(mPitch));

	mForward = glm::normalize(dir);
	mRight = glm::normalize(glm::cross(mForward, glm::vec3(0.f, 1.f, 0.f)));
	mUp = glm::normalize(glm::cross(mRight, mForward));
}

void Camera::endRotation()
{
	mIsRotating = false;
}

void Camera::update()
{
	glm::vec3 move(0.f);

	if (mMovementStatus[0])
		move += mMovementSpeed * mForward;
	if (mMovementStatus[1])
		move -= mMovementSpeed * mForward;
	if (mMovementStatus[2])
		move += mMovementSpeed * mRight;
	if (mMovementStatus[3])
		move -= mMovementSpeed * mRight;

	mPosition += move;
}

void Camera::startMoving(const Camera::Direction dir)
{
	mMovementStatus[(unsigned)dir] = true;
}

void Camera::stopMoving(const Camera::Direction dir)
{
	mMovementStatus[(unsigned)dir] = false;
}