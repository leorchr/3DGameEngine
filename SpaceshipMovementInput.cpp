#include "SpaceshipMovementInput.h"
#include "InputSystem.h"
#define INVERT_MOUSE_AXIS false

SpaceshipMovementInput::SpaceshipMovementInput(Actor* ownerP) : SpaceshipMoveComponent(ownerP){}

void SpaceshipMovementInput::update(float dt)
{
	SpaceshipMoveComponent::update(dt);

	float roll = owner.getRotation().getRoll() * 180 / Maths::pi;
	float acceptation = rotationAcceptance;
	if(isRolling) return;
	if(roll < 45.0f && roll > -45.0f)
	{
		if(std::abs(roll) < acceptation)
		{
			isYawing = false;
			return;
		}

		// Current Rotation
		Quaternion newRotation = owner.getRotation();

		// Slerp Rotation
		Quaternion finalRotation;
		
		Vector3 forward = Vector3::transform(Vector3::unitX, newRotation);
		
		if(roll > 0) finalRotation = Quaternion(forward, -dt);
		else finalRotation = Quaternion(forward, dt);

		// End Rotation
		// Apply Rotation
		Quaternion endRotation = Quaternion::concatenate(newRotation, finalRotation);
		Quaternion stepRotation = Quaternion::slerp(owner.getRotation(), endRotation, rollSpeedSlerp);

		// Apply
		owner.setRotation(stepRotation);
	}

	if(roll < 135.0f && roll > 45.0f)
	{
		if(std::abs(roll-90) < acceptation)
		{
			isYawing = false;
			return;
		}
		// Current Rotation
		Quaternion newRotation = owner.getRotation();
		Vector3 forward = Vector3::transform(Vector3::unitX, newRotation);

		// End Rotation
		Quaternion finalRotation;
		if(roll > 90) finalRotation = Quaternion(forward, -dt);
		else finalRotation = Quaternion(forward, dt);

		// Slerp Rotation
		newRotation = Quaternion::concatenate(newRotation, finalRotation);
		Quaternion stepRotation = Quaternion::slerp(owner.getRotation(), newRotation, rollSpeedSlerp);

		// Apply
		owner.setRotation(stepRotation);
	}

	if(roll < -45.0f && roll > -135.0f)
	{
		if(std::abs(roll+90) < acceptation)
		{
			isYawing = false;
			return;
		}
		// Current Rotation
		Quaternion newRotation = owner.getRotation();
		Vector3 forward = Vector3::transform(Vector3::unitX, newRotation);
	
		// End Rotation
		Quaternion finalRotation;
		if(roll > -90) finalRotation = Quaternion(forward, -dt);
		else finalRotation = Quaternion(forward, dt);
	
		// Slerp Rotation
		newRotation = Quaternion::concatenate(newRotation, finalRotation);
		Quaternion stepRotation = Quaternion::slerp(owner.getRotation(), newRotation, rollSpeedSlerp);
	
		// Apply
		owner.setRotation(stepRotation);
	}
	
	if(roll > 135.0f || roll < -135.0f)
	{
		if(180-std::abs(roll) < acceptation)
		{
			isYawing = false;
			return;
		}
		// Current Rotation
		Quaternion newRotation = owner.getRotation();
		Vector3 forward = Vector3::transform(Vector3::unitX, newRotation);
	
		// End Rotation
		Quaternion finalRotation;
		if(roll < 0) finalRotation = Quaternion(forward, -dt);
		else finalRotation = Quaternion(forward, dt);
	
		// Slerp Rotation
		newRotation = Quaternion::concatenate(newRotation, finalRotation);
		Quaternion stepRotation = Quaternion::slerp(owner.getRotation(), newRotation, rollSpeedSlerp);
	
		// Apply
		owner.setRotation(stepRotation);
	}
	
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
		strafeSpeed -= moveSpeed;
	}
	if (inputState.keyboard.getKeyValue(SDL_SCANCODE_D))
	{
		strafeSpeed += moveSpeed;
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

	if(Maths::nearZero(y) || Maths::nearZero(x)) rollSpeedSlerp = .5f;
	else
	{
		isYawing = true;
		rollSpeedSlerp = rollSpeedWhenYawSlerp;
	}

	
	const float maxAngularSpeed = Maths::pi * 8;
	float yawSpeed = 0.0f;
	if (x != 0)
	{
		yawSpeed = x / maxMouseSpeed;
		yawSpeed *= maxAngularSpeed;
	}
	setYawSpeed(yawSpeed);

	
	const float maxPitchSpeed = Maths::pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		pitchSpeed = y / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	setPitchSpeed(pitchSpeed);
}