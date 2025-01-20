#pragma once
#include "Actor.h"

class SpaceshipActor : public Actor
{
public:
	SpaceshipActor();

	void updateActor(float dt) override;

private:
	class SpaceshipMovementInput* moveInputComponent;
	class SpaceshipCameraComponent* cameraComponent;
	class BoxComponent* boxComponent;
	class PhysicsComponent* physicsComponent;
};