#pragma once
#include "Actor.h"
#include "SpaceshipMoveComponent.h"

class SpaceshipMovementInput : public SpaceshipMoveComponent
{
public:
	SpaceshipMovementInput(Actor* ownerP);
	SpaceshipMovementInput() = delete;
	SpaceshipMovementInput(const SpaceshipMovementInput&) = delete;
	SpaceshipMovementInput& operator=(const SpaceshipMovementInput&) = delete;
	
	void update(float dt) override;
	void processInput(const struct InputState& inputState) override;

private:
	
	bool isYawing = false;
	bool isRolling = false;
	float rollSpeedSlerp = 1.f;
	float rollSpeedWhenYawSlerp = .2f;
	const float moveSpeed = 100.f;
	const float upSpeed = 100.f;
	const float rotationAcceptance = .5f;
};
