#pragma once
#include "Actor.h"

class SpaceshipActor : public Actor
{
public:
	SpaceshipActor();

	void updateActor(float dt) override;
	void actorInput(const struct InputState& inputState) override;

private:
	class MoveComponent* moveComponent;
	class SpaceshipCameraComponent* cameraComponent;
	bool isYawing = false;
	
	bool isRolling = false;

	float rollSpeedSlerp = 1.f;
	const float moveSpeed = 100.f;
	const float upSpeed = 100.f;
};