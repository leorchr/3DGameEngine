#pragma once
#include "Actor.h"

class SpaceshipActor : public Actor
{
public:
	SpaceshipActor();

	void updateActor(float dt) override;
	void actorInput(const InputState& inputState) override;

private:
	class SpaceshipMovementInput* moveInputComponent;
	class SpaceshipCameraComponent* cameraComponent;
	class SpaceshipCollisionsComponent* physicsComponent;
};