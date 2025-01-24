#include "SpaceshipMovementInput.h"
#include "InputSystem.h"
#include <algorithm>
#define INVERT_MOUSE_AXIS false

SpaceshipMovementInput::SpaceshipMovementInput(Actor* ownerP) : SpaceshipMoveComponent(ownerP){}

void SpaceshipMovementInput::update(float dt)
{
	SpaceshipMoveComponent::update(dt);
	if(isRolling) return;

	Vector3 right = owner.getRight();
	Vector3 forward = owner.getForward();
	Vector3 globalUp = Vector3::unitZ;

	float angle = Maths::acos(Vector3::dot(right,globalUp));
	Vector3 cross = Vector3::cross(right, globalUp);
	float sign = Vector3::dot(cross, forward) >= 0.0f ? 1.0f : -1.0f;

	float currentRoll = angle * sign;
	
	float goalRoll = Maths::snapTo(currentRoll, 0.f, -Maths::twoPi, Maths::twoPi, Maths::piOver2,
								 -Maths::piOver2, Maths::pi, -Maths::pi);

	if(std::abs(currentRoll - goalRoll) < 0.02f) return;

	float lerpedRoll = Maths::lerp(currentRoll, goalRoll, std::clamp(dt * rollSpeedSlerp, 0.01f, 1.f));

	if((currentRoll - lerpedRoll) == 0.0f) return;
	Quaternion angledRoll = Quaternion(forward, currentRoll - lerpedRoll);

	Quaternion endPosition = Quaternion::concatenate(owner.getRotation(), angledRoll); 
	owner.setRotation(endPosition);
}

void SpaceshipMovementInput::processInput(const InputState& inputState)
{
	SpaceshipMoveComponent::processInput(inputState);

	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	// wasd movement
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_W))
	{
		forwardSpeed += moveSpeed;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_S))
	{
		forwardSpeed -= moveSpeed;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_A))
	{
		strafeSpeed += moveSpeed;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		strafeSpeed -= moveSpeed;
	}
	setForwardSpeed(forwardSpeed);
	setStrafeSpeed(strafeSpeed);

	float upDownSpeed = 0.f;
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_LCTRL))
	{
		upDownSpeed -= upSpeed;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_SPACE))
	{
		upDownSpeed += upSpeed;
	}
	setUpSpeed(upDownSpeed);

	float rollSpeed = 0.0f;
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_Q))
	{
		rollSpeed += 2.0f;
		isRolling = true;
	}
	else if (inputState.keyboard.getKeyValue(SDL_SCANCODE_E))
	{
		rollSpeed -= 2.0f;
		isRolling = true;
	}
	else isRolling = false;
	setRollSpeed(rollSpeed);
	
	// Mouse mouvement
	Vector2 mousePosition = inputState.mouse.getPosition();
	float x = mousePosition.x;
	
	float y = mousePosition.y * (INVERT_MOUSE_AXIS ? -1.f : 1.f);
	const int maxMouseSpeed = 500;
	
	const float maxAngularSpeed = Maths::pi * 8;
	float yawSpeed = 0.0f;
	if (x != 0)
	{
		yawSpeed = x / maxMouseSpeed;
		yawSpeed *= maxAngularSpeed;
		if(std::abs(x) > 20.0f) rollSpeed += x / maxMouseSpeed * -5;
	}
	setYawSpeed(yawSpeed);
	setRollSpeed(rollSpeed);
	
	const float maxPitchSpeed = Maths::pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		pitchSpeed = y / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed * -1.0f;
	}
	setPitchSpeed(pitchSpeed);
}