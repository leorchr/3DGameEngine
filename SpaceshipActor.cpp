#include "SpaceshipActor.h"
#include "SpaceshipCameraComponent.h"
#include "InputSystem.h"
#include "MoveComponent.h"
#include "Quaternion.h"
#include <iostream>

#define INVERT_MOUSE_AXIS false

SpaceshipActor::SpaceshipActor() :
	Actor(),
	moveComponent(nullptr),
	cameraComponent(nullptr)
{
	moveComponent = new MoveComponent(this);
	cameraComponent = new SpaceshipCameraComponent(this);
}

void SpaceshipActor::updateActor(float dt)
{
	Actor::updateActor(dt);
	
	float roll = getRotation().getRoll() * 180 / Maths::pi;
	
	float acceptation = .5f;
	if(isRolling) return;
	if(roll < 45.0f && roll > -45.0f)
	{
		if(std::abs(roll) < acceptation)
		{
			isYawing = false;
			return;
		}
		// Current Rotation
		Quaternion newRotation = getRotation();
		Vector3 forward = Vector3::transform(Vector3::unitX, newRotation);

		// End Rotation
		Quaternion finalRotation;
		if(roll > 0) finalRotation = Quaternion(forward, -dt);
		else finalRotation = Quaternion(forward, dt);

		// Slerp Rotation
		newRotation = Quaternion::concatenate(newRotation, finalRotation);
		Quaternion stepRotation = Quaternion::slerp(getRotation(), newRotation, rollSpeedSlerp);

		// Apply
		setRotation(stepRotation);
	}

	if(roll < 135.0f && roll > 45.0f)
	{
		if(std::abs(roll-90) < acceptation)
		{
			isYawing = false;
			return;
		}
		// Current Rotation
		Quaternion newRotation = getRotation();
		Vector3 forward = Vector3::transform(Vector3::unitX, newRotation);

		// End Rotation
		Quaternion finalRotation;
		if(roll > 90) finalRotation = Quaternion(forward, -dt);
		else finalRotation = Quaternion(forward, dt);

		// Slerp Rotation
		newRotation = Quaternion::concatenate(newRotation, finalRotation);
		Quaternion stepRotation = Quaternion::slerp(getRotation(), newRotation, rollSpeedSlerp);

		// Apply
		setRotation(stepRotation);
	}

	if(roll < -45.0f && roll > -135.0f)
	{
		if(std::abs(roll+90) < acceptation)
		{
			isYawing = false;
			return;
		}
		// Current Rotation
		Quaternion newRotation = getRotation();
		Vector3 forward = Vector3::transform(Vector3::unitX, newRotation);
	
		// End Rotation
		Quaternion finalRotation;
		if(roll > -90) finalRotation = Quaternion(forward, -dt);
		else finalRotation = Quaternion(forward, dt);
	
		// Slerp Rotation
		newRotation = Quaternion::concatenate(newRotation, finalRotation);
		Quaternion stepRotation = Quaternion::slerp(getRotation(), newRotation, rollSpeedSlerp);
	
		// Apply
		setRotation(stepRotation);
	}
	
	if(roll > 135.0f || roll < -135.0f)
	{
		if(180-std::abs(roll) < acceptation)
		{
			isYawing = false;
			return;
		}
		// Current Rotation
		Quaternion newRotation = getRotation();
		Vector3 forward = Vector3::transform(Vector3::unitX, newRotation);
	
		// End Rotation
		Quaternion finalRotation;
		if(roll < 0) finalRotation = Quaternion(forward, -dt);
		else finalRotation = Quaternion(forward, dt);
	
		// Slerp Rotation
		newRotation = Quaternion::concatenate(newRotation, finalRotation);
		Quaternion stepRotation = Quaternion::slerp(getRotation(), newRotation, rollSpeedSlerp);
	
		// Apply
		setRotation(stepRotation);
	}
}

void SpaceshipActor::actorInput(const InputState& inputState)
{
	Actor::actorInput(inputState);

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
		strafeSpeed -= moveSpeed;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		strafeSpeed += moveSpeed;
	}
	moveComponent->setForwardSpeed(forwardSpeed);
	moveComponent->setStrafeSpeed(strafeSpeed);

	float upDownSpeed = 0.f;
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_LCTRL))
	{
		upDownSpeed -= upSpeed;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_SPACE))
	{
		upDownSpeed += upSpeed;
	}
	moveComponent->setUpSpeed(upDownSpeed);

	float rollSpeed = 0.0f;
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_Q))
	{
		rollSpeed += 2.0f;
		isRolling = true;
	}
	else	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_E))
	{
		rollSpeed -= 2.0f;
		isRolling = true;
	}
	else isRolling = false;
	moveComponent->setRollSpeed(rollSpeed);
	
	// Mouse mouvement
	Vector2 mousePosition = inputState.mouse.getPosition();
	float x = mousePosition.x;
	
	float y = mousePosition.y * (INVERT_MOUSE_AXIS ? -1.f : 1.f);
	const int maxMouseSpeed = 500;

	if(Maths::nearZero(y) || Maths::nearZero(x)) rollSpeedSlerp = .5f;
	else
	{
		isYawing = true;
		rollSpeedSlerp = .2f;
	}

	
	const float maxAngularSpeed = Maths::pi * 8;
	float yawSpeed = 0.0f;
	if (x != 0)
	{
		yawSpeed = x / maxMouseSpeed;
		yawSpeed *= maxAngularSpeed;
	}
	moveComponent->setYawSpeed(yawSpeed);

	
	const float maxPitchSpeed = Maths::pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		pitchSpeed = y / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	moveComponent->setPitchSpeed(pitchSpeed);
}